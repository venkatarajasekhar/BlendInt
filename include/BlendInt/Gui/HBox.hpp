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

#ifndef _BLENDINT_GUI_HBOX_HPP_
#define _BLENDINT_GUI_HBOX_HPP_

#include <BlendInt/Gui/AbstractDequeContainer.hpp>

namespace BlendInt {

	class HBox: public AbstractDequeContainer
	{
		DISALLOW_COPY_AND_ASSIGN(HBox);

	public:

		HBox(int align = AlignHorizontalCenter, int space = 4);

		virtual ~HBox ();

		bool Add (AbstractWidget* widget);

		bool Remove (AbstractWidget* widget);

		int alignment () const
		{
			return m_alignment;
		}

		void SetAlignment (int align);

		void SetSpace (int space);

		int space () const
		{
			return m_space;
		}

		virtual Size GetPreferredSize () const;

	protected:

		virtual bool UpdateTest (const UpdateRequest& request);

		virtual void Update (const UpdateRequest& request);

		virtual ResponseType Draw (const RedrawEvent& event);

		virtual ResponseType CursorEnterEvent (bool entered);

		virtual ResponseType KeyPressEvent (const KeyEvent& event);

		virtual ResponseType ContextMenuPressEvent (const ContextMenuEvent& event);

		virtual ResponseType ContextMenuReleaseEvent (const ContextMenuEvent& event);

		virtual ResponseType MousePressEvent (const MouseEvent& event);

		virtual ResponseType MouseReleaseEvent (const MouseEvent& event);

		virtual ResponseType MouseMoveEvent (const MouseEvent& event);

		void set_alignment (int align)
		{
			m_alignment = align;
		}

		void set_space (int space)
		{
			m_space = space;
		}

	private:

		void FillSubWidgetsInHBox (const Point& out_pos, const Size& out_size, const Margin& margin, int alignment, int space);

		void FillSubWidgetsInHBox (const Point& pos, const Size& size, int alignment, int space);

		void FillSubWidgetsProportionally (int x, int y, unsigned int width, unsigned int height, int alignment, int space);

		/**
		 * @brief distribute horizontally with preferred size
		 */
		void DistributeWithPreferredWidth (int x,
				int space,
		        const std::deque<unsigned int>* expandable_preferred_widths,
		        const std::deque<unsigned int>* unexpandable_preferred_widths);

		void DistributeWithSmallWidth (int x,
						unsigned int width,
						int space,
						const std::deque<unsigned int>* expandable_preferred_widths,
						unsigned int expandable_prefer_sum,
						const std::deque<unsigned int>* unexpandable_preferred_widths,
						unsigned int unexpandable_prefer_sum);

		void DistributeWithLargeWidth (int x,
						unsigned int width,
						int space,
						const std::deque<unsigned int>* expandable_preferred_widths,
						unsigned int expandable_prefer_sum,
						const std::deque<unsigned int>* unexpandable_preferred_widths,
						unsigned int unexpandable_prefer_sum);

		void Align (int y, unsigned int height, int alignment, const std::deque<unsigned int>* unexpandable_preferred_heights);

		int m_alignment;

		int m_space;

	};

}

#endif /* _BLENDINT_GUI_HBOX_HPP_ */
