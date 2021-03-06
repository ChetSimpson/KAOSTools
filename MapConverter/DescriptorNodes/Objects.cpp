//	Tiled Map Converter for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#include "DescriptorNodes/Objects.h"
#include "DescriptorNodes/Object.h"
#include <KAOS/Common/Logging.h>
#include <algorithm>


namespace DescriptorNodes
{

	const Objects::factory_type& Objects::GetExemplars() const
	{
		static const factory_type exemplars
		{
			{ "Object", std::make_unique<Object> }
		};

		return exemplars;
	}


	bool Objects::ParseRequiresSymbol() const
	{
		return false;
	}


	std::string Objects::GetSymbolAttributeName() const
	{
		return "id";
	}


	std::optional<Objects::collection_type> Objects::ParseMembers(const pugi::xml_node& node, const factory_type& exemplars) const
	{
		auto members(CompositeNode::ParseMembers(node, exemplars));
		if (members.has_value())
		{
			auto& objects(*members);
			for (auto& objectNode : objects)
			{
				auto object(dynamic_cast<Object*>(objectNode.get()));
				if (!object)
				{
					members.reset();
					break;
				}
				const auto& baseTypeName(object->GetBaseTypeName());
				if (!baseTypeName.empty())
				{
					auto objectCompare = [&baseTypeName](const collection_type::value_type& value) -> bool
					{
						auto otherObject(std::dynamic_pointer_cast<Object>(value));

						return otherObject && otherObject->GetTypeName() == baseTypeName;
					};
					auto baseType(std::find_if(members->begin(), members->end(), objectCompare));
					if (baseType == objects.end())
					{
						KAOS::Logging::Error("Base object type `" + baseTypeName + "` not found for `" + object->GetTypeName());

						members.reset();
						return members;
					}

					object->SetBaseType(std::dynamic_pointer_cast<Object>(*baseType));
				}
			}

		}

		return members;
	}


	std::shared_ptr<Object> Objects::Query(const std::string& name) const
	{
		auto predicate = [&name](const collection_type::value_type& value) -> bool
		{
			auto object(std::dynamic_pointer_cast<Object>(value));

			return object && object->GetTypeName() == name;
		};

		return QueryMemberIf<Object>(predicate);
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
