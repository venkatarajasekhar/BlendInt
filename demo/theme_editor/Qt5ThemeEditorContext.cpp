/*
 * Qt5Context.cpp
 */

#include "Qt5ThemeEditorContext.hpp"
#include <QtGui/QCursor>

#include <BlendInt/Gui/ColorSelector.hpp>
#include <BlendInt/Gui/HBlockLayout.hpp>

#include <BlendInt/Gui/Screen.hpp>

Qt5ThemeEditorContext::Qt5ThemeEditorContext (QWindow* window)
: BI::Context (), window_(window)
{

	Initialize();

}

Qt5ThemeEditorContext::~Qt5ThemeEditorContext ()
{
}

void Qt5ThemeEditorContext::SetCursor(int cursor_type)
{
	if(window_) {
		window_->setCursor(QCursor(static_cast<Qt::CursorShape>(cursor_type)));
	}
}

void Qt5ThemeEditorContext::Initialize()
{
	using namespace BI;

	StaticPanel* panel1 = CreateButtonPanel();
	panel1->SetPosition(940, 720);
	panel1->Resize(panel1->GetPreferredSize());

	StaticPanel* panel2 = CreateColorPanel();
	panel2->SetPosition(980, 200);
	panel2->Resize(panel2->GetPreferredSize());

	Screen* screen = Manage(new Screen);
	screen->Resize(panel1->size());
	screen->Setup(panel1);

	Screen* screen1 = Manage(new Screen);
	screen1->Resize(panel2->size());
	screen1->Setup(panel2);

	AddScreen(screen);
	AddScreen(screen1);

}

BI::StaticPanel* Qt5ThemeEditorContext::CreateButtonPanel ()
{
	using namespace BI;

	StaticPanel* panel = Manage(new StaticPanel);

	HBlockLayout* block = Manage(new HBlockLayout);

	Button* btn1 = Manage(new Button("Button"));
	Button* btn2 = Manage(new Button("ToggleButton"));
	Button* btn3 = Manage(new Button("ToolButton"));
	Button* btn4 = Manage(new Button("Button"));

	block->Append(btn1);
	block->Append(btn2);
	block->Append(btn3);
	block->Append(btn4);

	panel->Setup(block);

	return panel;
}

BI::StaticPanel* Qt5ThemeEditorContext::CreateColorPanel ()
{
	using namespace BI;

	StaticPanel* panel = Manage(new StaticPanel);

	ColorSelector* cs = Manage(new ColorSelector);

	panel->Setup(cs);

	return panel;
}
