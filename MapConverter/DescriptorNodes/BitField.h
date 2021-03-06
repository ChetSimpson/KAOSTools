//	Tiled Map Converter for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#pragma once
#include "DescriptorNodes/CompositeNode.h"
#include "DescriptorNodes/PropertyQuery.h"


namespace DescriptorNodes
{

	class BitField : public CompositeNode
	{
	public:

		using size_type = size_t;
		using mask_type = uint32_t;


		bool Parse(const pugi::xml_node& node) override;
		virtual void SetInfo(mask_type mask, size_type shiftCount);

		virtual size_type GetBitsUsed() const;

		bool CompileDefinition(definitionbuilder_type& builder) const override;
		bool CompileInstance(databuilder_type& builder, const datasource_type& dataSource, const Configuration& configuration) const override;


	protected:

		const factory_type& GetExemplars() const override;

		virtual std::optional<KAOS::Tiled::NamedProperty> Resolve(const datasource_type& dataSource) const;

		
	private:

		std::unique_ptr<PropertyQuery>	m_Query;
		size_type	m_Bits = 0;
		mask_type	m_Mask = 0;
		size_type	m_Shift = 0;
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
