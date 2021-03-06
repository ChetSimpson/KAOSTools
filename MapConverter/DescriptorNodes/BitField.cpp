//	Tiled Map Converter for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#include "DescriptorNodes/BitField.h"
#include "DescriptorNodes/Option.h"
#include "DescriptorNodes/TypedPropertyQuery.h"
#include <KAOS/Common/Logging.h>


namespace DescriptorNodes
{

	const BitField::factory_type& BitField::GetExemplars() const
	{
		static const factory_type exemplars
		{
			{ "Option", std::make_unique<Option> }
		};

		return exemplars;
	}



	bool BitField::Parse(const pugi::xml_node& node)
	{
		auto query(std::make_unique<TypedPropertyQuery>());
		if (!query->Parse(node))
		{
			return false;
		}


		///////////////////////////////////////////////////////////////////////////
		//
		//	Bits is not optional
		//
		///////////////////////////////////////////////////////////////////////////
		const auto& bitsAttr(node.attribute("bits"));
		if (bitsAttr.empty())
		{
			KAOS::Logging::MissingAttributeError(node, "bits");
			return false;
		}

		const auto bitsValue = bitsAttr.as_uint();
		if (!bitsValue)
		{
			KAOS::Logging::NodeError(node, "has `bits` attribute with invalid or zero value.");
			return false;
		}


		if (!CompositeNode::Parse(node))
		{
			return false;
		}


		m_Query = move(query);
		m_Bits = bitsValue;
		m_Mask = 0;
		m_Shift = 0;

		return true;
	}



	BitField::size_type BitField::GetBitsUsed() const
	{
		return m_Bits;
	}


	void BitField::SetInfo(mask_type mask, size_type shiftCount)
	{
		m_Mask = mask;
		m_Shift = shiftCount;
	}



	bool BitField::CompileDefinition(definitionbuilder_type& builder) const
	{
		if (!builder.AddSymbolic(GetSymbol(), m_Mask))
		{
			return false;
		}

		//	Option values need to be shifted to match the bits allocated to the field
		Builder::DefinitionBuilder::Shifter shifter(builder, m_Shift);

		return CompileMemberDefinitions(builder);
	}


	bool BitField::CompileInstance(databuilder_type& builder, const datasource_type& dataSource, const Configuration& /*configuration*/) const
	{
		const auto tileProperty(Resolve(dataSource));
		if (!tileProperty.has_value())
		{
			return false;
		}


		const auto propertyType(tileProperty->GetType());
		if (   propertyType != KAOS::Common::Property::id_type::Boolean
			&& propertyType != KAOS::Common::Property::id_type::Integer)
		{
			throw std::runtime_error("Unsupported type");
		}

		//	FIXME: Check for value overflow and apply mask
		using NativeProperty = KAOS::Common::NativeProperty;
		builder.EmitValue(GetSymbol(), NativeProperty(NativeProperty::quad_type(tileProperty->ToInteger() << m_Shift)));
		return true;

	}


	std::optional<KAOS::Tiled::NamedProperty> BitField::Resolve(const datasource_type& dataSource) const
	{
		if (!m_Query)
		{
			return std::optional<KAOS::Tiled::NamedProperty>();
		}

		return m_Query->Resolve(dataSource, *m_OptionTable, PropertyQuery::outputtypeid_type::Integer);
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
