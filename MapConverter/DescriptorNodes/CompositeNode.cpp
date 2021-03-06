//	Tiled Map Converter for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#include "DescriptorNodes/CompositeNode.h"
#include <KAOS/Common/Logging.h>
#include <KAOS/Common/Utilities.h>


namespace DescriptorNodes
{

	bool CompositeNode::CompileDefinition(definitionbuilder_type& builder) const
	{
		return CompileMemberDefinitions(builder);
	}


	bool CompositeNode::CompileInstance(databuilder_type& builder, const datasource_type& dataSource, const Configuration& configuration) const
	{
		return CompileInstanceMembers(builder, dataSource, configuration);
	}




	bool CompositeNode::CompileMemberDefinitions(definitionbuilder_type& builder) const
	{
		for (const auto& member : m_Members)
		{
			if (!member->CompileDefinition(builder))
			{
				return false;
			}
		}

		return true;
	}


	bool CompositeNode::CompileInstanceMembers(databuilder_type& builder, const datasource_type& dataSource, const Configuration& configuration) const
	{
		for (const auto& member : m_Members)
		{
			if (!member->CompileInstance(builder, dataSource, configuration))
			{
				return false;
			}
		}

		return true;
	}




	bool CompositeNode::Parse(const pugi::xml_node& node)
	{
		auto members(ParseMembers(node, GetExemplars()));
		if (!members.has_value())
		{
			return false;
		}

		auto optionTable = CreateOptionTable(*members);
		if (!optionTable)
		{
			return false;
		}


		if (!DescriptorNode::Parse(node))
		{
			return false;
		}

		m_OptionTable = move(optionTable);
		m_Members = move(*members);

		return true;
	}




	std::optional<CompositeNode::collection_type> CompositeNode::ParseMembers(
		const pugi::xml_node& node,
		const factory_type& exemplars) const
	{
		collection_type	members;

		for (const auto& child : node.children())
		{
			const std::string childName(child.name());
			if (childName.size())
			{
				const auto factory(exemplars.find(childName));
				if (factory == exemplars.end())
				{
					KAOS::Logging::Error("Unknown node type `" + childName + "` encountered while parsing tileset definition");
					return std::optional<collection_type>();
				}

				auto member(factory->second());
				if (!member)
				{
					return std::optional<collection_type>();
				}

				if (!member->Parse(child))
				{
					return std::optional<collection_type>();
				}

				members.emplace_back(move(member));
			}
		}

		return members;
	}

	
	const CompositeNode::factory_type& CompositeNode::GetExemplars() const
	{
		static const factory_type exemplars;

		return exemplars;
	}

	std::unique_ptr<OptionTable> CompositeNode::CreateOptionTable(const collection_type& members) const
	{
		OptionTable::collection_type options;
		for (const auto& member : members)
		{
			auto option(std::dynamic_pointer_cast<Option>(member));
			if (option)
			{
				options[option->GetKey()] = option;
			}
		}

		return std::make_unique<OptionTable>(move(options));
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
