#pragma once

#include <mud/infra.h>
#include <mud/type.h>




#ifndef MUD_POOL_EXPORT
#define MUD_POOL_EXPORT MUD_IMPORT
#endif

namespace mud
{
	template <class T> class VecPool;
	template <class T> class TPool;

	template <class T> struct SparseHandle;
	template <class T> struct OwnedHandle;

	class HandlePool;
    class Pool;
    class ObjectPool;
}

#ifdef MUD_META_GENERATOR
#include <stl/vector.h>
namespace stl
{
}
#endif


#include <stl/memory.h>
#include <stl/vector.h>
//  Copyright (c) 2015 Hugo Amiard hugo.amiard@laposte.net



#include <cstddef>

namespace mud
{
	export_ class refl_ MUD_POOL_EXPORT Pool
	{
	public:
		virtual ~Pool() {}

		virtual void reset() = 0;
		virtual void clear() = 0;

		virtual void alloc(Ref& ref) = 0;
		virtual Ref alloc() = 0;

		virtual void destroy(Ref object) = 0;
		virtual void free(Ref object) = 0;
	};

	export_ template <class T>
	class TPool : public Pool
	{
	public:
		TPool(size_t size = 12);
		~TPool();

		inline T* talloc();
		inline void tdestroy(T& object);
		inline void tfree(T& object);

		virtual void alloc(Ref& ref);
		virtual Ref alloc();

		virtual void destroy(Ref object);
		virtual void free(Ref object);

		inline void reset(size_t size);
		virtual void reset();
		virtual void clear();

		template <class... Types>
		inline T& construct(Types&&... args);

		template <class T_Func>
		inline void iterate(T_Func func) const;

		template <class T_Test>
		inline T* find(T_Test test) const;

		unique<VecPool<T>> m_vec_pool;
	};
}

namespace mud
{
	export_ class MUD_POOL_EXPORT ObjectPool //
	{
	public:
		ObjectPool();

		ObjectPool(const ObjectPool& other) = delete;
		ObjectPool& operator=(const ObjectPool& other) = delete;

		void clear();

		inline Pool& pool(Type& type);

		template <class T>
		inline TPool<T>& pool();

		template <class T>
		inline TPool<T>& create_pool(size_t size = 12);

		vector<unique<Pool>> m_pools;
	};

	//export_ extern MUD_POOL_EXPORT vector<unique<Pool>> g_pool_makers;
	export_ extern MUD_POOL_EXPORT vector<unique<Pool>> g_pools;

	template <class T>
	inline TPool<T>& global_pool();
}
//  Copyright (c) 2015 Hugo Amiard hugo.amiard@laposte.net


#include <stl/unordered_map.h>
#include <stl/vector.h>

#include <stdint.h>

namespace mud
{
	export_ class refl_ MUD_POOL_EXPORT HandlePool
	{
	public:
		virtual ~HandlePool() {}
		virtual void clear() = 0;
	};

	template <class T>
	class SparsePool;

	template <class T>
	struct refl_ struct_ SparseHandle
	{
		SparseHandle();
		SparseHandle(SparsePool<T>& pool, uint32_t handle);

		explicit operator bool() const;

		void destroy();

		operator T&();
		operator const T&() const;

		T* operator->();
		T& operator*();
		const T* operator->() const;
		const T& operator*() const;

		SparsePool<T>* m_pool = nullptr;
		uint32_t m_handle = UINT32_MAX;
	};

	template <class T>
	struct OwnedHandle;

	template <class T>
	struct DestroyHandle
	{
		static void destroy(const OwnedHandle<T>& handle) { UNUSED(handle); }
	};

	template <class T>
	struct refl_ struct_ nocopy_ OwnedHandle : public SparseHandle<T>
	{
		OwnedHandle();
		OwnedHandle(SparsePool<T>& pool, uint32_t handle);
		~OwnedHandle();

		OwnedHandle(OwnedHandle& other) = delete;
		OwnedHandle& operator=(OwnedHandle& other) = delete;

		OwnedHandle(OwnedHandle&& other);
		OwnedHandle& operator=(OwnedHandle&& other);

		void swap(OwnedHandle& other);

		operator SparseHandle<T>() const;
	};

	//template <bool Dense = false>
	//using SparseIndexMap = vector<uint32_t>;
	//template <>
	//using SparseIndexMap<true> = unordered_map<uint32_t, uint32_t>;

	template <bool Dense = false>
	class SparseIndices
	{
	public:
		SparseIndices();

		void add();
		void clear();
		void erase(uint32_t handle);
		void ensure(uint32_t capacity);
		uint32_t& operator[](uint32_t at);
		uint32_t size() const;

	private:
		vector<uint32_t> m_indices;
	};

	template <>
	class SparseIndices<true>
	{
	public:
		SparseIndices();

		void add();
		void clear();
		void erase(uint32_t handle);
		void ensure(uint32_t capacity);
		uint32_t& operator[](uint32_t at);
		uint32_t size() const;

	private:
		unordered_map<uint32_t, uint32_t> m_indices;
	};

	template <bool Dense = false>
	class SparseHandles
	{
	public:
		SparseHandles();

		void ensure(uint32_t capacity);
		uint32_t alloc();
		uint32_t create();
		void add(uint32_t handle);
		uint32_t remove(uint32_t handle);
		void clear();

		uint32_t& operator[](uint32_t at);

		uint32_t size() const;
		uint32_t count() const;

		uint32_t reverse(uint32_t index) const;
		uint32_t handle(uint32_t index) const;

	private:
		SparseIndices<Dense> m_indices;
		vector<uint32_t> m_handles;
	};

	export_ template <class T>
	class SparsePool : public HandlePool
	{
	public:
		SparsePool();
		~SparsePool();

		template <class... Types>
		inline OwnedHandle<T> create(Types&&... args);

		void destroy(uint32_t handle);
		T& get(uint32_t handle);

		virtual void clear();

	public:
		SparseHandles<false> m_handles;
		vector<T> m_objects;
		vector<uint32_t> m_available;
	};
}

#include <stdint.h>
#include <stl/string.h>
#include <stl/vector.h>

#if !defined MUD_MODULES || defined MUD_TYPE_LIB
#endif

#ifndef MUD_MODULES
#endif


namespace mud
{
    // Exported types
    
    
    export_ template <> MUD_POOL_EXPORT Type& type<mud::HandlePool>();
    export_ template <> MUD_POOL_EXPORT Type& type<mud::Pool>();
}


#include <stl/vector.h>

namespace mud
{
	export_ template <class T>
	class VecPool
	{
	public:
		VecPool(size_t size = 256);
		~VecPool();

		T* alloc();
		void destroy(T* object);
		void free(T* object);

	public:
		template <class... Types>
		inline T& construct(Types&&... args);

	public:
		size_t m_size;
		vector<T*> m_available;
		vector<T*> m_objects;
		void* m_chunk;
		T* m_memory;
		T* m_last;

		unique<VecPool<T>> m_next;

		static int s_count;
	};
}



//  Copyright (c) 2015 Hugo Amiard hugo.amiard@laposte.net


#include <stl/new.h>


#include <stl/algorithm.h>

namespace mud
{
	template <class T>
	VecPool<T>::VecPool(size_t size)
		: m_size(size)
		, m_chunk(operator new(size * sizeof(T)))
		, m_memory((T*)m_chunk)
		, m_last(m_memory + size - 1)
	{
		++s_count;
		//printf("VecPool for type %s, count %u, size %u\n", type<T>().name().c_str(), s_count, size * sizeof(T));

		m_available.reserve(size);
		m_objects.reserve(size);

		for(size_t i = 0; i < size; ++i)
			m_available.push_back(&m_memory[i]);
	}

	template <class T>
	VecPool<T>::~VecPool()
	{
		--s_count;

		for(T* object : m_objects)
			any_destruct(*object);
		operator delete(m_chunk);
	}

	template <class T>
	T* VecPool<T>::alloc()
	{
		if(m_available.empty() && !m_next)
			m_next = make_unique<VecPool<T>>(m_size * 2);
		if(m_available.empty())
			return m_next->alloc();

		T* object = m_available.back();
		m_available.pop_back();
		m_objects.push_back(object);
		return object;
	}

	template <class T>
	void VecPool<T>::destroy(T* object)
	{
		any_destruct(*object);
		this->free(object);
	}

	template <class T>
	void VecPool<T>::free(T* object)
	{
		if(object < m_memory || object > m_last)
			return m_next->free(object);

		m_available.push_back(object);
		remove(m_objects, object);
	}

	template <class T>
	template <class... Types>
	inline T& VecPool<T>::construct(Types&&... args)
	{
		T* at = this->alloc();
		new (at) T(static_cast<Types&&>(args)...);
		return *at;
	}

	template <class T>
	int VecPool<T>::s_count = 0;
}

namespace mud
{
	template <class T>
	TPool<T>::TPool(size_t size) : m_vec_pool(make_unique<VecPool<T>>(size)) {}
	template <class T>
	TPool<T>::~TPool() {}

	template <class T>
	inline T* TPool<T>::talloc() { return m_vec_pool->alloc(); }
	template <class T>
	inline void TPool<T>::tdestroy(T& object) { m_vec_pool->destroy(&object); }
	template <class T>
	inline void TPool<T>::tfree(T& object) { m_vec_pool->free(&object); }

	template <class T>
	void TPool<T>::alloc(Ref& ref) { setval<T>(ref, m_vec_pool->alloc()); }
	template <class T>
	Ref TPool<T>::alloc() { return Ref(m_vec_pool->alloc(), type<T>()); }
	template <class T>
	void TPool<T>::destroy(Ref object) { m_vec_pool->destroy(&val<T>(object)); }
	template <class T>
	void TPool<T>::free(Ref object) { m_vec_pool->free(&val<T>(object)); }

	template <class T>
	inline void TPool<T>::reset(size_t size) { m_vec_pool = make_unique<VecPool<T>>(size); }
	template <class T>
	void TPool<T>::reset() { m_vec_pool = make_unique<VecPool<T>>(12); }
	template <class T>
	void TPool<T>::clear() { m_vec_pool = nullptr; }

	template <class T>
	template <class... Types>
	inline T& TPool<T>::construct(Types&&... args)
	{
		T* at = this->talloc();
		new (stl::placeholder(), at) T(static_cast<Types&&>(args)...);
		return *at;
	}

	template <class T>
	template <class T_Func>
	inline void TPool<T>::iterate(T_Func func) const
	{
		VecPool<T>* pool = m_vec_pool.get();
		for(; pool; pool = pool->m_next.get())
			for(T* object : pool->m_objects)
				func(*object);
	}

	template <class T>
	template <class T_Test>
	inline T* TPool<T>::find(T_Test test) const
	{
		VecPool<T>* pool = m_vec_pool.get();
		for(; pool; pool = pool->m_next.get())
			for(T* object : pool->m_objects)
				if(test(*object))
					return object;
		return nullptr;
	}
}

namespace mud
{
	inline Pool& ObjectPool::pool(Type& type) { return *m_pools[type.m_id].get(); }

	template <class T>
	inline TPool<T>& ObjectPool::pool()
	{
		if(!m_pools[type<T>().m_id])
			m_pools[type<T>().m_id] = make_unique<TPool<T>>();
		return as<TPool<T>>(*m_pools[type<T>().m_id].get());
	}

	template <class T>
	inline TPool<T>& ObjectPool::create_pool(size_t size)
	{
		m_pools[type<T>().m_id] = make_unique<TPool<T>>(size);
		return pool<T>();
	}

	template <class T>
	inline TPool<T>& global_pool()
	{
		if(!g_pools[type<T>().m_id])
			g_pools[type<T>().m_id] = make_unique<TPool<T>>();
		return as<TPool<T>>(*g_pools[type<T>().m_id].get());
	}
}
//  Copyright (c) 2015 Hugo Amiard hugo.amiard@laposte.net


#include <stl/algorithm.h>
#include <stl/swap.h>

namespace mud
{
	template <class T>
	inline SparseHandle<T>::SparseHandle() {}
	template <class T>
	inline SparseHandle<T>::SparseHandle(SparsePool<T>& pool, uint32_t handle) : m_pool(&pool), m_handle(handle) {}

	template <class T>
	inline void SparseHandle<T>::destroy() { if(m_pool && m_handle != UINT32_MAX) { DestroyHandle<T>::destroy(*this); m_pool->destroy(m_handle); } }

	template <class T>
	inline SparseHandle<T>::operator bool() const { return m_handle != UINT32_MAX; }
	template <class T>
	inline SparseHandle<T>::operator T&() { return m_pool->get(m_handle); }
	template <class T>
	inline SparseHandle<T>::operator const T&() const { return m_pool->get(m_handle); }
	template <class T>
	inline T* SparseHandle<T>::operator->() { return &((T&)*this); }
	template <class T>
	inline T& SparseHandle<T>::operator*() { return *this; }
	template <class T>
	inline const T*SparseHandle<T>::operator->() const { return &((const T&)*this); }
	template <class T>
	inline const T& SparseHandle<T>::operator*() const { return *this; }

	template <class T>
	inline OwnedHandle<T>::OwnedHandle() {}
	template <class T>
	inline OwnedHandle<T>::OwnedHandle(SparsePool<T>& pool, uint32_t handle) : SparseHandle<T>(pool, handle) {}
	template <class T>
	inline OwnedHandle<T>::~OwnedHandle() { this->destroy(); }

	template <class T>
	inline OwnedHandle<T>::OwnedHandle(OwnedHandle&& other) { other.swap(*this); }
	template <class T>
	inline OwnedHandle<T>& OwnedHandle<T>::operator=(OwnedHandle&& other) { other.swap(*this); return *this; }

	template <class T>
	inline void OwnedHandle<T>::swap(OwnedHandle& other) { using stl::swap; swap(this->m_handle, other.m_handle); swap(this->m_pool, other.m_pool); }

	template <class T>
	inline OwnedHandle<T>::operator SparseHandle<T>() const { return { *this->m_pool, this->m_handle }; }
	

	template <> inline SparseIndices<false>::SparseIndices() {}
	template <> inline void SparseIndices<false>::add() { m_indices.emplace_back(); }
	template <> inline void SparseIndices<false>::clear() { m_indices.resize(m_indices.size(), UINT32_MAX); }
	template <> inline void SparseIndices<false>::erase(uint32_t handle) { m_indices[handle] = UINT32_MAX; }
	template <> inline void SparseIndices<false>::ensure(uint32_t capacity) { m_indices.resize(capacity, UINT32_MAX); }
	template <> inline uint32_t& SparseIndices<false>::operator[](uint32_t at) { return m_indices[at]; }
	template <> inline uint32_t SparseIndices<false>::size() const { return uint32_t(m_indices.size()); }

	inline SparseIndices<true>::SparseIndices() {}
	inline void SparseIndices<true>::add() {}
	inline void SparseIndices<true>::clear() { m_indices.clear(); }
	inline void SparseIndices<true>::erase(uint32_t handle) { m_indices.erase(handle); }
	inline void SparseIndices<true>::ensure(uint32_t capacity) { UNUSED(capacity); }
	inline uint32_t& SparseIndices<true>::operator[](uint32_t at) { return m_indices[at]; }
	inline uint32_t SparseIndices<true>::size() const { return uint32_t(m_indices.size()); }

	template <bool Dense>
	inline SparseHandles<Dense>::SparseHandles() {}
	template <bool Dense>
	inline void SparseHandles<Dense>::ensure(uint32_t capacity)
	{
		m_indices.ensure(capacity);
	}

	template <bool Dense>
	inline uint32_t SparseHandles<Dense>::alloc()
	{
		const uint32_t count = uint32_t(m_indices.size());
		m_indices.add();
		return count;
	}

	template <bool Dense>
	inline uint32_t SparseHandles<Dense>::create()
	{
		uint32_t handle = this->alloc();
		this->add(handle);
		return handle;
	}

	template <bool Dense>
	inline void SparseHandles<Dense>::add(uint32_t handle)
	{
		const uint32_t index = uint32_t(m_handles.size());
		m_indices[handle] = index;
		m_handles.push_back(handle);
	}

	template <bool Dense>
	inline uint32_t SparseHandles<Dense>::remove(uint32_t handle)
	{
		const uint32_t index = m_indices[handle];
		swap_pop(m_handles, index);

		const uint32_t moved = m_handles[index];
		m_indices[moved] = index;
		m_indices.erase(handle);
		return index;
	}

	template <bool Dense>
	inline void SparseHandles<Dense>::clear()
	{
		m_indices.clear();
		m_handles.clear();
	}

	template <bool Dense>
	inline uint32_t& SparseHandles<Dense>::operator[](uint32_t at) { return m_indices[at]; }

	template <bool Dense>
	inline uint32_t SparseHandles<Dense>::size() const { return uint32_t(m_handles.size()); }

	template <bool Dense>
	inline uint32_t SparseHandles<Dense>::count() const { return uint32_t(m_handles.size()); }

	template <bool Dense>
	inline uint32_t SparseHandles<Dense>::reverse(uint32_t index) const { return m_handles[index]; }

	template <bool Dense>
	inline uint32_t SparseHandles<Dense>::handle(uint32_t index) const { return m_handles[index]; }

	template <class T>
	inline SparsePool<T>::SparsePool() {}
	template <class T>
	inline SparsePool<T>::~SparsePool() {}

	template <class T>
	template <class... Types>
	inline OwnedHandle<T> SparsePool<T>::create(Types&&... args)
	{
		uint32_t handle = m_available.empty() ? m_handles.alloc() : pop(m_available);
		m_objects.push_back(T(static_cast<Types&&>(args)...));
		m_handles.add(handle);
		return { *this, handle };
	}

	template <class T>
	inline void SparsePool<T>::destroy(uint32_t handle)
	{
		uint32_t index = m_handles.remove(handle);
		swap_pop(m_objects, index);
		m_available.push_back(handle);
	}

	template <class T>
	inline T& SparsePool<T>::get(uint32_t handle)
	{
		uint32_t index = m_handles[handle];
		return m_objects[index];
	}

	template <class T>
	inline void SparsePool<T>::clear()
	{
		m_handles.clear();
		m_objects.clear();
		m_available.clear();
	}
}
