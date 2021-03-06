#pragma once

#if !defined MUD_MODULES || defined MUD_TYPE_LIB
#include <refl/Module.h>
#endif

#include <gfx-pbr/Forward.h>
//#include <gfx-pbr/Types.h>

#ifndef MUD_GFX_PBR_REFL_EXPORT
#define MUD_GFX_PBR_REFL_EXPORT MUD_IMPORT
#endif

namespace mud
{
	export_ class MUD_GFX_PBR_REFL_EXPORT mud_gfx_pbr : public mud::Module
	{
	private:
		mud_gfx_pbr();
		
	public:
		static mud_gfx_pbr& m() { static mud_gfx_pbr instance; return instance; }
	};
}

#ifdef MUD_GFX_PBR_MODULE
extern "C"
MUD_GFX_PBR_REFL_EXPORT Module& getModule();
#endif
