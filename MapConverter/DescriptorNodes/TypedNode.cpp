//	Tiled Map Converter for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#include "DescriptorNodes/TypedNode.h"
#include <KAOS/Common/Logging.h>


namespace DescriptorNodes
{

	TypedNode::TypedNode(typeid_type outputType)
		: m_OutputType(outputType)
	{}



	TypedNode::typeid_type TypedNode::GetOutputType() const
	{
		return m_OutputType;
	}



	bool TypedNode::CompileDefinition(definitionbuilder_type& builder) const
	{
		typeid_type memberType;
		switch (m_OutputType)
		{
		case typeid_type::Byte:
			memberType = typeid_type::Byte;
			break;

		case typeid_type::Word:
			memberType = typeid_type::Word;
			break;

		case typeid_type::Quad:
			memberType = typeid_type::Quad;
			break;

		default:
			throw std::runtime_error("Unsupported output type");
		}

		if (!builder.AddVariable(GetSymbol(), memberType))
		{
			return false;
		}


		return CompositeNode::CompileDefinition(builder);
	}




	std::optional<TypedNode::typeid_type> TypedNode::StringToTypeId(const std::string& type) const
	{
		std::optional<typeid_type>  id;

		if (type == "byte")
		{
			id = typeid_type::Byte;
		}
		else if (type == "word")
		{
			id = typeid_type::Word;
		}
		else if (type == "quad")
		{
			id = typeid_type::Quad;
		}
		else if (type == "string")
		{
			id = typeid_type::String;
		}
		else
		{
			KAOS::Logging::Error("Unknown property type `" + type + "`");
		}

		return id;
	}


	unsigned int TypedNode::GetTypeBitSize(typeid_type type) const
	{
		auto maxBits(0U);
		switch (type)
		{
		case typeid_type::Byte:
			maxBits = 8;
			break;

		case typeid_type::Word:
			maxBits = 16;
			break;

		case typeid_type::Quad:
			maxBits = 32;
			break;
		}

		return maxBits;
	}

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
