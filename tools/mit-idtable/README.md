# mit_idtable.py

Builds, checks and dumps MIT id table files, the id source the mit-3.7 fork reads on Skyrim SE 1.7.104.0.
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
    --csv idmap-1.7.104.csv \
    --rva-col rva_1_7_104 \
    --where kind=id,vtable,rtti \
    --where confidence=EXACT,UNIQUE-SIG,XREF \
    --where crosscheck=pass,na \
    --out Data/SKSE/Plugins/
```

This writes `Data/SKSE/Plugins/mit-idtable-1-7-104-0.bin`.

- `--where COL=A,B` keeps a row only when COL is one of the listed values. Give it several times and every one
  must hold. Rows that pass the filters must have a numeric id and an RVA, or the build stops and names the row.
  A table never drops an id quietly.
- `--csv` can be given several times, so new mappings can live in their own file. An id that appears twice must
  have the same RVA everywhere, or the build stops.
- Every RVA must fall inside a section of the executable.
- After writing, the tool reads its own output back under the same rules the fork applies.

The command above is the one used for the table this fork was tested with: 354 ids from
`_research/1.7.104-idmap/idmap-1.7.104.csv` (365 of 366 ids mapped by disassembly; 11 rows are raw RVAs, not ids,
and the one unresolved id, 69188, is left out).

## Check a table

```
python3 tools/mit-idtable/mit_idtable.py check mit-idtable-1-7-104-0.bin --exe SkyrimSE.exe
```

Checks the format, the checksum, the sort order, and with `--exe` that the header matches that executable and
every RVA lies inside it. Add the same `--csv`, `--rva-col` and `--where` arguments as the build to prove the file
holds exactly those rows.

## Dump a table

```
python3 tools/mit-idtable/mit_idtable.py dump mit-idtable-1-7-104-0.bin
```

## Growing the table

Coverage grows by adding rows to a CSV (or adding a CSV) and building again. The id numbering is the AE column
of the Address Library (the second id in `RELOCATION_ID(se, ae)`), because 1.7.x continues it. Only add an id once
its RVA is proven on the exact executable, with the evidence kept next to the row. A wrong RVA in this file is worse
than a missing one: a missing id stops the game with its number, a wrong one runs the wrong code.
