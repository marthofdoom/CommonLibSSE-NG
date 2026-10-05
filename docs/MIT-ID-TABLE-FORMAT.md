# MIT id table, format 1.0

This is the table the MIT CommonLibSSE-NG 3.7.0 line (github.com/marthofdoom/CommonLibSSE-NG, branch `main`) uses
on Skyrim SE **1.7.104.0** in place of the Nexus Address Library. It maps Address Library ids (the AE id column,
the same ids `RELOCATION_ID(se, ae)` and `REL::VariantID` already name) to RVAs in one exact game executable.

The format is ours. It was written from scratch for this fork. It is not the Address Library's format and it
does not read or reuse any Address Library file.

## Where the table lives

The table is built into the library. The fork's source keeps it as a file in this format:

```
data/mit-idtable-v<formatMajor>-<major>-<minor>-<patch>-<build>.bin
```

For format 1 and 1.7.104.0 that is `data/mit-idtable-v1-1-7-104-0.bin`. The game version in the name is the
executable's ProductVersion, the same string the Address Library file names use. The build turns the file into a
byte array (`cmake/bin2c.cmake`, or xmake's `utils.bin2c` rule) and `src/REL/MitIdTable.cpp` compiles it in. So
every plugin built with the fork carries the table inside its DLL, and players install nothing for it. The fork
reads no table file at runtime and has no switch to read one.

**Why the format major is in the name.** A later major is, by definition, a layout a major-1 reader cannot read.
The name keeps the two apart in the fork's source and in the tool. A plugin carries the table its own reader was
built for, so a new major never breaks an older plugin. Minor versions only add things a major-1 reader can skip,
so they keep the name.

The fork only uses this table on exactly 1.7.104.0. Every other build keeps its usual source (the Nexus Address
Library on 1.5.97 and 1.6.x, the VR CSV on VR). Any other 1.7.x build is refused at load with a message, because
nothing in the fork is verified for it.

**The table binds to one executable: the Steam 1.7.104.0 SkyrimSE.exe** (PE TimeDateStamp 0x6A8C7046, SizeOfImage
0x3929000). Any other 1.7.104 build, for example from another store, is refused with a message, because its
addresses can differ.

## Byte layout

Little endian. Offsets are from the start of the file.

### Header, `headerSize` bytes (64 in format 1.0)

| offset | type | field | rule |
|---|---|---|---|
| 0 | char[8] | magic | `MITIDTAB` (no terminator) |
| 8 | u16 | formatMajor | `1`. A reader refuses any other major. |
| 10 | u16 | formatMinor | `0` today. A major-1 reader reads every minor. |
| 12 | u32 | headerSize | at least 64. A reader skips bytes past the fields it knows. |
| 16 | u16[4] | gameVersion | major, minor, patch, build of the executable, e.g. 1, 7, 104, 0 |
| 24 | u32 | peTimeDateStamp | `IMAGE_FILE_HEADER.TimeDateStamp` of that executable |
| 28 | u32 | peSizeOfImage | `IMAGE_OPTIONAL_HEADER64.SizeOfImage` of that executable |
| 32 | u32 | recordCount | number of records, n |
| 36 | u32 | recordSize | at least 16, at most 4096. A reader reads the first 16 bytes of each record and skips the rest. |
| 40 | u32 | flags | `0`. Flags are must-understand: a major-1 reader refuses any bit it does not know. |
| 44 | u32 | tableRevision | at least 1. Goes up with every published table for this game version. |
| 48 | char[16] | moduleName | `SkyrimSE.exe`, NUL padded |

### Records, n times `recordSize` bytes, starting at offset `headerSize`

| offset in record | type | field |
|---|---|---|
| 0 | u64 | id |
| 8 | u64 | rva |

- Records are sorted by `id`, strictly ascending. No id appears twice.
- `rva` is the offset from the image base, less than `peSizeOfImage`.
- `rva` 0 is an **absent record**: the id is known NOT to exist in this executable (the game removed the function
  or object, inlined it into its callers, or the id was retired from the AE column). It is never an address.
- An id that is not in the file is NOT mapped. There is no "next id" and no default.

### Trailer, 8 bytes, at offset `headerSize + recordSize * n`

| type | field |
|---|---|
| u64 | checksum: FNV-1a 64 over every byte from offset 0 up to the trailer |

FNV-1a 64: start at `0xCBF29CE484222325`. For each byte, `h ^= byte` then `h *= 0x100000001B3` (mod 2^64).

The file size is exactly `headerSize + recordSize * n + 8`. Anything else is refused.

## Growing the format

- A **minor** version may append header fields (headerSize grows), append record fields (recordSize grows), and
  give meaning to bytes a reader already skips. It never moves or changes a field listed above. Every major-1
  reader reads every minor.
- A **major** version is anything else. It ships under a new file name (see above).
- A new flag bit is must-understand: a major-1 reader refuses it. Use a flag only for a change an old reader must
  not silently ignore.

## What a reader checks, in order

Every failure is fatal and says which rule failed. Nothing falls back to another source. This is the order the fork's
`IDDatabase::load_mit_table` uses.

1. The game version is exactly 1.7.104.0 (any other 1.7.x is refused before the table is read).
2. The bytes are the table built into the plugin. The fork opens no file. (A standalone reader such as
   `mit_idtable.py check` reads a file here.)
3. Size is at least 72 bytes.
4. magic is `MITIDTAB`.
5. formatMajor is 1.
6. headerSize is at least 64 and inside the file, recordSize is between 16 and 4096.
7. flags is 0.
8. Size equals `headerSize + recordSize * recordCount + 8`.
9. The checksum matches.
10. tableRevision is not 0.
11. gameVersion equals the running executable's version (all four fields).
12. moduleName matches the running executable's file name (case-insensitive).
13. peTimeDateStamp and peSizeOfImage equal the values in the running executable's own PE headers in memory. This
    binds the file to one exact build, not just one version number.
14. Records are strictly ascending by id, and every rva is below the running image's SizeOfImage (0 is an absent
    record).
15. If the plugin already declared a minimum revision (see below), tableRevision is at least that.

The fork runs every rule above on the built-in bytes at load, then copies the records into a buffer owned by the
plugin (one copy per DLL). It never puts them in the shared,
writable `CommonLibSSEOffsets-v2-<version>` file mapping that the Address Library path uses, so one plugin's table
can never change what another plugin reads.

## Lookups

A lookup of an id that is not in the table stops the game with a message that names the id, the table's revision,
the game version, and the revision the plugin declared it needs (or that it declared none). It says the table does
not cover every id yet and asks the user to report it to the plugin's author.

A lookup of an absent record stops the game with a message that the id does not exist in this game version (the
game removed or inlined it), and asks the user to report it to the plugin's author. Updating the table cannot fix
that one: the plugin needs another way.

Neither ever returns a neighbouring id's address. `REL::IDDatabase::try_id2offset` is the quiet version for
self-checks: it returns nothing for both.

## Revisions and distribution

Every plugin carries its own copy of the table, the one built into the fork release it was built against. So:

1. **The table ships inside every DLL built with the fork.** There is no separate download and no file to install.
   Two plugins never share or overwrite one table.
2. **Each published table is a strict superset of the previous one**, and a newer fork release carries it. No id is
   removed, no mapped id's RVA changes, no mapped id becomes absent, and tableRevision goes up. An absent id may
   become mapped, as a correction. `mit_idtable.py build --previous <last published file>` enforces all of it. The
   one exception is an RVA proven wrong: `--correct ID --correct-evidence CSV` changes it, with one evidence row per
   id, and the build prints it. A wrong RVA runs the wrong code, so fixing it beats keeping the superset. A plugin
   picks up a new table only when its author rebuilds it against the newer fork release.
3. **A plugin declares the lowest revision it needs**, right after `SKSE::Init`:

   ```cpp
   REL::IDDatabase::RequireMitTableRevision(1);
   ```

   On 1.7.104 it checks the table built into the DLL. If the plugin was built against a fork whose table is older,
   the game stops at load with a message naming both revisions, instead of failing later on one id. That is the
   author's build error, and the first launch shows it. On every other build the call only records the value. `REL::IDDatabase::get().MitTableRevision()`
   returns the revision in use (0 when the ids come from the Address Library).

## Building a file

`tools/mit-idtable/mit_idtable.py` builds a file from a CSV and the executable, checks a file, and dumps one.
See `tools/mit-idtable/README.md`.
