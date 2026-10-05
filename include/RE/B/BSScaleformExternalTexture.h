#pragma once

#include "RE/B/BSFixedString.h"
#include "RE/N/NiTexture.h"

namespace RE
{
	class BSScaleformExternalTexture
	{
	public:
		BSScaleformExternalTexture() :
			gamebryoTexture{ nullptr },
			renderTarget(0),
			pad0C(0)
		{}

		~BSScaleformExternalTexture()
		{
			ReleaseTexture();
		}

		BSScaleformExternalTexture(const BSScaleformExternalTexture&) = delete;
		BSScaleformExternalTexture(BSScaleformExternalTexture&&) = delete;
		BSScaleformExternalTexture& operator=(const BSScaleformExternalTexture&) = delete;
		BSScaleformExternalTexture& operator=(BSScaleformExternalTexture&&) = delete;

		bool LoadPNG(const BSFixedString& a_path)
		{
			using func_t = decltype(&BSScaleformExternalTexture::LoadPNG);
			REL::Relocation<func_t> func{ RELOCATION_ID(80298, 82321) };
			return func(this, a_path);
		}

		void ReleaseTexture()
		{
			using func_t = decltype(&BSScaleformExternalTexture::ReleaseTexture);
			// mit-3.7: AE id 82317 up to 1.6.659, 443440 from 1.6.1130 on (the library renumbered it). 1.6.1170
			// 0xFAF570 is the 1.5.97 function (0xECDEE0) instruction for instruction (data/idmap-1.7.104-fixes.csv).
			REL::Relocation<func_t> func{ REL::RelocationIDByBuild(80294, 82317, 443440) };
			return func(this);
		}

		bool SetTexture(NiTexture* a_texture)
		{
			using func_t = decltype(&BSScaleformExternalTexture::SetTexture);
			REL::Relocation<func_t> func{ RELOCATION_ID(80295, 82318) };
			return func(this, a_texture);
		}

		// members
		NiPointer<NiTexture> gamebryoTexture;  // 00
		std::uint32_t        renderTarget;     // 08
		std::uint32_t        pad0C;            // 0C
		BSFixedString        filePath;         // 10
	};
	static_assert(sizeof(BSScaleformExternalTexture) == 0x18);
}
