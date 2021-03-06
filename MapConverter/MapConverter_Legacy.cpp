//	Tiled Map Converter for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#include "DescriptorNodes/Root.h"
#include <Tiled/Map.h>
#include <Tiled/Tileset.h>
#include <Tiled/TilesetCache.h>
#include <KAOS/Common/Utilities.h>
#include <pugixml/pugixml.hpp>
#include <iostream>
#include <deque>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <optional>
#include <functional>


namespace
{

	bool ParseObjectList(
		const pugi::xml_node& root,
		std::map<std::string, unsigned int>& objectList)
	{
		//////////////////
		auto objectdefs(root.children("objectdefs"));
		if (std::distance(objectdefs.begin(), objectdefs.end()) > 1)
		{
			std::cerr << "Multiple maps not supported\n";
			return false;
		}

		auto objects(objectdefs.begin()->children("object"));
		for (const auto& object : objects)
		{
			const auto idAttribute(object.attribute("id"));
			const auto id(idAttribute.as_int());
			const auto objectName(object.attribute("name"));
			objectList[objectName.as_string()] = id;
		}


		return true;
	}



	bool GenerateCode(
		std::ostream& output,
		KAOS::Tiled::TilesetCache tilesetCache,
		const KAOS::Tiled::Map& map,
		const std::map<std::string, unsigned int>& objectList,
		const unsigned int emptyCellId)
	{
		const auto allLayers(map.GetLayers());
		std::vector<std::shared_ptr<const KAOS::Tiled::TilesetLayer>> tilesetLayers;
		std::vector<std::shared_ptr<const KAOS::Tiled::ObjectGroupLayer>> objectGroupLayers;


		for (const auto& layer : allLayers)
		{
			auto tilesetLayer(std::dynamic_pointer_cast<const KAOS::Tiled::TilesetLayer>(layer));
			if (tilesetLayer)
			{
				tilesetLayers.emplace_back(tilesetLayer);
				continue;
			}

			auto objectGroupLayer(std::dynamic_pointer_cast<const KAOS::Tiled::ObjectGroupLayer>(layer));
			if (objectGroupLayer)
			{
				objectGroupLayers.emplace_back(objectGroupLayer);
				continue;
			}
		}

		if (tilesetLayers.size() > 1)
		{
			std::cerr << "Multiple layers not supported\n";
			return false;
		}



		bool hasError(false);

		const auto mapDimensions(map.GetDimensions());
		const auto mapName(map.GetName());
		auto rightEdge(map.GetRightEdge());
		if (!rightEdge.has_value())
		{
			rightEdge = map.GetDimensions().GetWidth();
		}

		KAOS::Tiled::NamedProperty::int_type frameDivider(0);
		auto frameDividerValue(map.QueryProperty("FrameDivider"));
		if (frameDividerValue.has_value())
		{
			frameDividerValue->QueryValue(frameDivider);
		}

		output << "*\n";
		output << "\tFCC\t`MAPX`\n";

		output << "*\n";
		output << "* Map Config\n";
		output << "*\n";
		output << "\tFDB\t"
			<< '$' << std::setfill('0') << std::hex << std::fixed << std::setw(4) << mapDimensions.GetWidth()
			<< ','
			<< '$' << std::setfill('0') << std::hex << std::fixed << std::setw(4) << mapDimensions.GetHeight()
			<< "\t\t* Map Size\n";


		output << "\tFDB\t$" << std::hex << std::fixed << std::setw(4) << *rightEdge << "\t\t\t* Right edge of map\n";
		output << "\tFCB\t$" << std::hex << std::fixed << std::setw(2) << frameDivider << "\t\t\t* Frame rate divider\n";
		output << "\tFCB\t$00\t\t\t\t* 0 = uncompressed. <>0 = compressed \n";

		output << "*\n";
		output << "* Map Name\n";
		output << "*\n";
		output << "\tFCC\t`" << mapName << "`\n";
		output << "\tZMB\t$" << std::hex << std::fixed << std::setw(4) << (32 - mapName.size()) << "\n";

		for (const auto& objectGroup : objectGroupLayers)
		{
			output
				<< "*\n"
				<< "* Object Group: " << objectGroup->GetName() << "\n"
				<< "*\n";

			output << "\tFDB\t"
				<< '$' << std::setfill('0') << std::hex << std::fixed << std::setw(4) << objectGroup->size() << "\n";

			for (const auto& object : *objectGroup)
			{
				const auto typeIdPtr(objectList.find(object->GetType()));
				const auto typeId(typeIdPtr == objectList.end() ? -1 : typeIdPtr->second);
				if (typeIdPtr == objectList.end())
				{
					std::cerr << "Object type `" << object->GetType() << "` not found.\n";
					hasError = true;
				}

				output << "\tFDB\t"
					<< '$' << std::setfill('0') << std::hex << std::fixed << std::setw(4) << typeId
					<< ','
					<< '$' << std::setfill('0') << std::hex << std::fixed << std::setw(4) << object->GetXPos()
					<< ','
					<< '$' << std::setfill('0') << std::hex << std::fixed << std::setw(4) << object->GetYPos()
					<< "\t\t*\t" << object->GetName()
					<< '\n';
			}
		}




		output << "*\n";
		output << "* Map Data\n";
		output << "*\n";

		for (const auto& layer : tilesetLayers)
		{
			unsigned int column = 0;
			for (const auto& id : *layer)
			{
				if (column == 0)
				{
					output << "\tFCB\t";
				}
				else
				{
					output << ',';
				}

				const auto convertedId(id == 0 ? emptyCellId : id - 1);

				output << '$' << std::setfill('0') << std::hex << std::fixed << std::setw(2) << convertedId;

				++column;
				if (column == 32 || column == layer->GetDimensions().GetWidth())
				{
					column = 0;
					output << '\n';
				}
			}

			if (column)
			{
				output << '\n';
			}
		}

		return !hasError;
	}

}




int main_legacy(std::deque<std::string> args)
{
	std::string outputDirectory;
	std::optional<unsigned int> emptyId;
	std::optional<std::string> filename;
	std::optional<std::string> objectListFilename;
	std::optional<std::string> explicitOutputFilename;
	bool hasError(false);


	while (!args.empty())
	{
		const auto originalArg(args.front());
		args.pop_front();
		if (originalArg.empty())
		{
			continue;
		}

		if (originalArg[0] == '-')
		{
			auto arg(originalArg);

			arg.erase(arg.begin());
			if (arg.empty() || arg[0] != '-')
			{
				std::cerr << "Unknown option `" << arg << "`\n";
				return EXIT_FAILURE;
			}
			arg.erase(arg.begin());


			const auto argOffset(arg.find('='));
			std::string value;

			if (argOffset != arg.npos)
			{
				value = arg.substr(argOffset + 1);
				arg.resize(argOffset);
			}


			if (arg == "output")
			{
				if (!outputDirectory.empty())
				{
					std::cerr << "Warning: Output directory file already set to `" << outputDirectory << "`\n";
				}
				else
				{
					outputDirectory = value;
				}
			}
			else if (arg == "outputname")
			{
				if (explicitOutputFilename.has_value())
				{
					std::cerr << "Warning: Output filename file already set to `" << *explicitOutputFilename << "`\n";
				}
				else
				{
					explicitOutputFilename = value;
				}
			}
			else if (arg == "objectlist")
			{
				if (objectListFilename.has_value())
				{
					std::cerr << "Warning: Object list file already set to `" << *objectListFilename << "`\n";
				}
				else
				{
					objectListFilename = value;
				}
			}
			else if (arg == "emptyid")
			{
				if (emptyId.has_value())
				{
					std::cerr << "Warning: Empty tile ID already set to `" << emptyId.value() << "`\n";
				}
				else
				{
					emptyId = std::stoul(value);
				}
			}
			else
			{
				std::cerr << "Unknown argument `" << originalArg << "`\n";
				return EXIT_FAILURE;
			}
		}

		else
		{
			if (filename.has_value())
			{
				std::cerr << "Multiple files not supported. File `" << originalArg << " ignored.`\n";
				hasError = true;
			}

			filename = originalArg;
		}
	}


	//	Errors
	if (!filename.has_value())
	{
		std::cerr << "Input filename not set\n";
		hasError = true;
	}

	if (outputDirectory.empty())
	{
		std::cerr << "WARNING: Output directory is not set. Using current working directory.\n";
		outputDirectory = ".";
	}

	if (hasError)
	{
		return EXIT_FAILURE;
	}


	//	Warnings
	if (!emptyId.has_value())
	{
		std::cerr << "Empty tile ID not set. Defaulting to 0\n";
		emptyId = 0;
	}


	std::map<std::string, unsigned int> objectList;
	DescriptorNodes::Root descriptorsRoot;

	if (objectListFilename.has_value())
	{
		pugi::xml_document doc;

		auto result(doc.load_file(objectListFilename->c_str()));
		if (!result)
		{
			std::cerr << "Unable to open `" << *objectListFilename << "`\n";
			return EXIT_FAILURE;
		}

		//////////////////
		const auto& defsRoots(doc.children("Root"));
		const auto rootCount(std::distance(defsRoots.begin(), defsRoots.end()));
		if (rootCount == 0)
		{
			std::cerr << "Definitions root not defined\n";
			return EXIT_FAILURE;
		}

		if (rootCount > 1)
		{
			std::cerr << "Multiple definitions root defined\n";
			return EXIT_FAILURE;
		}

		const auto& root(*defsRoots.begin());

		descriptorsRoot.Parse(root);

		if (!ParseObjectList(*defsRoots.begin(), objectList))
		{
			return EXIT_FAILURE;
		}
	}


	KAOS::Tiled::Map map;
	if(!map.Load(*filename))
	{
		return EXIT_FAILURE;
	}

	KAOS::Tiled::TilesetCache tilesetCache;
	for (const auto& tilesetDescriptor : map.GetTilesets())
	{
		if (!tilesetCache.Load(tilesetDescriptor.GetSource()).has_value())
		{

		}
	}

	std::ostringstream outputString;
	if (!GenerateCode(outputString, tilesetCache, map, objectList, emptyId.value()))
	{
		return EXIT_FAILURE;
	}

	std::string outputFilename;
	if (explicitOutputFilename.has_value())
	{
		outputFilename = *explicitOutputFilename;
	}
	else
	{
		outputFilename = KAOS::Common::MakePath(
			outputDirectory,
			KAOS::Common::GetFilenameFromPath(map.GetFilename(), false) + ".asm");
	}

	std::ofstream output(outputFilename);
	if (!output.is_open())
	{
		std::cerr << "Unable to create `" << outputFilename << "`\n";
	}

	output << outputString.str();

	return EXIT_SUCCESS;
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
