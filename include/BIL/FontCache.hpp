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

#ifndef _BIL_FONTCACHE_H_
#define _BIL_FONTCACHE_H_

#include <map>
#include <functional>
#include <wchar.h>

#include <boost/array.hpp>

#include <BIL/FontEngine.hpp>
#include <BIL/Font.hpp>
#include <BIL/Glyph.hpp>

using namespace std;
using namespace boost;

namespace BIL {

	template<class T>
	struct greater_second: std::binary_function<T, T, bool>
	{
		inline bool operator() (const T& lhs, const T& rhs)
		{
			return lhs.second > rhs.second;
		}
	};

	/**
	 * @brief Class in charge of caching fonts
	 */
	class FontCache
	{
	public:
		// static member functions

		/**
		 * @brief create or get the FontCache object
		 * @param font Font, default is regular "Sans"
		 * @param force if true, remove unused cache and create one
		 * @return cache object created, NULL for failure
		 */
		static FontCache* create (const Font& font = Font("Sans"), bool force =
		        true);

		static FontCache* getCache (const Font& font = Font("Sans"));

		static bool release (const Font& font = Font("Sans"));

		static void releaseAll (void);

		static void setCacheSize (unsigned int size)
		{
			cacheSize = size;
		}

		static void setMaxCaches (unsigned int size)
		{
			maxCaches = size;
		}


#ifdef DEBUG
		static void list (void);
#endif

	public:

		/**
		 * @brief Initialize glyph database
		 * @return true for success, false for failure
		 */
		bool initialize (void);

		/**
		 * @brief query the Glyph of a wchar
		 * @param charcode wchar character
		 * @param create create a glyph if not found
		 * @return pointer to a Glyph object
		 *
		 * @warning Do not delete the pointer get from this funciton
		 */
		Glyph* query (wchar_t charcode, bool create = true);

		int getHeight (void)
		{
			if(fontengine_ == NULL) return 0;
			
			return fontengine_->getHeight();
		}

		int getAscender (void)
		{
			if(fontengine_ == NULL) return 0;
			return fontengine_->getAscender();
		}

		int getDescender (void)
		{
			if(fontengine_ == NULL) return 0;
			return fontengine_->getDescender();
		}

		int getMaxAdvance (void)
		{
			if(fontengine_ == NULL) return 0;
			return fontengine_->getMaxAdvance();
		}

		Tuple2l getKerning (const Glyph& left, const Glyph& right,
				FT_UInt kern_mode = FT_KERNING_DEFAULT)
		{
			return fontengine_->getKerning(left, right, kern_mode);
		}

		const FontEngine* fontengine (void) const
		{
			return fontengine_;
		}

#ifdef DEBUG
		void printcount (void);
#endif

	private:

		/**
		 * @brief Default constructor
		 * @param font Font type
		 * @param dpi the DPI to be used
		 */
		FontCache (const Font& font, unsigned int dpi = 96);

		/**
		 * @brief private destructor
		 *
		 * Move destructor to private area to avoid this object be deleted
		 * in anywhere.
		 */
		virtual ~FontCache ();

	private:
		// member variables

		Font font_;

		unsigned int dpi_;

		FontEngine* fontengine_;

		bool initialized_;

		array<Glyph*, 128> ascii_db_;    // use arrary to store ascii for speed

		map<wchar_t, Glyph*> glyph_db_;

		map<wchar_t, unsigned long> count_db_;

	private:
		// static members

		static unsigned int cacheSize;

		static unsigned int maxCaches;

		static map<Font, FontCache*> cacheDB;
		static map<Font, unsigned long> cacheCountDB;

	private:
		// member functions disabled

		FontCache ();

		FontCache (const FontCache& orig);

		FontCache& operator = (const FontCache& orig);
	};

} /* namespace BIL */
#endif /* FONTCACHE_H_ */
