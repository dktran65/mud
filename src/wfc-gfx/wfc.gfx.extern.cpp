#ifndef USE_STL
#ifdef MUD_MODULES
module mud.wfc.gfx;
#else
#include <stl/vector.hpp>
#include <wfc-gfx/Api.h>
#endif

namespace stl
{
	using namespace mud;
	template class MUD_WFC_GFX_EXPORT vector<TileModel>;
}
#endif
