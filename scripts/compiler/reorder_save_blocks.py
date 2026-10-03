"""Reorder intact save blocks: FPR before GPR on saves, GPR before FPR on loads."""
from pathlib import Path
import sys

p = Path(sys.argv[1])
s = p.read_text(encoding="utf-8")
a = s.index("  /* Save GP registers if needed.  */")
b = s.index("  /* Save floating point registers if needed.  */", a)
c = s.index("\n}\n\f", b)
gp, fp = s[a:b], s[b:c]
assert "for (regno = GP_REG_FIRST; regno <= GP_REG_LAST; regno++)" in gp
assert "regno -= fp_inc)" in fp
assert "RAC2 : FPR saves precede GPR saves" not in s
fp = fp.replace("already set up for gp registers above", "reuse an already established save-area base")
body = """  /* RAC2 : FPR saves precede GPR saves; restores keep GPR before FPR.
     Keep both emission blocks and their offsets/directions intact.  The
     first FPR pass needs the base initialized before either block runs.  */
  {
    int rac2_save_pass;
    base_reg_rtx = 0;
    base_offset = 0;
    for (rac2_save_pass = 0; rac2_save_pass < 2; rac2_save_pass++)
      {
        if ((store_p != 0) == (rac2_save_pass == 0))
          {
"""
body += "\n".join("          " + line if line else "" for line in fp.splitlines())
body += "\n          }\n        else\n          {\n"
body += "\n".join("          " + line if line else "" for line in gp.splitlines())
body += "\n          }\n      }\n  }\n"
p.write_text(s[:a] + body + s[c:], encoding="utf-8", newline="")
print("FPR save block moved; GPR/FPR restores preserved")
