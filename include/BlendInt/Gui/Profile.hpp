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

#ifndef _BLENDINT_GUI_PROFILE_HPP_
#define _BLENDINT_GUI_PROFILE_HPP_

#include <BlendInt/OpenGL/GLArrayBuffer.hpp>

namespace BlendInt {

	class Context;
	class Section;

	/**
	 * @brief Global setting for widget drawing
	 *
	 * Profile is created in Context -- the root container in a OpenGL window,
	 * and was passed to each widget when drawing.
	 *
	 * It stores the global setting of the opengl context, as well as provide
	 * the methods to manipulate the Stencil Buffer.
	 *
	 * Usage of stencil buffer methods:
	 *
	 * @code
	 * BeginPushStencil();
	 * draw a shape (usually draw the inner buffer)
	 * EndPushStencil();
	 *
	 * and draw something you'd like
	 *
	 * BeginPopStencil();
	 * draw the same shape again
	 * EndPopStencil();
	 * @endcode
	 *
	 * @note Push and pop to stencil buffer must be called in pairs
	 */
	class Profile
	{
	public:

		Profile ()
		: context_(0),
		  section_(0),
		  stencil_count_(0)
		{
		}

		Profile (const Profile& orig)
		: context_(orig.context_),
		  section_(orig.section_),
		  stencil_count_(0)
		{
		}

		~Profile ()
		{
		}

		Context* context () const
		{
			return context_;
		}

		Section* section () const
		{
			return section_;
		}

		void BeginPushStencil ();

		void EndPushStencil ();

		void BeginPopStencil ();

		void EndPopStencil ();

	private:

		friend class Context;
		friend class Section;

		Context* context_;
		Section* section_;

		GLuint stencil_count_;
	};

}

#endif /* _BLENDINT_GUI_PROFILE_HPP_ */
