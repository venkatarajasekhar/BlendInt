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

#ifndef _BLENDINT_SCROLLWIDGET_HPP_
#define _BLENDINT_SCROLLWIDGET_HPP_

//#include <BlendInt/ScrollBar.hpp>
#include <BlendInt/Widget.hpp>

namespace BlendInt {

	class Slider;

	class ScrollWidget: public Widget
	{

	public:

		ScrollWidget ();

		ScrollWidget (Drawable* parent);

		virtual ~ScrollWidget ();

		void set_viewport (Widget* widget);

		Widget* viewport () const {return m_viewport;}

	protected:

		virtual void render ();

	private:

		Widget* m_viewport;

		// demo code

		Slider* m_hslider;
		Slider* m_vslider;
	};

}

#endif /* _BIL_SCROLLWIDGET_HPP_ */