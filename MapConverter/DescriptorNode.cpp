//	Tiled Map Converter for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#include "DescriptorNode.h"
#include <KAOS/Common/Logging.h>
#include <iostream>


DescriptorNode::name_type DescriptorNode::GetSymbol() const
{
	return m_Symbol;
}


bool DescriptorNode::ParseRequiresSymbol() const
{
	return true;
}


std::string DescriptorNode::GetSymbolAttributeName() const
{
	return "symbol";
}


bool DescriptorNode::Parse(const pugi::xml_node& node)
{
	//	Parse Symbol
	const auto& attributeName(GetSymbolAttributeName());
	std::string symbol;
	if (!attributeName.empty())
	{
		const auto& symbolAttr(node.attribute(attributeName.c_str()));
		if (symbolAttr.empty() && ParseRequiresSymbol())
		{
			KAOS::Logging::MissingAttributeError(node, GetSymbolAttributeName());
			return false;
		}
		symbol = symbolAttr.as_string();
	}


	m_Symbol = move(symbol);

	return true;
}




void DescriptorNode::AddValue(
	databuilder_type& builder,
	KAOS::Common::NativeProperty::typeid_type outputType,
	const KAOS::Tiled::NamedProperty& property) const
{
	using NativeProperty = KAOS::Common::NativeProperty;

	switch (outputType)
	{
	case NativeProperty::typeid_type::Byte:
		AddInteger<NativeProperty::byte_type>(builder, property);
		break;

	case NativeProperty::typeid_type::Word:
		AddInteger<NativeProperty::word_type>(builder, property);
		break;

	case NativeProperty::typeid_type::Quad:
		AddInteger<NativeProperty::quad_type>(builder, property);
		break;

	case NativeProperty::typeid_type::String:
		builder.EmitValue(std::string(), KAOS::Common::NativeProperty(property.ToString()), GetSymbol());
		break;

	default:
		throw std::runtime_error("Unsupported output type");
	}

}




bool DescriptorNode::CompileSubInstance(databuilder_type& builder, const datasource_type& dataSource, const Configuration& configuration) const
{
	return CompileInstance(builder, dataSource, configuration);
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
