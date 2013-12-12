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

#include <iostream>

#include <BlendInt/StockIcon.hpp>

namespace BlendInt {

	StockIcon* StockIcon::stock_icon = 0;

	bool StockIcon::Initialize()
	{
		if (!stock_icon)
			stock_icon = new StockIcon;

		return true;
	}

	void StockIcon::Release()
	{
		if (!stock_icon) {
			delete stock_icon;
			stock_icon = 0;
		}
	}

	StockIcon* StockIcon::Instance()
	{
		if (!stock_icon) {
			std::cerr << "Stock Icons are not initialized successfully! Exit"
			        << std::endl;
			exit(EXIT_FAILURE);
		}

		return stock_icon;
	}

	StockIcon::StockIcon()
	: m_menu(0), m_circle(0), m_checkbox(0), m_num(0)
	{
		CreateIcons();
	}

	StockIcon::~StockIcon()
	{
	}

	void StockIcon::CreateIcons()
	{
		float vec[16][2];

		m_menu = new VertexIcon;
		for(size_t i = 0; i < 6; i++)
		{
			vec[i][0] = 0.5 * 17 * VertexIcon::menu_tria_vert[i][0];
			vec[i][1] = 0.5 * 17 * VertexIcon::menu_tria_vert[i][1];
		}

		m_menu->load (vec, 6, VertexIcon::menu_tria_face, 2);

		m_circle = new VertexIcon;

		for(size_t i = 0; i < 16; i++)
		{
			vec[i][0] = 0.5 * 10 * VertexIcon::scroll_circle_vert[i][0];
			vec[i][1] = 0.5 * 10 * VertexIcon::scroll_circle_vert[i][1];
		}

		m_circle->load (vec, 16, VertexIcon::scroll_circle_face, 14);

		m_checkbox = new VertexIcon;

		for(size_t i = 0; i < 6; i++)
		{
			vec[i][0] = 0.5 * 14 * VertexIcon::check_tria_vert[i][0];
			vec[i][1] = 0.5 * 14 * VertexIcon::check_tria_vert[i][1];
		}

		m_checkbox->load (vec, 6, VertexIcon::check_tria_face, 4);

		m_num = new VertexIcon;

		for(size_t i = 0; i < 3; i++)
		{
			vec[i][0] = 0.5 * 14 * VertexIcon::num_tria_vert[i][0];
			vec[i][1] = 0.5 * 14 * VertexIcon::num_tria_vert[i][1];
		}

		m_num->load (vec, 3, VertexIcon::num_tria_face, 1);

		// check icon

	}
}