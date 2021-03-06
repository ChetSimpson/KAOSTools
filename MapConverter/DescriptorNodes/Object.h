//	Tiled Map Converter for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#pragma once
#include "DescriptorNodes/CompositeNode.h"
#include "Builder/MapDataSource.h"

namespace DescriptorNodes
{

	class Object
		:
		public CompositeNode,
		public Builder::MapDataSource::objectdescriptor_type
	{
	public:

		using typename_type = std::string;
		using typeid_type = uint64_t;


	public:

		bool Parse(const pugi::xml_node& node) override;
		virtual void SetBaseType(std::shared_ptr<Object> baseType);


		virtual typename_type GetBaseTypeName() const;
		virtual typename_type GetTypeName() const;
		virtual typeid_type	GetTypeId() const;


		bool CompileDefinition(definitionbuilder_type& builder) const override;
		virtual bool CompileDefinitionEx(definitionbuilder_type& builder) const;
		bool CompileSubInstance(databuilder_type& builder, const datasource_type& dataSource, const Configuration& configuration) const override;
		bool CompileInstance(databuilder_type& builder, const datasource_type& dataSource, const Configuration& configuration) const override;


	protected:

		const factory_type& GetExemplars() const override;
		bool ParseRequiresSymbol() const override;


	private:

		typename_type			m_BaseTypeName;
		typename_type			m_TypeName;
		typeid_type				m_TypeId;
		std::shared_ptr<Object>	m_BaseType;
	};

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
