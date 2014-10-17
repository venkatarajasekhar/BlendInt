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

#ifndef _BLENDINT_GUI_TOOLBOX_HPP_
#define _BLENDINT_GUI_TOOLBOX_HPP_

#include <BlendInt/Gui/Layout.hpp>
#include <BlendInt/OpenGL/GLBuffer.hpp>
#include <BlendInt/Gui/AbstractFrame.hpp>

namespace BlendInt {

	class ToolBoxExt: public AbstractFrame
	{
	public:

		ToolBoxExt ();

		virtual ~ToolBoxExt ();

		void AddWidget (Widget* widget);

		virtual bool IsExpandX () const;

		virtual bool IsExpandY () const;

		virtual Size GetPreferredSize () const;

		Widget* focused_widget () const
		{
			return focused_widget_;
		}

		Widget* hovered_widget () const
		{
			return hovered_widget_;
		}

	protected:

		virtual bool SizeUpdateTest (const SizeUpdateRequest& request);

		virtual void PerformPositionUpdate (const PositionUpdateRequest& request);

		virtual void PerformSizeUpdate (const SizeUpdateRequest& request);

		virtual void PreDraw (Profile& profile);

		virtual ResponseType Draw (Profile& profile);

		virtual void PostDraw (Profile& profile);

		virtual void FocusEvent (bool focus);

		virtual void MouseHoverInEvent (const MouseEvent& event);

		virtual void MouseHoverOutEvent (const MouseEvent& event);

		virtual ResponseType KeyPressEvent (const KeyEvent& event);

		virtual ResponseType MousePressEvent (const MouseEvent& event);

		virtual ResponseType MouseReleaseEvent (const MouseEvent& event);

		virtual ResponseType MouseMoveEvent (const MouseEvent& event);

		virtual void DispatchHoverEvent (const MouseEvent& event);

	private:

		void FillSubWidgets ();

		int GetLastPosition () const;

		void SetFocusedWidget (Widget* widget);

		void OnFocusedWidgetDestroyed (Widget* widget);

		void OnHoverWidgetDestroyed (Widget* widget);

		glm::mat4 projection_matrix_;

		glm::mat4 model_matrix_;

		Widget* focused_widget_;

		Widget* hovered_widget_;

		int space_;

		GLuint vao_;

		GLBuffer<> inner_;

		Margin margin_;
	};

	// -------------------------------------

	class ToolBox: public Layout
	{
		DISALLOW_COPY_AND_ASSIGN(ToolBox);

	public:

		ToolBox();

		virtual ~ToolBox();

		void Append (AbstractWidget* widget);

		virtual bool IsExpandY () const;

		virtual Size GetPreferredSize () const;

	protected:

		virtual bool SizeUpdateTest (const SizeUpdateRequest& request);

		virtual void PerformMarginUpdate (const Margin& request);

		virtual void PerformSizeUpdate (const SizeUpdateRequest& request);

		virtual ResponseType Draw (Profile& profile);

	private:

		void FillSubWidgets (const Size& out_size, const Margin& margin, int space);

		void FillSubWidgets (int x, int y, int width, int height, int space);

		int GetLastPosition () const;

		GLuint vao_;

		int space_;

		GLBuffer<> inner_;

	};

}

#endif /* _BLENDINT_GUI_TOOLBOX_HPP_ */
