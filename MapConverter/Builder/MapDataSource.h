//	Tiled Map Converter for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#pragma once
#include "Builder/DataSource.h"
#include <Tiled/Map.h>


namespace Builder
{

	class MapDataSource : public DataSource
	{
	private:

		class ObjectDescriptor
		{
		public:

			virtual ~ObjectDescriptor() = default;
		};


	public:
	
		using layer_type = KAOS::Tiled::Layer;
		using map_type = KAOS::Tiled::Map;
		using objectlayer_type = KAOS::Tiled::ObjectGroupLayer;
		using tilesetlayer_type = KAOS::Tiled::TilesetLayer;
		using objectdescriptor_type = ObjectDescriptor;


	public:

		explicit MapDataSource(
			const std::shared_ptr<map_type> map,
			std::shared_ptr<tilesetcache_type> tilesetCache,
			std::shared_ptr<const descriptors_type> descriptors,
			Configuration configuration);

		std::optional<property_type> QueryProperty(const std::string& name) const override;

		virtual std::shared_ptr<objectdescriptor_type> QueryObjectDescriptor(const std::string& name) const;

		virtual std::shared_ptr<layer_type> QueryLayer(const std::string& name) const;
		virtual std::shared_ptr<objectlayer_type> QueryObjectLayer(const std::string& name) const;
		virtual std::shared_ptr<tilesetlayer_type> QueryTilesetLayer(const std::string& name) const;
		virtual std::shared_ptr<tilesetlayer_type> QueryTilesetLayer(uint64_t index) const;
	};


	template<class ObjectSourceType_>
	class MapDataSourceEx : public MapDataSource
	{
	public:

		MapDataSourceEx(
			ObjectSourceType_& objectSource,
			const std::shared_ptr<map_type> map,
			std::shared_ptr<tilesetcache_type> tilesetCache,
			std::shared_ptr<const descriptors_type> descriptors,
			Configuration configuration)
			:
			MapDataSource(move(map), move(tilesetCache), move(descriptors), std::move(configuration)),
			m_ObjectSource(objectSource)
		{}


		std::shared_ptr<objectdescriptor_type> QueryObjectDescriptor(const std::string& name) const override
		{
			return m_ObjectSource.Query(name);
		}


	protected:

		ObjectSourceType_& m_ObjectSource;
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
