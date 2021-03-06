//	Tiled Map Converter for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#include "DescriptorNodes/ObjectLayer.h"
#include "DescriptorNodes/Object.h"
#include "Builder/MapDataSource.h"
#include "Builder/ObjectDataSource.h"
#include <KAOS/Common/xml.h>
#include <KAOS/Common/Logging.h>


namespace DescriptorNodes
{

	bool ObjectLayer::ParseRequiresSymbol() const
	{
		return false;
	}


	ObjectLayer::signature_type ObjectLayer::GetSignature() const
	{
		return Signature;
	}


	std::string ObjectLayer::GetType() const
	{
		return "Object";
	}




	bool ObjectLayer::CompileDefinition(definitionbuilder_type& /*builder*/) const
	{
		return true;
	}


	bool ObjectLayer::CompileInstanceHeader(databuilder_type& builder, const datasource_type& dataSource, const Configuration& configuration) const
	{
		auto& mapDataSource(dynamic_cast<const Builder::MapDataSource&>(dataSource));
		auto layer(mapDataSource.QueryObjectLayer(m_SourceLayerName));
		if (!layer)
		{
			KAOS::Logging::Error("Unable to find object layer `" + m_SourceLayerName + "`");
			return false;
		}

		if (!Layer::CompileInstanceHeader(builder, dataSource, configuration))
		{
			return false;
		}

		builder.EmitValue(std::string(), static_cast<Builder::DataBuilder::property_type::word_type>(layer->size()), "Number of objects");
		builder.EmitComment("");

		return true;
	}


	bool ObjectLayer::CompileInstanceBody(databuilder_type& builder, const datasource_type& dataSource, const Configuration& configuration) const
	{
		auto& mapDataSource(dynamic_cast<const Builder::MapDataSource&>(dataSource));
		auto layer(mapDataSource.QueryObjectLayer(m_SourceLayerName));
		if (!layer)
		{
			KAOS::Logging::Error("Unable to find object layer `" + m_SourceLayerName + "`");
			return false;
		}


		if (!Layer::CompileInstanceBody(builder, dataSource, configuration))
		{
			return false;
		}


		bool hasError(false);
		for (const auto& object : *layer)
		{
			const auto& objectTypename(object->GetType());
			if (objectTypename.empty())
			{
				KAOS::Logging::Error("Object does not have a type assigend to it. Unable to generate code.");
				hasError = true;
				continue;
			}

			const auto objectDescriptorTmp(mapDataSource.QueryObjectDescriptor(object->GetType()));
			if (!objectDescriptorTmp)
			{
				KAOS::Logging::Error("Unable to find object descriptor `" + objectTypename + "`");
				hasError = true;
				continue;
			}


			const auto objectDescriptor(std::dynamic_pointer_cast<Object>(objectDescriptorTmp));
			if (!objectDescriptor)
			{
				throw std::runtime_error("Unable to convert object to descriptor.");
			}
			
			Builder::ObjectDataSource objectDataSource(
				*object,
				objectDescriptor->GetTypeId(),
				dataSource.QueryMap(),
				dataSource.QueryTilesetCache(),
				dataSource.QueryDescriptors(),
				configuration);
			if (!objectDescriptor->CompileInstance(builder, objectDataSource, configuration))
			{
				hasError = true;
			}
		}

		return !hasError;
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
