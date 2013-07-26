/* Test case for FontFace */

#ifndef _GLYPH_TEST_H
#define _GLYPH_TEST_H

#include <cppunit/extensions/HelperMacros.h>
#include <iostream>
#include <string>

using namespace std;

class GlyphTest: public CppUnit::TestFixture
{
CPPUNIT_TEST_SUITE(GlyphTest);

	CPPUNIT_TEST(create1);
	//CPPUNIT_TEST(create2);
	CPPUNIT_TEST(printtext1);
	//CPPUNIT_TEST(create4);
	//CPPUNIT_TEST(create5);

	CPPUNIT_TEST_SUITE_END()
	;

public:

	GlyphTest ();

	virtual ~GlyphTest ();

	void setUp ();

	void tearDown ();

	void runTest ();

private:

	void create1 ();
	void create2 ();
	void printtext1 ();
	void create4 ();
	void create5 ();
};

#endif  /* _GLYPH_TEST_H */