//	Tiled Map Converter for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#pragma once
#include "DescriptorNodes/TypedNode.h"


namespace DescriptorNodes
{

	class PackedValue : public TypedNode
	{
	public:

		using TypedNode::TypedNode;


	protected:

		const factory_type& GetExemplars() const override;

		std::optional<collection_type> ParseMembers(const pugi::xml_node& node, const factory_type& exemplars) const override;

		bool CompileInstanceMembers(databuilder_type& builder, const datasource_type& dataSource, const Configuration& configuration) const override;
	};


	template<TypedNode::typeid_type TypeId_>
	class PackedValueT : public PackedValue
	{
	public:

		using PackedValue::PackedValue;

		PackedValueT()
			: PackedValue(TypeId_)
		{}

	};

	using PackedByteProperty = PackedValueT<TypedNode::typeid_type::Byte>;
	using PackedWordProperty = PackedValueT<TypedNode::typeid_type::Word>;
	using PackedQuadProperty = PackedValueT<TypedNode::typeid_type::Quad>;

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
