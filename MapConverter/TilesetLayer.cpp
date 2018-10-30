//	Tiled Map Converter for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#include "TilesetLayer.h"
#include "Utils.h"
#include <iostream>


bool TilesetLayer::Parse(const pugi::xml_node& layer)
{
	if (!Layer::Parse(layer))
	{
		return false;
	}

	Size size;
	if (!size.Parse(layer))
	{
		return false;
	}


	const auto& layerData(layer.child("data"));
	if (layerData.empty())
	{
		std::cerr << "Layer does not have any data\n";
		return false;
	}

	const auto& layerDataEncodingAttr(layerData.attribute("encoding"));
	if (layerDataEncodingAttr.empty())
	{
		std::cerr << "Layer data does not have an encoding attribute\n";
		return false;
	}

	const std::string layerDataEncoding(layerDataEncodingAttr.as_string());
	if (layerDataEncoding == "csv")
	{
		if (layerData.first_child().empty())
		{
			std::cerr << "Layer data is missing child container\n";
			return false;
		}

		return ParseCSV(size, layerData.first_child().value());
	}

	std::cerr << "Unknown encoding `" << layerDataEncoding << "`\n";

	return false;
}




bool TilesetLayer::ParseCSV(Size size, const std::string& data)
{
	auto values(ConvertToInteger(SplitString(data, ",")));

	if (size.GetCount() != values.size())
	{
		std::cerr << "Size mismatch. Width * Height does not match size of data\n";
		return false;
	}

	m_Size = size;
	m_Data = move(values);

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
