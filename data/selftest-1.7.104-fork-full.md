## self-test: signature/xref resolver vs RTTI-exact vtable slot pairs (2527 AE slot functions, not used as anchors)

- UNIQUE-SIG sig:L1-body: right 28, WRONG 0, unresolved 0
- UNIQUE-SIG sig:L1-body(short): right 18, WRONG 0, unresolved 0
- UNIQUE-SIG sig:L1-body+block: right 536, WRONG 0, unresolved 0
- UNIQUE-SIG sig:L1-body+block+callee: right 247, WRONG 0, unresolved 0
- UNIQUE-SIG sig:L1-body+callee: right 20, WRONG 0, unresolved 0
- UNIQUE-SIG sig:prefix: right 34, WRONG 0, unresolved 0
- UNIQUE-SIG sig:shape: right 69, WRONG 0, unresolved 0
- UNRESOLVED: right 0, WRONG 0, unresolved 851
- XREF xref:block+shape: right 569, WRONG 0, unresolved 0
- XREF xref:graph: right 39, WRONG 0, unresolved 0
- XREF xref:identical-caller: right 14, WRONG 0, unresolved 0
- XREF xref:table: right 102, WRONG 0, unresolved 0

## self-test 2: 3000 L1-anchored functions held out, re-derived by xref methods only (own bytes unusable; held-out set removed from the anchors)

- UNRESOLVED: right 0, WRONG 0, unresolved 375
- XREF xref:block+shape: right 930, WRONG 0, unresolved 0
- XREF xref:fuzzy: right 6, WRONG 0, unresolved 0
- XREF xref:graph: right 321, WRONG 0, unresolved 0
- XREF xref:identical-caller: right 1214, WRONG 0, unresolved 0
- XREF xref:table: right 154, WRONG 0, unresolved 0

## self-test 5: leaf-getter (vtable-slot) route vs byte-alignment voting route, on globals both can resolve

- agree: 225
- getter route silent: 837

## self-test 6: block lock-step predictor on 20000 L1 anchors (flanks computed without the anchor itself)

- WRONG: 6
- no prediction: 3125
- right: 16869
  - WRONG AE 0x1293040 truth 0x12AFB00 predicted 0x12AFBF0
  - WRONG AE 0x10B590 truth 0x10EB50 predicted 0x10EB40
  - WRONG AE 0x1738BD0 truth 0x17B0A60 predicted 0x17B0A50
  - WRONG AE 0x56A1A0 truth 0x572020 predicted 0x572010
  - WRONG AE 0x12928F0 truth 0x12AF3E0 predicted 0x12AF4A0
  - WRONG AE 0x4EE440 truth 0x4F5F00 predicted 0x4F5F30

## self-test 5b: NiRTTI name-thunk route vs the independent byte-voting / getter-slot routes

- name-thunk agrees with getter-slot: 372
- name-thunk agrees with voting: 140

## self-test 7: data-block predictor + corroboration (same referencing-site count and identical referencing bodies) on 6000 globals, each hidden from its own flanks

- no prediction: 1042
- prediction rejected by corroboration: 1058
- right: 3900

## self-test 3: 2500 .rdata string globals, truth = identical string content (unique in both images), resolved by xref:global

- right 2460, WRONG 0, unresolved 40

## self-test 4: library-known RTTI/VTABLE ids re-derived from the fork symbol name alone (name:symbol binding vs library row)

- rtti nomatch/ambiguous: 145
- rtti right: 7176
- vtable WRONG: 2
- vtable nomatch/ambiguous: 214
- vtable right: 7801
  - id 246208 VTABLE_BSTDerivedCreator_MovementMessageFreezeDirection_MovementMessage_: library says 0x1A45480, name says 0x1A45440
  - id 246214 VTABLE_AutoRegisterCreator_MovementMessageFreezeDirection_BSTSmartPointerPathingFactoryManager_MovementMessage_64__: library says 0x1A45440, name says 0x1A45480
