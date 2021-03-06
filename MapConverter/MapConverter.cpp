//	Tiled Map Converter for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#include "DescriptorNodes/Root.h"
#include "Builder/MapDataSource.h"
#include "Builder/TileDataSource.h"
#include <Tiled/Map.h>
#include <Tiled/Tileset.h>
#include <Tiled/TilesetCache.h>
#include <KAOS/Common/Logging.h>


bool GenerateMapCode(
	Builder::DataBuilder& dataBuilder,
	const std::shared_ptr<KAOS::Tiled::Map> map,
	const std::shared_ptr<DescriptorNodes::Root> descriptors,
	const std::shared_ptr<KAOS::Tiled::TilesetCache> tilesetCache,
	const std::string& mapDescriptorName,
	const Configuration& configuration)
{
	const auto objectDescriptors(descriptors->QueryObjectsDescriptor(""));
	if (!objectDescriptors)
	{
		return false;
	}

	const auto mapFileDescriptor(descriptors->QueryMapFileDescriptor(mapDescriptorName));
	if (!mapFileDescriptor)
	{
		return false;
	}

	//	Generate object instances
	Builder::MapDataSourceEx<DescriptorNodes::Objects> dataSource(*objectDescriptors, map, tilesetCache, descriptors, configuration);
	if (!mapFileDescriptor->CompileInstance(dataBuilder, dataSource, configuration))
	{
		return false;
	}

	return true;
}



bool GenerateDefinitions(
	KAOS::Common::EventConsole& eventConsole,
	std::ostream& output,
	const std::shared_ptr<DescriptorNodes::Root> descriptors)
{
	const auto tileDescriptor(descriptors->QueryTileDescriptor("TileDescriptors"));
	if (!tileDescriptor)
	{
		return false;
	}

	const auto objectDescriptors(descriptors->QueryObjectsDescriptor(""));
	//if (!objectDescriptors)
	//{
	//	return false;
	//}


	//	Generate descriptor definitions
	Builder::DefinitionBuilder tileDescriptorDefinitionBuilder(eventConsole);
	if (!tileDescriptor->CompileDefinition(tileDescriptorDefinitionBuilder))
	{
		return false;
	}

	Builder::DefinitionBuilder objectDescriptorDefinitionBuilder(eventConsole);
	if (objectDescriptors && !objectDescriptors->CompileDefinition(objectDescriptorDefinitionBuilder))
	{
		return false;
	}

	Builder::AsmFormatter descriptorFormatter(output);
	tileDescriptorDefinitionBuilder.Generate(descriptorFormatter);
	objectDescriptorDefinitionBuilder.Generate(descriptorFormatter);


	return true;
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
