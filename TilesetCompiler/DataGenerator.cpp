//	Tiled Map Converter for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#include "DataGenerator.h"
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



DataGenerator::DataGenerator(bool generateFlatFile)
	:
	Generator(false),
	generateFlatFile_(generateFlatFile)
{}


bool DataGenerator::IsGeneratingFlat() const
{
	return generateFlatFile_;
}




void DataGenerator::GenerateTile(
	std::ostream& output,
	IntermediateImage image,
	unsigned int id) const
{
	output << "*\n";
	for (const auto& row : image)
	{
		output << "*\t\t" << KAOS::Common::to_hex_string(row.GetPixelsAsQuad(0), 8) << "\n";
	}
	output << "*\n";

	std::ostringstream imageData;
	for (const auto& row : image)
	{
		std::string line;

		for (const auto& pixel : row)
		{
			if (line.empty())
			{
				line = "\tFCB\t";
			}
			else
			{
				line += ",";
			}

			line += ("$" + KAOS::Common::to_hex_string(pixel, 2));
		}

		imageData << line << "\n";
	}

	const auto labelBase("Tile" + std::to_string(id));

	output
		<< "***************************************************************\n"
		<< "* Tile #" << id << "\n"
		<< "***************************************************************\n"
		<< labelBase << "\n";
	if (!generateFlatFile_)
	{
		output
			<< "\tFDB\t" << std::to_string(id) << "\n"
			<< "\tFDB\t" << labelBase << "DataEnd-" << labelBase << "DataBegin"
			<< "\n";
	}

	output
		<< labelBase << "DataBegin\n"
		<< imageData.str()
		<< labelBase << "DataEnd\n"
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
