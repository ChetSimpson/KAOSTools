//	Tiled Map Converter for KAOS on the Color Computer III
//	------------------------------------------------------
//	Copyright (C) 2018, by Chet Simpson
//	
//	This file is distributed under the MIT License. See notice at the end
//	of this file.
#pragma once
#include <DefinitionNodes/Defintion.h>
#include <KAOS/Common/EventConsole.h>
#include <pugixml/pugixml.hpp>
#include <vector>
#include <memory>


namespace Builder
{

	class DefinitionBuilder
	{
	public:

		using definition_type = DefinitionNodes::Defintion;
		using typeid_type = definition_type::typeid_type;
		using name_type = definition_type::name_type;
		using formatter_type = DefinitionNodes::Defintion::formatter_type;
		using shiftvalue_type = unsigned int;

		class Shifter
		{
		public:

			Shifter(DefinitionBuilder& builder, shiftvalue_type shift)
				:
				m_Builder(builder),
				m_Shift(shift),
				m_OldShiftValue(builder.SetValueShift(shift))
			{}

			~Shifter()
			{
				m_Builder.SetValueShift(m_OldShiftValue);
			}

		private:

			DefinitionBuilder&		m_Builder;
			const shiftvalue_type	m_OldShiftValue;
			const shiftvalue_type	m_Shift;
		};

	public:

		explicit DefinitionBuilder(KAOS::Common::EventConsole& eventConsole, unsigned int shift = 0);

		shiftvalue_type SetValueShift(shiftvalue_type shift);
		shiftvalue_type GetValueShift() const;

		virtual bool Begin(name_type name);
		virtual bool End();

		virtual bool AddVariable(name_type name, typeid_type type);
		virtual bool AddSymbolic(name_type name, long value);

		virtual bool Generate(formatter_type& formatter) const;


	private:

		KAOS::Common::EventConsole&						m_EventConsole;
		shiftvalue_type									m_Shift;
		definition_type*								m_CurrentDefinition = nullptr;
		std::vector<std::unique_ptr<definition_type>>	m_Definitions;
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
