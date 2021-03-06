//	Tiled Map Converter for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#pragma once
#include "Generator.h"
#include "CodeSegment.h"
#include <vector>


class CodeGenerator : public Generator
{
public:

	using BitmapRow = std::vector<unsigned char>;
	using BitmapRows = std::vector<BitmapRow>;

	explicit CodeGenerator(int64_t renderStride);

	bool IsGeneratingFlat() const override;

	void GenerateTile(
		std::ostream& output,
		IntermediateImage image,
		unsigned int id) const override;


protected:

	struct RowInfo
	{
		using offset_type = int64_t;
		using offsetlist_type = std::vector<int64_t>;
		using size_type = offsetlist_type::size_type;
		using row_type = IntermediateImageRow;

		RowInfo(row_type row, offset_type offset);

		size_type GetOffsetCount() const;
		const offsetlist_type& GetOffsets() const;
		void AddOffsets(const offsetlist_type& offsets);
		void ClearOffsets();

		row_type		row;
		offsetlist_type	m_Offsets;
	};

	using imagerowlist_type = std::vector<RowInfo>;


private:


	CodeSegment Generate4ByteRowCode(const imagerowlist_type& imageRows) const;


private:


	const int64_t renderStride_;
};




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
