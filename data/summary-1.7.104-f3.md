# idmap 1.7.104 summary

ground truth: 107 pairs, HIT 104, MISMATCH 0, UNRESOLVED 3

| scope | EXACT | NAME | UNIQUE-SIG | XREF | UNRESOLVED | total |
|---|---|---|---|---|---|---|
| ours | 0 | 0 | 18 | 1 | 0 | 19 |
| ALL | 0 | 0 | 18 | 1 | 0 | 19 |

## final states (MAPPED / REMOVED / INLINED / HARD)

| category | total | MAPPED | REMOVED | INLINED | HARD |
|---|---|---|---|---|---|
| functions | 19 | 19 | 0 | 0 | 0 |
| ALL | 19 | 19 | 0 | 0 | 0 |

## mapped rows by category and method

| category | method | rows |
|---|---|---|
| functions | UNIQUE-SIG sig:L1-body | 15 |
| functions | UNIQUE-SIG sig:L1-body(short) | 1 |
| functions | UNIQUE-SIG sig:L1-body+block | 1 |
| functions | UNIQUE-SIG sig:shape | 1 |
| functions | XREF xref:identical-caller | 1 |

## crosscheck tally (mapped rows only)
{'pass': 19}

## layout flags (class hierarchy / slot layout changes seen on exact-RTTI matches)

## ground-truth mismatches (tool bugs to investigate)

## ground-truth unresolved
- AE 0xCD8F40 expect 0xCF95F0 [`Board.cpp:1607-1615` `PollInputDevices` `RelocationID(67315] :: masked head scan: AE hits 1, 1.7 hits 0 | no unique signature and xref anchoring too weak (46 insn)
- AE 0xCD9E00 expect 0xCFA650 [PollInputDevices +0x7B target (row 61)] :: masked head scan: AE hits 3, 1.7 hits 3 | no unique signature and xref anchoring too weak (199 insn)
- AE 0xCDACA0 expect 0xCFBA10 [PollInputDevices +0x87 target (row 61)] :: masked head scan: AE hits 1, 1.7 hits 0 | no unique signature and xref anchoring too weak (29 insn)

## HARD ids (handed back)

## INLINED ids

## REMOVED ids by reason
