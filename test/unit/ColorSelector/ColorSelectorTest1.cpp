#include "ColorSelectorTest1.hpp"
#include <BlendInt/Gui/ColorSelector.hpp>

using namespace BlendInt;

ColorSelectorTest1::ColorSelectorTest1()
: testing::Test()
{
	// TODO: add constructor code
}

ColorSelectorTest1::~ColorSelectorTest1()
{
	// TODO: add destructor code
}

/**
 * test Foo() method
 *
 * Expected result: 
 */
TEST_F(ColorSelectorTest1, Foo1)
{
	Init ();

    GLFWwindow* win = CreateWindow("ColorSelector - Foo1");

    // TODO: add test code here
	Context* context = Manage (new Context);
    Interface::instance->SetCurrentContext(context);

    ColorSelector* cs = Manage(new ColorSelector);
    cs->SetPosition(200, 200);

    context->PushBack(cs);

    RunLoop(win);

    Interface::Release();

    Terminate();

	ASSERT_TRUE(true);
}