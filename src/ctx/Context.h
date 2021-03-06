//  Copyright (c) 2019 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#pragma once

#ifndef MUD_MODULES
#include <stl/string.h>
#include <type/Unique.h>
#include <math/Vec.h>
#endif
#include <ctx/Forward.h>

namespace mud
{
	using cstring = const char*;

	export_ class refl_ MUD_CTX_EXPORT RenderSystem
	{
	public:
		RenderSystem(const string& resource_path, bool manual_render);
		virtual ~RenderSystem() {}

		virtual void begin_frame() = 0;
		virtual bool next_frame() = 0;

		virtual object<Context> create_context(const string& name, uvec2 size, bool fullScreen) = 0;
		
		const string m_resource_path;
		const bool m_manual_render;
	};

	export_ class refl_ MUD_CTX_EXPORT Context
	{
	public:
		Context(RenderSystem& render_system, const string& title, uvec2 size, bool full_screen = false);
		virtual ~Context();

		RenderSystem& m_render_system;
		attr_ const string m_resource_path;

		attr_ string m_title;
		attr_ uvec2 m_size;
		attr_ uvec2 m_fb_size;
		attr_ bool m_full_screen;

		attr_ float m_pixel_ratio;

		size_t m_handle = 0;
		void* m_native_handle = nullptr;
		void* m_native_target = nullptr;

		attr_ bool m_active = true;
		attr_ bool m_shutdown = false;

		attr_ vec2 m_cursor;
		attr_ bool m_mouse_lock = false;

		meth_ virtual void reset_fb(const uvec2& size) = 0;
		meth_ virtual void init_input(Mouse& mouse, Keyboard& keyboard) = 0;

		meth_ virtual bool next_frame() = 0;

		meth_ virtual void lock_mouse(bool locked) = 0;
	};

}
