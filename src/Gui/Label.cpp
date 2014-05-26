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

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <BlendInt/Gui/Label.hpp>
#include <BlendInt/Service/ShaderManager.hpp>

namespace BlendInt {

	Label::Label (const String& text)
		: Widget(),
		  m_text(text),
		  m_text_length(0),
		  m_background_color(0x00000000),
		  m_vao(0)
	{
		InitLabel(text);
	}

	Label::~Label ()
	{
		glDeleteVertexArrays(1, &m_vao);
	}

	void Label::SetText (const String& text)
	{
		m_text = text;
		m_text_length = UpdateTextPosition(size(), text, m_font);
	}

	void Label::SetFont (const Font& font)
	{
		m_font = font;
		m_text_length = UpdateTextPosition(size(), m_text, m_font);
	}

	void Label::UpdateGeometry (const WidgetUpdateRequest& request)
	{
		switch (request.type()) {

			case WidgetSize: {
				const Size* size_p = static_cast<const Size*>(request.data());
				m_text_length = UpdateTextPosition(*size_p, m_text, m_font);

				glBindVertexArray(m_vao);
				m_rect->Bind();

				std::vector<GLfloat> vertices(12);
				GenerateFlatRectVertices(*size_p, 0.f, &vertices);

				m_rect->SetData(sizeof(GLfloat) * vertices.size(),
								&vertices[0]);

				m_rect->Reset();

				glBindVertexArray(0);

				break;
			}

			default:
				Widget::UpdateGeometry(request);
		}

	}

	ResponseType Label::Draw (const RedrawEvent& event)
	{
		glm::vec3 pos((float)position().x(), (float)position().y(), (float)z());
		glm::mat4 mvp = glm::translate(event.projection_matrix() * event.view_matrix(), pos);

		glBindVertexArray(m_vao);

		RefPtr<GLSLProgram> program = ShaderManager::instance->default_triangle_program();
		program->Use();

		program->SetUniformMatrix4fv("MVP", 1, GL_FALSE, glm::value_ptr(mvp));
		program->SetVertexAttrib4f("Color",
						m_background_color.r() / 255.f,
						m_background_color.g() / 255.f,
						m_background_color.b() / 255.f,
						m_background_color.a() / 255.f);
		program->SetUniform1i("AA", 0);

		m_rect->Bind();
		glEnableVertexAttribArray(0);	// 0 is the locaiton in shader

		glVertexAttribPointer(
						0, // attribute
						2,		// number of elements per vertex, here (x,y)
						GL_FLOAT,	// the type of each element
						GL_FALSE,	// take our values as-is
						0,		// no extra data between each position
						BUFFER_OFFSET(0)	// the first element
		);

		glDrawArrays(GL_TRIANGLE_FAN, 0, 6);

		glDisableVertexAttribArray(0);

		m_rect->Reset();

		program->Reset();
		glBindVertexArray(0);

		if(m_text.length()) {
			m_font.Print(mvp, m_text, m_text_length, 0);
		}

		return Accept;
	}

	size_t Label::UpdateTextPosition(const Size& size, const String& text, Font& font)
	{
		size_t str_len = 0;

		// If size changed, we need to update the text length for printing too.
		bool cal_width = true;

		int width = size.width() - 2 - 2;
		int height = size.height() - 2 - 2;

		if(width <= 0 || height <= 0) {
			return 0;
		}

		if(text.length() == 0) {
			return 0;
		}

		Rect text_outline = font.GetTextOutline(text);

		if(static_cast<unsigned int>(height) < text_outline.height()) {
			str_len = 0;
			cal_width = false;
		}

		if(cal_width) {
			if(static_cast<unsigned int>(width) < text_outline.width()) {
				str_len = GetValidTextSize(size, text, font);
			} else {
				str_len = text.length();
			}
		}

		font.set_pen((size.width() - text_outline.width()) / 2,
						(size.height() - font.GetHeight()) / 2
										+ std::abs(font.GetDescender()));
		return str_len;
	}

	size_t Label::GetValidTextSize(const Size& size, const String& text, const Font& font)
	{
		size_t width = 0;
		size_t str_len = text.length();

		width = font.GetTextWidth(text, str_len, 0);

		unsigned int text_width_space = size.width() - 2 - 2;

		if(width > text_width_space) {
			while(str_len > 0) {
				width = font.GetTextWidth(text, str_len, 0);
				if(width < text_width_space) break;
				str_len--;
			}
		}

		return str_len;
	}
	
	Size Label::GetPreferredSize () const
	{
		Size preferred_size;

		int max_font_height = font().GetHeight();

		preferred_size.set_height(max_font_height + 2 + 2);	// top padding: 2, bottom padding: 2

		if (m_text.empty()) {
			preferred_size.set_width(
							max_font_height + 2 + 2);
		} else {
			size_t width = font().GetTextWidth(m_text);
			preferred_size.set_width(
							static_cast<unsigned int>(width)
											+ 2 + 2);	// left padding: 2, right padding: 2
		}

		return preferred_size;
	}

	bool Label::IsExpandX() const
	{
		return true;
	}

	void Label::InitLabel (const String& text)
	{
		m_text = text;

		unsigned int h = m_font.GetHeight();

		if(text.empty()) {
			set_size (h + 2 + 2,
							h + 2 + 2);
		} else {
			m_text_length = text.length();
			Rect text_outline = m_font.GetTextOutline(text);

			unsigned int width = text_outline.width() + 2 + 2;
			unsigned int height = h + 2 + 2;
			set_size(width, height);
			m_font.set_pen((width - text_outline.width()) / 2,
							(height - m_font.GetHeight()) / 2 +
											std::abs(m_font.GetDescender()));
		}

		glGenVertexArrays(1, &m_vao);
		glBindVertexArray(m_vao);
		m_rect.reset(new GLArrayBuffer);
		m_rect->Generate();
		m_rect->Bind();
		std::vector<GLfloat> vertices(12);
		GenerateFlatRectVertices(size(), 0.f, &vertices);
		m_rect->SetData(sizeof(GLfloat) * vertices.size(), &vertices[0]);
		m_rect->Reset();
		glBindVertexArray(0);
	}

} /* namespace BlendInt */

