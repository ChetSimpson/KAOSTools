//	Tiled Map Converter for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#pragma once
#include <Tiled/PropertyBag.h>
#include <Tiled/TilesetCache.h>
#include <Tiled/Map.h>
#include "Configuration.h"


namespace DescriptorNodes
{
	class Root;
}

namespace Builder
{

	class DataSource
	{
	public:

		using property_type = KAOS::Tiled::PropertyBag::value_type;
		using tilesetcache_type = KAOS::Tiled::TilesetCache;
		using map_type = KAOS::Tiled::Map;
		using descriptors_type = DescriptorNodes::Root;

	public:

		DataSource(
			std::shared_ptr<map_type> map,
			std::shared_ptr<tilesetcache_type> tilesetCache,
			std::shared_ptr<const descriptors_type> descriptors,
			Configuration configuration);

		virtual ~DataSource() = default;

		virtual std::optional<property_type> QueryProperty(const std::string& name) const = 0;
		virtual std::shared_ptr<tilesetcache_type> QueryTilesetCache() const;
		virtual std::shared_ptr<map_type> QueryMap() const;
		virtual std::shared_ptr<const descriptors_type> QueryDescriptors() const;


	protected:

		std::shared_ptr<map_type>				m_Map;
		std::shared_ptr<tilesetcache_type>		m_TilesetCache;
		std::shared_ptr<const descriptors_type> m_Descriptors;
		Configuration							m_Configuration;
	};

}




//	Copyright (c) 2018 Chet Simpson
//	
//	Permission is hereby granted, free of charge, to any person
//	obtaining a copy of this software and associated documentation
//	files (the "Software"), to deal in the Software without
//	restriction, including without limitation the rights to use,
//	copy, modify, merge, publish, distribute, sublicense, and/or sell
//	copies of the Software, and to permit persons to whom the
//	Software is furnished to do so, subject to the following
//	conditions:
//	
//	The above copyright notice and this permission notice shall be
//	included in all copies or substantial portions of the Software.
//	
//	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//	EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
//	OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
//	NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
//	HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
//	WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//	FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
//	OTHER DEALINGS IN THE SOFTWARE.
