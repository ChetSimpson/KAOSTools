//	Tiled Map Converter for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#include "Generator.h"
#include "Registers.h"
#include <KAOS/Common/Utilities.h>
#include <KAOS/Imaging/ImageUtils.h>
#include <fstream>
#include <iostream>
#include <array>
#include <string>
#include <vector>
#include <type_traits>
#include <sstream>


Generator::Generator(bool canGenerateAliases)
	: canGenerateAliases_(canGenerateAliases)
{}




bool Generator::CanGenerateAlias() const
{
	return canGenerateAliases_;
}




void Generator::GeneratePalette(
	std::ostream& output,
	const KAOS::Imaging::Palette& palette) const
{
	output << "TilesetPalette\tFCB\t";

	for (const auto& color : palette)
	{
		output
			<< (&color != &palette.front() ? "," : "")
			<< static_cast<int>(KAOS::Imaging::ConvertColorToRGBRGB(color));
	}
}


void Generator::GenerateTileAlias(
	std::ostream& output,
	unsigned int id,
	unsigned int aliasId) const
{
	if (!CanGenerateAlias())
	{
		throw std::runtime_error("Generator cannot generate aliases (ATM).");
	}

	const auto labelBase("Tile" + std::to_string(id));

	output
		<< "***************************************************************\n"
		<< "* Tile #" << id << "\n"
		<< "***************************************************************\n"
		<< labelBase << "\n"
		<< "\tFDB\t$" << KAOS::Common::to_hex_string(id, 4) << "\n"
		<< "\tFDB\t$" << KAOS::Common::to_hex_string(0x8000 | aliasId, 4) << "\n"
		<< "*\n";
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
