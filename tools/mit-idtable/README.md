# mit_idtable.py

Builds, checks and dumps MIT id table files, the id source the MIT CommonLibSSE-NG 3.7.0 line (this repository, branch `main`) reads on Skyrim SE 1.7.104.0.
The file format is in [docs/MIT-ID-TABLE-FORMAT.md](../../docs/MIT-ID-TABLE-FORMAT.md).

Needs Python 3.8 or newer and `pefile` (`pip install pefile`), which is only used to read the executable's
ProductVersion string.

## Build a table

You need two things:

1. The exact game executable the RVAs were found in. Its version, PE TimeDateStamp and SizeOfImage go into the
   header, and the fork refuses the file on any other executable.
2. One or more CSV files with a decimal id column and an RVA column. Any other columns can filter rows.

```
python3 tools/mit-idtable/mit_idtable.py build \
    --exe path/to/1.7.104/SkyrimSE.exe \
    --csv data/idmap-1.7.104-fork-full.csv \
    --csv data/idmap-1.7.104-sync.csv \
    --csv data/idmap-1.7.104-fixes.csv \
    --rva-col rva_1_7_104 \
    --where kind=id,vtable,rtti,nirtti \
    --where final_state=MAPPED \
    --where confidence=EXACT,UNIQUE-SIG,XREF \
    --where crosscheck=pass,na \
    --absent-where final_state=REMOVED,INLINED,ABSENT \
    --revision 3 --previous <revision 2 file> \
    --out Data/SKSE/Plugins/
```

This writes `Data/SKSE/Plugins/mit-idtable-v1-1-7-104-0.bin` (format 1.0, revision 3). It is exactly the table in
`data/`. Revisions 1 and 2 were built from fewer rows (see the README changelog).

- `--where COL=A,B` keeps a row only when COL is one of the listed values. Give it several times and every one
  must hold. Rows that pass the filters must have a numeric id and an RVA, or the build stops and names the row.
  A table never drops an id quietly.
- `--absent-where COL=A,B` turns every row that matches all of them into an ABSENT record (rva 0): the id is known
  not to exist in this executable, and a lookup says so instead of "not covered yet". These rows skip `--where`.
- `--csv` can be given several times, so new mappings can live in their own file. An id that appears twice must
  have the same RVA (or be absent) everywhere, or the build stops.
- Every RVA must fall inside a section of the executable.
- `--revision N` is required. It goes into the header and must be higher than every table published before for
  this game version.
- `--previous FILE` names the last published table. The build then refuses to drop any of its ids, change a mapped
  id's RVA, make a mapped id absent, or keep the revision. Always pass it when you build a table to publish: every
  published table must be a strict superset of the one before (docs/MIT-ID-TABLE-FORMAT.md, "Revisions and
  distribution").
- `--module NAME` is the module name the game runs under, `SkyrimSE.exe` by default. The tool warns when the
  executable you pass is named differently (for example a renamed copy), since the fork compares the name.
- After writing, the tool reads its own output back under the same rules the fork applies.

### What the published table holds (revision 3)

The inputs are committed in `data/` with their evidence, so anyone can rebuild and audit them:

- `idmap-1.7.104-fork-full.csv`: every id this fork names (17,677) plus 127 ids my mods use, mapped from 1.6.1170
  to 1.7.104 by disassembly. `summary-1.7.104-fork-full.md` and `selftest-1.7.104-fork-full.md` are the mapper's
  report: ground truth 107 of 107, every precision self-test 0 wrong.
- `idmap-1.7.104-sync.csv` and `summary-1.7.104-sync.md`: the 60 ids the upstream sync 2024-09 added, mapped the
  same way (58 mapped, 2 retired ids).
- `idmap-1.7.104-fixes.csv`: six fork ids that were retired from the AE column and their current ids
  (443410, 439876, 441582, 504099, 443440, 441567, each with its evidence), and `VTABLE_std__bad_weak_ptr` (248775) recorded
  as absent.

The build above gives 17,751 records: **16,502 mapped** and **1,249 absent** (1,247 ids REMOVED, 1 INLINED, 1 with no
vtable in any executable). 43 rows are left out on purpose:

- 11 raw-RVA rows (seat addresses for one mod, not ids).
- 25 NAME-tier rows. Their ids are not in the 1.6.1170 Address Library, so the only thing tying them to a meaning
  is the fork's own label, and this round found fork labels that were wrong. An id in the table must mean what the
  AE library says it means. On 1.6.1170 these ids fail too.
- 7 HARD rows: six retired ids (replaced above) and 248775 (added above as absent).

The table binds to the Steam 1.7.104.0 SkyrimSE.exe. Another build of 1.7.104 needs its own table built from its
own executable.

## Check a table

```
python3 tools/mit-idtable/mit_idtable.py check mit-idtable-v1-1-7-104-0.bin --exe SkyrimSE.exe
```

Checks the format, the checksum, the sort order, and with `--exe` that the header matches that executable and
every RVA lies inside it. Add the same `--csv`, `--rva-col` and `--where` arguments as the build to prove the file
holds exactly those rows.

## Dump a table

```
python3 tools/mit-idtable/mit_idtable.py dump mit-idtable-v1-1-7-104-0.bin
```

## Growing the table

Coverage grows by adding rows to a CSV (or adding a CSV) and building again with a higher `--revision` and
`--previous` set to the last published file. The id numbering is the AE column
of the Address Library (the second id in `RELOCATION_ID(se, ae)`), because 1.7.x continues it. Only add an id once
its RVA is proven on the exact executable, with the evidence kept next to the row. A wrong RVA in this file is worse
than a missing one: a missing id stops the game with its number, a wrong one runs the wrong code.
