#pragma once

// mit-3.7: REL::SelfCheck. Checks, at startup, that the Address Library in use maps every
// hook-critical id to the RVA our own disassembly of that exact build says it must, and
// optionally that the bytes there are the ones our hooks expect.
//
// Why. The Address Library is trusted blindly by everything else in REL: a library for the
// wrong build of the same version (1.6.1170 has two, about 0x930 apart), a damaged file, or
// a row that moved would put a hook in the middle of the wrong function. A self-check row is
// a fact we derived from our own disassembly of the binary we verified on, so a mismatch is
// caught before anything is written.
//
// How a consumer uses it:
//   1. A generated table per exact runtime (the consumer's VerifiedAddresses.h), one Row per
//      id its hooks and seats depend on.
//   2. Run(tables) once after SKSE::Init. It picks the table whose version equals the running
//      executable's EXACT version. If none does, the result is "not covered" and nothing is
//      verified.
//   3. Before each hook write or seat call, ask IsVerifiedAddress(address). If it is false,
//      REFUSE that one seat, by name, in the log. Never substitute the expected RVA for the
//      library's answer, and never install anyway (MFO CLAUDE.md principle 7).
//
// A Row never makes anything work that did not already resolve: it only takes things away.

#include "SKSE/Impl/PCH.h"

namespace REL::SelfCheck
{
	// Names this library build in a consumer's startup log line. Bump with each fork stage.
	inline constexpr std::string_view kLibrary = "CommonLibSSE-NG 3.7.0 mit-3.7 F1b (exact id match, exact-build layouts, SelfCheck, locked form lookup)"sv;

	struct Row
	{
	public:
		// members
		std::uint64_t                id;           // Address Library id on this build (0 = no id)
		std::uint32_t                rva;          // expected RVA, from our disassembly of this build
		std::uint32_t                bytesOffset;  // where `bytes` must match, relative to rva
		std::uint8_t                 bytesLen;     // 0 = no byte check
		std::array<std::uint8_t, 15> bytes;        // expected bytes at rva + bytesOffset
		const char*                  label;        // stable seat name, for the log
	};

	struct Table
	{
	public:
		// members
		Version     version;  // the EXACT build this table was derived from
		const Row*  rows;
		std::size_t count;
	};

	struct Failure
	{
	public:
		// members
		const Row*  row;
		std::string reason;
	};

	class Result
	{
	public:
		// Was there a table for the running build at all?
		[[nodiscard]] bool Covered() const noexcept { return _covered; }

		[[nodiscard]] Version GameVersion() const noexcept { return _version; }

		[[nodiscard]] std::size_t Checked() const noexcept { return _checked; }

		[[nodiscard]] std::size_t Passed() const noexcept { return _verified.size(); }

		[[nodiscard]] const std::vector<Failure>& Failures() const noexcept { return _failures; }

		// True only for base + rva + bytesOffset of a row that passed (base + rva for a row
		// without an offset). An address no row covers is NOT verified: the caller refuses it.
		[[nodiscard]] bool IsVerifiedAddress(std::uintptr_t a_address) const noexcept
		{
			return std::binary_search(_verified.begin(), _verified.end(), a_address);
		}

		[[nodiscard]] bool IsVerifiedLabel(std::string_view a_label) const noexcept
		{
			return std::find(_labels.begin(), _labels.end(), a_label) != _labels.end();
		}

	private:
		friend Result Run(std::span<const Table> a_tables);

		// members
		bool                          _covered{ false };
		Version                       _version;
		std::size_t                   _checked{ 0 };
		std::vector<std::uintptr_t>   _verified;
		std::vector<std::string_view> _labels;
		std::vector<Failure>          _failures;
	};

	[[nodiscard]] inline Result Run(std::span<const Table> a_tables)
	{
		Result result;
		auto&  module = Module::get();
		result._version = module.version();

		const Table* table = nullptr;
		for (const auto& candidate : a_tables) {
			if (candidate.version == result._version) {
				table = std::addressof(candidate);
				break;
			}
		}
		if (!table) {
			return result;
		}
		result._covered = true;

		const auto  base = module.base();
		const auto& db = IDDatabase::get();
		for (std::size_t i = 0; i < table->count; ++i) {
			const auto& row = table->rows[i];
			++result._checked;

			if (row.id != 0) {
				const auto offset = db.try_id2offset(row.id);
				if (!offset) {
					result._failures.push_back({ std::addressof(row),
						fmt::format("id {} is not in the Address Library (expected RVA 0x{:X})"sv, row.id, row.rva) });
					continue;
				}
				if (*offset != row.rva) {
					result._failures.push_back({ std::addressof(row),
						fmt::format("id {} resolves to RVA 0x{:X}, our disassembly says 0x{:X}"sv, row.id, *offset, row.rva) });
					continue;
				}
			}

			if (row.bytesLen != 0) {
				const auto have = reinterpret_cast<const std::uint8_t*>(base + row.rva + row.bytesOffset);
				if (!std::equal(have, have + row.bytesLen, row.bytes.begin())) {
					std::string haveHex;
					std::string wantHex;
					for (std::size_t b = 0; b < row.bytesLen; ++b) {
						haveHex += fmt::format("{:02X}"sv, have[b]);
						wantHex += fmt::format("{:02X}"sv, row.bytes[b]);
					}
					result._failures.push_back({ std::addressof(row),
						fmt::format("bytes at RVA 0x{:X}+0x{:X} are {}, expected {}"sv, row.rva, row.bytesOffset, haveHex, wantHex) });
					continue;
				}
			}

			// A row with a byte check at an offset (a call site) verifies that exact
			// address; every other row verifies base + rva.
			result._verified.push_back(base + row.rva + row.bytesOffset);
			result._labels.emplace_back(row.label);
		}
		std::sort(result._verified.begin(), result._verified.end());
		return result;
	}
}
