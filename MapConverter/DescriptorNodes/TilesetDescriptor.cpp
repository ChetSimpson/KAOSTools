//	Tiled Map Converter for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#include "DescriptorNodes/TilesetDescriptor.h"
#include "DescriptorNodes/Object.h"
#include "DescriptorNodes/Root.h"
#include "Builder/MapDataSource.h"
#include "Builder/ObjectDataSource.h"
#include "Builder/TileDataSource.h"
#include <KAOS/Common/xml.h>
#include <KAOS/Common/Logging.h>


namespace DescriptorNodes
{

	bool TilesetDescriptor::Parse(const pugi::xml_node& node)
	{
		decltype(m_Signature) signature;
		KAOS::Common::XML::LoadAttribute(node, "signature", signature);

		decltype(m_SourceName) sourceName;
		if (   !KAOS::Common::XML::LoadAttribute(node, "src", sourceName)
			|| !DescriptorNode::Parse(node))
		{
			return false;
		}

		m_Signature = move(signature);
		m_SourceName = move(sourceName);

		return true;
	}




	bool TilesetDescriptor::CompileDefinition(definitionbuilder_type& /*builder*/) const
	{
		return true;
	}

	bool TilesetDescriptor::CompileInstance(databuilder_type& builder, const datasource_type& dataSource, const Configuration& configuration) const
	{
		return
			CompileInstanceHeader(builder, dataSource, configuration)
			&& CompileInstanceBody(builder, dataSource, configuration)
			&& CompileInstanceFooter(builder, dataSource, configuration);
	}


	bool TilesetDescriptor::CompileInstanceHeader(databuilder_type& builder, const datasource_type& /*dataSource*/, const Configuration& /*configuration*/) const
	{
		builder.EmitBlank();
		builder.EmitSeparatorComment();
		builder.EmitComment("Tileset attributes");
		builder.EmitSeparatorComment();
		if (!GetSymbol().empty())
		{
			builder.EmitLabel(GetSymbol());
		}

		const signature_type signature(m_Signature.has_value() ? static_cast<uint16_t>(m_Signature.value()) : Signature);
		builder.EmitValue(std::string(), signature, "Block signature");

		return true;
	}


	bool TilesetDescriptor::CompileInstanceBody(databuilder_type& builder, const datasource_type& dataSource, const Configuration& configuration) const
	{
		auto tilesetCache(dataSource.QueryTilesetCache());
		if (!tilesetCache)
		{
			return false;
		}

		auto map(dataSource.QueryMap());
		if (!map)
		{
			return false;
		}

		auto descriptors(dataSource.QueryDescriptors());
		if (!descriptors)
		{
			return false;
		}

		const auto tileDescriptor(descriptors->QueryTileDescriptor(m_SourceName));
		if (!tileDescriptor)
		{
			return false;
		}

		for (const auto& tilesetDescriptor : map->GetTilesets())
		{
			auto tilesetPtr(tilesetCache->Load(tilesetDescriptor.GetSource()));
			if (!tilesetPtr.has_value() || !*tilesetPtr)
			{
				KAOS::Logging::Error("Unable to load tileset from `" + tilesetDescriptor.GetSource() + "`\n");
				return false;
			}

			const auto& tileset(**tilesetPtr);
			std::vector<KAOS::Tiled::Tile> tiles(tileset.GetTileCount());
			for (const auto& tile : tileset.GetTiles())
			{
				tiles[tile.first] = tile.second;
			}


			static const Builder::DataBuilder::property_type::word_type Signature = ('T' << 8) | 'D';


			builder.EmitValue(std::string(), static_cast<Builder::DataBuilder::property_type::word_type>(tiles.size()), "Number of tiles");
			builder.EmitComment("", false);
			builder.EmitComment("Attributes for tileset `" + tileset.GetName() + "`");
			builder.EmitComment("", false);

			for (const auto& tile : tiles)
			{
				tileDescriptor->CompileInstance(builder, Builder::TileDataSource(tile, map, tilesetCache, descriptors, configuration), configuration);
			}
		}

		return true;
	}

	bool TilesetDescriptor::CompileInstanceFooter(databuilder_type& /*builder*/, const datasource_type& /*dataSource*/, const Configuration& /*configuration*/) const
	{
		return true;
	}

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
