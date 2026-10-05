#include "REL/ID.h"

#include "REX/W32/KERNEL32.h"

#ifdef ENABLE_SKYRIM_VR
#include <rapidcsv.h>
#endif

namespace REL
{
	namespace detail
	{
		bool memory_map::open(stl::zwstring a_name, std::size_t a_size)
		{
			close();

			REX::W32::ULARGE_INTEGER bytes;
			bytes.value = a_size;

			_mapping = REX::W32::OpenFileMappingW(
				REX::W32::FILE_MAP_READ | REX::W32::FILE_MAP_WRITE,
				false,
				a_name.data());
			if (!_mapping) {
				close();
				return false;
			}

			_view = REX::W32::MapViewOfFile(
				_mapping,
				REX::W32::FILE_MAP_READ | REX::W32::FILE_MAP_WRITE,
				0,
				0,
				bytes.value);
			if (!_view) {
				close();
				return false;
			}

			return true;
		}

		bool memory_map::create(stl::zwstring a_name, std::size_t a_size)
		{
			close();

			REX::W32::ULARGE_INTEGER bytes;
			bytes.value = a_size;

			_mapping = REX::W32::OpenFileMappingW(
				REX::W32::FILE_MAP_READ | REX::W32::FILE_MAP_WRITE,
				false,
				a_name.data());
			if (!_mapping) {
				_mapping = REX::W32::CreateFileMappingW(
					REX::W32::INVALID_HANDLE_VALUE,
					nullptr,
					REX::W32::PAGE_READWRITE,
					bytes.hi,
					bytes.lo,
					a_name.data());
				if (!_mapping) {
					return false;
				}
			}

			_view = REX::W32::MapViewOfFile(
				_mapping,
				REX::W32::FILE_MAP_READ | REX::W32::FILE_MAP_WRITE,
				0,
				0,
				bytes.value);
			if (!_view) {
				return false;
			}

			return true;
		}

		void memory_map::close()
		{
			if (_view) {
				REX::W32::UnmapViewOfFile(static_cast<const void*>(_view));
				_view = nullptr;
			}

			if (_mapping) {
				REX::W32::CloseHandle(_mapping);
				_mapping = nullptr;
			}
		}
	}

	IDDatabase IDDatabase::_instance;

	bool IDDatabase::load_file(stl::zwstring a_filename, Version a_version, std::uint8_t a_formatVersion, bool a_failOnError)
	{
		try {
			istream_t in(a_filename.data(), std::ios::in | std::ios::binary);
			header_t header;
			header.read(in, a_formatVersion);
			if (header.version() != a_version) {
				return stl::report_and_error("version mismatch"sv, a_failOnError);
			}

			auto mapname = L"CommonLibSSEOffsets-v2-"s;
			mapname += a_version.wstring();
			const auto byteSize = static_cast<std::size_t>(header.address_count()) * sizeof(mapping_t);
			if (_mmap.open(mapname, byteSize)) {
				_id2offset = { static_cast<mapping_t*>(_mmap.data()), header.address_count() };
			} else if (_mmap.create(mapname, byteSize)) {
				_id2offset = { static_cast<mapping_t*>(_mmap.data()), header.address_count() };
				unpack_file(in, header, a_failOnError);
				std::sort(_id2offset.begin(), _id2offset.end(), [](auto&& a_lhs, auto&& a_rhs) {
					return a_lhs.id < a_rhs.id;
				});
			} else {
				return stl::report_and_error("failed to create shared mapping"sv, a_failOnError);
			}
		} catch (const std::system_error &) {
			return stl::report_and_error(
					std::format(
							"Failed to locate an appropriate address library with the path: {}\n"
							"This means you are missing the address library for this specific version of "
							"the game. Please continue to the mod page for address library to download "
							"an appropriate version. If one is not available, then it is likely that "
							"address library has not yet added support for this version of the game."sv,
							stl::utf16_to_utf8(a_filename).value_or("<unknown filename>"s)), a_failOnError);
			return false;
		}

		return true;
	}

#ifdef ENABLE_SKYRIM_VR
	bool IDDatabase::load_csv(stl::zwstring a_filename, Version a_version, bool a_failOnError)
	{
		auto nstring = SKSE::stl::utf16_to_utf8(a_filename).value_or(""s);
		if (!std::filesystem::exists(nstring)) {
			return stl::report_and_error(
					std::format("Required VR Address Library file {} does not exist"sv, nstring),
					a_failOnError);
		}

		rapidcsv::Document in(nstring);
		std::size_t id, address_count;
		std::string version, offset;
		auto mapname = L"CommonLibSSEOffsets-v2-"s;
		mapname += a_version.wstring();
		address_count = in.GetCell<std::size_t>(0, 0);
		version = in.GetCell<std::string>(1, 0);
		const auto byteSize = static_cast<std::size_t>(address_count * sizeof(mapping_t));
		if (!_mmap.open(mapname, byteSize) &&
			!_mmap.create(mapname, byteSize)) {
			return stl::report_and_error("failed to create shared mapping"sv, a_failOnError);
		}

		_id2offset = {static_cast<mapping_t *>(_mmap.data()), static_cast<std::size_t>(address_count)};
		if (in.GetRowCount() > address_count + 1) {
			return stl::report_and_error(
					std::format("VR Address Library {} tried to exceed {} allocated entries."sv,
								version, address_count), a_failOnError);
		} else if (in.GetRowCount() < address_count + 1) {
			return stl::report_and_error(
					std::format("VR Address Library {} loaded only {} entries but expected {}. Please redownload."sv,
								version, in.GetRowCount() - 1, address_count), a_failOnError);
		}

		std::size_t index = 1;
		for (; index < in.GetRowCount(); ++index) {
			id = in.GetCell<std::size_t>(0, index);
			offset = in.GetCell<std::string>(1, index);
			_id2offset[index - 1] = {static_cast<std::uint64_t>(id),
										static_cast<std::uint64_t>(std::stoul(offset, nullptr, 16))};
		}

		std::sort(_id2offset.begin(),_id2offset.end(), [](auto&& a_lhs, auto&& a_rhs) {
			return a_lhs.id < a_rhs.id;
		});

		return true;
	}
#endif

	// mit-3.7: the MIT id table reader for 1.7.104.0 (docs/MIT-ID-TABLE-FORMAT.md). See ID.h.
	void IDDatabase::load_mit_table(Version a_version)
	{
            if (a_version != Version(1, 7, 104, 0)) {
                stl::report_and_fail(
                        std::format(
                                "Skyrim {} is not supported by this build of CommonLibSSE-NG (mit-3.7).\n"
                                "Of the 1.7 versions, only 1.7.104.0 is verified, and only it has an id table. "
                                "No address library was loaded and no address was guessed."sv,
                                a_version.string(".")));
            }

            const auto path = mit_table_path(a_version);
            const auto fail = [&](std::string_view a_why) {
                stl::report_and_fail(
                        std::format(
                                "The id table {} cannot be used: {}.\n"
                                "On Skyrim {} this plugin reads its addresses from that file, not from the Address "
                                "Library. Install or update the MIT id table for Skyrim {} (one standalone download "
                                "that every plugin using it shares). If it is installed and current, report this to "
                                "the plugin's author."sv,
                                path, a_why, a_version.string("."), a_version.string(".")));
            };

            std::vector<std::uint8_t> data;
            {
                std::ifstream in(path, std::ios::in | std::ios::binary);
                if (!in.is_open()) {
                    fail("the file is missing or cannot be opened"sv);
                }
                in.seekg(0, std::ios::end);
                const auto size = static_cast<std::streamoff>(in.tellg());
                if (size < 0 || size > static_cast<std::streamoff>(256u * 1024u * 1024u)) {
                    fail("its size cannot be read, or is over 256 MB"sv);
                }
                data.resize(static_cast<std::size_t>(size));
                in.seekg(0, std::ios::beg);
                if (!data.empty() && !in.read(reinterpret_cast<char*>(data.data()), static_cast<std::streamsize>(data.size()))) {
                    fail("reading it failed"sv);
                }
            }

            constexpr std::size_t minHeaderSize = 64;
            constexpr std::size_t minRecordSize = 16;
            constexpr std::size_t trailerSize = 8;
            constexpr std::uint16_t formatMajor = 1;
            const auto u16at = [&](std::size_t a_off) { std::uint16_t v; std::memcpy(&v, data.data() + a_off, sizeof(v)); return v; };
            const auto u32at = [&](std::size_t a_off) { std::uint32_t v; std::memcpy(&v, data.data() + a_off, sizeof(v)); return v; };
            const auto u64at = [&](std::size_t a_off) { std::uint64_t v; std::memcpy(&v, data.data() + a_off, sizeof(v)); return v; };

            // 1. Size, magic, format, sizes, flags (the order of the format note).
            if (data.size() < minHeaderSize + trailerSize) {
                fail(std::format("it is {} bytes, smaller than a header and a checksum"sv, data.size()));
            }
            if (std::memcmp(data.data(), "MITIDTAB", 8) != 0) {
                fail("it is not an MIT id table (bad magic)"sv);
            }
            if (const auto major = u16at(8); major != formatMajor) {
                fail(std::format("it is format {}.{}, this library reads format {}.x"sv, major, u16at(10), formatMajor));
            }
            const std::size_t headerSize = u32at(12);
            const std::size_t recordSize = u32at(36);
            if (headerSize < minHeaderSize || recordSize < minRecordSize || headerSize > data.size() || recordSize > 4096) {
                fail(std::format("its header size {} or record size {} is not valid (at least {} and {})"sv,
                                 headerSize, recordSize, minHeaderSize, minRecordSize));
            }
            if (const auto flags = u32at(40); flags != 0) {
                fail(std::format("it sets flags {:X} this library does not know"sv, flags));
            }
            const std::size_t count = u32at(32);
            if (data.size() != headerSize + recordSize * count + trailerSize) {
                fail(std::format("it is {} bytes, but {} records need exactly {}"sv,
                                 data.size(), count, headerSize + recordSize * count + trailerSize));
            }
            // 2. Checksum.
            {
                std::uint64_t hash = 0xCBF29CE484222325ull;
                for (std::size_t i = 0; i < data.size() - trailerSize; ++i) {
                    hash ^= data[i];
                    hash *= 0x100000001B3ull;
                }
                if (hash != u64at(data.size() - trailerSize)) {
                    fail("its checksum does not match (the file is damaged)"sv);
                }
            }
            const auto revision = u32at(44);
            if (revision == 0) {
                fail("its table revision is 0"sv);
            }
            // 3. The executable: version, module name, PE stamp and image size.
            const Version fileVersion(u16at(16), u16at(18), u16at(20), u16at(22));
            if (fileVersion != a_version) {
                fail(std::format("it is for game version {}"sv, fileVersion.string(".")));
            }
            {
                char name[17]{};
                std::memcpy(name, data.data() + 48, 16);
                const auto exe = stl::utf16_to_utf8(Module::get().filename()).value_or(""s);
                const std::string_view want(name);
                if (exe.size() != want.size() ||
                    !std::equal(exe.begin(), exe.end(), want.begin(), [](char a_l, char a_r) {
                        const auto lower = [](char a_c) { return (a_c >= 'A' && a_c <= 'Z') ? static_cast<char>(a_c - 'A' + 'a') : a_c; };
                        return lower(a_l) == lower(a_r);
                    })) {
                    fail(std::format("it is for {}, the game is {}"sv, want, exe));
                }
            }
            // The running image's own PE headers (mapped at the module base).
            const auto base = reinterpret_cast<const std::uint8_t*>(Module::get().base());
            std::uint32_t peOffset;
            std::memcpy(&peOffset, base + 0x3C, sizeof(peOffset));
            std::uint32_t signature, timeDateStamp, sizeOfImage;
            std::memcpy(&signature, base + peOffset, sizeof(signature));
            std::memcpy(&timeDateStamp, base + peOffset + 8, sizeof(timeDateStamp));
            std::memcpy(&sizeOfImage, base + peOffset + 24 + 56, sizeof(sizeOfImage));
            if (signature != 0x00004550u) {
                fail("the game executable's PE header could not be read"sv);
            }
            if (timeDateStamp != u32at(24) || sizeOfImage != u32at(28)) {
                fail(std::format(
                        "it was built from a different {} executable (file: TimeDateStamp {:08X}, SizeOfImage {:X}; "
                        "game: {:08X}, {:X}). It is for the Steam build, and the same version number can be another "
                        "build"sv,
                        a_version.string("."), u32at(24), u32at(28), timeDateStamp, sizeOfImage));
            }
            // 4. Records: strictly ascending ids, every RVA inside the image (0 marks an id known
            // not to exist in it). Bytes past the first 16 of a record belong to a later minor
            // and are skipped.
            _mitTable = std::make_unique<mapping_t[]>(count);
            std::uint64_t prev = 0;
            for (std::size_t i = 0; i < count; ++i) {
                const auto at = headerSize + recordSize * i;
                const auto id = u64at(at);
                const auto rva = u64at(at + 8);
                if (i != 0 && id <= prev) {
                    fail(std::format("its records are not strictly sorted by id (id {} after {})"sv, id, prev));
                }
                if (rva >= sizeOfImage) {  // 0 is an absent record: known not to exist
                    fail(std::format("id {} has RVA {:X}, outside the executable"sv, id, rva));
                }
                _mitTable[i] = { id, rva };
                prev = id;
            }

            _mitTableRevision = revision;
            _id2offset = { _mitTable.get(), count };
            if (const auto required = _mitRequiredRevision.load(std::memory_order_relaxed); required > revision) {
                fail_old_revision(required);
            }
        }

	void IDDatabase::fail_old_revision(std::uint32_t a_required) const
	{
            const auto version = Module::get().version();
            stl::report_and_fail(
                    std::format(
                            "The id table {} is revision {}, and this plugin needs revision {} or newer.\n"
                            "Update the MIT id table for Skyrim {} (one standalone download that every plugin using "
                            "it shares). A plugin must not ship its own copy of the table."sv,
                            mit_table_path(version), _mitTableRevision, a_required, version.string(".")));
        }
}
