//	Tiled Map Converter for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#include "DescriptorNodes/Root.h"
#include "Builder/SimpleDataBuilder.h"
#include "Configuration.h"
#include <Tiled/Map.h>
#include <Tiled/Tileset.h>
#include <Tiled/TilesetCache.h>
#include <KAOS/Common/Logging.h>
#include <KAOS/Common/Utilities.h>
#include <iostream>
#include <deque>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <optional>


bool GenerateDefinitions(
	KAOS::Common::EventConsole& eventConsole,
	std::ostream& output,
	const std::shared_ptr<DescriptorNodes::Root> descriptors);

bool GenerateMapCode(
	Builder::DataBuilder& dataBuilder,
	const std::shared_ptr<KAOS::Tiled::Map> map,
	const std::shared_ptr<DescriptorNodes::Root> descriptors,
	const std::shared_ptr<KAOS::Tiled::TilesetCache> tilesetCache,
	const std::string& mapDescriptorName,
	const Configuration& configuration);


int main_legacy(std::deque<std::string> args);

int main(int argc, const char **argv)
{
	KAOS::Common::EventConsole eventConsole;

	std::deque<std::string> args(argv + 1, argv + argc);

#if 0
	return main_legacy(move(args));
#else
	std::string outputDirectory;
	std::optional<unsigned int> emptyId;
	std::optional<std::string> mapFilename;
	std::optional<std::string> defsInputFilename;
	std::optional<std::string> defsOutputFilename;
	std::optional<std::string> outputFilename;
	std::optional<std::string> mapDescriptorNameID;
	Configuration configuration;
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
				KAOS::Logging::Error("Unknown option `" + originalArg + "`");
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


			if (arg == "silent")
			{
				KAOS::Logging::enabled = false;
			}
			else if (arg == "compress-layer")
			{
				configuration.compressTileLayers = true;
			}
			else if (arg == "empty-tile-id")
			{
				if (emptyId.has_value())
				{
					KAOS::Logging::Warn("Empty tile ID already set to `" + std::to_string(emptyId.value()) + "`");
				}
				else
				{
					emptyId = std::stoul(value);
				}
			}
			

			else if (arg == "map-type-id")
			{
				if (mapDescriptorNameID.has_value())
				{
					KAOS::Logging::Warn("Output directory already set to `" + mapDescriptorNameID.value() + "`");
				}
				else if (value.empty())
				{
					KAOS::Logging::Warn("Empty argument for option --" + arg + " ignored.");
				}
				else
				{
					mapDescriptorNameID = value;
				}
			}



			else if (arg == "output-dir")
			{
				if (!outputDirectory.empty())
				{
					KAOS::Logging::Warn("Output directory already set to `" + outputDirectory + "`");
				}
				else
				{
					outputDirectory = value;
				}
			}
			else if (arg == "defs-input-file")
			{
				if (defsInputFilename.has_value())
				{
					KAOS::Logging::Warn("Definition file already set to `" + *defsInputFilename + "`");
				}
				else
				{
					defsInputFilename = value;
				}
			}
			else if (arg == "defs-output-file")
			{
				if (defsOutputFilename.has_value())
				{
					KAOS::Logging::Warn("Definition output file already set to `" + *defsOutputFilename + "`");
				}
				else
				{
					defsOutputFilename = value;
				}
			}
			else if (arg == "output-file")
			{
				if (outputFilename.has_value())
				{
					KAOS::Logging::Warn("Output file already set to `" + *outputFilename + "`");
				}
				else
				{
					outputFilename = value;
				}
			}
			else
			{
				KAOS::Logging::Error("Unknown argument `" + originalArg + "`");
				hasError = true;
			}
		}
		else
		{
			if (mapFilename.has_value())
			{
				KAOS::Logging::Error("Multiple map files not supported. File `" + originalArg + " ignored.`");
				hasError = true;
			}
			else
			{
				mapFilename = originalArg;
			}
		}
	}


	//	Errors
	if (!defsInputFilename.has_value())
	{
		KAOS::Logging::Error("Input definition file is not set.");
		hasError = true;
	}

	if (!mapFilename.has_value() && !defsOutputFilename.has_value())
	{
		KAOS::Logging::Error("Noting to do. No map file or output file(s) specified.");
		hasError = true;
	}

	if (outputDirectory.empty())
	{
		KAOS::Logging::Warn("Output directory is not set. Using current working directory.");
		outputDirectory = ".";
	}

	//	Warnings
	if (!emptyId.has_value())
	{
		KAOS::Logging::Warn("Empty tile ID not set. Defaulting to 0");
		emptyId = 0;
	}

	if (hasError)
	{
		return EXIT_FAILURE;
	}



	///////////////////////////////////////////////////////////////////////////////
	//
	//	Load descriptor file
	//
	///////////////////////////////////////////////////////////////////////////////
	pugi::xml_document doc;
	auto result(doc.load_file(defsInputFilename->c_str()));
	if (!result)
	{
		KAOS::Logging::Error("Unable to open `" + *defsInputFilename + "`");
		return EXIT_FAILURE;
	}

	//////////////////
	const auto& defsRoots(doc.children("Root"));
	const auto rootCount(std::distance(defsRoots.begin(), defsRoots.end()));
	if (rootCount == 0)
	{
		KAOS::Logging::Error("Definitions root not defined");
		return EXIT_FAILURE;
	}

	if (rootCount > 1)
	{
		KAOS::Logging::Error("Multiple definitions root defined");
		return EXIT_FAILURE;
	}

	auto descriptorsRoot(std::make_shared<DescriptorNodes::Root>());
	if (!descriptorsRoot->Parse(*defsRoots.begin()))
	{
		eventConsole.Error("Unable to parse definitions root");
		return EXIT_FAILURE;
	}



	///////////////////////////////////////////////////////////////////////////////
	//
	//	Generate the definitions
	//
	///////////////////////////////////////////////////////////////////////////////
	if (defsOutputFilename.has_value())
	{
		std::string outputFilename(*defsOutputFilename);

		std::ofstream output(outputFilename);
		if (!output.is_open())
		{
			KAOS::Logging::Error("Unable to create `" + outputFilename + "`");
		}

		GenerateDefinitions(eventConsole, output, descriptorsRoot);
	}



	///////////////////////////////////////////////////////////////////////////////
	//
	//	Load map file and preload the tilesets
	//
	///////////////////////////////////////////////////////////////////////////////
	if (mapFilename.has_value())
	{
		auto map(std::make_shared<KAOS::Tiled::Map>());
		if (!map->Load(*mapFilename))
		{
			return EXIT_FAILURE;
		}

		auto tilesetCache(std::make_shared<KAOS::Tiled::TilesetCache >());
		for (const auto& tilesetDescriptor : map->GetTilesets())
		{
			if (!tilesetCache->Load(tilesetDescriptor.GetSource()).has_value())
			{
				return EXIT_FAILURE;
			}
		}

		std::string finalOutputFilename;
		if (outputFilename.has_value())
		{
			finalOutputFilename = *outputFilename;
		}
		else
		{
			finalOutputFilename = KAOS::Common::GetFilenameFromPath(map->GetFilename(), false) + "_map.asm";
			finalOutputFilename = KAOS::Common::MakePath(outputDirectory, finalOutputFilename);
		}

		std::ofstream output(finalOutputFilename);
		if (!output.is_open())
		{
			KAOS::Logging::Error("Unable to create `" + finalOutputFilename + "` when generating map.");
		}

		Builder::SimpleDataBuilder dataBuilder(output);

		if (!mapDescriptorNameID.has_value())
		{
			mapDescriptorNameID = "";
		}

		configuration.emptyCellId = emptyId.value();
		if (!GenerateMapCode(dataBuilder, map, descriptorsRoot, tilesetCache, mapDescriptorNameID.value(), configuration))
		{
			return EXIT_FAILURE;
		}

		dataBuilder.EmitComment("", false);
		dataBuilder.EmitComment("", false);
		dataBuilder.EmitComment("", false);
		dataBuilder.EmitSeparatorComment();
		dataBuilder.EmitComment("End of file");
		dataBuilder.EmitSeparatorComment();
		dataBuilder.EmitComment("", false);

		static const Builder::DataBuilder::property_type::word_type Signature = 0;
		dataBuilder.EmitValue(std::string(), Signature, "Signature 0x0000");
		dataBuilder.EmitComment("", false);

		//	FIXME: This should be done automatically when dataBuilder closes.
		dataBuilder.Flush();
	}

#endif
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
