/*
 * This file is part of BIL (Blender Interface Library).
 *
 * BIL (Blender Interface Library) is free software: you can
 * redistribute it and/or modify it under the terms of the GNU Lesser
 * General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * BIL (Blender Interface Library) is distributed in the hope that it
 * will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BIL.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#include <GL/glew.h>
#include <GL/gl.h>

#include <BIL/FontEngine.hpp>
#include <BIL/FontConfig.hpp>
#include <BIL/TextBuffer.hpp>

#include <BIL/Tuple.hpp>

namespace BIL {

	TextBuffer::TextBuffer (const Font& font)
		: rowspacing_(1.0),
		  foreground_(1.0, 1.0, 1.0, 1.0),
		  background_(0.0, 0.0, 0.0, 0.0)
	{
		set_font(font);
		//glShadeModel(GL_FLAT);
		//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	}

	TextBuffer::TextBuffer (const wstring& text, const Font& font)
		: rowspacing_(1.0), foreground_(1.0, 1.0, 1.0, 1.0), background_(0.0, 0.0, 0.0, 0.0)
	{
		set_font(font);
		Append(text);

		//glShadeModel(GL_FLAT);
		//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	}

	TextBuffer::~TextBuffer ()
	{
	}

	void TextBuffer::Append (const wstring& text)
	{
		if (text.empty())
			return;

		text_.append(text);
	}

	void TextBuffer::Append (wchar_t charcode)
	{
		text_.push_back(charcode);
	}

	Vec2ui TextBuffer::calculateBox (void)
	{
		wstring::const_iterator it;
		wstring::const_iterator next;
		Vec2ui box;
		Vec2l kerning;
		Glyph* glyph = NULL;

		unsigned int line_width = 0;
		unsigned int line = 1;

		for (it = text_.begin(); it != text_.end(); it++) {
			if (*it == '\n') {
				line++;
				box.vec.x = box.vec.x > line_width ? box.vec.x : line_width;
			}

			glyph = fontcache_->query(*it);
			if (glyph != NULL) {
				// add kerning support
				next = it + 1;
				if(next != text_.end()) {
					kerning = fontcache_->getKerning(*it, *next);
				}
				line_width = glyph->getMetrics().horiAdvance + kerning.vec.x + line_width;
			}
		}

		box.vec.x = box.vec.x > line_width ? box.vec.x : line_width;
		box.vec.y = fontcache_->getHeight() * line + (line - 1) * rowspacing_;

		return box;
	}

	void TextBuffer::Render (void)
	{
		Glyph* glyph = NULL;

		//glDisable(GL_LIGHTING);
		//glDisable(GL_DEPTH_TEST);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glColor4f(foreground_.rgba.r, foreground_.rgba.g, foreground_.rgba.b, foreground_.rgba.a);

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glTranslatef(origin_.coord.x,
					 origin_.coord.y - fontcache_->getDescender(),
					 origin_.coord.z);

		int line = 0;
		wstring::const_iterator it;
		for (it = text_.begin(); it != text_.end(); it++) {
			if (*it == '\n') {
				line++;
				glLoadIdentity();
				glTranslatef(origin_.coord.x,
							 origin_.coord.y
							 - rowspacing_ * fontcache_->getHeight() * line,
							 origin_.coord.z);
				continue;
			}

			glyph = fontcache_->query(*it);
			if (glyph != NULL) {
				glyph->Render();
				glTranslatef(glyph->getMetrics().horiAdvance, 0, 0);
			}
		}

		glPopMatrix();
		glDisable(GL_BLEND);

	}

	void TextBuffer::Clear (void)
	{
		text_.clear();
	}

} /* namespace BIL */
