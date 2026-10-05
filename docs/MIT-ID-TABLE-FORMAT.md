# MIT id table, format 1

This is the file the mit-3.7 fork reads on Skyrim SE **1.7.104.0** in place of the Nexus Address Library.
It maps Address Library ids (the AE id column, the same ids `RELOCATION_ID(se, ae)` and `REL::VariantID`
already name) to RVAs in one exact game executable.

The format is ours. It was written from scratch for this fork. It is not the Address Library's format and
it does not read or reuse any Address Library file.

## Where the file lives

```
Data/SKSE/Plugins/mit-idtable-<major>-<minor>-<patch>-<build>.bin
```

For 1.7.104.0 that is `Data/SKSE/Plugins/mit-idtable-1-7-104-0.bin`. The version in the name is the game
executable's ProductVersion, the same string the Address Library file names use.

The fork only reads this file on exactly 1.7.104.0. Every other build keeps its usual source (the Nexus
Address Library on 1.5.97 and 1.6.x, the VR CSV on VR). Any other 1.7.x build is refused at load with a
message, because nothing in the fork is verified for it.

## Byte layout

Little endian. No padding beyond what is listed. Offsets are from the start of the file.

### Header, 64 bytes

| offset | type | field | rule |
|---|---|---|---|
| 0 | char[8] | magic | `MITIDTAB` (no terminator) |
| 8 | u32 | formatVersion | `1` |
| 12 | u32 | headerSize | `64` |
| 16 | u16[4] | gameVersion | major, minor, patch, build of the executable, e.g. 1, 7, 104, 0 |
| 24 | u32 | peTimeDateStamp | `IMAGE_FILE_HEADER.TimeDateStamp` of that executable |
| 28 | u32 | peSizeOfImage | `IMAGE_OPTIONAL_HEADER64.SizeOfImage` of that executable |
| 32 | u32 | recordCount | number of records, n |
| 36 | u32 | recordSize | `16` |
| 40 | u32 | flags | `0` (reserved; a reader refuses any other value) |
| 44 | u32 | reserved | `0` |
| 48 | char[16] | moduleName | `SkyrimSE.exe`, NUL padded |

### Records, n times 16 bytes, starting at offset 64

| offset in record | type | field |
|---|---|---|
| 0 | u64 | id |
| 8 | u64 | rva |

- Records are sorted by `id`, strictly ascending. No id appears twice.
- `rva` is the offset from the image base, never 0, and less than `peSizeOfImage`.
- An id that is not in the file is NOT mapped. There is no "next id", no default and no zero entry.

### Trailer, 8 bytes, at offset 64 + 16n

| type | field |
|---|---|
| u64 | checksum: FNV-1a 64 over every byte from offset 0 up to the trailer |

FNV-1a 64: start at `0xCBF29CE484222325`; for each byte, `h ^= byte; h *= 0x100000001B3` (mod 2^64).

The file size is exactly `64 + 16 * n + 8`. Anything else is refused.

## What a reader must check, in order

Every failure is fatal and names the file. Nothing falls back to another source.

1. The file exists and can be read.
2. Size is at least 72 bytes, and equals `64 + 16 * recordCount + 8`.
3. magic, formatVersion, headerSize, recordSize, flags, reserved hold the values above.
4. The checksum matches.
5. gameVersion equals the running executable's version (all four fields).
6. moduleName matches the running executable's file name (case-insensitive).
7. peTimeDateStamp and peSizeOfImage equal the values in the running executable's own PE headers in memory.
   This binds the file to one exact build, not just one version number. Two different executables can carry
   the same version (1.6.1170 has two), and their addresses differ.
8. Records are strictly ascending by id, and every rva is non-zero and below peSizeOfImage.

The fork loads the records into a buffer owned by the plugin (one copy per DLL). It never puts them in the
shared, writable `CommonLibSSEOffsets-v2-<version>` file mapping that the Address Library path uses, so one
plugin's table can never change what another plugin reads.

## Lookups

A lookup of an id that is not in the file stops the game with a message that names the id, the file, and
the game version, and says that the table does not cover that id yet. It never returns a neighbouring id's
address. `REL::IDDatabase::try_id2offset` is the quiet version for self-checks: it returns nothing.

## Coverage grows over time

The format holds any number of records, so it can grow from the ids a few plugins need to the whole id space.
A larger table is just more records. Readers do not change. If a later need ever requires more fields, the
formatVersion goes up, and readers of version 1 refuse the new file by name instead of misreading it.

## Building a file

`tools/mit-idtable/mit_idtable.py` builds a file from a CSV and the executable, checks a file, and dumps one.
See `tools/mit-idtable/README.md`.
