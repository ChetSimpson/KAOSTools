//	Tiled Map Converter for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#include "DescriptorNodes/TypedPropertyQuery.h"
#include <KAOS/Common/Logging.h>


namespace DescriptorNodes
{

	bool TypedPropertyQuery::Parse(const pugi::xml_node& node)
	{
		///////////////////////////////////////////////////////////////////////////
		//
		//	Parse name
		//
		///////////////////////////////////////////////////////////////////////////
		const auto& nameAttr(node.attribute("var"));
		const auto& valueAttr(node.attribute("value"));
		if (nameAttr.empty() && valueAttr.empty())
		{
			KAOS::Logging::MissingAttributeError(node, "var");
			return false;
		}


		///////////////////////////////////////////////////////////////////////////
		//
		//	Parse type
		//
		///////////////////////////////////////////////////////////////////////////
		const auto& varTypeAttr(node.attribute("vartype"));
		if (varTypeAttr.empty())
		{
			KAOS::Logging::MissingAttributeError(node, "vartype");
			return false;
		}

		const auto varType(ParsePropertyType(varTypeAttr.as_string()));
		if (!varType.has_value())
		{
			return false;
		}


		///////////////////////////////////////////////////////////////////////////
		//
		//	Parse value (Optional. If value is not specified property is required
		//
		///////////////////////////////////////////////////////////////////////////

		std::optional<value_type> varValue;
		if (!valueAttr.empty())
		{
			switch (*varType)
			{
			case vartype_id::Bool:
				varValue = valueAttr.as_bool();	//	FIXME: This isn't going to work so well
				break;

			case vartype_id::Int:
				varValue = valueAttr.as_int();
				break;

			case vartype_id::String:
				varValue = valueAttr.as_string();
				break;

			default:
				throw std::runtime_error("Unknown value type encountered");
			}
		}

		m_Key = nameAttr.as_string();
		m_Type = move(varType);
		m_Value = move(varValue);

		return true;
	}




	std::optional<TypedPropertyQuery::vartype_id> TypedPropertyQuery::ParsePropertyType(const std::string& type) const
	{
		std::optional<vartype_id>  id;

		if (type == "bool")
		{
			id = vartype_id::Bool;
		}
		else if (type == "int")
		{
			id = vartype_id::Int;
		}
		else if (type == "string")
		{
			id = vartype_id::String;
		}
		else
		{
			KAOS::Logging::Error("Unknown property vartype `" + type + "`");
		}

		return id;
	}


	std::optional<KAOS::Tiled::NamedProperty> TypedPropertyQuery::Resolve(
		const datasource_type& dataSource,
		const optiontable_type& optionTable,
		outputtypeid_type /*conversionType*/) const
	{
		std::optional<KAOS::Tiled::NamedProperty> property;
		if (!m_Key.empty())
		{
			property = dataSource.QueryProperty(m_Key);
		}

		if (!property.has_value())
		{
			if (m_Value.has_value())
			{
				const auto& varValue(*m_Value);

				if (!m_Type.has_value())
				{
					throw std::runtime_error("Value type not set.");
				}

				switch (*m_Type)
				{
				case vartype_id::Bool:
				{
					auto valuePtr(std::get_if<std::uint32_t>(&varValue));
					if (!valuePtr)
					{
						throw std::runtime_error("Unable to convert value");
					}

					property = KAOS::Tiled::NamedProperty(
						m_Key,
						KAOS::Tiled::NamedProperty::bool_type(*valuePtr ? true : false));
					break;
				}

				case vartype_id::Int:
				{
					auto valuePtr(std::get_if<std::uint32_t>(&varValue));
					if (!valuePtr)
					{
						throw std::runtime_error("Unable to convert value");
					}

					property = KAOS::Tiled::NamedProperty(
						m_Key,
						KAOS::Tiled::NamedProperty::int_type(*valuePtr));
					break;
				}

				case vartype_id::String:
				{
					auto valuePtr(std::get_if<std::string>(&varValue));
					if (!valuePtr)
					{
						throw std::runtime_error("Unable to convert value");
					}

					property = KAOS::Tiled::NamedProperty(
						m_Key,
						KAOS::Tiled::NamedProperty::string_type(*valuePtr));
					break;
				}
				}
			}
			else
			{
				KAOS::Logging::Error("Unable to resolve required property `" + m_Key + "` from data source <FIXME>");
			}
		}


		if (property.has_value() && property->IsStringType())
		{
			auto option(optionTable.Query(property->ToString()));
			if (option)
			{
				property->Set(option->GetValue());
			}
		}


		return move(property);
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
