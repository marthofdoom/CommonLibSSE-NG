# idmap 1.7.104 summary

ground truth: 107 pairs, HIT 104, MISMATCH 0, UNRESOLVED 3

| scope | EXACT | NAME | UNIQUE-SIG | XREF | UNRESOLVED | total |
|---|---|---|---|---|---|---|
| fork | 0 | 0 | 40 | 18 | 2 | 60 |
| ALL | 0 | 0 | 40 | 18 | 2 | 60 |

## final states (MAPPED / REMOVED / INLINED / HARD)

| category | total | MAPPED | REMOVED | INLINED | HARD |
|---|---|---|---|---|---|
| functions | 50 | 50 | 0 | 0 | 0 |
| functions/globals with no 1.6.1170 row | 2 | 0 | 0 | 0 | 2 |
| globals/data | 8 | 8 | 0 | 0 | 0 |
| ALL | 60 | 58 | 0 | 0 | 2 |

## mapped rows by category and method

| category | method | rows |
|---|---|---|
| functions | UNIQUE-SIG sig:L1-body | 28 |
| functions | UNIQUE-SIG sig:L1-body(short) | 3 |
| functions | UNIQUE-SIG sig:L1-body+block | 2 |
| functions | UNIQUE-SIG sig:L1-body+block+callee | 1 |
| functions | UNIQUE-SIG sig:L1-whole | 2 |
| functions | UNIQUE-SIG sig:prefix | 2 |
| functions | UNIQUE-SIG sig:shape | 2 |
| functions | XREF xref:identical-caller | 10 |
| globals/data | XREF xref:global | 8 |

## crosscheck tally (mapped rows only)
{'pass': 57, 'na': 1}

## layout flags (class hierarchy / slot layout changes seen on exact-RTTI matches)

## ground-truth mismatches (tool bugs to investigate)

## ground-truth unresolved
- AE 0xCD8F40 expect 0xCF95F0 [`Board.cpp:1607-1615` `PollInputDevices` `RelocationID(67315] :: masked head scan: AE hits 1, 1.7 hits 0 | no unique signature and xref anchoring too weak (46 insn)
- AE 0xCD9E00 expect 0xCFA650 [PollInputDevices +0x7B target (row 61)] :: masked head scan: AE hits 3, 1.7 hits 3 | no unique signature and xref anchoring too weak (199 insn)
- AE 0xCDACA0 expect 0xCFBA10 [PollInputDevices +0x87 target (row 61)] :: masked head scan: AE hits 1, 1.7 hits 0 | no unique signature and xref anchoring too weak (29 insn)

## HARD ids (handed back)
- 82317 id fork :: RELOCATION_ID include/RE/B/BSScaleformExternalTexture.h:37 :: tried: (1) present in Address Library 1.6.317,1.6.318,1.6.323,1.6.342,1.6.353,1.6.629,1.6.640,1.6.659, absent in 1.6.1130,1.6.1170,1.6.1179: a retired id, so there is no 1.6.1170 RVA to start the 1.6.1170->1.7.104 ladder from; (2) the fork SE column id is present in the 1.6.1170 library but maps to unrelated code (149 fork pairs with both ids present never share an RVA); (3) no RTTI, string or vtable tie names the function; (4) older libraries give 1.6.317-659 RVAs of binaries we do not hold. Fo
- 16118 id fork :: RELOCATION_ID src/RE/I/InventoryChanges.cpp:94 :: tried: (1) present in Address Library 1.6.317,1.6.318,1.6.323,1.6.342,1.6.353,1.6.629,1.6.640,1.6.659, absent in 1.6.1130,1.6.1170,1.6.1179: a retired id, so there is no 1.6.1170 RVA to start the 1.6.1170->1.7.104 ladder from; (2) the fork SE column id is present in the 1.6.1170 library but maps to unrelated code (149 fork pairs with both ids present never share an RVA); (3) no RTTI, string or vtable tie names the function; (4) older libraries give 1.6.317-659 RVAs of binaries we do not hold. Fo

## INLINED ids

## REMOVED ids by reason
