#pragma once
#include "memory.hpp"

#include <map>
#include <string>
#include <utility>	
#include <vector>

class RTTI;

class ClassRTTI {
	protected:
		std::string m_mangled_name;
		std::string m_name;
		uintptr_t   m_offset;
		uint32_t    m_bases;

		ClassRTTI() = delete;
		ClassRTTI(
			std::string const&  mangled,
			std::string const&  name,
			uintptr_t    		offset,
			uint32_t    		bases
		);
	
		friend class RTTI;

	public:
		std::string const& get_name() const;
		std::string const& get_mangled_name() const;
		uintptr_t		   get_offset() const;
		uint32_t		   get_bases() const;
};

class RTTI : public MemoryMgr
{
protected:
	using rtti_t  = std::vector<ClassRTTI>;
	using pair_t  = std::pair< uint32_t, rtti_t >;
	using cache_t = std::map< uint32_t, rtti_t >;

	RTTI() :
		m_rtti_cache{},
		m_rtti      {}
	{}
	~RTTI(){}

	/// <summary>
	/// Stores a cache to avoid always getting a pointer's rtti.
	/// </summary>
	cache_t     m_rtti_cache;
	/// <summary>
	/// Temporary RTTI
	/// </summary>
	rtti_t 		m_rtti;

	/// <summary>
	/// Searches for an address in the cache.
	/// </summary>
	/// <param name="address"></param>
	/// <returns></returns>
	const bool  find_cache( const uint32_t address );
	/// <summary>
	/// Formats the symbol for better visualization.
	/// </summary>
	/// <param name="name"></param>
	/// <returns></returns>
	std::string undecorate_symbol_name( const std::string name );
	/// <summary>
	/// Searches RTTI for a specified address.
	/// </summary>
	/// <param name="object_locator"></param>
	/// <returns></returns>
	rtti_t 		read_rtti32( const uint32_t object_locator );
	/// <summary>
	/// Checks whether the address is valid and is not present in the cache.
	/// </summary>
	/// <param name="vftable"></param>
	/// <returns></returns>
	rtti_t 		read_rtti( const uint32_t vftable );

public:
	RTTI( const RTTI& )            = delete;
	RTTI& operator=( const RTTI& ) = delete;
};
