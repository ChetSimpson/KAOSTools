//	Tiled Map Converter for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#pragma once
#include "Builder/DataGenerator.h"
#include <KAOS/Common/NativeProperty.h>


namespace Builder
{

	class DataBuilder
	{
	public:

		using property_type = KAOS::Common::NativeProperty;


	public:

		virtual ~DataBuilder() = default;

		virtual bool Flush();

		virtual bool EmitBlank();
		virtual bool EmitComment(std::string comment, bool addSpacing = true);
		virtual bool EmitSeparatorComment();
		virtual bool EmitLabel(std::string symbol);
		virtual bool EmitValue(std::string symbol, property_type nativeProperty, std::string comment = std::string()) = 0;
	};

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
