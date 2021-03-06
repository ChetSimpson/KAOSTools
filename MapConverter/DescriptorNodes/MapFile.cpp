//	Tiled Map Converter for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#include "DescriptorNodes/MapFile.h"
#include "DescriptorNodes/ObjectLayer.h"
#include "DescriptorNodes/TiledLayer.h"
#include "DescriptorNodes/TypedValue.h"
#include "DescriptorNodes/PackedValue.h"
#include "DescriptorNodes/TilesetDescriptor.h"
#include "Builder/MapDataSource.h"
#include <KAOS/Common/Logging.h>


namespace DescriptorNodes
{

	const MapFile::factory_type& MapFile::GetExemplars() const
	{
		static const factory_type exemplars
		{
			{ "String", std::make_unique<StringProperty> },
			{ "Byte", std::make_unique<ByteProperty> },
			{ "Word", std::make_unique<WordProperty> },
			{ "Quad", std::make_unique<QuadProperty> },
			{ "PackedByte", std::make_unique<PackedByteProperty> },
			{ "PackedWord", std::make_unique<PackedWordProperty> },
			{ "PackedQuad", std::make_unique<PackedQuadProperty> },
			{ "ObjectLayer", std::make_unique<ObjectLayer> },
			{ "TilesetDesc", std::make_unique<DescriptorNodes::TilesetDescriptor> },
			{ "TiledLayer", std::make_unique<TiledLayer> },
		};

		return exemplars;
	}


	bool MapFile::ParseRequiresSymbol() const
	{
		return false;
	}


	std::string MapFile::GetSymbolAttributeName() const
	{
		return "id";
	}


	bool MapFile::CompileDefinition(definitionbuilder_type& builder) const
	{
		builder.Begin(!GetSymbol().empty() ? GetSymbol() : "MapFile");
		if (!CompositeNode::CompileDefinition(builder))
		{
			return false;
		}
		builder.End();

		return true;
	}


	bool MapFile::CompileInstance(databuilder_type& builder, const datasource_type& dataSource, const Configuration& configuration) const
	{
		auto& mapDataSource(dynamic_cast<const Builder::MapDataSource&>(dataSource));
		const auto map(mapDataSource.QueryMap());
		if (!map)
		{
			KAOS::Logging::Error("Map not available.");
			return false;
		}

		builder.EmitSeparatorComment();
		builder.EmitComment("", false);
		builder.EmitComment("Map file for `" + map->GetName() + "`");
		builder.EmitComment("", false);
		builder.EmitSeparatorComment();
		builder.EmitBlank();
		builder.EmitSeparatorComment();
		builder.EmitComment("Map header");
		builder.EmitSeparatorComment();
		builder.EmitLabel("Header");

		if (!CompositeNode::CompileInstance(builder, dataSource, configuration))
		{
			return false;
		}

		builder.Flush();

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
