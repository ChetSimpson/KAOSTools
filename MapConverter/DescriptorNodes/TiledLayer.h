//	Tiled Map Converter for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#pragma once
#include "DescriptorNode.h"
#include "Builder/MapDataSource.h"


namespace DescriptorNodes
{

	class TiledLayer : public DescriptorNode
	{
	public:

		using signature_type = databuilder_type::property_type::word_type;
		static const signature_type Signature = ('T' << 8) | 'L';


	public:

		bool Parse(const pugi::xml_node& node) override;

		bool CompileDefinition(definitionbuilder_type& builder) const override;
		bool CompileInstance(databuilder_type& builder, const datasource_type& dataSource, const Configuration& config) const override;

	protected:

		using rowcontainer_type = std::vector<Builder::MapDataSource::tilesetlayer_type::cell_type>;
		using rlecontainer_type = std::vector<std::pair<unsigned int, rowcontainer_type::value_type>>;

		virtual rowcontainer_type compressRowData(rowcontainer_type rowData) const;
		virtual rlecontainer_type mergeRunLenths(rlecontainer_type rleData) const;

	private:

		std::optional<uint64_t>		m_LayerIndex;
		std::optional<std::string>	m_LayerName;
		std::optional<uint64_t>		m_Signature;
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
