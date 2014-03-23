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

#include <BlendInt/Types.hpp>

#include <BlendInt/Core/Freetype.hpp>

#ifdef USE_FONTCONFIG
#include <BlendInt/Core/FontConfig.hpp>
#endif

namespace BlendInt {

	Freetype::Freetype()
	: m_library(0),
	  m_face(0),
	  m_stroker(0),
	  m_valid(false),
	  m_unicode(false),
	  m_height(0),
	  m_ascender(0),
	  m_descender(0),
	  m_max_advance(0),
	  m_dpi(96)
	{

	}

	bool Freetype::Open (const std::string& filename,
							unsigned int size,
							unsigned int dpi)
	{
		// make sure close opened resources
		Close();

		FT_Error error;

		error = FT_Init_FreeType(&m_library);
		if (error) {
			DBG_PRINT_MSG("%s", "Cannot initialize FreeType library");
			Close ();
			return false;
		}

		error = FT_New_Face(m_library, filename.c_str(), 0, &m_face);
		if (error == FT_Err_Unknown_File_Format) {
			DBG_PRINT_MSG("Unknown font file format: %s", filename.c_str());
			Close ();
			return false;
		}
		if (error) {
			DBG_PRINT_MSG("%s", "Fail to generate a new Font Face");
			Close ();
			return false;
		}

		m_valid = true;          // now treat it success

		error = FT_Select_Charmap(m_face, FT_ENCODING_UNICODE);
		if (error) {
			DBG_PRINT_MSG("%s", "Cannot set the unicode character map");
		} else {
			m_unicode = true;
		}

		SetCharSize(size, dpi);
		return true;
	}

	bool Freetype::Open (const FT_Byte* buffer,
							FT_Long bufsize,
							FT_Long index,
							unsigned int size,
							unsigned int dpi)
	{
		// make sure close opened resources
		Close();

		FT_Error error;

		error = FT_Init_FreeType(&m_library);
		if (error) {
			DBG_PRINT_MSG("%s", "Cannot initialize FreeType library");
			Close ();
			return false;
		}

		error = FT_New_Memory_Face(m_library, buffer, bufsize, index, &m_face);
		if (error) {
			DBG_PRINT_MSG("%s", "Fail to generate a new Font Face from memory");
			Close ();
			return false;
		}

		m_valid = true;          // now treat it success

		error = FT_Select_Charmap(m_face, FT_ENCODING_UNICODE);
		if (error) {
			DBG_PRINT_MSG("%s", "Cannot set the unicode character map");
		} else {
			m_unicode = true;
		}
		
		SetCharSize(size, dpi);
		return true;
	}

	void Freetype::Close ()
	{
		if (m_stroker) {
			FT_Stroker_Done(m_stroker);
			m_stroker = 0;
		}

		if (m_face) {
			FT_Done_Face(m_face);
			m_face = 0;
		}

		if (m_library) {
			FT_Done_FreeType(m_library);
			m_library = 0;
		}

		m_valid = false;
		m_unicode = false;
	}

	Freetype::~Freetype ()
	{
		Close();
	}


	bool Freetype::isUseKerning (void)
	{
		if(!m_valid)
			return false;

		return FT_HAS_KERNING(m_face);
	}

	bool Freetype::LoadGlyph (FT_UInt glyph_index, FT_Int32 load_flags)
	{
		if (!m_valid)
			return false;

		FT_Error error;

		error = FT_Load_Glyph(m_face, glyph_index, load_flags);

		if (error) {
			DBG_PRINT_MSG("%s", "Fail to load glyph");
			return false;
		}

		return true;
	}

	/*
	bool FontEngine::setFontSize (unsigned int size, unsigned int dpi)
	{
		if (!_valid)
			return false;

		FT_Error error;

		_font.size = size; _dpi = dpi;

		error = FT_Set_Char_Size(_face, 0, (FT_F26Dot6) (_font.size * 64), _dpi, _dpi);
		if (error) {
			cerr << "The current font don't support the size, " << size
			        << " and dpi " << _dpi << endl;
			return false;
		}

		return true;
	}
	*/

	bool Freetype::SetCharSize (unsigned int size, unsigned int dpi)
	{
		if (!m_valid)
			return false;

		FT_Error error;
		m_dpi = dpi;

		// size_t hres = 64;

		/* Set char size */
		// error = FT_Set_Char_Size (_face, (int)(size * 64), 0, 72*hres, 72);
		//For some twisted reason, Freetype measures font size
		//in terms of 1/64ths of pixels.  Thus, to make a font
		//h pixels high, we need to request a size of h*64.
		//(h << 6 is just a prettier way of writting h*64)
		error = FT_Set_Char_Size(m_face, (long) (size << 6), 0, m_dpi, 0);
		if (error) {
			DBG_PRINT_MSG("The current font don't support the size, %ud and dpi %ud", size
					,m_dpi);
			return false;
		}

		m_height = m_face->size->metrics.height >> 6;
		m_ascender = m_face->size->metrics.ascender >> 6;
		m_descender = m_face->size->metrics.descender >> 6;
		m_max_advance = m_face->size->metrics.max_advance >> 6;

		return true;
	}

	bool Freetype::SetPixelSize(unsigned int width, unsigned int height)
	{
		FT_Error error;
		if(!m_valid) return false;

		error = FT_Set_Pixel_Sizes (m_face, width, height);
		if (error) {
			DBG_PRINT_MSG("%s", "Fail to set pixel sizes");
			return false;
		}

		return true;
	}

	FT_UInt Freetype::GetCharIndex (const FT_ULong charcode)
	{
		if (!m_valid)
			return 0;

		return (FT_Get_Char_Index(m_face, charcode));
	}

	bool Freetype::LoadCharacter (FT_ULong charcode, FT_Int32 load_flags)
	{
		if (!m_valid)
			return false;

		FT_Error error;

		error = FT_Load_Char(m_face, charcode, load_flags);
		if (error) {
			DBG_PRINT_MSG("%s", "Fail to load Character");
			return false;
		}

		return true;
	}

	bool Freetype::RenderGlyph (FT_Render_Mode render_mode)
	{
		if (!m_valid)
			return false;
		FT_Error error;

		error = FT_Render_Glyph(m_face->glyph, // glyph slot
		        render_mode);
		if (error) {
			DBG_PRINT_MSG("%s", "Fail to Render glyph");
			return false;
		}

		return true;
	}

	FT_GlyphSlot Freetype::GetGlyphSlot () const
	{
		if(!m_valid)
			return 0;

		return m_face->glyph;
	}

	bool Freetype::getKerning (FT_UInt left_glyph, FT_UInt right_glyph,
	        FT_UInt kern_mode, FT_Vector* akerning)
	{
		if ((!m_valid) || (m_face == NULL))
			return false;

		FT_Error error;
		error = FT_Get_Kerning(m_face, left_glyph, right_glyph, kern_mode,
		        akerning);
		if (error) {
			DBG_PRINT_MSG("%s", "Fail to get kerning");
			return false;
		}

		return true;
	}

	FT_Vector Freetype::GetKerning(FT_UInt left, FT_UInt right, FT_UInt mode)
	{
		FT_Vector kerning = {0, 0};

		if(m_valid && FT_HAS_KERNING(m_face)) {
			FT_Error error;
			error = FT_Get_Kerning(m_face, left, right, mode, &kerning);

			if(error) {
				DBG_PRINT_MSG("Fail to get kerning of %ud and %ud", left, right);
			}
		}

		return kerning;
	}


//	Tuple2l Freetype::getKerning (const wchar_t& left, const wchar_t& right,
//	        FT_UInt kerning_mode)
//	{
//		Tuple2l ret;	// {0, 0}
//
//		if ((!valid_) || (!face_))
//			return ret;
//
//		if (! FT_HAS_KERNING(face_))  return ret;
//
//		FT_Vector kerning;
//
//		bool result = getKerning(getCharIndex(left),
//		        getCharIndex(right), kerning_mode, &kerning);
//
//		if (!result)
//			return ret;
//
//		ret.vec.x = kerning.x;
//		ret.vec.y = kerning.y;
//
//		return ret;
//	}

	bool Freetype::SetLcdFilter (FT_LcdFilter filter)
	{
		if (!m_valid)
			return false;

		FT_Error error;

		error = FT_Library_SetLcdFilter(m_library, filter);

		if (error) {
			DBG_PRINT_MSG("%s", "Fail to set lcd filter");
			return false;
		}

		return true;
	}

	bool Freetype::SetLcdFilterWeights (unsigned char* weights)
	{
		if (!m_valid)
			return false;

		FT_Error error;

		error = FT_Library_SetLcdFilterWeights(m_library, weights);
		if (error) {
			DBG_PRINT_MSG("%s", "Fail to set lcd filter weights");
			return false;
		}

		return true;
	}

} /* namespace BlendInt */
