// Apply the dispatch-table names of docs/moby-dispatch.tsv to the level overlay that is
// currently open in Ghidra.
//
// Usage (Ghidra script, not a build script):
//   run this script on the overlay program with one argument: the level name as it appears
//   in the TSV, e.g.  "1_oozla".  A second optional argument overrides the TSV path.
//
// Address forms: a level overlay is loaded either at its ELF virtual addresses or, if it was
// imported as a raw image, at file offsets. The script builds both candidates from the ELF
// section table on disk and uses whichever one holds a function.
//
// A function that already carries a name (not FUN_*) is left alone, and a function shared by
// several classes keeps the first name it is given: both are counted and reported.
import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.HashSet;
import java.util.Set;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.FunctionManager;
import ghidra.program.model.symbol.SourceType;

public class ApplyMobyNames extends GhidraScript {

    @Override
    public void run() throws Exception {
        String[] argv = getScriptArgs();
        if (argv.length < 1) {
            println("usage: ApplyMobyNames <level name> [tsv path]");
            return;
        }
        String level = argv[0];
        String tsv = argv.length > 1 ? argv[1] : "docs/moby-dispatch.tsv";

        String elfPath = currentProgram.getExecutablePath();
        if (elfPath.startsWith("/")) {
            elfPath = elfPath.substring(1);
        }
        long[] text = textSection(elfPath);
        println("level " + level + " : .text vaddr=0x" + Long.toHexString(text[0])
                + " file offset=0x" + Long.toHexString(text[1]));

        FunctionManager functions = currentProgram.getFunctionManager();
        BufferedReader reader = new BufferedReader(new FileReader(tsv));
        String line;
        int rows = 0, renamed = 0, alreadyNamed = 0, noFunction = 0, shared = 0;
        Set<Long> done = new HashSet<Long>();
        while ((line = reader.readLine()) != null) {
            String[] c = line.split("\t");
            if (c.length < 8 || !c[1].equals(level) || c[6].isEmpty()) {
                continue;
            }
            rows++;
            long vaddr = Long.parseLong(c[4].substring(2), 16);
            long offset = text[1] + (vaddr - text[0]);
            Function f = functions.getFunctionAt(address(vaddr));
            long key = vaddr;
            if (f == null) {
                f = functions.getFunctionAt(address(offset));
                key = offset;
            }
            if (f == null) {
                noFunction++;
                continue;
            }
            if (!f.getName().startsWith("FUN_")) {
                alreadyNamed++;
                continue;
            }
            if (!done.add(key)) {
                shared++;
                continue;
            }
            f.setName(c[6], SourceType.USER_DEFINED);
            renamed++;
        }
        reader.close();
        println("rows for this level: " + rows + " | renamed: " + renamed
                + " | already named: " + alreadyNamed + " | no function at that address: " + noFunction
                + " | shared handler: " + shared);
        println("names are not saved until the program is saved.");
    }

    /** Returns { .text virtual address, .text file offset } read from the ELF on disk. */
    private long[] textSection(String elfPath) throws Exception {
        byte[] elf = Files.readAllBytes(Paths.get(new File(elfPath).getAbsolutePath()));
        ByteBuffer bb = ByteBuffer.wrap(elf).order(ByteOrder.LITTLE_ENDIAN);
        long shoff = bb.getInt(0x20) & 0xFFFFFFFFL;
        int shentsize = bb.getShort(0x2E) & 0xFFFF;
        int shnum = bb.getShort(0x30) & 0xFFFF;
        int shstrndx = bb.getShort(0x32) & 0xFFFF;
        long namesOff = bb.getInt((int) (shoff + shstrndx * shentsize + 16)) & 0xFFFFFFFFL;
        long namesSize = bb.getInt((int) (shoff + shstrndx * shentsize + 20)) & 0xFFFFFFFFL;
        byte[] names = new byte[(int) namesSize];
        System.arraycopy(elf, (int) namesOff, names, 0, (int) namesSize);
        for (int i = 0; i < shnum; i++) {
            long base = shoff + i * shentsize;
            long nameOff = bb.getInt((int) base) & 0xFFFFFFFFL;
            int end = (int) nameOff;
            while (end < names.length && names[end] != 0) {
                end++;
            }
            String name = new String(names, (int) nameOff, end - (int) nameOff, "ASCII");
            if (name.equals(".text")) {
                return new long[] {
                    bb.getInt((int) base + 12) & 0xFFFFFFFFL,
                    bb.getInt((int) base + 16) & 0xFFFFFFFFL
                };
            }
        }
        throw new IllegalStateException("no .text section in " + elfPath);
    }

    private Address address(long value) {
        return currentProgram.getAddressFactory().getDefaultAddressSpace().getAddress(value);
    }
}
