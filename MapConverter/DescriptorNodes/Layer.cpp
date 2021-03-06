//	Tiled Map Converter for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#include "DescriptorNodes/Layer.h"
#include "DescriptorNodes/Object.h"
#include "Builder/MapDataSource.h"
#include "Builder/ObjectDataSource.h"
#include <KAOS/Common/xml.h>
#include <KAOS/Common/Logging.h>


namespace DescriptorNodes
{

	bool Layer::Parse(const pugi::xml_node& node)
	{
		decltype(m_SourceLayerName) sourceLayerName;
		decltype(m_LayerId) layerId;

		if (   !KAOS::Common::XML::LoadAttribute(node, "src", sourceLayerName)
			|| !KAOS::Common::XML::LoadAttribute(node, "id", layerId)
			|| !DescriptorNode::Parse(node))
		{
			return false;
		}

		decltype(m_Signature) signature;
		KAOS::Common::XML::LoadAttribute(node, "signature", signature);
		

		m_SourceLayerName = move(sourceLayerName);
		m_LayerId = move(layerId);
		m_Signature = move(signature);

		return true;
	}




	bool Layer::CompileInstance(databuilder_type& builder, const datasource_type& dataSource, const Configuration& configuration) const
	{
		return
			CompileInstanceHeader(builder, dataSource, configuration)
			&& CompileInstanceBody(builder, dataSource, configuration)
			&& CompileInstanceFooter(builder, dataSource, configuration);
	}


	bool Layer::CompileInstanceHeader(databuilder_type& builder, const datasource_type& /*dataSource*/, const Configuration& /*configuration*/) const
	{
		builder.EmitBlank();
		builder.EmitSeparatorComment();
		builder.EmitComment(GetType() + " layer `" + m_SourceLayerName + "`");
		builder.EmitSeparatorComment();
		if (!GetSymbol().empty())
		{
			builder.EmitLabel(GetSymbol());
		}

		const signature_type signature(m_Signature.has_value() ? static_cast<uint16_t>(m_Signature.value()) : GetSignature());
		builder.EmitValue(std::string(), signature, "Block signature");
		if (m_LayerId.has_value())
		{
			builder.EmitValue(std::string(), static_cast<Builder::DataBuilder::property_type::word_type>(*m_LayerId), "Layer Id");
		}

		return true;
	}


	bool Layer::CompileInstanceBody(databuilder_type& /*builder*/, const datasource_type& /*dataSource*/, const Configuration& /*configuration*/) const
	{
		return true;
	}

	bool Layer::CompileInstanceFooter(databuilder_type& /*builder*/, const datasource_type& /*dataSource*/, const Configuration& /*configuration*/) const
	{
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
