"""Reproduce the retail assembler's division-erratum padding in GNU `as`.

The retail EE assembler (SN ProDG `Ps2EeAs`) refuses to place a
single-precision COP1 division opcode too near a possible branch destination.
Its own message is "DIV related opcode too near possible branch destination",
and for a division inside a delay slot it says "DIV related opcode used in
branch delay slot -- Automatic padding cannot take place".

Measured rule. The oracle is the retail assembler itself, driven by a private
diagnostic harness that is deliberately kept outside this repository; the
harness reconstructs a witness around each sampled retail division site and
asks the retail assembler to regenerate the run length. The witnesses live with
that private harness, not here, and no binary, object or image from it is
published.

  (A) Padding.  When a division opcode is emitted fewer than two instructions
      after the most recent label -- any label, even one never branched to --
      the assembler emits `2 - n` nops before it, where `n` counts the
      instructions emitted since that label.  The division therefore lands
      exactly two slots after the label.  `n` counts every instruction,
      including a `nop` written in the source.  No label has been defined yet
      in the file => no padding (the division may be the first instruction).

  (B) The floor is a maximum, not a sum.  A coprocessor hazard that already
      asks for a nop before the division (a `mtc1` immediately before a
      division that reads the written FPR, or `sync.p` immediately before it)
      satisfies one of the two slots; it is not added.  Measured: a label one
      instruction back together with an adjacent `mtc1` yields one nop, and
      `sync.p` followed by `mtc1` followed by the division yields one nop,
      not two.

  (C) `sync.p` immediately before such a division also asks for one nop on
      its own.  It is the exact `sync.p` opcode, not the `INSN_SYNC` class:
      `sync` and `sync.l` share that flag and do not trigger.  The trigger
      pairs with the same division family and not with the integer opcodes.

The opcode family is measured, not assumed.  `div.s` (funct 0x03), `sqrt.s`
(0x04) and `rsqrt.s` (0x16) carry the padding.  `add.s`, `sub.s`, `mul.s`,
`neg.s`, `mov.s`, `madd.s`, `msub.s`, `adda.s`, `cvt.w.s` do not; the integer
HI/LO `div`/`divu` do not; and this SDK assembler does not accept the D forms
at all.  This is narrower than the family names sometimes quoted for the wall,
and it is what the oracle answers.

The delay-slot half is NOT fixed here.  Our `cc1` places a `div.s` in the
delay slot of the following `jal`, where the retail never does (measured 0 of
19 806).  The accumulator below is skipped in noreorder regions, which is the
same refusal the retail gives, so a division our scheduler parks in a delay
slot still loses the padding.  See docs/COMPILER-NOTES.md.

Usage: pad_div_erratum_nops.py <gas source directory or tc-mips.c>
"""
from pathlib import Path
import sys

STATE_ANCHOR = "static int insn_uses_fpr_exact PARAMS ((struct mips_cl_insn *ip,"
HELPER_ANCHOR = "\nstatic void\nmacro_build (char *place,"

HELPER = '''
/* RAC2 : etat du rembourrage d erratum de division du retail.  Insns comptees
   depuis la derniere etiquette (toute etiquette), et temoin "une etiquette a
   deja ete vue dans le fichier" qui porte l exemption de tete de section.  */
static int rac2_div_insns_since_label;
static int rac2_div_label_defined;

/* RAC2 : famille mesuree a l oracle retail Ps2EeAs.  Seules les divisions
   COP1 simple precision portent le rembourrage : div.s (fonction 0x03),
   sqrt.s (0x04) et rsqrt.s (0x16).  add.s/sub.s/mul.s/neg.s/mov.s/madd.s/
   msub.s/adda.s/cvt.w.s ne le portent pas, les div/divu entiers HI/LO non
   plus, et cet assembleur SDK n accepte pas les formes D.  */
static int
rac2_div_erratum_p (ip)
     struct mips_cl_insn *ip;
{
  if (ip == 0 || ip->insn_mo == 0)
    return 0;
  if ((ip->insn_opcode >> 26) != 0x11)              /* COP1 */
    return 0;
  if (((ip->insn_opcode >> 21) & 0x1F) != 0x10)     /* single precision */
    return 0;
  switch (ip->insn_opcode & 0x3F)
    {
    case 0x03:                                      /* div.s */
    case 0x04:                                      /* sqrt.s */
    case 0x16:                                      /* rsqrt.s */
      return 1;
    default:
      return 0;
    }
}
'''

PAD_ANCHOR = """      /* If the previous instruction was in a noreorder section, then
         we don't want to insert the nop after all.  */
      /* Itbl support may require additional care here. */
      if (prev_insn_unreordered)
	nops = 0;
"""

PAD = """      /* RAC2 : erratum de division du retail.  Le retail refuse un opcode
	 de division COP1 simple precision trop pres d une destination de
	 branchement possible et emet 2 - n nops devant lui, n comptant les
	 instructions emises depuis la derniere etiquette (n'importe quelle
	 etiquette).  C est un PLANCHER, pas une addition : un alea
	 coprocesseur qui demande deja un nop (mtc1 adjacent lu par la
	 division, ou sync.p adjacent) occupe l un des deux emplacements.
	 Mesure : etiquette a une instruction + mtc1 adjacent donne un nop ;
	 sync.p + mtc1 + division donne un nop, pas deux.  Aucune etiquette
	 dans le fichier : pas de rembourrage.  L exemption de « aucune
	 instruction precedente » est portee par rac2_div_label_defined.  */
      if (rac2_div_label_defined
	  && ! mips_opts.mips16
	  && rac2_div_erratum_p (ip)
	  && rac2_div_insns_since_label < 2)
	{
	  int rac2_need = 2 - rac2_div_insns_since_label;
	  if (nops < rac2_need)
	    nops = rac2_need;
	}

      /* RAC2 : un sync.p immediatement devant l opcode de division demande
	 aussi un nop au retail.  Mesure : sync.p oui, sync et sync.l non
	 (les trois partagent le drapeau INSN_SYNC : c est bien l opcode
	 precis qui compte), et seulement devant div.s/sqrt.s/rsqrt.s -- ni
	 devant les div/divu entiers, ni devant add.s.  C est encore un
	 plancher, pas une addition.  */
      if (! mips_opts.mips16
	  && rac2_div_erratum_p (ip)
	  && prev_insn.insn_mo != 0
	  && prev_insn.insn_mo != &dummy_opcode
	  && strcmp (prev_insn.insn_mo->name, "sync.p") == 0
	  && nops < 1)
	nops = 1;

"""

COUNT_ANCHOR = """  /* We just output an insn, so the next one doesn't have a label.  */
  mips_clear_insn_labels ();
"""

COUNT = """  /* RAC2 : compter l instruction emise pour le rembourrage de division.  */
  ++rac2_div_insns_since_label;

  /* We just output an insn, so the next one doesn't have a label.  */
  mips_clear_insn_labels ();
"""

LABEL_ANCHOR = """  l->label = sym;
  l->next = insn_labels;
  insn_labels = l;
"""

LABEL = """  l->label = sym;
  l->next = insn_labels;
  insn_labels = l;

  /* RAC2 : toute etiquette remet le compteur du rembourrage de division a
     zero, meme une etiquette jamais ciblee.  */
  rac2_div_insns_since_label = 0;
  rac2_div_label_defined = 1;
"""


def main() -> None:
    p = Path(sys.argv[1])
    if p.is_dir():
        p = p / "gas" / "config" / "tc-mips.c"
    source = p.read_text(encoding="utf-8")

    assert "static int rac2_div_erratum_p" not in source, \
        "already transformed: this transformer is not idempotent by design"
    assert source.count(STATE_ANCHOR) == 1, "anchor: insn_uses_fpr_exact declaration"
    assert source.count(HELPER_ANCHOR) == 1, "anchor: macro_build definition"
    assert source.count(PAD_ANCHOR) == 1, "anchor: noreorder nop reset"
    assert source.count(COUNT_ANCHOR) == 1, "anchor: append_insn instruction tail"
    assert source.count(LABEL_ANCHOR) == 1, "anchor: mips_define_label body"

    end = source.index(";", source.index(STATE_ANCHOR)) + 1
    source = source[:end] + "\n" + HELPER + source[end:]
    source = source.replace(PAD_ANCHOR, PAD + PAD_ANCHOR, 1)
    source = source.replace(COUNT_ANCHOR, COUNT, 1)
    source = source.replace(LABEL_ANCHOR, LABEL, 1)

    p.write_text(source, encoding="utf-8", newline="")
    print("Division-erratum padding installed in %s" % p)


if __name__ == "__main__":
    main()
