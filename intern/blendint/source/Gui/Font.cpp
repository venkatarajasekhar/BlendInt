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

#ifdef __UNIX__
#ifdef __APPLE__
#include <gl3.h>
#include <gl3ext.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif
#endif  // __UNIX__

#include <string.h>
#include <cwchar>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <BlendInt/Core/Types.hpp>
#include <BlendInt/Gui/Font.hpp>
#include <BlendInt/Stock/Theme.hpp>
#include <BlendInt/Stock/Shaders.hpp>

namespace BlendInt {

	using Stock::Shaders;

	Font::Font (const std::string& name, unsigned int size, int flag)
	: m_shadow(false),
	  m_shadow_offset_x(1.5f),
	  m_shadow_offset_y(-1.5f)
	{
		m_data.name = name;
		m_data.size = size;
		m_data.flag = flag;
		m_data.dpi = Theme::instance->dpi();
		m_color = 0x000000FF;
		m_cache = FontCache::Create(m_data);
	}

	Font::Font (const Font& orig)
	: m_data(orig.m_data),
	  m_pen(orig.m_pen),
	  m_color(orig.m_color),
	  m_shadow(false),
	  m_shadow_offset_x(1.5f),
	  m_shadow_offset_y(-1.5f)
	{
		m_cache = orig.m_cache;
	}

	Font& Font::operator = (const Font& orig)
	{
		m_data = orig.m_data;
		m_pen = orig.m_pen;
		m_color = orig.m_color;
		m_cache = orig.m_cache;
		m_shadow = orig.m_shadow;
		m_shadow_offset_x = orig.m_shadow_offset_x;
		m_shadow_offset_y = orig.m_shadow_offset_y;

		m_cache = orig.m_cache;

		return *this;
	}

	void Font::SetName (const std::string& name)
	{
		m_data.name = name;
		m_cache = FontCache::Create(m_data);
	}

	void Font::SetSize (unsigned int size)
	{
		m_data.size = size;
		m_cache = FontCache::Create(m_data);
	}

	void Font::SetBold (bool bold)
	{
		if(bold) {
			SETBIT(m_data.flag, FontStyleBold);
		} else {
			CLRBIT(m_data.flag, FontStyleBold);
		}

		m_cache = FontCache::Create(m_data);
	}

	void Font::SetItalic (bool italic)
	{
		if(italic) {
			SETBIT(m_data.flag, FontStyleItalic);
		} else {
			CLRBIT(m_data.flag, FontStyleItalic);
		}

		m_cache = FontCache::Create(m_data);
	}

	void Font::SetOutline(bool outline, float thickness)
	{
		if(outline) {
			SETBIT(m_data.flag, FontStyleOutline);
		} else {
			CLRBIT(m_data.flag, FontStyleOutline);
		}
		m_data.thickness = thickness;

		m_cache = FontCache::Create(m_data);
	}

	void Font::SetShadow(bool shadow, float offset_x, float offset_y)
	{
		m_shadow = shadow;
		m_shadow_offset_x = offset_x;
		m_shadow_offset_y = offset_y;
	}

	int Font::Print (const Point& pos, const std::string& string,
	        size_t start) const
	{
		return Print(pos.x(), pos.y(), string, string.length(), start);
	}

	int Font::Print (const Point& pos, const std::string& string, size_t length,
	        size_t start) const
	{
		return Print(pos.x(), pos.y(), string, length, start);
	}

	int Font::Print (const glm::vec2& pos, const std::string& string, size_t length, size_t start) const
	{
		return Print(pos.x, pos.y, string, length, start);
	}

	int Font::Print (const glm::vec3& pos, const std::string& string, size_t length, size_t start) const
	{
		return Print(pos.x, pos.y, string, length, start);
	}

	int Font::Print (float x, float y, const std::string& string, size_t start) const
	{
		return Print(x, y, string, string.length(), start);
	}

	int Font::Print (float x, float y, const std::string& string, size_t length, size_t start) const
	{
		if(length == 0)	return 0;

		int advance = 0;	// the return value

		RefPtr<GLSLProgram> program = Shaders::instance->widget_text_program();
		program->use();

		glActiveTexture(GL_TEXTURE0);

		glUniform1i(Shaders::instance->location(Stock::WIDGET_TEXT_TEXTURE), 0);

		size_t str_length = std::min(string.length(), length);

		// TODO: support left->right, and right->left text
		std::string::const_iterator it;
		const GlyphExt* glyph_p = 0;

		glBindVertexArray(m_cache->m_vao);
		glBindBuffer(GL_ARRAY_BUFFER, m_cache->m_vbo);

		float tx = x + m_pen.x() + m_shadow_offset_x;
		float ty = y + m_pen.y() + m_shadow_offset_y;

		if(m_shadow) {

			glUniform2f(Shaders::instance->location(Stock::WIDGET_TEXT_POSITION), tx, ty);
			glUniform4f(Shaders::instance->location(Stock::WIDGET_TEXT_COLOR),
					m_color.r() / 4,
					m_color.g() / 4,
					m_color.b() / 4,
					m_color.a() / 4);

			it = string.begin();
			std::advance(it, start);

			for (size_t i = 0; i < str_length; it++, i++) {
				glyph_p = m_cache->Query(m_data, *it);
				glBindTexture(GL_TEXTURE_2D, glyph_p->texture->id());

				advance += glyph_p->advance_x;
				glBufferData(GL_ARRAY_BUFFER, sizeof(GlyphVertex) * 4,
								&(glyph_p->vertices[0]),
								GL_DYNAMIC_DRAW);
				glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

				tx += glyph_p->advance_x;
				glUniform2f(Shaders::instance->location(Stock::WIDGET_TEXT_POSITION), tx, ty);
			}

		}

		tx = x + m_pen.x();
		ty = y + m_pen.y();

		glUniform2f(Shaders::instance->location(Stock::WIDGET_TEXT_POSITION), tx, ty);
		glUniform4fv(Shaders::instance->location(Stock::WIDGET_TEXT_COLOR), 1, m_color.data());

		it = string.begin();
		std::advance(it, start);

		for (size_t i = 0; i < str_length; it++, i++) {

			glyph_p = m_cache->Query(m_data, *it);
			glBindTexture(GL_TEXTURE_2D, glyph_p->texture->id());

			advance += glyph_p->advance_x;

			glBufferData(GL_ARRAY_BUFFER, sizeof(GlyphVertex) * 4,
							&(glyph_p->vertices[0]),
							GL_DYNAMIC_DRAW);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

			tx += glyph_p->advance_x;
			glUniform2f(Shaders::instance->location(Stock::WIDGET_TEXT_POSITION), tx, ty);
		}

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
		program->reset();

		return advance;
	}

	int Font::Print (const Point& pos, const String& string, size_t length,
	        size_t start) const
	{
		return Print(pos.x(), pos.y(), string, length, start);
	}

	int Font::Print (const Point& pos, const String& string, size_t start) const
	{
		return Print(pos.x(), pos.y(), string, string.length(), start);
	}

	int Font::Print (const glm::vec2& pos, const String& string, size_t length,
	        size_t start) const
	{
		return Print(pos.x, pos.y, string, length, start);
	}

	int Font::Print (const glm::vec3& pos, const String& string, size_t length,
	        size_t start) const
	{
		return Print(pos.x, pos.y, string, length, start);
	}

	int Font::Print (float x, float y, const String& string, size_t start) const
	{
		return Print(x, y, string, string.length(), start);
	}

	int Font::Print (float x, float y, const String& string, size_t length,
	        size_t start) const
	{
		if(length == 0)	return 0;

		int advance = 0;	// the return value

		RefPtr<GLSLProgram> program = Shaders::instance->widget_text_program();
		program->use();

		glActiveTexture(GL_TEXTURE0);

		glUniform1i(Shaders::instance->location(Stock::WIDGET_TEXT_TEXTURE), 0);

		size_t str_length = std::min(string.length(), length);

		// TODO: support left->right, and right->left text
		String::const_iterator it;
		const GlyphExt* glyph_p = 0;

		glBindVertexArray(m_cache->m_vao);
		glBindBuffer(GL_ARRAY_BUFFER, m_cache->m_vbo);

		float tx = x + m_pen.x() + m_shadow_offset_x;
		float ty = y + m_pen.y() + m_shadow_offset_y;

		if(m_shadow) {

			glUniform2f(Shaders::instance->location(Stock::WIDGET_TEXT_POSITION), tx, ty);
			glUniform4f(Shaders::instance->location(Stock::WIDGET_TEXT_COLOR),
					m_color.r() / 4,
					m_color.g() / 4,
					m_color.b() / 4,
					m_color.a() / 4);

			it = string.begin();
			std::advance(it, start);

			for (size_t i = 0; i < str_length; it++, i++) {
				glyph_p = m_cache->Query(m_data, *it);
				glBindTexture(GL_TEXTURE_2D, glyph_p->texture->id());

				advance += glyph_p->advance_x;
				glBufferData(GL_ARRAY_BUFFER, sizeof(GlyphVertex) * 4,
								&(glyph_p->vertices[0]),
								GL_DYNAMIC_DRAW);
				glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

				tx += glyph_p->advance_x;
				glUniform2f(Shaders::instance->location(Stock::WIDGET_TEXT_POSITION), tx, ty);
			}

		}

		tx = x + m_pen.x();
		ty = y + m_pen.y();

		glUniform2f(Shaders::instance->location(Stock::WIDGET_TEXT_POSITION), tx, ty);
		glUniform4fv(Shaders::instance->location(Stock::WIDGET_TEXT_COLOR), 1, m_color.data());

		it = string.begin();
		std::advance(it, start);

		for (size_t i = 0; i < str_length; it++, i++) {

			glyph_p = m_cache->Query(m_data, *it);
			glBindTexture(GL_TEXTURE_2D, glyph_p->texture->id());

			advance += glyph_p->advance_x;

			glBufferData(GL_ARRAY_BUFFER, sizeof(GlyphVertex) * 4,
							&(glyph_p->vertices[0]),
							GL_DYNAMIC_DRAW);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

			tx += glyph_p->advance_x;
			glUniform2f(Shaders::instance->location(Stock::WIDGET_TEXT_POSITION), tx, ty);
		}

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
		program->reset();

		return advance;
	}

	Rect Font::GetTextOutline (const String& string) const
	{
		int xmin = 0;
		int ymin = 0;
		int xmax = 0;
		int ymax = 0;

		const GlyphExt* glyph_p = 0;

		for (String::const_iterator it = string.begin(); it != string.end(); it++)
		{
			glyph_p = m_cache->Query(m_data, *it);
			xmax += glyph_p->advance_x;
			ymin = std::min(static_cast<int>(glyph_p->bitmap_top - glyph_p->bitmap_height), ymin);
			ymax = std::max(static_cast<int>(glyph_p->bitmap_top), ymax);
		}

		return Rect(Point(xmin, ymin), Point(xmax, ymax));
	}

	size_t Font::GetTextWidth (const std::string& string) const
	{
		size_t width = 0;

		std::string::const_iterator it = string.begin();

		while(it != string.end()) {
			width += m_cache->Query(m_data, *it)->advance_x;
			it++;
		}

		return width;
	}

	size_t Font::GetTextWidth (const std::string& string, size_t length,
	        size_t start) const
	{
		size_t width = 0;
		std::string::const_iterator it = string.begin();
		std::advance(it, start);
		size_t i = 0;

		while(it != string.end() && (i < length)) {
			width += m_cache->Query(m_data, *it)->advance_x;
			it++;
			i++;
		}

		return width;
	}

	size_t Font::GetTextWidth (const String& string) const
	{
		size_t width = 0;

		String::const_iterator it = string.begin();

		while(it != string.end()) {
			width += m_cache->Query(m_data, *it)->advance_x;
			it++;
		}

		return width;
	}

	size_t Font::GetTextWidth (const String& string, size_t length,
	        size_t start) const
	{
		size_t width = 0;
		String::const_iterator it = string.begin();
		std::advance(it, start);
		size_t i = 0;

		while(it != string.end() && (i < length)) {
			width += m_cache->Query(m_data, *it)->advance_x;
			it++;
			i++;
		}

		return width;
	}

	size_t Font::GetReversedTextWidth (const std::string& string) const
	{
		size_t width = 0;

		std::string::const_reverse_iterator it = string.rbegin();

		while (it != string.rend()) {
			width += m_cache->Query(m_data, *it)->advance_x;
			it++;
		}

		return width;
	}

	size_t Font::GetReversedTextWidth (const std::string& string,
	        size_t length, size_t start) const
	{
		size_t width = 0;
		std::string::const_reverse_iterator it = string.rbegin();
		std::advance(it, start);
		size_t i = 0;

		while (it != string.rend() && (i < length)) {
			width += m_cache->Query(m_data, *it)->advance_x;
			it++;
			i++;
		}

		return width;
	}

	size_t Font::GetReversedTextWidth (const String& string) const
	{
		size_t width = 0;

		String::const_reverse_iterator it = string.rbegin();

		while (it != string.rend()) {
			width += m_cache->Query(m_data, *it)->advance_x;
			it++;
		}

		return width;
	}

	size_t Font::GetReversedTextWidth (const String& string,
	        size_t length, size_t start) const
	{
		size_t width = 0;
		String::const_reverse_iterator it = string.rbegin();
		std::advance(it, start);
		size_t i = 0;

		while (it != string.rend() && (i < length)) {
			width += m_cache->Query(m_data, *it)->advance_x;
			it++;
			i++;
		}

		return width;
	}

}