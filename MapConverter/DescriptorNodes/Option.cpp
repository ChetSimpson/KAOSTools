//	Tiled Map Converter for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#include "DescriptorNodes/Option.h"
#include <KAOS/Common/xml.h>
#include <KAOS/Common/Logging.h>


namespace DescriptorNodes
{

	
	std::string	Option::GetSymbol() const
	{
		return m_Symbol;
	}

	
	std::string	Option::GetKey() const
	{
		return m_Key;
	}


	int64_t Option::GetValue() const
	{
		return m_Value;
	}


	bool Option::Parse(const pugi::xml_node& node)
	{
		const auto& keyAttr(node.attribute("key"));
		const auto& symbolAttr(node.attribute("symbol"));
		if (symbolAttr.empty())
		{
			if (!keyAttr.empty())
			{
				KAOS::Logging::Error("Option with key `" + std::string(keyAttr.as_string()) + "` does not have a `symbol` attribute");
			}
			else
			{
				KAOS::Logging::MissingAttributeError(node, "symbol");
			}
			return false;
		}

		if (keyAttr.empty())
		{
			if (!symbolAttr.empty())
			{
				KAOS::Logging::Error("Option with symbol `" + std::string(symbolAttr.as_string()) + "` does not have a `key` attribute");
			}
			else
			{
				KAOS::Logging::MissingAttributeError(node, "key");
			}
			return false;
		}

		decltype(m_Value) value;
		if(!KAOS::Common::XML::LoadAttribute(node, "value", value))
		{
			return false;
		}

		std::string symbol(symbolAttr.as_string());
		std::string key(keyAttr.as_string());

		m_Symbol = move(symbol);
		m_Key = move(key);
		m_Value = value;


		return true;
	}




	bool Option::CompileDefinition(definitionbuilder_type& builder) const
	{
		return builder.AddSymbolic(m_Symbol, m_Value);
	}


	bool Option::CompileInstance(databuilder_type& /*builder*/, const datasource_type& /*dataSource*/, const Configuration& /*configuration*/) const
	{
		return true;
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
