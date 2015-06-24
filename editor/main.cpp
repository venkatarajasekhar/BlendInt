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

#include "editor-window.hpp"
#include <iostream>

#include <blendint/gui/dialog.hpp>
#include <blendint/gui/frame.hpp>
#include <blendint/gui/linear-layout.hpp>
#include <blendint/gui/push-button.hpp>
#include <blendint/gui/toggle-button.hpp>
#include <blendint/gui/color-button.hpp>
#include <blendint/gui/label.hpp>
#include <blendint/gui/menu.hpp>
#include <blendint/gui/radio-button.hpp>
#include <blendint/gui/combo-box.hpp>
#include <blendint/gui/textentry.hpp>
#include <blendint/gui/splitter.hpp>
#include <blendint/gui/node-view.hpp>
#include <blendint/gui/filebutton.hpp>
#include <blendint/gui/menu-button.hpp>

int main (int argc, char* argv[])
{
  using namespace BlendInt;

  std::cout << "sizeof (AbstractView): " << sizeof(AbstractView) << std::endl;
  if (Window::Initialize()) {
    //EditorWindow win(1280, 800, "UI Editor");

    Window win(1000, 600, "UI Editor");

    Dialog* frame = new Dialog("Test");
    //Menu* frame = new Menu;
    // Frame* frame = new Frame(new LinearLayout(Horizontal));
    frame->Resize(500, 400);
    frame->MoveTo((win.size().width() - frame->size().width()) / 2,
                  (win.size().height() - frame->size().height()) / 2);
    
    // frame->EnableViewBuffer();

    Label* label = new Label("Hello World!");
    label->SetBackground(Color(0xE02F2F9F));
    frame->AddWidget(label);

    frame->AddWidget(new PushButton("Push Button"));
    frame->AddWidget(new ToggleButton("Toggle Button"));
    frame->AddWidget(new RadioButton("Radio Button"));
    frame->AddWidget(new ComboBox);
    frame->AddWidget(new TextEntry);
    frame->AddWidget(new ColorButton);
    frame->AddWidget(new FileButton);
    frame->AddWidget(new MenuButton("Test Menu Button"));

    win.AddFrame(frame);

    Dialog* dlg2 = new Dialog("Test2");

    Splitter* splitter = new Splitter(Vertical);
    PushButton* btn1 = new PushButton("Button1");
    PushButton* btn2 = new PushButton("Button2");
    PushButton* btn3 = new PushButton("Button3");

    splitter->AddWidget(btn1);
    splitter->AddWidget(btn2);
    splitter->AddWidget(btn3);

    dlg2->AddWidget(splitter);
    
    win.AddFrame(dlg2);

    Dialog* dlg3 = new Dialog("Test3");

    Splitter* splitter2 = new Splitter(Horizontal);
    PushButton* btn4 = new PushButton("Button4");
    PushButton* btn5 = new PushButton("Button5");
    PushButton* btn6 = new PushButton("Button6");

    splitter2->AddWidget(btn4);
    splitter2->AddWidget(btn5);
    splitter2->AddWidget(btn6);

    dlg3->AddWidget(splitter2);
    dlg3->MoveTo(200, 200);
    
    win.AddFrame(dlg3);

    Dialog* dlg4 = new Dialog("Test grid-guides");

    NodeView* view = new NodeView();
    dlg4->AddWidget(view);

    win.AddFrame(dlg4);
    
    win.Exec();
    Window::Terminate();
  }
  
  return 0;
}
