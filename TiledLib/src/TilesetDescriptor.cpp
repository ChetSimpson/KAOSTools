//	Tiled Map Converter for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#include <Tiled/TilesetDescriptor.h>
#include <KAOS/Common/Utilities.h>
#include <iostream>


namespace KAOS { namespace Tiled
{

	bool TilesetDescriptor::Parse(const pugi::xml_node& node, const std::string& mapDirectory)
	{
		const auto gidAttr(node.attribute("firstgid"));
		if (gidAttr.empty())
		{
			std::cerr << "Missing firstgid attribute\n";
			return false;
		}

		const auto gid(gidAttr.as_int());
		if(gid <= 0)
		{
			std::cerr << "Invalid first GID in tileset\n";
			return false;
		}


		const auto sourceAttr(node.attribute("source"));
		if (sourceAttr.empty())
		{
			std::cerr << "Missing firstgid attribute\n";
			return false;
		}

		std::string source(sourceAttr.as_string());
		if (source.empty())
		{
			std::cerr << "source attribute is empty\n";
			return false;
		}
		if (!Common::IsAbsolutePath(source))
		{
			source = Common::EnsureAbsolutePath(mapDirectory + source);
		}

		m_Gid = gid;
		m_Source = move(source);

		return true;
	}




	size_t TilesetDescriptor::GetGid() const
	{
		return m_Gid;
	}


	std::string TilesetDescriptor::GetSource() const
	{
		return m_Source;
	}

}}




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
