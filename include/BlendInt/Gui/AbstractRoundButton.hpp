/*
 * This file is part of BlendInt (a Blender-like Interface Library in
 * OpenGL).
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is free
 * software: you can redistribute it and/or modify it under the terms
 * of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is
 * distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General
 * Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BlendInt.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#ifndef _BLENDINT_GUI_ABSTRACTROUNDBUTTON_HPP_
#define _BLENDINT_GUI_ABSTRACTROUNDBUTTON_HPP_

#include <BlendInt/Gui/AbstractButton.hpp>
#include <BlendInt/Gui/RoundShapeBase.hpp>

namespace BlendInt {

	/**
	 * @brief The most common button class
	 *
	 * @ingroup widgets
	 */
	class AbstractRoundButton: public AbstractButton, public RoundShapeBase
	{
		DISALLOW_COPY_AND_ASSIGN(AbstractRoundButton);

	public:

		AbstractRoundButton ();

		virtual ~AbstractRoundButton ();

		/**
		 * @brief set round type
		 * @param type
		 *
		 * Hide the same function in RoundBoxBase to call Update
		 */
		void SetRoundType (int type);

		/**
		 * @brief set round radius
		 * @param radius
		 * Hide the same function in RoundBoxBase to call Update
		 */
		void SetRadius (float radius);
	};

}



#endif /* _BLENDINT_GUI_ABSTRACTROUNDBUTTON_HPP_ */