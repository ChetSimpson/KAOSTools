//	Tiled Map Converter for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#include "DescriptorNodes/PackedValue.h"
#include "DescriptorNodes/BitField.h"
#include "Builder/ValueDataBuilder.h"
#include <KAOS/Common/Logging.h>


namespace DescriptorNodes
{

	const PackedValue::factory_type& PackedValue::GetExemplars() const
	{
		static const factory_type exemplars
		{
			{ "Field", std::make_unique<BitField> }
		};

		return exemplars;
	}




	std::optional<CompositeNode::collection_type> PackedValue::ParseMembers(
		const pugi::xml_node& node,
		const factory_type& exemplars) const
	{
		auto members(TypedNode::ParseMembers(node, exemplars));
		if (!members.has_value())
		{
			return members;
		}

		bool hasErrors(false);
		uint32_t currentBit(0);
		uint32_t bitsRemaining(0);
		switch (GetOutputType())
		{
		case typeid_type::Byte:
			currentBit = 0x80;
			bitsRemaining = 8;
			break;

		case typeid_type::Word:
			currentBit = 0x8000;
			bitsRemaining = 16;
			break;

		case typeid_type::Quad:
			currentBit = 0x80000000;
			bitsRemaining = 32;
			break;

		default:
			break;
		}

		for (auto& member : *members)
		{
			auto bitfieldMember(std::dynamic_pointer_cast<BitField>(member));
			if (!bitfieldMember)
			{
				throw std::runtime_error("bitfieldMember is not correct type");
			}

			if (bitfieldMember->GetBitsUsed() > bitsRemaining)
			{
				KAOS::Logging::Error("PackedValue node with symbol `" + bitfieldMember->GetSymbol() + "` is too big.");
				hasErrors = true;
				continue;
			}

			auto mask(0U);
			for (size_t i(0U), size(bitfieldMember->GetBitsUsed()); i < size; ++i)
			{
				mask |= currentBit;

				currentBit >>= 1;
				--bitsRemaining;
			}

			bitfieldMember->SetInfo(mask, bitsRemaining);
		}

		if (hasErrors)
		{
			members.reset();
		}

		return move(members);
	}


	bool PackedValue::CompileInstanceMembers(databuilder_type& builder, const datasource_type& dataSource, const Configuration& configuration) const
	{
		KAOS::Tiled::NamedProperty::int_type value(0);
		bool touchStatus(true);

		const auto memberProcessor = [&touchStatus, &value, &dataSource, &configuration](const collection_type::value_type& member) -> void
		{
			Builder::ValueDataBuilder valueBuilder;
			if (!member->CompileInstance(valueBuilder, dataSource, configuration))
			{
				touchStatus = false;
			}
			else
			{
				value |= valueBuilder.GetValue();
			}
		};

		ForEachMember(memberProcessor);

		if (!touchStatus)
		{
			return false;
		}

		AddValue(builder, GetOutputType(), KAOS::Tiled::NamedProperty(GetSymbol(), value));

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
