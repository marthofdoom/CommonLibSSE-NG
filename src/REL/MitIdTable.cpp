// SPDX-License-Identifier: MIT
// Copyright (c) 2026 marth

#include "MitIdTable.h"

namespace REL::detail
{
	// mit-3.7: the generated header holds data/mit-idtable-v1-1-7-104-0.bin as a byte list.
	const std::uint8_t mit_idtable_1_7_104[] = {
#include "mit-idtable-v1-1-7-104-0.bin.h"
	};
	const std::size_t mit_idtable_1_7_104_size = sizeof(mit_idtable_1_7_104);
}
