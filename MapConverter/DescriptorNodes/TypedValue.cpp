//	Tiled Map Converter for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#include "DescriptorNodes/TypedValue.h"
#include "DescriptorNodes/Option.h"
#include "DescriptorNodes/TypedPropertyQuery.h"


namespace DescriptorNodes
{

	const TypedValue::factory_type& TypedValue::GetExemplars() const
	{
		static const factory_type exemplars
		{
			{ "Option", std::make_unique<Option> }
		};

		return exemplars;
	}


	bool TypedValue::Parse(const pugi::xml_node& node)
	{
		auto query(std::make_unique<TypedPropertyQuery>());
		if (!query->Parse(node))
		{
			return false;
		}


		if (!TypedNode::Parse(node))
		{
			return false;
		}

		m_Query = move(query);

		return true;
	}




	bool TypedValue::CompileInstance(databuilder_type& builder, const datasource_type& dataSource, const Configuration& /*configuration*/) const
	{
		const auto tileProperty(Resolve(dataSource));
		if (!tileProperty.has_value())
		{
			return false;
		}


		AddValue(builder, GetOutputType(), *tileProperty);

		return true;
	}


	std::optional<KAOS::Tiled::NamedProperty> TypedValue::Resolve(const datasource_type& dataSource) const
	{
		if (!m_Query)
		{
			return std::optional<KAOS::Tiled::NamedProperty>();
		}

		PropertyQuery::outputtypeid_type outputType(PropertyQuery::outputtypeid_type::Boolean);

		switch (GetOutputType())
		{
		default:
			throw std::runtime_error("Output type unknown.");

		case typeid_type::Empty:
			throw std::runtime_error("Output type cannot be empty.");

		case typeid_type::Byte:
		case typeid_type::Word:
		case typeid_type::Quad:
			outputType = PropertyQuery::outputtypeid_type::Integer;
			break;

		case typeid_type::String:
			outputType = PropertyQuery::outputtypeid_type::String;
			break;

		}

		return m_Query->Resolve(dataSource, *m_OptionTable, outputType);
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
