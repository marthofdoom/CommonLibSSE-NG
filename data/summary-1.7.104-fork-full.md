# idmap 1.7.104 summary

ground truth: 107 pairs, HIT 107, MISMATCH 0, UNRESOLVED 0

| scope | EXACT | NAME | UNIQUE-SIG | XREF | UNRESOLVED | total |
|---|---|---|---|---|---|---|
| cl+fork | 0 | 0 | 86 | 32 | 1 | 119 |
| cl+fork+ours | 0 | 0 | 12 | 1 | 0 | 13 |
| fork | 15645 | 25 | 385 | 127 | 1252 | 17434 |
| fork+infra | 1 | 0 | 7 | 2 | 0 | 10 |
| fork+ours | 101 | 0 | 0 | 0 | 0 | 101 |
| ours | 77 | 0 | 42 | 8 | 0 | 127 |
| ALL | 15824 | 25 | 532 | 170 | 1253 | 17804 |

## final states (MAPPED / REMOVED / INLINED / HARD)

| category | total | MAPPED | REMOVED | INLINED | HARD |
|---|---|---|---|---|---|
| NiRTTI objects (.data globals) | 410 | 410 | 0 | 0 | 0 |
| RTTI TypeDescriptors | 7907 | 7335 | 572 | 0 | 0 |
| functions | 587 | 587 | 0 | 0 | 0 |
| functions/globals with no 1.6.1170 row | 5 | 0 | 0 | 1 | 4 |
| globals/data | 116 | 116 | 0 | 0 | 0 |
| vtables | 8779 | 8103 | 675 | 0 | 1 |
| ALL | 17804 | 16551 | 1247 | 1 | 5 |

## mapped rows by category and method

| category | method | rows |
|---|---|---|
| NiRTTI objects (.data globals) | EXACT nirtti:name-thunk | 410 |
| RTTI TypeDescriptors | EXACT rtti:typedescriptor | 6880 |
| RTTI TypeDescriptors | EXACT rtti:typedescriptor(anon) | 440 |
| RTTI TypeDescriptors | NAME name:symbol | 15 |
| functions | EXACT import:name | 1 |
| functions | UNIQUE-SIG sig:L1-body | 415 |
| functions | UNIQUE-SIG sig:L1-body(short) | 26 |
| functions | UNIQUE-SIG sig:L1-body+block | 27 |
| functions | UNIQUE-SIG sig:L1-body+block+callee | 14 |
| functions | UNIQUE-SIG sig:L1-body+callee | 2 |
| functions | UNIQUE-SIG sig:L1-body+slot | 2 |
| functions | UNIQUE-SIG sig:L1-whole | 4 |
| functions | UNIQUE-SIG sig:prefix | 9 |
| functions | UNIQUE-SIG sig:shape | 33 |
| functions | XREF thunk | 1 |
| functions | XREF xref:block+shape | 5 |
| functions | XREF xref:graph | 4 |
| functions | XREF xref:identical-caller | 44 |
| globals/data | XREF xref:data-block | 4 |
| globals/data | XREF xref:global | 106 |
| globals/data | XREF xref:global+block | 6 |
| vtables | EXACT rtti:vtable | 8093 |
| vtables | NAME name:symbol+rtti:vtable | 10 |

## crosscheck tally (mapped rows only)
{'pass': 9209, 'na': 7342}

## layout flags (class hierarchy / slot layout changes seen on exact-RTTI matches)
- 205234 VTABLE_PlayerInputHandler[0] include/RE/Offsets_VTABLE.h:177 0x18ECBC8 :: LAYOUT CHANGE: slot count AE 5 vs 1.7 7; class hierarchy identical (1 bases)
- 205238 VTABLE_ThirdPersonState[1] include/RE/Offsets_VTABLE.h:1780 0x18ECC90 :: LAYOUT CHANGE: slot count AE 5 vs 1.7 7; anchored slots AE->1.7: 1->1 4->6; class hierarchy identical (4 bases)
- 205242 VTABLE_DragonCameraState[1] include/RE/Offsets_VTABLE.h:1781 0x18ECD58 :: LAYOUT CHANGE: slot count AE 5 vs 1.7 7; anchored slots AE->1.7: 1->1; class hierarchy identical (5 bases)
- 205371 VTABLE_MenuEventHandler[0] include/RE/Offsets_VTABLE.h:1799 0x18F02E0 :: LAYOUT CHANGE: slot count AE 6 vs 1.7 8; class hierarchy identical (2 bases)
- 205640 VTABLE_ModManagerMenu[1] include/RE/Offsets_VTABLE.h:1827 0x18F4680 :: LAYOUT CHANGE: slot count AE 6 vs 1.7 8; anchored slots AE->1.7: 3->5; class hierarchy identical (14 bases)
- 208040 VTABLE_PlayerCharacter[0] include/RE/Offsets_VTABLE.h:2231 0x19296C0 :: LAYOUT CHANGE: 1.7.104 adds base(s) ['.?AV?$BSTEventSink@VBSSystemEvent@@@@'] to the class hierarchy (25 -> 26 bases); subobject offsets/sizeof may shift, vtable identity by base name is unaffected
- 208042 VTABLE_PlayerCharacter[1] include/RE/Offsets_VTABLE.h:2231 0x192A040 :: LAYOUT CHANGE: 1.7.104 adds base(s) ['.?AV?$BSTEventSink@VBSSystemEvent@@@@'] to the class hierarchy (25 -> 26 bases); subobject offsets/sizeof may shift, vtable identity by base name is unaffected
- 208044 VTABLE_PlayerCharacter[2] include/RE/Offsets_VTABLE.h:2231 0x192A058 :: LAYOUT CHANGE: 1.7.104 adds base(s) ['.?AV?$BSTEventSink@VBSSystemEvent@@@@'] to the class hierarchy (25 -> 26 bases); subobject offsets/sizeof may shift, vtable identity by base name is unaffected
- 208046 VTABLE_PlayerCharacter[3] include/RE/Offsets_VTABLE.h:2231 0x192A070 :: LAYOUT CHANGE: 1.7.104 adds base(s) ['.?AV?$BSTEventSink@VBSSystemEvent@@@@'] to the class hierarchy (25 -> 26 bases); subobject offsets/sizeof may shift, vtable identity by base name is unaffected
- 208048 VTABLE_PlayerCharacter[4] include/RE/Offsets_VTABLE.h:2231 0x192A110 :: LAYOUT CHANGE: 1.7.104 adds base(s) ['.?AV?$BSTEventSink@VBSSystemEvent@@@@'] to the class hierarchy (25 -> 26 bases); subobject offsets/sizeof may shift, vtable identity by base name is unaffected
- 208050 VTABLE_PlayerCharacter[5] include/RE/Offsets_VTABLE.h:2231 0x192A178 :: LAYOUT CHANGE: 1.7.104 adds base(s) ['.?AV?$BSTEventSink@VBSSystemEvent@@@@'] to the class hierarchy (25 -> 26 bases); subobject offsets/sizeof may shift, vtable identity by base name is unaffected
- 208052 VTABLE_PlayerCharacter[6] include/RE/Offsets_VTABLE.h:2231 0x192A1C8 :: LAYOUT CHANGE: 1.7.104 adds base(s) ['.?AV?$BSTEventSink@VBSSystemEvent@@@@'] to the class hierarchy (25 -> 26 bases); subobject offsets/sizeof may shift, vtable identity by base name is unaffected
- 208054 VTABLE_PlayerCharacter[7] include/RE/Offsets_VTABLE.h:2231 0x192A280 :: LAYOUT CHANGE: 1.7.104 adds base(s) ['.?AV?$BSTEventSink@VBSSystemEvent@@@@'] to the class hierarchy (25 -> 26 bases); subobject offsets/sizeof may shift, vtable identity by base name is unaffected
- 208056 VTABLE_PlayerCharacter[8] include/RE/Offsets_VTABLE.h:2231 0x192A298 :: LAYOUT CHANGE: 1.7.104 adds base(s) ['.?AV?$BSTEventSink@VBSSystemEvent@@@@'] to the class hierarchy (25 -> 26 bases); subobject offsets/sizeof may shift, vtable identity by base name is unaffected
- 208058 VTABLE_PlayerCharacter[9] include/RE/Offsets_VTABLE.h:2231 0x192A2B0 :: LAYOUT CHANGE: 1.7.104 adds base(s) ['.?AV?$BSTEventSink@VBSSystemEvent@@@@'] to the class hierarchy (25 -> 26 bases); subobject offsets/sizeof may shift, vtable identity by base name is unaffected
- 208060 VTABLE_PlayerCharacter[10] include/RE/Offsets_VTABLE.h:2231 0x192A2C8 :: LAYOUT CHANGE: 1.7.104 adds base(s) ['.?AV?$BSTEventSink@VBSSystemEvent@@@@'] to the class hierarchy (25 -> 26 bases); subobject offsets/sizeof may shift, vtable identity by base name is unaffected
- 208062 VTABLE_PlayerCharacter[11] include/RE/Offsets_VTABLE.h:2231 0x192A2E0 :: LAYOUT CHANGE: 1.7.104 adds base(s) ['.?AV?$BSTEventSink@VBSSystemEvent@@@@'] to the class hierarchy (25 -> 26 bases); subobject offsets/sizeof may shift, vtable identity by base name is unaffected
- 208064 VTABLE_PlayerCharacter[12] include/RE/Offsets_VTABLE.h:2231 0x192A2F8 :: LAYOUT CHANGE: 1.7.104 adds base(s) ['.?AV?$BSTEventSink@VBSSystemEvent@@@@'] to the class hierarchy (25 -> 26 bases); subobject offsets/sizeof may shift, vtable identity by base name is unaffected
- 208066 VTABLE_PlayerCharacter[13] include/RE/Offsets_VTABLE.h:2231 0x192A310 :: LAYOUT CHANGE: 1.7.104 adds base(s) ['.?AV?$BSTEventSink@VBSSystemEvent@@@@'] to the class hierarchy (25 -> 26 bases); subobject offsets/sizeof may shift, vtable identity by base name is unaffected
- 208684 VTABLE_HeldStateHandler[0] include/RE/Offsets_VTABLE.h:2346 0x1935008 :: LAYOUT CHANGE: slot count AE 7 vs 1.7 9; class hierarchy identical (2 bases)
- 208710 VTABLE_LookHandler[0] include/RE/Offsets_VTABLE.h:2352 0x1935150 :: LAYOUT CHANGE: slot count AE 5 vs 1.7 7; class hierarchy identical (2 bases)
- 208715 VTABLE_MovementHandler[0] include/RE/Offsets_VTABLE.h:2353 0x19351C8 :: LAYOUT CHANGE: slot count AE 5 vs 1.7 7; anchored slots AE->1.7: 1->1 4->6; class hierarchy identical (2 bases)
- 208717 VTABLE_SprintHandler[0] include/RE/Offsets_VTABLE.h:2354 0x1935208 :: LAYOUT CHANGE: slot count AE 7 vs 1.7 9; anchored slots AE->1.7: 1->1; class hierarchy identical (3 bases)
- 208719 VTABLE_AttackBlockHandler[0] include/RE/Offsets_VTABLE.h:235 0x1935258 :: LAYOUT CHANGE: slot count AE 7 vs 1.7 9; anchored slots AE->1.7: 1->1; class hierarchy identical (3 bases)
- 208721 VTABLE_ReadyWeaponHandler[0] include/RE/Offsets_VTABLE.h:235 0x1935348 :: LAYOUT CHANGE: slot count AE 5 vs 1.7 7; anchored slots AE->1.7: 1->1; class hierarchy identical (2 bases)
- 208723 VTABLE_ActivateHandler[0] include/RE/Offsets_VTABLE.h:2357 0x1935388 :: LAYOUT CHANGE: slot count AE 7 vs 1.7 9; anchored slots AE->1.7: 1->1; class hierarchy identical (3 bases)
- 208725 VTABLE_AutoMoveHandler[0] include/RE/Offsets_VTABLE.h:2358 0x19353D8 :: LAYOUT CHANGE: slot count AE 5 vs 1.7 7; anchored slots AE->1.7: 1->1; class hierarchy identical (2 bases)
- 208727 VTABLE_ToggleRunHandler[0] include/RE/Offsets_VTABLE.h:2359 0x1935418 :: LAYOUT CHANGE: slot count AE 5 vs 1.7 7; anchored slots AE->1.7: 1->1; class hierarchy identical (2 bases)
- 208729 VTABLE_RunHandler[0] include/RE/Offsets_VTABLE.h:2360 0x1935458 :: LAYOUT CHANGE: slot count AE 7 vs 1.7 9; anchored slots AE->1.7: 1->1; class hierarchy identical (3 bases)
- 208731 VTABLE_JumpHandler[0] include/RE/Offsets_VTABLE.h:2361 0x19354A8 :: LAYOUT CHANGE: slot count AE 5 vs 1.7 7; anchored slots AE->1.7: 1->1; class hierarchy identical (2 bases)
- 208733 VTABLE_SneakHandler[0] include/RE/Offsets_VTABLE.h:2362 0x19354E8 :: LAYOUT CHANGE: slot count AE 5 vs 1.7 7; anchored slots AE->1.7: 1->1; class hierarchy identical (2 bases)
- 208735 VTABLE_ShoutHandler[0] include/RE/Offsets_VTABLE.h:2363 0x1935528 :: LAYOUT CHANGE: slot count AE 5 vs 1.7 7; anchored slots AE->1.7: 1->1 4->6; class hierarchy identical (2 bases)
- 208737 VTABLE_TogglePOVHandler[0] include/RE/Offsets_VTABLE.h:2364 0x1935568 :: LAYOUT CHANGE: slot count AE 7 vs 1.7 9; anchored slots AE->1.7: 1->1 4->6; class hierarchy identical (3 bases)
- 214839 VTABLE_HorseCameraState[1] include/RE/Offsets_VTABLE.h:3983 0x196CED0 :: LAYOUT CHANGE: slot count AE 5 vs 1.7 7; anchored slots AE->1.7: 1->1; class hierarchy identical (5 bases)
- 214857 VTABLE_FirstPersonState[1] include/RE/Offsets_VTABLE.h:3984 0x196D940 :: LAYOUT CHANGE: slot count AE 5 vs 1.7 7; anchored slots AE->1.7: 1->1; class hierarchy identical (4 bases)
- 214865 VTABLE_FreeCameraState[1] include/RE/Offsets_VTABLE.h:3987 0x196DA40 :: LAYOUT CHANGE: slot count AE 5 vs 1.7 7; anchored slots AE->1.7: 1->1 4->6; class hierarchy identical (4 bases)
- 214875 VTABLE_BleedoutCameraState[1] include/RE/Offsets_VTABLE.h:39 0x196DBF8 :: LAYOUT CHANGE: slot count AE 5 vs 1.7 7; anchored slots AE->1.7: 1->1 4->6; class hierarchy identical (5 bases)
- 215248 VTABLE_CursorMenu[1] include/RE/Offsets_VTABLE.h:4032 0x19745D8 :: LAYOUT CHANGE: slot count AE 6 vs 1.7 8; anchored slots AE->1.7: 1->1; class hierarchy identical (9 bases)
- 215309 VTABLE_FavoritesMenu[1] include/RE/Offsets_VTABLE.h:4035 0x19751D8 :: LAYOUT CHANGE: slot count AE 6 vs 1.7 8; anchored slots AE->1.7: 1->1 2->4 5->7; class hierarchy identical (9 bases)
- 215473 VTABLE_Inventory3DManager[0] include/RE/Offsets_VTABLE.h:405 0x19778D8 :: LAYOUT CHANGE: slot count AE 6 vs 1.7 8; anchored slots AE->1.7: 0->0 1->1 3->5 4->6; class hierarchy identical (7 bases)
- 215606 VTABLE_LockpickingMenu[1] include/RE/Offsets_VTABLE.h:4060 0x1979A60 :: LAYOUT CHANGE: slot count AE 6 vs 1.7 8; class hierarchy identical (10 bases)
- 215777 VTABLE_ClickHandler[0] include/RE/Offsets_VTABLE.h:4078 0x197C278 :: LAYOUT CHANGE: slot count AE 6 vs 1.7 8; anchored slots AE->1.7: 1->1 5->7; class hierarchy identical (3 bases)
- 215779 VTABLE_DirectionHandler[0] include/RE/Offsets_VTABLE.h:4079 0x197C2C0 :: LAYOUT CHANGE: slot count AE 6 vs 1.7 8; anchored slots AE->1.7: 1->1 3->5 5->7; class hierarchy identical (3 bases)
- 215781 VTABLE_ConsoleOpenHandler[0] include/RE/Offsets_VTABLE.h:408 0x197C308 :: LAYOUT CHANGE: slot count AE 6 vs 1.7 8; anchored slots AE->1.7: 1->1 5->7; class hierarchy identical (3 bases)
- 215783 VTABLE_MenuOpenHandler[0] include/RE/Offsets_VTABLE.h:4081 0x197C350 :: LAYOUT CHANGE: slot count AE 6 vs 1.7 8; anchored slots AE->1.7: 1->1 2->4; class hierarchy identical (3 bases)
- 215785 VTABLE_FavoritesHandler[0] include/RE/Offsets_VTABLE.h:4082 0x197C398 :: LAYOUT CHANGE: slot count AE 6 vs 1.7 8; anchored slots AE->1.7: 1->1 2->4; class hierarchy identical (3 bases)
- 215787 VTABLE_ScreenshotHandler[0] include/RE/Offsets_VTABLE.h:4083 0x197C3E0 :: LAYOUT CHANGE: slot count AE 6 vs 1.7 8; anchored slots AE->1.7: 1->1 5->7; class hierarchy identical (3 bases)
- 215789 VTABLE_QuickSaveLoadHandler[0] include/RE/Offsets_VTABLE.h:4 0x197C428 :: LAYOUT CHANGE: slot count AE 6 vs 1.7 8; anchored slots AE->1.7: 1->1 5->7; class hierarchy identical (3 bases)
- 215852 VTABLE_MistMenu[2] include/RE/Offsets_VTABLE.h:4087 0x197DE98 :: LAYOUT CHANGE: slot count AE 6 vs 1.7 8; anchored slots AE->1.7: 3->5 4->6; class hierarchy identical (11 bases)
- 215887 VTABLE_RaceSexMenu[1] include/RE/Offsets_VTABLE.h:4089 0x197EE00 :: LAYOUT CHANGE: slot count AE 6 vs 1.7 8; anchored slots AE->1.7: 1->1 3->5 4->6; class hierarchy identical (9 bases)
- 215975 VTABLE_StatsMenu[1] include/RE/Offsets_VTABLE.h:4097 0x1980178 :: LAYOUT CHANGE: slot count AE 6 vs 1.7 8; anchored slots AE->1.7: 1->1 3->5 5->7; class hierarchy identical (9 bases)
- 216412 VTABLE_LocalMapMenu__InputHandler[0] include/RE/Offsets_VTAB 0x1985B70 :: LAYOUT CHANGE: slot count AE 6 vs 1.7 8; anchored slots AE->1.7: 1->1 3->5 4->6 5->7; class hierarchy identical (3 bases)
- 216489 VTABLE_MapInputHandler[0] include/RE/Offsets_VTABLE.h:4168 0x1987128 :: LAYOUT CHANGE: slot count AE 6 vs 1.7 8; class hierarchy identical (3 bases)
- 216496 VTABLE_MapLookHandler[0] include/RE/Offsets_VTABLE.h:4169 0x1987170 :: LAYOUT CHANGE: slot count AE 6 vs 1.7 8; anchored slots AE->1.7: 1->1 3->5 4->6 5->7; class hierarchy identical (4 bases)
- 216498 VTABLE_MapMoveHandler[0] include/RE/Offsets_VTABLE.h:4170 0x19871B8 :: LAYOUT CHANGE: slot count AE 6 vs 1.7 8; anchored slots AE->1.7: 1->1; class hierarchy identical (4 bases)
- 216500 VTABLE_MapZoomHandler[0] include/RE/Offsets_VTABLE.h:4171 0x1987200 :: LAYOUT CHANGE: slot count AE 6 vs 1.7 8; anchored slots AE->1.7: 1->1 5->7; class hierarchy identical (4 bases)
- 216687 VTABLE_JournalMenu[1] include/RE/Offsets_VTABLE.h:4193 0x1989DE8 :: LAYOUT CHANGE: slot count AE 6 vs 1.7 8; anchored slots AE->1.7: 1->1; class hierarchy identical (10 bases)
- 217055 VTABLE_SkyrimVM[0] include/RE/Offsets_VTABLE.h:4307 0x19915B0 :: LAYOUT CHANGE: 1.7.104 adds base(s) ['.?AV?$BSTEventSink@UTESAmiiboTouchEvent@@@@', '.?AV?$BSTEventSink@UTESAmiiboForcedStopDetectionEvent@@@@'] to the class hierarchy (58 -> 60 bases); subobject offsets/sizeof may shift, vtable identity by base name is unaffe
- 217057 VTABLE_SkyrimVM[1] include/RE/Offsets_VTABLE.h:4307 0x19915C8 :: LAYOUT CHANGE: 1.7.104 adds base(s) ['.?AV?$BSTEventSink@UTESAmiiboTouchEvent@@@@', '.?AV?$BSTEventSink@UTESAmiiboForcedStopDetectionEvent@@@@'] to the class hierarchy (58 -> 60 bases); subobject offsets/sizeof may shift, vtable identity by base name is unaffe
- 217059 VTABLE_SkyrimVM[2] include/RE/Offsets_VTABLE.h:4307 0x19915E8 :: LAYOUT CHANGE: 1.7.104 adds base(s) ['.?AV?$BSTEventSink@UTESAmiiboTouchEvent@@@@', '.?AV?$BSTEventSink@UTESAmiiboForcedStopDetectionEvent@@@@'] to the class hierarchy (58 -> 60 bases); subobject offsets/sizeof may shift, vtable identity by base name is unaffe
- 217061 VTABLE_SkyrimVM[3] include/RE/Offsets_VTABLE.h:4307 0x1991600 :: LAYOUT CHANGE: 1.7.104 adds base(s) ['.?AV?$BSTEventSink@UTESAmiiboTouchEvent@@@@', '.?AV?$BSTEventSink@UTESAmiiboForcedStopDetectionEvent@@@@'] to the class hierarchy (58 -> 60 bases); subobject offsets/sizeof may shift, vtable identity by base name is unaffe
- 217063 VTABLE_SkyrimVM[4] include/RE/Offsets_VTABLE.h:4307 0x1991618 :: LAYOUT CHANGE: 1.7.104 adds base(s) ['.?AV?$BSTEventSink@UTESAmiiboTouchEvent@@@@', '.?AV?$BSTEventSink@UTESAmiiboForcedStopDetectionEvent@@@@'] to the class hierarchy (58 -> 60 bases); subobject offsets/sizeof may shift, vtable identity by base name is unaffe
- 217065 VTABLE_SkyrimVM[5] include/RE/Offsets_VTABLE.h:4307 0x1991630 :: LAYOUT CHANGE: 1.7.104 adds base(s) ['.?AV?$BSTEventSink@UTESAmiiboTouchEvent@@@@', '.?AV?$BSTEventSink@UTESAmiiboForcedStopDetectionEvent@@@@'] to the class hierarchy (58 -> 60 bases); subobject offsets/sizeof may shift, vtable identity by base name is unaffe
- 217067 VTABLE_SkyrimVM[6] include/RE/Offsets_VTABLE.h:4307 0x1991648 :: LAYOUT CHANGE: 1.7.104 adds base(s) ['.?AV?$BSTEventSink@UTESAmiiboTouchEvent@@@@', '.?AV?$BSTEventSink@UTESAmiiboForcedStopDetectionEvent@@@@'] to the class hierarchy (58 -> 60 bases); subobject offsets/sizeof may shift, vtable identity by base name is unaffe
- 217069 VTABLE_SkyrimVM[7] include/RE/Offsets_VTABLE.h:4307 0x1991660 :: LAYOUT CHANGE: 1.7.104 adds base(s) ['.?AV?$BSTEventSink@UTESAmiiboTouchEvent@@@@', '.?AV?$BSTEventSink@UTESAmiiboForcedStopDetectionEvent@@@@'] to the class hierarchy (58 -> 60 bases); subobject offsets/sizeof may shift, vtable identity by base name is unaffe
- 217071 VTABLE_SkyrimVM[8] include/RE/Offsets_VTABLE.h:4307 0x1991678 :: LAYOUT CHANGE: 1.7.104 adds base(s) ['.?AV?$BSTEventSink@UTESAmiiboTouchEvent@@@@', '.?AV?$BSTEventSink@UTESAmiiboForcedStopDetectionEvent@@@@'] to the class hierarchy (58 -> 60 bases); subobject offsets/sizeof may shift, vtable identity by base name is unaffe
- 217073 VTABLE_SkyrimVM[9] include/RE/Offsets_VTABLE.h:4307 0x1991690 :: LAYOUT CHANGE: 1.7.104 adds base(s) ['.?AV?$BSTEventSink@UTESAmiiboTouchEvent@@@@', '.?AV?$BSTEventSink@UTESAmiiboForcedStopDetectionEvent@@@@'] to the class hierarchy (58 -> 60 bases); subobject offsets/sizeof may shift, vtable identity by base name is unaffe
- 217075 VTABLE_SkyrimVM[10] include/RE/Offsets_VTABLE.h:4307 0x19916A8 :: LAYOUT CHANGE: 1.7.104 adds base(s) ['.?AV?$BSTEventSink@UTESAmiiboTouchEvent@@@@', '.?AV?$BSTEventSink@UTESAmiiboForcedStopDetectionEvent@@@@'] to the class hierarchy (58 -> 60 bases); subobject offsets/sizeof may shift, vtable identity by base name is unaffe
- 217077 VTABLE_SkyrimVM[11] include/RE/Offsets_VTABLE.h:4307 0x19916C0 :: LAYOUT CHANGE: 1.7.104 adds base(s) ['.?AV?$BSTEventSink@UTESAmiiboTouchEvent@@@@', '.?AV?$BSTEventSink@UTESAmiiboForcedStopDetectionEvent@@@@'] to the class hierarchy (58 -> 60 bases); subobject offsets/sizeof may shift, vtable identity by base name is unaffe
- 217079 VTABLE_SkyrimVM[12] include/RE/Offsets_VTABLE.h:4307 0x19916D8 :: LAYOUT CHANGE: 1.7.104 adds base(s) ['.?AV?$BSTEventSink@UTESAmiiboTouchEvent@@@@', '.?AV?$BSTEventSink@UTESAmiiboForcedStopDetectionEvent@@@@'] to the class hierarchy (58 -> 60 bases); subobject offsets/sizeof may shift, vtable identity by base name is unaffe
- 217081 VTABLE_SkyrimVM[13] include/RE/Offsets_VTABLE.h:4307 0x19916F0 :: LAYOUT CHANGE: 1.7.104 adds base(s) ['.?AV?$BSTEventSink@UTESAmiiboTouchEvent@@@@', '.?AV?$BSTEventSink@UTESAmiiboForcedStopDetectionEvent@@@@'] to the class hierarchy (58 -> 60 bases); subobject offsets/sizeof may shift, vtable identity by base name is unaffe
- 217083 VTABLE_SkyrimVM[14] include/RE/Offsets_VTABLE.h:4307 0x1991708 :: LAYOUT CHANGE: 1.7.104 adds base(s) ['.?AV?$BSTEventSink@UTESAmiiboTouchEvent@@@@', '.?AV?$BSTEventSink@UTESAmiiboForcedStopDetectionEvent@@@@'] to the class hierarchy (58 -> 60 bases); subobject offsets/sizeof may shift, vtable identity by base name is unaffe
- 217085 VTABLE_SkyrimVM[15] include/RE/Offsets_VTABLE.h:4307 0x1991720 :: LAYOUT CHANGE: 1.7.104 adds base(s) ['.?AV?$BSTEventSink@UTESAmiiboTouchEvent@@@@', '.?AV?$BSTEventSink@UTESAmiiboForcedStopDetectionEvent@@@@'] to the class hierarchy (58 -> 60 bases); subobject offsets/sizeof may shift, vtable identity by base name is unaffe
- 217087 VTABLE_SkyrimVM[16] include/RE/Offsets_VTABLE.h:4307 0x1991738 :: LAYOUT CHANGE: 1.7.104 adds base(s) ['.?AV?$BSTEventSink@UTESAmiiboTouchEvent@@@@', '.?AV?$BSTEventSink@UTESAmiiboForcedStopDetectionEvent@@@@'] to the class hierarchy (58 -> 60 bases); subobject offsets/sizeof may shift, vtable identity by base name is unaffe
- 217089 VTABLE_SkyrimVM[17] include/RE/Offsets_VTABLE.h:4307 0x1991750 :: LAYOUT CHANGE: 1.7.104 adds base(s) ['.?AV?$BSTEventSink@UTESAmiiboTouchEvent@@@@', '.?AV?$BSTEventSink@UTESAmiiboForcedStopDetectionEvent@@@@'] to the class hierarchy (58 -> 60 bases); subobject offsets/sizeof may shift, vtable identity by base name is unaffe
- 217091 VTABLE_SkyrimVM[18] include/RE/Offsets_VTABLE.h:4307 0x1991768 :: LAYOUT CHANGE: 1.7.104 adds base(s) ['.?AV?$BSTEventSink@UTESAmiiboTouchEvent@@@@', '.?AV?$BSTEventSink@UTESAmiiboForcedStopDetectionEvent@@@@'] to the class hierarchy (58 -> 60 bases); subobject offsets/sizeof may shift, vtable identity by base name is unaffe
- 217093 VTABLE_SkyrimVM[19] include/RE/Offsets_VTABLE.h:4307 0x1991780 :: LAYOUT CHANGE: 1.7.104 adds base(s) ['.?AV?$BSTEventSink@UTESAmiiboTouchEvent@@@@', '.?AV?$BSTEventSink@UTESAmiiboForcedStopDetectionEvent@@@@'] to the class hierarchy (58 -> 60 bases); subobject offsets/sizeof may shift, vtable identity by base name is unaffe
- 217095 VTABLE_SkyrimVM[20] include/RE/Offsets_VTABLE.h:4307 0x1991798 :: LAYOUT CHANGE: 1.7.104 adds base(s) ['.?AV?$BSTEventSink@UTESAmiiboTouchEvent@@@@', '.?AV?$BSTEventSink@UTESAmiiboForcedStopDetectionEvent@@@@'] to the class hierarchy (58 -> 60 bases); subobject offsets/sizeof may shift, vtable identity by base name is unaffe
- 217097 VTABLE_SkyrimVM[21] include/RE/Offsets_VTABLE.h:4307 0x19917B0 :: LAYOUT CHANGE: 1.7.104 adds base(s) ['.?AV?$BSTEventSink@UTESAmiiboTouchEvent@@@@', '.?AV?$BSTEventSink@UTESAmiiboForcedStopDetectionEvent@@@@'] to the class hierarchy (58 -> 60 bases); subobject offsets/sizeof may shift, vtable identity by base name is unaffe
- 217099 VTABLE_SkyrimVM[22] include/RE/Offsets_VTABLE.h:4307 0x19917C8 :: LAYOUT CHANGE: 1.7.104 adds base(s) ['.?AV?$BSTEventSink@UTESAmiiboTouchEvent@@@@', '.?AV?$BSTEventSink@UTESAmiiboForcedStopDetectionEvent@@@@'] to the class hierarchy (58 -> 60 bases); subobject offsets/sizeof may shift, vtable identity by base name is unaffe
- 217101 VTABLE_SkyrimVM[23] include/RE/Offsets_VTABLE.h:4307 0x19917E0 :: LAYOUT CHANGE: 1.7.104 adds base(s) ['.?AV?$BSTEventSink@UTESAmiiboTouchEvent@@@@', '.?AV?$BSTEventSink@UTESAmiiboForcedStopDetectionEvent@@@@'] to the class hierarchy (58 -> 60 bases); subobject offsets/sizeof may shift, vtable identity by base name is unaffe
- 217103 VTABLE_SkyrimVM[24] include/RE/Offsets_VTABLE.h:4307 0x19917F8 :: LAYOUT CHANGE: 1.7.104 adds base(s) ['.?AV?$BSTEventSink@UTESAmiiboTouchEvent@@@@', '.?AV?$BSTEventSink@UTESAmiiboForcedStopDetectionEvent@@@@'] to the class hierarchy (58 -> 60 bases); subobject offsets/sizeof may shift, vtable identity by base name is unaffe
- 217105 VTABLE_SkyrimVM[25] include/RE/Offsets_VTABLE.h:4307 0x1991810 :: LAYOUT CHANGE: 1.7.104 adds base(s) ['.?AV?$BSTEventSink@UTESAmiiboTouchEvent@@@@', '.?AV?$BSTEventSink@UTESAmiiboForcedStopDetectionEvent@@@@'] to the class hierarchy (58 -> 60 bases); subobject offsets/sizeof may shift, vtable identity by base name is unaffe
- 217107 VTABLE_SkyrimVM[26] include/RE/Offsets_VTABLE.h:4307 0x1991828 :: LAYOUT CHANGE: 1.7.104 adds base(s) ['.?AV?$BSTEventSink@UTESAmiiboTouchEvent@@@@', '.?AV?$BSTEventSink@UTESAmiiboForcedStopDetectionEvent@@@@'] to the class hierarchy (58 -> 60 bases); subobject offsets/sizeof may shift, vtable identity by base name is unaffe
- 217109 VTABLE_SkyrimVM[27] include/RE/Offsets_VTABLE.h:4307 0x1991840 :: LAYOUT CHANGE: 1.7.104 adds base(s) ['.?AV?$BSTEventSink@UTESAmiiboTouchEvent@@@@', '.?AV?$BSTEventSink@UTESAmiiboForcedStopDetectionEvent@@@@'] to the class hierarchy (58 -> 60 bases); subobject offsets/sizeof may shift, vtable identity by base name is unaffe
- 217111 VTABLE_SkyrimVM[28] include/RE/Offsets_VTABLE.h:4307 0x1991858 :: LAYOUT CHANGE: 1.7.104 adds base(s) ['.?AV?$BSTEventSink@UTESAmiiboTouchEvent@@@@', '.?AV?$BSTEventSink@UTESAmiiboForcedStopDetectionEvent@@@@'] to the class hierarchy (58 -> 60 bases); subobject offsets/sizeof may shift, vtable identity by base name is unaffe
- 217113 VTABLE_SkyrimVM[29] include/RE/Offsets_VTABLE.h:4307 0x1991870 :: LAYOUT CHANGE: 1.7.104 adds base(s) ['.?AV?$BSTEventSink@UTESAmiiboTouchEvent@@@@', '.?AV?$BSTEventSink@UTESAmiiboForcedStopDetectionEvent@@@@'] to the class hierarchy (58 -> 60 bases); subobject offsets/sizeof may shift, vtable identity by base name is unaffe
- 217115 VTABLE_SkyrimVM[30] include/RE/Offsets_VTABLE.h:4307 0x1991888 :: LAYOUT CHANGE: 1.7.104 adds base(s) ['.?AV?$BSTEventSink@UTESAmiiboTouchEvent@@@@', '.?AV?$BSTEventSink@UTESAmiiboForcedStopDetectionEvent@@@@'] to the class hierarchy (58 -> 60 bases); subobject offsets/sizeof may shift, vtable identity by base name is unaffe
- 217117 VTABLE_SkyrimVM[31] include/RE/Offsets_VTABLE.h:4307 0x19918A0 :: LAYOUT CHANGE: 1.7.104 adds base(s) ['.?AV?$BSTEventSink@UTESAmiiboTouchEvent@@@@', '.?AV?$BSTEventSink@UTESAmiiboForcedStopDetectionEvent@@@@'] to the class hierarchy (58 -> 60 bases); subobject offsets/sizeof may shift, vtable identity by base name is unaffe
- 217119 VTABLE_SkyrimVM[32] include/RE/Offsets_VTABLE.h:4307 0x19918B8 :: LAYOUT CHANGE: 1.7.104 adds base(s) ['.?AV?$BSTEventSink@UTESAmiiboTouchEvent@@@@', '.?AV?$BSTEventSink@UTESAmiiboForcedStopDetectionEvent@@@@'] to the class hierarchy (58 -> 60 bases); subobject offsets/sizeof may shift, vtable identity by base name is unaffe
- 217121 VTABLE_SkyrimVM[33] include/RE/Offsets_VTABLE.h:4307 0x19918D0 :: LAYOUT CHANGE: 1.7.104 adds base(s) ['.?AV?$BSTEventSink@UTESAmiiboTouchEvent@@@@', '.?AV?$BSTEventSink@UTESAmiiboForcedStopDetectionEvent@@@@'] to the class hierarchy (58 -> 60 bases); subobject offsets/sizeof may shift, vtable identity by base name is unaffe
- 217123 VTABLE_SkyrimVM[34] include/RE/Offsets_VTABLE.h:4307 0x19918E8 :: LAYOUT CHANGE: 1.7.104 adds base(s) ['.?AV?$BSTEventSink@UTESAmiiboTouchEvent@@@@', '.?AV?$BSTEventSink@UTESAmiiboForcedStopDetectionEvent@@@@'] to the class hierarchy (58 -> 60 bases); subobject offsets/sizeof may shift, vtable identity by base name is unaffe
- 217125 VTABLE_SkyrimVM[35] include/RE/Offsets_VTABLE.h:4307 0x1991900 :: LAYOUT CHANGE: 1.7.104 adds base(s) ['.?AV?$BSTEventSink@UTESAmiiboTouchEvent@@@@', '.?AV?$BSTEventSink@UTESAmiiboForcedStopDetectionEvent@@@@'] to the class hierarchy (58 -> 60 bases); subobject offsets/sizeof may shift, vtable identity by base name is unaffe
- 217127 VTABLE_SkyrimVM[36] include/RE/Offsets_VTABLE.h:4307 0x1991918 :: LAYOUT CHANGE: 1.7.104 adds base(s) ['.?AV?$BSTEventSink@UTESAmiiboTouchEvent@@@@', '.?AV?$BSTEventSink@UTESAmiiboForcedStopDetectionEvent@@@@'] to the class hierarchy (58 -> 60 bases); subobject offsets/sizeof may shift, vtable identity by base name is unaffe
- 217129 VTABLE_SkyrimVM[37] include/RE/Offsets_VTABLE.h:4307 0x1991930 :: LAYOUT CHANGE: 1.7.104 adds base(s) ['.?AV?$BSTEventSink@UTESAmiiboTouchEvent@@@@', '.?AV?$BSTEventSink@UTESAmiiboForcedStopDetectionEvent@@@@'] to the class hierarchy (58 -> 60 bases); subobject offsets/sizeof may shift, vtable identity by base name is unaffe
- 217131 VTABLE_SkyrimVM[38] include/RE/Offsets_VTABLE.h:4307 0x1991948 :: LAYOUT CHANGE: 1.7.104 adds base(s) ['.?AV?$BSTEventSink@UTESAmiiboTouchEvent@@@@', '.?AV?$BSTEventSink@UTESAmiiboForcedStopDetectionEvent@@@@'] to the class hierarchy (58 -> 60 bases); subobject offsets/sizeof may shift, vtable identity by base name is unaffe
- 217133 VTABLE_SkyrimVM[39] include/RE/Offsets_VTABLE.h:4307 0x1991960 :: LAYOUT CHANGE: 1.7.104 adds base(s) ['.?AV?$BSTEventSink@UTESAmiiboTouchEvent@@@@', '.?AV?$BSTEventSink@UTESAmiiboForcedStopDetectionEvent@@@@'] to the class hierarchy (58 -> 60 bases); subobject offsets/sizeof may shift, vtable identity by base name is unaffe
- 217135 VTABLE_SkyrimVM[40] include/RE/Offsets_VTABLE.h:4307 0x1991978 :: LAYOUT CHANGE: 1.7.104 adds base(s) ['.?AV?$BSTEventSink@UTESAmiiboTouchEvent@@@@', '.?AV?$BSTEventSink@UTESAmiiboForcedStopDetectionEvent@@@@'] to the class hierarchy (58 -> 60 bases); subobject offsets/sizeof may shift, vtable identity by base name is unaffe
- 217137 VTABLE_SkyrimVM[41] include/RE/Offsets_VTABLE.h:4307 0x1991990 :: LAYOUT CHANGE: 1.7.104 adds base(s) ['.?AV?$BSTEventSink@UTESAmiiboTouchEvent@@@@', '.?AV?$BSTEventSink@UTESAmiiboForcedStopDetectionEvent@@@@'] to the class hierarchy (58 -> 60 bases); subobject offsets/sizeof may shift, vtable identity by base name is unaffe
- 217139 VTABLE_SkyrimVM[42] include/RE/Offsets_VTABLE.h:4307 0x19919A8 :: LAYOUT CHANGE: 1.7.104 adds base(s) ['.?AV?$BSTEventSink@UTESAmiiboTouchEvent@@@@', '.?AV?$BSTEventSink@UTESAmiiboForcedStopDetectionEvent@@@@'] to the class hierarchy (58 -> 60 bases); subobject offsets/sizeof may shift, vtable identity by base name is unaffe
- 217141 VTABLE_SkyrimVM[43] include/RE/Offsets_VTABLE.h:4307 0x19919C0 :: LAYOUT CHANGE: 1.7.104 adds base(s) ['.?AV?$BSTEventSink@UTESAmiiboTouchEvent@@@@', '.?AV?$BSTEventSink@UTESAmiiboForcedStopDetectionEvent@@@@'] to the class hierarchy (58 -> 60 bases); subobject offsets/sizeof may shift, vtable identity by base name is unaffe
- 217143 VTABLE_SkyrimVM[44] include/RE/Offsets_VTABLE.h:4307 0x19919D8 :: LAYOUT CHANGE: 1.7.104 adds base(s) ['.?AV?$BSTEventSink@UTESAmiiboTouchEvent@@@@', '.?AV?$BSTEventSink@UTESAmiiboForcedStopDetectionEvent@@@@'] to the class hierarchy (58 -> 60 bases); subobject offsets/sizeof may shift, vtable identity by base name is unaffe
- 217145 VTABLE_SkyrimVM[45] include/RE/Offsets_VTABLE.h:4307 0x19919F0 :: LAYOUT CHANGE: 1.7.104 adds base(s) ['.?AV?$BSTEventSink@UTESAmiiboTouchEvent@@@@', '.?AV?$BSTEventSink@UTESAmiiboForcedStopDetectionEvent@@@@'] to the class hierarchy (58 -> 60 bases); subobject offsets/sizeof may shift, vtable identity by base name is unaffe
- 217147 VTABLE_SkyrimVM[46] include/RE/Offsets_VTABLE.h:4307 0x1991A08 :: LAYOUT CHANGE: 1.7.104 adds base(s) ['.?AV?$BSTEventSink@UTESAmiiboTouchEvent@@@@', '.?AV?$BSTEventSink@UTESAmiiboForcedStopDetectionEvent@@@@'] to the class hierarchy (58 -> 60 bases); subobject offsets/sizeof may shift, vtable identity by base name is unaffe
- 217149 VTABLE_SkyrimVM[47] include/RE/Offsets_VTABLE.h:4307 0x1991A20 :: LAYOUT CHANGE: 1.7.104 adds base(s) ['.?AV?$BSTEventSink@UTESAmiiboTouchEvent@@@@', '.?AV?$BSTEventSink@UTESAmiiboForcedStopDetectionEvent@@@@'] to the class hierarchy (58 -> 60 bases); subobject offsets/sizeof may shift, vtable identity by base name is unaffe
- 217151 VTABLE_SkyrimVM[48] include/RE/Offsets_VTABLE.h:4307 0x1991A68 :: LAYOUT CHANGE: 1.7.104 adds base(s) ['.?AV?$BSTEventSink@UTESAmiiboTouchEvent@@@@', '.?AV?$BSTEventSink@UTESAmiiboForcedStopDetectionEvent@@@@'] to the class hierarchy (58 -> 60 bases); subobject offsets/sizeof may shift, vtable identity by base name is unaffe
- 217153 VTABLE_SkyrimVM[49] include/RE/Offsets_VTABLE.h:4307 0x1991A80 :: LAYOUT CHANGE: 1.7.104 adds base(s) ['.?AV?$BSTEventSink@UTESAmiiboTouchEvent@@@@', '.?AV?$BSTEventSink@UTESAmiiboForcedStopDetectionEvent@@@@'] to the class hierarchy (58 -> 60 bases); subobject offsets/sizeof may shift, vtable identity by base name is unaffe
- 217155 VTABLE_SkyrimVM[50] include/RE/Offsets_VTABLE.h:4307 0x1991A98 :: LAYOUT CHANGE: 1.7.104 adds base(s) ['.?AV?$BSTEventSink@UTESAmiiboTouchEvent@@@@', '.?AV?$BSTEventSink@UTESAmiiboForcedStopDetectionEvent@@@@'] to the class hierarchy (58 -> 60 bases); subobject offsets/sizeof may shift, vtable identity by base name is unaffe
- 217157 VTABLE_SkyrimVM[51] include/RE/Offsets_VTABLE.h:4307 0x1991AB0 :: LAYOUT CHANGE: 1.7.104 adds base(s) ['.?AV?$BSTEventSink@UTESAmiiboTouchEvent@@@@', '.?AV?$BSTEventSink@UTESAmiiboForcedStopDetectionEvent@@@@'] to the class hierarchy (58 -> 60 bases); subobject offsets/sizeof may shift, vtable identity by base name is unaffe
- 236711 VTABLE_BSWin32GamerProfile[0] include/RE/Offsets_VTABLE.h:54 0x1A23CA8 :: LAYOUT CHANGE: slot count AE 6 vs 1.7 8; anchored slots AE->1.7: 0->0; class hierarchy identical (4 bases)
- 236733 VTABLE_BSGamerProfile[0] include/RE/Offsets_VTABLE.h:5434 0x1A27538 :: LAYOUT CHANGE: slot count AE 6 vs 1.7 8; anchored slots AE->1.7: 0->0; class hierarchy identical (3 bases)
- 242930 VTABLE_BSScaleformMovieLoadTask[0] include/RE/Offsets_VTABLE 0x1A9C860 :: LAYOUT CHANGE: slot count AE 13 vs 1.7 107; anchored slots AE->1.7: 0->0 1->1; class hierarchy identical (3 bases); SLOT CONTRADICTION: 3 of the 6 L1-anchored AE slot functions are not at the corresponding 1.7 slot (virtual layout/semantics changed; identity r
- 255903 VTABLE_BSSystemUtility[0] include/RE/Offsets_VTABLE.h:7333 0x1B51900 :: LAYOUT CHANGE: slot count AE 19 vs 1.7 20; anchored slots AE->1.7: 0->0; class hierarchy identical (3 bases)
- 255912 VTABLE_BSWin32SaveDataSystemUtility[0] include/RE/Offsets_VT 0x1B51C10 :: LAYOUT CHANGE: slot count AE 18 vs 1.7 24; anchored slots AE->1.7: 0->0 1->1 2->2 5->11 6->12 8->14 9->15 17->23; class hierarchy identical (3 bases)
- 255925 VTABLE_BSSaveDataSystemUtility[0] include/RE/Offsets_VTABLE. 0x1B51DE0 :: LAYOUT CHANGE: slot count AE 18 vs 1.7 24; anchored slots AE->1.7: 0->0; class hierarchy identical (2 bases)
- 255953 VTABLE_BSWin32SystemUtility[0] include/RE/Offsets_VTABLE.h:7 0x1B525B0 :: LAYOUT CHANGE: slot count AE 19 vs 1.7 20; anchored slots AE->1.7: 0->0 16->17 18->19; class hierarchy identical (4 bases)

## ground-truth mismatches (tool bugs to investigate)

## ground-truth unresolved

## HARD ids (handed back)
- 69188 id cl+fork :: Offset::GetCachedString include/RE/Offsets.h:116 :: tried: (1) present in Address Library 1.6.317,1.6.318,1.6.323,1.6.342,1.6.353,1.6.629,1.6.640,1.6.659, absent in 1.6.1130,1.6.1170,1.6.1179: a retired id, so there is no 1.6.1170 RVA to start the 1.6.1170->1.7.104 ladder from; (2) the fork SE column id is present in the 1.6.1170 library but maps to unrelated code (149 fork pairs with both ids present never share an RVA); (3) no RTTI, string or vtable tie names the function; (4) older libraries give 1.6.317-659 RVAs of binaries we do not hold. Fo
- 11044 id fork :: Offset::Set_CStr include/RE/Offsets.h:151 :: tried: (1) present in Address Library 1.6.317,1.6.318,1.6.323,1.6.342,1.6.353,1.6.629,1.6.640,1.6.659, absent in 1.6.1130,1.6.1170,1.6.1179: a retired id, so there is no 1.6.1170 RVA to start the 1.6.1170->1.7.104 ladder from; (2) the fork SE column id is present in the 1.6.1170 library but maps to unrelated code (149 fork pairs with both ids present never share an RVA); (3) no RTTI, string or vtable tie names the function; (4) older libraries give 1.6.317-659 RVAs of binaries we do not hold. Fo
- 405935 id fork :: Offset::SelectedRef include/RE/Offsets.h:171 :: tried: (1) present in Address Library 1.6.317,1.6.318,1.6.323,1.6.342,1.6.353,1.6.629,1.6.640,1.6.659, absent in 1.6.1130,1.6.1170,1.6.1179: a retired id, so there is no 1.6.1170 RVA to start the 1.6.1170->1.7.104 ladder from; (2) the fork SE column id is present in the 1.6.1170 library but maps to unrelated code (149 fork pairs with both ids present never share an RVA); (3) no RTTI, string or vtable tie names the function; (4) older libraries give 1.6.317-659 RVAs of binaries we do not hold. Fo
- 21890 id fork :: Offset::CompileAndRun include/RE/Offsets.h:472 :: tried: (1) present in Address Library 1.6.317,1.6.318,1.6.323,1.6.342,1.6.353,1.6.629,1.6.640,1.6.659, absent in 1.6.1130,1.6.1170,1.6.1179: a retired id, so there is no 1.6.1170 RVA to start the 1.6.1170->1.7.104 ladder from; (2) the fork SE column id is present in the 1.6.1170 library but maps to unrelated code (149 fork pairs with both ids present never share an RVA); (3) no RTTI, string or vtable tie names the function; (4) older libraries give 1.6.317-659 RVAs of binaries we do not hold. Fo
- 248775 vtable fork :: VTABLE_std__bad_weak_ptr[0] include/RE/Offsets_VTABLE.h:6674 :: the class TypeDescriptor is unique in both images (std::bad_weak_ptr, CRT class) but NEITHER executable has a CompleteObjectLocator or vtable pointer for it (no RTTI vtable), so the RTTI/NAME routes have nothing to bind; tried: COL scan, TypeDescriptor-by-name, no 1.6.1170 library row to start the ladder from. Needs the vtable located by its constructor/throw-info users

## INLINED ids
- 99886 id :: REL::ID src/RE/B/BSShaderTextureSet.cpp:8 :: the only functions that store VTABLE_BSShaderTextureSet (AE 0x17BCB08, 1.7 0x1836258) are 4 in 1.6.1170 (0x14CEE70(209B) 0x14CF0B0(209B) 0x3270C0(335B) 0x14AC7C0(516B)) and 4 in 1.7.104 (0x153B2E0(209B) 0x153B520(209B) 0x32D720(335B) 0x1518970(516B)); none is a standalone constructor (smallest is 209 B in AE, 209 B in 1.7.104): the 209-byte pair are the allocate-and-construct bodies of BSShaderTextureSet::Create, so the constructor body was absorbed into its callers; the REL::ID(99886) standalone constructor exists only in older AE libraries (see lib presence)

## REMOVED ids by reason
- 1236 x present in Address Library 1.6.317,1.6.318,1.6.323,1.6.342,1...
- 8 x the class is a TypeDescriptor in the 1.6.1170 executable but...
- 3 x present in Address Library 1.6.342,1.6.353, absent in 1.6.11...
