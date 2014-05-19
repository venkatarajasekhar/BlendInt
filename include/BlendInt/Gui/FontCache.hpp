/*
 * This file is part of BlendInt (a Blender-like Interface Library in
 * OpenGL).
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is free software:
 * you can redistribute it and/or modify it under the terms of the GNU
 * Lesser General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is distributed in
 * the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
 * more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BlendInt.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#ifndef _BLENDINT_FONTCACHE_HPP_
#define _BLENDINT_FONTCACHE_HPP_

#include <map>
#include <functional>
#include <wchar.h>
#include <string>

#include <glm/mat4x4.hpp>

#include <BlendInt/Core/String.hpp>

#include <BlendInt/Core/Object.hpp>
#include <BlendInt/Core/Freetype.hpp>
#include <BlendInt/Gui/Glyph.hpp>
#include <BlendInt/Gui/TextureGlyph.hpp>
#include <BlendInt/Gui/TextureAtlas.hpp>

using namespace std;

namespace BlendInt {

	template<class T>
	struct greater_second: std::binary_function<T, T, bool>
	{
		inline bool operator() (const T& lhs, const T& rhs)
		{
			return lhs.second > rhs.second;
		}
	};

	enum FontStyleFlag {
		FontStyleBold = 0x0,
		FontStyleItalic = 0x1 << 1,
		FontStyleOutline = 0x1 << 2
	};

	/**
	 * @brief Basic structure to identify a font file and texture atlas
	 */
	struct FontTypeBase {

		FontTypeBase ()
		: size(9),
		  flag(0),
		  dpi(72),
		  thickness(0.5f)
		{

		}

		~FontTypeBase ()
		{

		}

		FontTypeBase (const FontTypeBase& orig)
		: name(orig.name),
		  size(orig.size),
		  flag(orig.flag),
		  dpi(orig.dpi),
		  thickness(orig.thickness)
		{

		}

		FontTypeBase& operator = (const FontTypeBase& orig)
		{
			name = orig.name;
			size = orig.size;
			flag = orig.flag;
			dpi = orig.dpi;
			thickness = orig.thickness;

			return *this;
		}

		std::string name;	/** The font name or filepath to the font file */
		unsigned int size;	/** Character size want to be loaded */
		unsigned int flag;	/** The font style flag, see FontStyleFlag */
		unsigned int dpi;	/** The DPI used in Freetype setting */
		float thickness;	/** The thickness of outline */
	};

	struct FontFileInfo {
		std::string file;	/** The path to the font file */
		unsigned int size;	/** Character size want to be loaded */
		unsigned int dpi;	/** The DPI used in Freetype setting */
		bool bold;			/** Search bold font file */
		bool italic;		/** Search itatlic font file */
	};

	extern bool operator < (const FontFileInfo& src, const FontFileInfo& dist);
	extern bool operator == (const FontFileInfo& src, const FontFileInfo& dist);

	extern bool operator < (const FontTypeBase& src, const FontTypeBase& dist);
	extern bool operator == (const FontTypeBase& src, const FontTypeBase& dist);

	class FontCacheExt: public Object
	{
	public:

		static RefPtr<FontCacheExt> Create (const FontTypeBase& data);

		static bool Release (const FontTypeBase& data);

		static void ReleaseAll ();

		static size_t GetCacheSize ();

		static void SetDefaultTextureSize (int width, int height);

		static int GetDefaultTextureWidth ();

		static int GetDefaultTextureHeight ();

#ifdef DEBUG
		static void list ();
#endif

		const GlyphExt* Query (const FontTypeBase& font_data, uint32_t charcode, bool create = true);

	private:

		friend class Font;
		template <typename T> friend class RefPtr;

		FontCacheExt (const FontTypeBase& data);

		~FontCacheExt ();

		void Initialize (const FontTypeBase& font_data, uint32_t char_code = 32, int size = 95);

		void SetGlyphData (GlyphExt& glyph, FT_GlyphSlot slot, const RefPtr<TextureAtlas2D>& atlas);

		void SetGlyphData (GlyphExt& glyph, FT_GlyphSlot slot, FT_BitmapGlyph bitmap_glyph, const RefPtr<TextureAtlas2D>& atlas);

		GLuint m_vao;

		GLuint m_vbo;

		uint32_t m_start;
		int m_size;

		std::vector<GlyphExt> m_preset;

		std::map<uint32_t, GlyphExt> m_extension;

		RefPtr<TextureAtlas2D> m_last;

		FTLibrary m_ft_lib;
		FTFace m_ft_face;

		static map<FontTypeBase, RefPtr<FontCacheExt> > cache_db;

		static int default_texture_width;
		static int default_texture_height;
	};

	// -----------------------------------------------------

	/**
	 * @brief Class in charge of caching fonts
	 */
	class FontCache: public Object
	{
	public:
		// static member functions

		/**
		 * @brief create or get the FontCache object
		 * @param font Font, default is regular "Sans"
		 * @param force if true, remove unused cache and create one
		 * @return cache object created, NULL for failure
		 */
		//static FontCache* create (const Font& font = Font("Sans"),
				//unsigned int dpi = 96, bool force = true);

		static RefPtr<FontCache> Create (const std::string& file, unsigned int size, unsigned int dpi, bool bold, bool italic, bool force = true);

		//static FontCache* getCache (const Font& font = Font("Sans"), unsigned int dpi = 96);

		//static bool release (const Font& font = Font("Sans"), unsigned int dpi = 96);

		static bool Release (const FontFileInfo& key);

		static void releaseAll ();

		static void setMaxCaches (unsigned int size)
		{
			maxCaches = size;
		}

#ifdef DEBUG
		static void list ();
#endif

	public:

		/**
		 * @brief query the Glyph of a wchar
		 * @param charcode wchar character
		 * @param create create a glyph if not found
		 * @return pointer to a Glyph object
		 *
		 * @warning Do not delete the pointer get from this funciton
		 */
		//Glyph* query (wchar_t charcode, bool create = true);

		//const GLuint queryTexture (wchar_t charcode, bool create = true);

		//unsigned int queryWidth (wchar_t charcode, bool create = true);

		//unsigned int queryHeight (wchar_t charcode, bool create = true);

		int get_height ()
		{
			return m_freetype.height();
		}

		int get_ascender ()
		{
			return m_freetype.ascender();
		}

		int get_descender ()
		{
			return m_freetype.descender();
		}

		int get_max_advance ()
		{
			return m_freetype.max_advance();
		}

//		Tuple2l getKerning (const wchar_t& left, const wchar_t& right,
//				FT_UInt kern_mode = FT_KERNING_DEFAULT)
//		{
//			return m_freetype->getKerning(left, right, kern_mode);
//		}

		const Freetype* fontengine () const
		{
			return &m_freetype;
		}

		void set_dpi (unsigned int dpi)
		{
			m_freetype.set_dpi (dpi);
		}

		/**
		 * @brief get the rectangle contains the given text
		 * @param string
		 * @return
		 */
		Rect GetTextOutline (const String& string);

		/**
		 * @brief Get the text width contains the given text with the length
		 * @param string
		 * @param length
		 * @param start The index where start to calculate the width
		 * @return
		 */
		size_t GetTextWidth (const String& string, size_t length, size_t start = 0);

		size_t GetReverseTextWidth (const String& string, size_t length, size_t start = 0);

#ifdef DEBUG
		void printcount ();
#endif

	private:

		friend class Font;
		template <typename T> friend class RefPtr;

		/**
		 * @brief Default constructor
		 * @param font Font type
		 * @param dpi the DPI to be used
		 */
		//FontCache (const Font& font, unsigned int dpi = 96);

		FontCache (const FontFileInfo& info, unsigned int dpi = 96);

		/**
		 * @brief private destructor
		 *
		 * Move destructor to private area to avoid this object be deleted
		 * in anywhere.
		 */
		~FontCache ();

		/**
		 * @brief Initialize glyph database
		 * @return true for success, false for failure
         *
         * @warning Do not simply initialize all characer in ascii
         * cause Render error. I dont't know why this happens
		 */
		bool Setup ();

		const Glyph& query (wchar_t charcode, bool create = true);

		GLuint m_vao;

		GLuint m_vbo;

		Freetype m_freetype;

		TextureAtlas m_atlas;

		map<wchar_t, TextureGlyph*> m_texture_fonts;

		static unsigned int maxCaches;

		static map<FontFileInfo, RefPtr<FontCache> > cacheDB;
		static map<FontFileInfo, unsigned long> cacheCountDB;

		// the following are disabled

		FontCache ();

		FontCache (const FontCache& orig);

		FontCache& operator = (const FontCache& orig);
	};

} /* namespace BlendInt */
#endif /* FONTCACHE_H_ */
