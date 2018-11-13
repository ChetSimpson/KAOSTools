//	Tiled Map Converter for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#include "CodeGenerator.h"
#include <KAOS/Common/Utilities.h>
#include <KAOS/Imaging/ImageUtils.h>
#include <fstream>
#include <iostream>
#include <array>
#include <string>
#include <vector>
#include <type_traits>
#include <sstream>


void CodeGenerator::GeneratePaletteCode(
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



void CodeGenerator::GenerateTileCode(
	std::ostream& output,
	IntermediateImage image,
	unsigned int id) const
{
	//	Consolidate
	for (auto i(0ull); i < image.size(); ++i)
	{
		auto& info(image[i]);
		for (auto x(i + 1), count(image.size()); x < count; )
		{
			auto& other(image[x]);
			if (info.ComparePixels(other))
			{
				info.AddOffsets(other.GetOffsets());
				image.erase_at(x);
				--count;
			}
			else
			{
				++x;
			}
		}
	}

	const auto textureCode(Generate4ByteRowCode(image));
	const auto labelBase("Tile" + std::to_string(id));

	output
		<< "***************************************************************\n"
		<< "* Tile #" << id << "\n"
		<< "***************************************************************\n"
		<< labelBase << "\n"
		<< "\tFDB\t" << std::to_string(id) << "\n"
		<< "\tFDB\t" << labelBase << "DataEnd-" << labelBase << "DataBegin\n"
		<< labelBase << "DataBegin\n"
		<< labelBase << "StaticTexture\n"
		<< textureCode
		<< "\trts\n"
		<< labelBase << "DataEnd\n"
		<< "*\n";
}




void CodeGenerator::GenerateTileAliasCode(
	std::ostream& output,
	unsigned int id,
	unsigned int aliasId) const
{
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




std::string CodeGenerator::Generate4ByteRowCode(const IntermediateImage& image) const
{
	std::ostringstream output;
	uint32_t accd(~0ULL);	//	FIXME: Need proper register state implementation (include acca/accb)
	uint32_t accw(~0ULL);	//	FIXME: Need proper register state implementation (include acce/accf)

	for(const auto& row : image)
	{
		if (row.GetOffsetCount() == 0)
		{
			continue;
		}

		const auto& offsets(row.GetOffsets());
		const auto& rowData(row.GetPixels());
		uint32_t highWord(rowData[0] << 8 | rowData[1]);
		uint32_t lowWord(rowData[2] << 8 | rowData[3]);

		if (highWord != accw && lowWord != accd)
		{
			accw = highWord;
			accd = lowWord;
			output << "\tLDQ\t#$" << KAOS::Common::to_hex_string(highWord << 16 | lowWord, 8) << "\n";
		}
		else if (highWord == accw && lowWord != accd)
		{
			accd = lowWord;
			output << "\tLDD\t#$" << KAOS::Common::to_hex_string(lowWord, 4) << "\n";
		}
		else if (highWord != accw && lowWord == accd)
		{
			accw = highWord;
			output << "\tLDW\t#$" << KAOS::Common::to_hex_string(highWord, 4) << "\n";
		}
		else
		{
			throw std::runtime_error("Encountered repeating row where a repeating row should not exist.");
		}

		for (const auto& offset : offsets)
		{
			output << "\tSTQ\t$" << KAOS::Common::to_hex_string(offset, 4) << ",y\n";
		}

		output << "*\n";
	}

	return output.str();
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
