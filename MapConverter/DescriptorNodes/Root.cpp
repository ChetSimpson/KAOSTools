//	Tiled Map Converter for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#include "DescriptorNodes/Root.h"
#include <KAOS/Common/Logging.h>


namespace DescriptorNodes
{

	namespace
	{
		class NullDescriptor : public DescriptorNode
		{
		public:

			bool CompileDefinition(definitionbuilder_type& /*builder*/) const override
			{
				return true;
			}

			bool CompileInstance(databuilder_type& /*builder*/, const datasource_type& /*dataSource*/, const Configuration& /*configuration*/) const override
			{
				return true;
			}

			bool ParseRequiresSymbol() const override
			{
				return false;
			}

		};


		template<class DestinationType_, class CollectionType_, class ElementType_>
		void AddToCollectionIf(const std::string& description, CollectionType_& collection, std::shared_ptr<ElementType_> element)
		{
			auto otherElement(std::dynamic_pointer_cast<DestinationType_>(element));
			if (otherElement)
			{
				const auto& name(otherElement->GetSymbol());
				if (collection.find(name) == collection.end())
				{
					collection[name] = otherElement;
				}
				else
				{
					if (name.empty())
					{
						KAOS::Logging::Error("Default " + description + " already set");
					}
					else
					{
						KAOS::Logging::Error(description + " `" + name + "` already set");
					}
				}

			}
		}
	}

	const Root::factory_type& Root::GetExemplars() const
	{
		static const factory_type exemplars
		{
			{ "Objects", std::make_unique<Objects> },
			{ "TileDescriptor", std::make_unique<TileDescriptor> },
			{ "MapFile", std::make_unique<MapFile> },
			{ "objectdefs", std::make_unique<NullDescriptor> }
		};

		return exemplars;
	}


	bool Root::Parse(const pugi::xml_node& node)
	{
		if (!CompositeNode::Parse(node))
		{
			return false;
		}


		objects_type objectCollections;
		tiledescriptors_type tileDescriptors;
		mapfiledescriptors_type mapFileDescriptors;

		auto memberProcessor = [&](std::shared_ptr<DescriptorNode> member) -> void
		{
			AddToCollectionIf<Objects>("Object Collection", objectCollections, member);
			AddToCollectionIf<TileDescriptor>("Tile Descriptor", tileDescriptors, member);
			AddToCollectionIf<MapFile>("Map File Descriptor", mapFileDescriptors, member);
		};


		ForEachMember(memberProcessor);


		m_ObjectCollections = move(objectCollections);
		m_TileDescriptors = move(tileDescriptors);
		m_MapFileDescriptors = move(mapFileDescriptors);

		return true;
	}


	bool Root::ParseRequiresSymbol() const
	{
		return false;
	}


	std::shared_ptr<Objects> Root::QueryObjectsDescriptor(const std::string& name) const
	{
		auto descriptor(m_ObjectCollections.find(name));
		return descriptor != m_ObjectCollections.end() ? descriptor->second : nullptr;
	}


	std::shared_ptr<TileDescriptor> Root::QueryTileDescriptor(const std::string& name) const
	{
		auto descriptor(m_TileDescriptors.find(name));
		return descriptor != m_TileDescriptors.end() ? descriptor->second : nullptr;
	}


	std::shared_ptr<MapFile> Root::QueryMapFileDescriptor(const std::string& name) const
	{
		auto descriptor(m_MapFileDescriptors.find(name));
		return descriptor != m_MapFileDescriptors.end() ? descriptor->second : nullptr;
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
