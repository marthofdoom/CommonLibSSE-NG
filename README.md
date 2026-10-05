# CommonLibSSE-NG, MIT line (3.7.0)

A continuation of [CharmedBaryon's CommonLibSSE-NG](https://github.com/CharmedBaryon/CommonLibSSE-NG) 3.7.0, kept on
its MIT license, with fixes verified against the game's own code and support for Skyrim SE 1.7.104. It is a
modders resource, like the library it continues. Use it the same way you would use CommonLibSSE-NG 3.7.0.

- Branch: `main`. It starts at the upstream v3.7.0 commit `c4ab853d095e81e3390b282d7ba01ab2f24ebf25`. The old
  upstream main is kept as the branch `upstream-main`.
- License: MIT, unchanged. The original LICENSE file and its copyright notice stay as they are. New files carry
  my own notice under the same MIT terms.

## Why this fork exists

The maintained CommonLibSSE-NG moved to GPL-3.0-or-later (with modding exceptions). My mods are MIT, so I keep an
MIT line of 3.7.0 instead.

**Clean room.** No code from the GPL line is in here. I read it only to learn that a problem exists. Every fix and
every addition here is written from the game executables and from file bytes, and each commit message carries the
addresses and instructions that prove it. If a fix here disagrees with a header somewhere else, the disassembly
note in the commit is the reason.

## Supported game versions

| game version | where ids come from | struct layouts |
|---|---|---|
| **1.5.97.0** | the Address Library, `version-1-5-97-0.bin` | verified |
| **1.6.1170.0** | the Address Library, `versionlib-1-6-1170-0.bin` | verified |
| **1.7.104.0** | this fork's own id table, `mit-idtable-1-7-104-0.bin` (see below) | verified |
| other 1.5.x and 1.6.x | the Address Library | upstream 3.7.0 values, not verified. The exact-build accessors refuse them by name. |
| other 1.7.x | none. The plugin stops at load with a message. | none |
| VR | the VR Address Library CSV | upstream 3.7.0 values, not verified |

**How exact-version gating works.** `REL::Module::IsAE()` and `IsSE()` are buckets: AE is every 1.6.x and 1.7.x,
SE is every 1.5.x. A layout that was verified on one build is not verified on the rest of its bucket. So this fork
adds `REL::Module::IsExactly(version)` and the constants `SKSE::RUNTIME_SSE_1_6_1170` and
`SKSE::RUNTIME_SSE_1_7_104`, and every layout it corrects is chosen by the EXACT build:

- Each verified build gets its own arm, with the offsets proven on that executable.
- A build with no arm is refused. Where a wrong value could crash, it stops the game with a message naming the
  game version (`ControlMap::GetRuntimeData`, `CombatController::GetRuntimeData`). Where a refusal is enough, it
  logs one critical line and returns nothing (`BGSDefaultObjectManager::GetObject`, `ControlMap::GetInputContext`).
- Nothing is guessed. A value is never taken from a neighbouring build.

Use `IsExactly` in your own code for anything you verified on one build only.

## Using this fork

The easy way is my vcpkg registry, [marthofdoom/vcpkg-registry](https://github.com/marthofdoom/vcpkg-registry).
It serves one port, `commonlibsse-ng`, built from this repository. In your project's `vcpkg-configuration.json`,
send that one port to the registry:

```json
{
    "default-registry": {
        "kind": "git",
        "repository": "https://github.com/microsoft/vcpkg.git",
        "baseline": "<a microsoft/vcpkg commit>"
    },
    "registries": [
        {
            "kind": "git",
            "repository": "https://github.com/marthofdoom/vcpkg-registry",
            "baseline": "<a marthofdoom/vcpkg-registry commit>",
            "packages": [ "commonlibsse-ng" ]
        }
    ]
}
```

Then keep `commonlibsse-ng` in `vcpkg.json` and use `find_package(CommonLibSSE CONFIG REQUIRED)` and
`add_commonlibsse_plugin(...)` as usual (see "Use" below). The registry baseline pins one exact commit of this
fork, so a newer fork needs a newer baseline. The port version goes up with every new fork commit. If your CI
caches vcpkg packages, put `vcpkg-configuration.json` in the cache key, or an old build of the library hides the
new one.

On 1.7.104 your plugin also needs the id table file in `Data/SKSE/Plugins/` (see below).

## What changed from 3.7.0

Each item is one commit, and the commit message carries the proof.

### F0: the fork

- This branch and the vcpkg registry. No source change from 3.7.0.

### F1: corrections, verified on 1.6.1170.0 and 1.5.97.0

- **An id that is not in the Address Library stops the game** with a message naming the id and the game version.
  Before, it quietly used the next id's address on SE and AE. `IDDatabase::try_id2offset` is the quiet version
  for self-checks.
- `SKSE::log::log_directory` uses the real AE id for the My Games folder name (502114, not 380738).
- `REL::Module::IsExactly(version)` and `SKSE::RUNTIME_SSE_1_6_1170`.
- `BGSDefaultObjectManager` reads the real object and flag arrays of the exact build (366 objects and flags at
  +0xB90 on 1.6.1170, 364 and +0xB80 on 1.5.97). The enums keep the 1.5.97 numbering and are translated.
- `CombatMagicCaster::GetMagicTarget` returns its 16-byte target the way the game does (a hidden out-slot).
- `ControlMap`: the members after the context array move by 8 on 1.6.1170 (18 contexts, not 17), so they sit
  behind `GetRuntimeData()`. `GetInputContext` maps kFavor to 17 there. `ToggleControls` calls the game's own
  function.
- `CombatController`: the members from +0x68 move by 8 on 1.6.1170, so they sit behind `GetRuntimeData()`.
- New bindings from my upstream PRs: `Actor::StartCombat` (#107), the `ExtraDataList` constructor (#108, with a
  heap size fix: the game object is 0x18 or 0x20 bytes, not 0x10), `SendInventoryUpdateMessage` (#109).
- `REL::SelfCheck`: a plugin lists the ids it hooks with the RVAs its own disassembly found, and refuses a hook
  whose id the loaded library places anywhere else. `SelfCheck::kLibrary` names the fork stage for a startup log.

### F1b

- `TESForm::LookupByID` and `LookupByEditorID` hold the game's own read lock on the form maps. In 3.7.0 they
  copied the lock and held nothing, so a lookup from another thread could read a map the game was changing.

### F2a: Skyrim 1.7.104

- 1.7.x is filed with AE, because it continues the AE id column. `SKSE::RUNTIME_SSE_1_7_104` names the build.
- On 1.7.104.0 the ids come from this fork's own id table, never from the Address Library. Every other 1.7.x
  build is refused when the plugin loads.
- The id table format ([docs/MIT-ID-TABLE-FORMAT.md](docs/MIT-ID-TABLE-FORMAT.md)) and its generator
  ([tools/mit-idtable](tools/mit-idtable/README.md)). The table built for this fork is in `data/`.
- Every exact-build layout from F1 has a 1.7.104 arm: the default object manager (372 objects, flags at +0xBC0,
  six objects inserted, enums translated), ControlMap (as 1.6.1170), CombatController (as 1.6.1170),
  `GetMagicTarget` (same shape).
- `PlayerCharacter` gained a base class on 1.7.104, so everything it declares itself sits 8 bytes further. Its
  accessors follow the exact build.
- `BSInputDeviceManager` has six device slots on 1.7.104, with the virtual keyboard in slot 5. Use `GetDevice`.
- `BSInputEventQueue` gained three event kinds on 1.7.104. The members after the six counts are accessors now.

Do not "fix" the missing Actor base classes in AE-enabled builds: needing `As*()` there is the safe behaviour.

## The 1.7.104 id table

I do not use the Nexus Address Library for 1.7.104. On 1.7.104.0 this fork reads
`Data/SKSE/Plugins/mit-idtable-1-7-104-0.bin` instead.

- **Format.** [docs/MIT-ID-TABLE-FORMAT.md](docs/MIT-ID-TABLE-FORMAT.md): a 64-byte header, sorted `{id, rva}`
  records, and a checksum. The ids are the AE ids, the second id in `RELOCATION_ID(se, ae)`, so nothing changes
  in your code.
- **Bound to one executable.** The header holds the version, the module name, and the PE timestamp and image size
  of the executable it was built from. The fork checks all of them against the running game. The same version
  number can be two different builds.
- **Private.** Each plugin reads the file into its own memory. Nothing is shared between plugins, and nothing goes
  into the shared mapping the Address Library path uses.
- **Coverage today: 354 ids.** These are the ids my two mods reach: 226 ids their own code uses, 118 ids inside
  CommonLib functions they call, and 10 that every plugin uses (memory manager, BSFixedString, RTDynamicCast, the
  log folder). By kind: 254 functions or globals, 89 vtables, 11 RTTI type descriptors. Each one was mapped from
  its 1.6.1170 address to 1.7.104 by disassembly, with its evidence and a crosscheck: 177 by exact RTTI or import
  name, 125 by a function signature that occurs once in each executable, 52 through callers and callees already
  mapped. The mapper got 107 of 107 known pairs right. One id it could not prove (69188,
  `BSScaleformTranslator::GetCachedString`) is left out on purpose.
- **A missing id stops the game, loudly.** If your plugin asks for an id the table does not have, the game stops
  with a message naming the id and the file, and saying that the table does not cover every id yet. It never
  returns a wrong address. A missing or damaged file, or a file for another executable, stops the game with a
  message naming the file.
- **Regenerate or extend it** with [tools/mit-idtable](tools/mit-idtable/README.md): map the id on the 1.7.104
  executable, add a row with its RVA to a CSV, and build the file again. The tool refuses a row it cannot use,
  an RVA outside the executable, and an id that two CSVs map differently. `check` proves a file matches its
  executable and its CSV.
- The file ships next to the plugins that need it, in `Data/SKSE/Plugins/`.
- On 1.7.104 the virtual keyboard sits in device slot 5, not in slot `INPUT_DEVICE::kVirtualKeyboard` (3).
  `GetDevice` and `GetVirtualKeyboard` handle that. The device number inside its input events is not verified.

---

# Upstream CommonLibSSE-NG 3.7.0 documentation

The rest of this page is the 3.7.0 README, unchanged. Where it names releases, CI or package feeds, those are
CharmedBaryon's, not this fork's.

## New Features
### Multiple Runtime Targets
![stability](https://img.shields.io/static/v1?label=stability&message=stable&color=dimgreen&style=flat)

CommonLibSSE NG has support for Skyrim SE, AE, and VR, and is able to create builds for any combination of these
runtimes, including all three. This makes it possible to create SKSE plugins with a single DLL that works in any
Skyrim runtime, improving developer productivity (only one build and CMake preset necessary) and improving the end-user
experience (no need to make a choice of multiple download options or select the right DLL from a FOMOD). For Skyrim AE,
both versions before 1.6.629 and those after are supported in a single DLL (both struct layouts are supported).

Builds that target multiple ABI-incompatible runtimes provide the necessary (but minimal) abstractions needed to work
transparently regardless of the Skyrim edition in use. All functionality and classes from all runtimes is always
available for potential use, with the necessary support for probing the features available. This allows single plugins
that dynamic alter their behavior to take advantage of the specific features of a single Skyrim edition (e.g. VR
features) when that runtime is present.

[Read about multi-targeting, and how you can take advantage in your project.](
https://github.com/CharmedBaryon/CommonLibSSE-NG/wiki/Runtime-Targeting)

### Simplified Plugin Declaration
![stability](https://img.shields.io/static/v1?label=stability&message=stable&color=dimgreen&style=flat)

Historically, plugins needed to define `SKSEPlugin_Version` or `SKSEPlugin_Query` to be detected as SKSE plugins. To
simplify code and ensure both functions are generated for correct cross-runtime compatibility, this is made code-free in
CommonLibSSE NG. Just replace `add_library` with `add_commonlibsse_plugin` in `CMakeLists.txt` and CMake will create
your shared library target, configure CommonLibSSE linkage, and auto-generate this content and inject it into the
plugin.

```cmake
find_package(CommonLibSSE REQUIRED)

add_commonlibsse_plugin(${PROJECT_NAME}
    SOURCES ${headers} ${sources})
```

[Read more on
the project wiki.](https://github.com/CharmedBaryon/CommonLibSSE-NG/wiki/Runtime-Targeting#cmake-integration)

### Clang Support
![stability-beta](https://img.shields.io/static/v1?label=stability&message=beta&color=yellow&style=flat)

Clang 13.x and newer are supported when built for MSVC ABI compatibility (versions built natively for Windows). It is
even hypothetically possible, with the proper setup, to cross-compile with Clang from a Linux host (testing and
instructions for this are pending). Note that currently linking must still be done with the Microsoft linker, pending
fixes to SKSE itself.

[Read more on
the project wiki.](https://github.com/CharmedBaryon/CommonLibSSE-NG/wiki/Compiling-with-Clang)

### Unit Testing Enhancements
![stability-stable](https://img.shields.io/static/v1?label=stability&message=stable&color=dimgreen&style=flat)

Improvements have been made to the way in which the Skyrim executable module is accessed for the sake of memory
relocation and handling of Address Library IDs. This makes it easier to run CommonLibSSE-based plugin code outside of
the context of a Skyrim process so that unit testing is possible. It is even possible to handle a minimal level of
functionality from the engine by loading in a Skyrim executable module programmatically during testing, allowing, for
example, tests to be run within a single test suite that vary between SE, AE, and VR executables.

[Read more on
the project wiki.](https://github.com/CharmedBaryon/CommonLibSSE-NG/wiki/Unit-Testing)

### Versioned Releases via Vcpkg and Conan
![stability-stable](https://img.shields.io/static/v1?label=vcpkg&message=stable&color=dimgreen&style=flat)

![stability-experimental](https://img.shields.io/static/v1?label=conan&message=experimental&color=orange&style=flat)

Traditionally CommonLibSSE is consumed via a Git submodule; this practice generally requires the project using it to add
CommonLibSSE's own Vcpkg dependencies and parts of it CMake configuration, as CommonLibSSE is built as a part of the
plugin project. CommonLibSSE NG instead uses specific releases with semantic versioning, and distributes them as their
own Vcpkg ports, through the [Color-Glass Studios Vcpkg repository](https://gitlab.com/colorglass/vcpkg-colorglass).
As a result, it is simpler to use, does not require absorbing transitive requirements into your own project, and needs
only be built once (cleaning will not require rebuilding CommonLibSSE, only your own code).

In addition, starting with version 3.5.0, CommonLibSSE NG can now be both built and consumed via the Conan package
manager, both as a source build or prebuilt packages targeting Windows on x86_64 with Visual Studio compiler versions 16
and 17 (2019 and 2022).

[See how to use CommonLibSSE NG in your project.](#use)

### Ninja Builds
![stability-stable](https://img.shields.io/static/v1?label=stability&message=stable&color=dimgreen&style=flat)

CommonLibSSE NG migrates the build system to Ninja, resulting in faster parallel builds than NMake.

## Other Changes
* Ability to define offsets and address IDs for objects which can exist in only a subset of runtimes, while being able
  dynamically test for feature support before using those offsets.
* Completely regenerated RTTI and vtable offsets, now with consistent naming and access across all runtimes.
* Updated GitHub Actions CI workflows to build for all likely target runtime combinations.
* Fully extensible native function binding traits (enables custom script object bindings in
  [Fully Dynamic Game Engine](https://gitlab.com/colorglass/fully-dynamic-game-engine)).
* Better support for the CLion IDE.

## Use
### Via Vcpkg
CommonLibSSE NG is available as a Vcpkg port. To add it to your project, create a `vcpkg-configuration.json` file in the
project root (next to `vcpkg.json`) with the following contents:

```json
{
    "registries": [
        {
            "kind": "git",
            "repository": "https://gitlab.com/colorglass/vcpkg-colorglass",
            // Update this baseline to the latest commit from the above repo.
            "baseline": "9eae9f03f03e0ca96fce5031f44f4e64cd6debdc",
            "packages": [
                "commonlibsse-ng",
                "commonlibsse-ng-ae",
                "commonlibsse-ng-se",
                "commonlibsse-ng-vr",
                "commonlibsse-ng-flatrim"
            ]
        }
    ]
}
```

Then add `commonlibsse-ng` as a dependency in `vcpkg.json`. There are also runtime-specific versions of the project:
* `commonlibsse-ng-ae`: Supports AE executables (1.6.x) only.
* `commonlibsse-ng-se`: Supports pre-AE executables (1.5.x) only.
* `commonlibsse-ng-vr`: Supports VR only.
* `commonlibsse-ng-flatrim`: Support for SE/AE, but not VR.

The runtime-specific ports will not attempt to dynamically lookup the version of Skyrim at runtime, and will enable
access to reverse engineered content that is specific to that version of Skyrim and non-portable (i.e. it does not exist
in all versions of Skyrim, or has not been reverse engineered on all versions of Skyrim).

### Via Conan
CommonLibSSE NG is now available via Conan. Add it as a requirement to your project's `conanfile.txt` or `conanfile.py`:

```ini
[requires]
commonlibsse-ng/3.5.2
```

```python
class MyProject:
    # ...
    requires = 'commonlibsse-ng/3.5.2'
```

Update the version number to the version constraints you want. Conan support was added in version 3.5.0, making that the
earliest version available. Currently Conan binaries are available for the following
`build_type`/`arch`/`os`/`compiler`/`compiler.version` combinations:

* Debug/x86_64/Windows/Visual Studio/16
* Debug/x86_64/Windows/Visual Studio/17
* Release/x86_64/Windows/Visual Studio/16
* Release/x86_64/Windows/Visual Studio/17

Selective runtime support is handled via package options:

```ini
[requires]
commonlibsse-ng/3.5.2

[options]
commonlibsse-ng:ae=True
commonlibsse-ng:se=True
commonlibsse-ng:vr=True
```

```python
class MyProject:
    # ...
    requires = 'commonlibsse-ng/3.5.2'
    default_options = {
      # ...
      'commonlibsse-ng:with_ae': True,
      'commonlibsse-ng:with_se': True,
      'commonlibsse-ng:with_vr': True
    }
```

The above shows the default values (which includes support for all runtimes). Disable any runtimes you do not want.

### Linking in CMake
You should have the following in `CMakeLists.txt` to compile and link successfully:
```cmake
find_package(CommonLibSSE REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC CommonLibSSE::CommonLibSSE)
```

For more information on how to use CommonLibSSE NG, you can look at the
[example plugin](https://gitlab.com/colorglass/commonlibsse-sample-plugin).

## Build Dependencies
* [fmt](https://fmt.dev/latest/index.html)
* [rapidcsv](https://github.com/d99kris/rapidcsv)
* [spdlog](https://github.com/gabime/spdlog)
* [Visual Studio 2022](https://visualstudio.microsoft.com/vs/)
  * Desktop development with C++

## End User Dependencies
* [Address Library for SKSE Plugins](https://www.nexusmods.com/skyrimspecialedition/mods/32444) or
  [VR Address Library for SKSEVR](https://www.nexusmods.com/skyrimspecialedition/mods/58101)
* [SKSE64](https://skse.silverlock.org/)

## Development
* [Address Library for SKSE Plugins](https://www.nexusmods.com/skyrimspecialedition/mods/32444) and
  [VR Address Library for SKSEVR](https://www.nexusmods.com/skyrimspecialedition/mods/58101)
* [clang-format 12.0.0](https://github.com/llvm/llvm-project/releases)
* [CMake](https://cmake.org/)
* [Conan](https://conan.io) or [Vcpkg](https://github.com/microsoft/vcpkg)

## Notes
* CommonLib is incompatible with SKSE and is intended to replace it as a static dependency. However, you will still need
* the runtime component.
