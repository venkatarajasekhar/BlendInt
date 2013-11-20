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

#ifndef _BLENDINT_SCROLLBAR_HPP_
#define _BLENDINT_SCROLLBAR_HPP_

#include <boost/smart_ptr.hpp>
#include <BlendInt/Slider.hpp>

#include <BlendInt/GLBuffer.hpp>

namespace BlendInt {

	/**
	 * The controller in the scrollbar
	 */
	class ScrollControl: public AbstractButton
	{
	public:

		ScrollControl ();

		ScrollControl (AbstractWidget* parent);

		virtual ~ScrollControl ();

	protected:

		virtual void Update (int type, const void* data);

		virtual void Render ();

		virtual void move_mouse (MouseEvent* event);

		virtual void press_mouse (MouseEvent* event);

		virtual void release_mouse (MouseEvent* event);

	private:

		void Init ();

		Point m_move_start;

		Point m_position_origin;

		boost::scoped_ptr<GLBuffer> m_buffer;
	};

	/**
	 * @brief A slider used in ScrollBar with a dark background
	 */
	class SliderBar: public AbstractSlider
	{
		DISALLOW_COPY_AND_ASSIGN(SliderBar);

	public:

		SliderBar (Orientation orientation = Horizontal);

		SliderBar (Orientation orientation, AbstractWidget* parent);

		virtual ~SliderBar ();

	protected:

		virtual void Update (int type, const void* data);

		virtual void Render ();

	private:

		void Init ();

		void set_control_size (size_t size);

		boost::scoped_ptr<GLBuffer> m_buffer;

		ScrollControl* m_control_button;
	};

	class ScrollBar: public AbstractSlider
	{
		DISALLOW_COPY_AND_ASSIGN(ScrollBar);

	public:

		ScrollBar (Orientation orientation = Horizontal);

		ScrollBar (Orientation orientation, AbstractWidget* parent);

		virtual ~ScrollBar ();

	protected:

		virtual void Update (int type, const void* data);

		virtual void Render ();

		virtual void move_mouse (MouseEvent* event);

		virtual void press_mouse (MouseEvent* event);

		virtual void release_mouse (MouseEvent* event);

	private:

		void update_shape (const Size* size);

		/**
		 * @brief calculate and return the space width in which scroll control can move
		 * @return
		 */
		int get_space ();

		ScrollControl* m_scroll_control;

		boost::scoped_ptr<GLBuffer> m_buffer;

	};

}

#endif /* _BIL_SCROLLBAR_HPP_ */
