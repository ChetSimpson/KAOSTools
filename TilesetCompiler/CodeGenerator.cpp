//	Tiled Map Converter for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#include "CodeGenerator.h"
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



CodeGenerator::RowInfo::RowInfo(row_type row, offset_type offset)
	:
	row(row),
	m_Offsets(1, offset)
{

}

CodeGenerator::RowInfo::size_type CodeGenerator::RowInfo::GetOffsetCount() const
{
	return m_Offsets.size();
}


const CodeGenerator::RowInfo::offsetlist_type& CodeGenerator::RowInfo::GetOffsets() const
{
	return m_Offsets;
}


void CodeGenerator::RowInfo::AddOffsets(const offsetlist_type& offsets)
{
	m_Offsets.insert(m_Offsets.end(), offsets.begin(), offsets.end());
}


void CodeGenerator::RowInfo::ClearOffsets()
{
	m_Offsets.clear();
}






CodeGenerator::CodeGenerator(int64_t renderStride)
	:
	Generator(true),
	renderStride_(renderStride)
{}


bool CodeGenerator::IsGeneratingFlat() const
{
	return false;
}




void CodeGenerator::GenerateTile(
	std::ostream& output,
	IntermediateImage intermediateImage,
	unsigned int id) const
{
	output << "*\n";
	for (const auto& row : intermediateImage)
	{
		output << "*\t\t" << KAOS::Common::to_hex_string(row.GetPixelsAsQuad(0), 8) << "\n";
	}
	output << "*\n";


	imagerowlist_type rows;
	imagerowlist_type::value_type::offset_type offset = 0;
	for (const auto& row : intermediateImage)
	{
		rows.emplace_back(row, offset);
		offset += renderStride_;
	}



	//	Consolidate
	for (auto i(0ull); i < rows.size(); ++i)
	{
		auto& info(rows[i]);
		for (auto x(i + 1), count(rows.size()); x < count; )
		{
			auto& other(rows[x]);
			if (info.row.ComparePixels(other.row))
			{
				info.AddOffsets(other.GetOffsets());
				auto ptr(rows.begin());
				advance(ptr, i);
				rows.erase(ptr);
				--count;
			}
			else
			{
				++x;
			}
		}
	}

	const auto labelBase("Tile" + std::to_string(id));
	auto codeSegment(Generate4ByteRowCode(rows));

	codeSegment.Append("rts", 4);

	output
		<< "***************************************************************\n"
		<< "* Tile #" << id << " (" << codeSegment.GetCycleCount() << " cycles)\n"
		<< "***************************************************************\n"
		<< labelBase << "\n"
		<< "\tFDB\t" << std::to_string(id) << "\n"
		<< "\tFDB\t" << labelBase << "DataEnd-" << labelBase << "DataBegin\n"
		<< labelBase << "DataBegin\n"
		<< codeSegment.GetCode()
		<< labelBase << "DataEnd\n"
		<< "*\n";
}




CodeSegment CodeGenerator::Generate4ByteRowCode(const imagerowlist_type& imageRows) const
{
	QRegister regState;
	CodeSegment codeSegment;

	for(const auto& row : imageRows)
	{
		if (row.GetOffsetCount() == 0)
		{
			continue;
		}

		codeSegment += regState.GenerateLoad(row.row.GetPixelsAsQuad(0));
		for (const auto& offset : row.GetOffsets())
		{
			auto cycleCount = 8;
			//	TODO: Move to helper function
			if (offset == 0)
			{
				cycleCount += 0;
			}
			else if (offset >= -128 && offset <= 127)
			{
				cycleCount += 1;
			}
			else
			{
				cycleCount += 3;
			}

			codeSegment.Append("STQ", "$" + KAOS::Common::to_hex_string(offset, 4) + ",y", cycleCount);
		}
	}

	return codeSegment;
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
