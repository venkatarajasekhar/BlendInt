/**
 * BlendInt demo
 */

#include <Cpp/Events.hpp>
#include <BlendInt/Core/Types.hpp>

#include <CVWindow/Window.hpp>
#include "MarkerBasedARContext.hpp"

using namespace BlendInt;
using namespace std;

int main(int argc, char* argv[])
{
	BLENDINT_EVENTS_INIT_ONCE_IN_MAIN;

	Init();

	GLFWwindow* win = CreateWindow("OpenCV Demo", 1280, 800);

	MarkerBasedARContext* context = Manage(new MarkerBasedARContext(win));
	DBG_SET_NAME(context, "Context");
	SetContext(context);
	context->Resize(1280, 800);

	RunLoop (win);
	Terminate();

	return 0;
}