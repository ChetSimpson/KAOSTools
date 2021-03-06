//	Tiled Map Converter for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#include "DescriptorNodes/Object.h"
#include "DescriptorNodes/TypedValue.h"
#include "DescriptorNodes/PackedValue.h"
#include <KAOS/Common/Logging.h>


namespace DescriptorNodes
{

	const Object::factory_type& Object::GetExemplars() const
	{
		static const factory_type exemplars
		{
			{ "Byte", std::make_unique<ByteProperty> },
			{ "Word", std::make_unique<WordProperty> },
			{ "Quad", std::make_unique<QuadProperty> },
			{ "PackedByte", std::make_unique<PackedByteProperty> },
			{ "PackedWord", std::make_unique<PackedWordProperty> },
			{ "PackedQuad", std::make_unique<PackedQuadProperty> }
		};

		return exemplars;
	}


	bool Object::ParseRequiresSymbol() const
	{
		return false;
	}


	bool Object::Parse(const pugi::xml_node& node)
	{
		///////////////////////////////////////////////////////////////////////////
		//
		//	Parse type name
		//
		///////////////////////////////////////////////////////////////////////////
		const auto& typenameAttr(node.attribute("type"));
		if (typenameAttr.empty())
		{
			KAOS::Logging::MissingAttributeError(node, "type");
			return false;
		}


		//	If id is not present the object can only be used as a base for other objects.
		const auto& typeidAttr(node.attribute("id"));

		const auto& extendsAttr(node.attribute("extends"));


		if (!CompositeNode::Parse(node))
		{
			return false;
		}
	

		m_TypeName = typenameAttr.as_string();
		m_TypeId = typeidAttr.as_ullong(0ULL);
		m_BaseTypeName = extendsAttr.as_string();

		return true;
	}




	void Object::SetBaseType(std::shared_ptr<Object> baseType)
	{
		m_BaseType = baseType;
	}




	Object::typename_type Object::GetBaseTypeName() const
	{
		return m_BaseTypeName;
	}

	Object::typename_type Object::GetTypeName() const
	{
		return m_TypeName;
	}

	Object::typeid_type	Object::GetTypeId() const
	{
		return m_TypeId;
	}




	bool Object::CompileDefinition(definitionbuilder_type& builder) const
	{
		builder.Begin(m_TypeName);
		builder.AddSymbolic("Id_", m_TypeId);
		if (!CompileDefinitionEx(builder))
		{
			return false;
		}
		builder.End();

		return true;
	}


	bool Object::CompileDefinitionEx(definitionbuilder_type& builder) const
	{
		if (m_BaseType)
		{
			m_BaseType->CompileDefinitionEx(builder);
		}

		return CompositeNode::CompileDefinition(builder);
	}


	bool Object::CompileSubInstance(databuilder_type& builder, const datasource_type& dataSource, const Configuration& configuration) const
	{
		if (m_BaseType)
		{
			m_BaseType->CompileSubInstance(builder, dataSource, configuration);
		}

		return CompositeNode::CompileInstance(builder, dataSource, configuration);
	}


	bool Object::CompileInstance(databuilder_type& builder, const datasource_type& dataSource, const Configuration& configuration) const
	{
		builder.EmitComment("Instance of object type `" + GetTypeName() + "`");

		if (m_BaseType)
		{
			m_BaseType->CompileSubInstance(builder, dataSource, configuration);
		}

		return CompositeNode::CompileInstance(builder, dataSource, configuration);
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
