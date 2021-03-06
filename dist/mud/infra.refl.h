#pragma once

#include <mud/refl.h>
#include <mud/infra.h>


#if !defined MUD_MODULES || defined MUD_TYPE_LIB
#endif

//#include <infra/Types.h>

#ifndef MUD_INFRA_REFL_EXPORT
#define MUD_INFRA_REFL_EXPORT MUD_IMPORT
#endif

namespace mud
{
	export_ class MUD_INFRA_REFL_EXPORT mud_infra : public mud::Module
	{
	private:
		mud_infra();
		
	public:
		static mud_infra& m() { static mud_infra instance; return instance; }
	};
}

#ifdef MUD_INFRA_MODULE
extern "C"
MUD_INFRA_REFL_EXPORT Module& getModule();
#endif


#if !defined MUD_MODULES || defined MUD_TYPE_LIB
#endif

namespace mud
{
	
}
