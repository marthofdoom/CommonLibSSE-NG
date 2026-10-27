#!/usr/bin/env python
# -*- coding: utf-8 -*-

# mit-3.7: Conan 2 recipe. Conan 2 removed the Conan 1 API (conans.*), and Conan 1 does not know current
# Visual Studio toolsets, so the recipe moved to the Conan 2 API. Packaging only: the library is built by the same
# CMake presets as before, with the same options and the same dependencies.

from os import path, remove

from conan import ConanFile
from conan.errors import ConanInvalidConfiguration
from conan.tools.cmake import CMakeDeps, CMakeToolchain
from conan.tools.files import copy
from conan.tools.scm import Version


required_conan_version = ">=2.0"


class CommonLibSSE(ConanFile):
    name = "commonlibsse-ng"
    version = "3.5.0"
    license = "MIT"
    url = "https://github.com/CharmedBaryon/CommonLibSSE"
    description = "A fork of CommonLibSSE with advanced features for modern SKSE development."
    package_type = "static-library"
    settings = "os", "compiler", "build_type", "arch"
    options = {
        "testing": [True, False],
        "ae": [True, False],
        "se": [True, False],
        "vr": [True, False],
        "xbyak": [True, False]
    }
    default_options = {
        "testing": False,
        "ae": True,
        "se": True,
        "vr": True,
        "xbyak": False,
        # Compiled spdlog, as vcpkg builds it: CMakeLists.txt links spdlog::spdlog, which the header-only package
        # does not define (it defines spdlog::spdlog_header_only).
        "spdlog/*:header_only": False
    }
    exports_sources = "CMakeLists.txt", "CMakePresets.json", "cmake/**", "include/**", "src/**", "tests/**", \
                      "data/mit-idtable-v1-1-7-104-0.bin", \
                      ".clang-format", "CommonLibSSE.natvis", "LICENSE*"

    def requirements(self):
        if self.options.testing:
            self.requires("catch2/3.1.0")
        self.requires("rapidcsv/8.62")
        # The same spdlog and fmt the vcpkg build resolves (vcpkg.json: its baseline's spdlog 1.17.0, fmt overridden
        # to 10.2.1). spdlog 1.10.0 pulled fmt 8.1.1, which no longer compiles in Debug with the current MSVC STL.
        self.requires("spdlog/1.17.0")
        self.requires("fmt/10.2.1", override=True)
        self.requires("xbyak/6.61.2")

    def layout(self):
        # The CMake presets read the toolchain from build/generators and build into build/<preset>.
        self.folders.source = "."
        self.folders.build = "build"
        self.folders.generators = path.join("build", "generators")

    def validate(self):
        if self.settings.os != "Windows":
            raise ConanInvalidConfiguration("Windows is the only supported OS")
        if self.settings.compiler != "msvc":
            raise ConanInvalidConfiguration("Unsupported compiler provided, only msvc (Visual Studio) is supported")
        if Version(self.settings.compiler.version) < "192":
            raise ConanInvalidConfiguration("Visual Studio 2019 (msvc 192) or newer is required")
        if self.settings.arch != "x86_64":
            raise ConanInvalidConfiguration("x86_64 is the only supported architecture")

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()
        tc = CMakeToolchain(self, generator="Ninja")
        # The project has its own CMakePresets.json: write no CMakeUserPresets.json next to it.
        tc.user_presets_path = False
        tc.generate()

        user_presets = path.join(self.source_folder, "CMakeUserPresets.json")
        if path.exists(user_presets):
            remove(user_presets)

    def build(self):
        defs = {"ENABLE_SKYRIM_AE": "ON" if self.options.ae else "OFF",
                "ENABLE_SKYRIM_SE": "ON" if self.options.se else "OFF",
                "ENABLE_SKYRIM_VR": "ON" if self.options.vr else "OFF",
                "BUILD_TESTS": "ON" if self.options.testing else "OFF",
                "SKSE_SUPPORT_XBYAK": "ON" if self.options.xbyak else "OFF"}
        def_args = " ".join(['-D{}="{}"'.format(k, v) for k, v in defs.items()])
        build_type = str(self.settings.build_type).lower()
        self.run(f"cmake --preset=build-{build_type}-msvc-conan-all {def_args}", cwd=self.source_folder)
        self.run(f"cmake --build --preset={build_type}-msvc-conan-all", cwd=self.source_folder)

    def package(self):
        build_type = str(self.settings.build_type).lower()
        self.run(f'cmake --install "{path.join(self.source_folder, "build", f"{build_type}-msvc-conan-all")}" ' +
                 f'--prefix "{self.package_folder}"', cwd=self.source_folder)
        copy(self, "LICENSE*", src=self.source_folder, dst=path.join(self.package_folder, "licenses"), keep_path=False)

    def package_info(self):
        self.cpp_info.libs = ["commonlibsse-ng"]
