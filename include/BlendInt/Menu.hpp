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

#ifndef _BLENDINT_MENU_HPP_
#define _BLENDINT_MENU_HPP_

#include <list>

#include <BlendInt/String.hpp>
#include <BlendInt/MenuItem.hpp>

namespace BlendInt {

	/**
	 * @brief The menu widget for use in menu bars, context menus,
	 * and other popup menus.
	 */
	class Menu
	{
	public:

		friend class MenuItem;

		Menu ();

		Menu (const String& title);

		~Menu();

		inline void set_title (const String& title)
		{
			m_title = title;
		}

		inline const String& title () const {return m_title;}

		inline size_t size () const {return m_list.size();}

		inline std::list<MenuItem*>& list () {return m_list;}

		void Add (const String& text);

		void Add (FormBase* icon, const String& text);

		void Add (MenuItem* item);

		void SetParent (MenuItem* item);

		void Remove (MenuItem* item);

		void Delete (MenuItem* item);

#ifdef DEBUG
		void print_menu_items ();
#endif

	private:

		String m_title;

		MenuItem* m_parent;

		std::list<MenuItem*> m_list;
	};

}

#endif /* _BLENDINT_MENU_HPP_ */
