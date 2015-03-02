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

#pragma once

#include <core/types.hpp>
#include <gui/abstract-layout.hpp>

namespace BlendInt {

	/**
	 * @brief AdaptiveLayout tries to arrange sub widgets with their preferred size
	 */
	class AdaptiveLayout: public AbstractLayout
	{
	public:

		AdaptiveLayout (Orientation orient = Horizontal,
		        int align = AlignCenter, int space = 4);

		AdaptiveLayout (int width, int height, const Margin& margin,
		        Orientation orient = Horizontal, int align = AlignCenter,
		        int space = 4);

		virtual ~AdaptiveLayout ();

		virtual bool AddWidget (AbstractWidget* widget);

		virtual bool InsertWidget (int index, AbstractWidget* widget);

		virtual bool InsertWidget (int row, int column, AbstractWidget* widget);

	protected:

		virtual void PerformMarginUpdate (const Margin& margin);

		virtual bool SizeUpdateTest (const SizeUpdateRequest& request);

		virtual bool PositionUpdateTest (const PositionUpdateRequest& request);

		virtual void PerformSizeUpdate (const SizeUpdateRequest& request);

	private:

		Orientation orientation_;

		int alignment_;

		int space_;

		DISALLOW_COPY_AND_ASSIGN(AdaptiveLayout);
	};

}