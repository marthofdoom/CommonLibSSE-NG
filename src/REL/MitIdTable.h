// SPDX-License-Identifier: MIT
// Copyright (c) 2026 marth
#pragma once

// mit-3.7: the MIT id table for Skyrim 1.7.104.0, built into the library (docs/MIT-ID-TABLE-FORMAT.md).
// The bytes are data/mit-idtable-v1-1-7-104-0.bin, exactly. The build turns that file into a byte list
// (cmake/bin2c.cmake, or xmake's utils.bin2c rule) and src/REL/MitIdTable.cpp compiles it in.
// IDDatabase::load_mit_table validates these bytes the same way it once validated the file.
// Private to the library: not installed, not part of the public headers.

#include <cstddef>
#include <cstdint>

namespace REL::detail
{
	extern const std::uint8_t mit_idtable_1_7_104[];
	extern const std::size_t  mit_idtable_1_7_104_size;
}
