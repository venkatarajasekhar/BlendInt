/**
 * Main Layout
 */

#include <BlendInt/Core/String.hpp>
#include <BlendInt/Gui/Menu.hpp>
#include <BlendInt/Stock/Icons.hpp>
#include <BlendInt/Gui/Button.hpp>
#include <BlendInt/Gui/Splitter.hpp>

#include <BlendInt/Gui/VBlockLayout.hpp>
#include <BlendInt/Gui/ComboBox.hpp>
#include <BlendInt/Gui/HLayout.hpp>
#include <BlendInt/Gui/NumericalSlider.hpp>
#include <BlendInt/Gui/ColorSelector.hpp>

#include "MainLayout.hpp"

using namespace BI;

MainLayout::MainLayout ()
: m_menubar(0),
  m_toolbar(0),
  m_tool_open(0),
  m_tab(0),
  m_scene(0),
  m_image_view(0),
  m_file_input(0),
  m_file_button(0),
  m_btn_open(0),
  m_msg_label(0)
{
	InitOnce();
}

MainLayout::~MainLayout ()
{

}

void MainLayout::InitOnce ()
{
	set_margin(0, 0, 0, 0);
	set_space(1);

	m_menubar = CreateMenuBar();
	m_toolbar = CreateToolBar();

    Splitter* splitter = Manage(new Splitter);
    splitter->SetMargin(0, 0, 0, 0);

    ToolBox* tbox = CreateToolBox();

    m_tab = CreateTab();

    ToolBar* bottom = CreateBottomBar();

    splitter->PushBack(m_tab);
    splitter->PushBack(tbox);

	PushBack(m_menubar);
	PushBack(m_toolbar);
    PushBack(splitter);
    PushBack(bottom);

	events()->connect(m_tool_open->clicked(), this, &MainLayout::OnOpenClick);
}

void MainLayout::OnOpenClick()
{
	if(m_file_input->text().empty()) return;

	std::string filename = ConvertFromString(m_file_input->text());

	int index = m_tab->GetIndex();

	switch(index) {
		case 0: {
			RefPtr<Mesh> monkey(new Mesh);

			if(monkey->Load(filename.c_str())) {
				m_scene->PushBack(monkey);

				m_msg_label->SetText("3D Mesh is loaded");
				m_msg_label->Resize(m_msg_label->GetPreferredSize());
			} else {
				m_msg_label->SetText("");
				m_msg_label->Resize(m_msg_label->GetPreferredSize());
			}
			break;
		}

		case 1: {

			if(m_image_view->Open(filename.c_str())) {
				m_msg_label->SetText("Image is loaded");
				
				Size prefer = m_msg_label->GetPreferredSize();
				DBG_PRINT_MSG("prefer size: %d, %d", prefer.width(), prefer.height());
				
				m_msg_label->Resize(m_msg_label->GetPreferredSize());

				DBG_PRINT_MSG("size: %d, %d", m_msg_label->size().width(), m_msg_label->size().height());

			} else {
				m_msg_label->SetText("");
				m_msg_label->Resize(m_msg_label->GetPreferredSize());
			}

			break;
		}

		default: {
			m_msg_label->SetText("");
			m_msg_label->Resize(m_msg_label->GetPreferredSize());
			break;
		}
	}
}

void MainLayout::OnResize (AbstractWidget* context, int type)
{
	Resize(context->size());
}

BI::ToolBar* MainLayout::CreateToolBar()
{
	ToolBar* toolbar = Manage(new ToolBar);

	HBlockLayout* hblock = Manage (new HBlockLayout);
	
	m_tool_open = Manage(new ToolButton);

	ToolButton* btn1 = Manage(new ToolButton);
	btn1->SetAction(BI::Stock::Icons::instance->seq_preview_16x16(), String("SEQ PREVIEW"));

	ToolButton* btn2 = Manage(new ToolButton);
	btn2->SetAction(BI::Stock::Icons::instance->seq_luma_waveform_16x16(), String("SEQ LUMA WAVEFORM"));

	ToolButton* btn3 = Manage(new ToolButton);
	btn3->SetAction(BI::Stock::Icons::instance->seq_chroma_scope_16x16(), String("SEQ CHROMA SCOPE"));

	ToolButton* btn4 = Manage(new ToolButton);
	btn4->SetAction(BI::Stock::Icons::instance->seq_histogram_16x16(), String("SEQ HISTOGRAM"));

	ToolButton* btn5 = Manage(new ToolButton);
	btn5->SetAction(BI::Stock::Icons::instance->seq_splitview_16x16(), String("SEQ SPLITVIEW"));

	hblock->PushBack(m_tool_open);
	hblock->PushBack(btn1);
	hblock->PushBack(btn2);
	hblock->PushBack(btn3);
	hblock->PushBack(btn4);
	hblock->PushBack(btn5);
	hblock->Resize(hblock->GetPreferredSize());

	HBlockLayout* hblock2 = Manage (new HBlockLayout);

	ToolButton* btn6 = Manage(new ToolButton);
	btn6->SetAction(BI::Stock::Icons::instance->image_rgb_16x16(), String("IMAGE RGB"));

	ToolButton* btn7 = Manage(new ToolButton);
	btn7->SetAction(BI::Stock::Icons::instance->image_rgb_alpha_16x16(), String("IMAGE RGB ALPHA"));

	ToolButton* btn8 = Manage(new ToolButton);
	btn8->SetAction(BI::Stock::Icons::instance->image_alpha_16x16(), String("IMAGE ALPHA"));

	ToolButton* btn9 = Manage(new ToolButton);
	btn9->SetAction(BI::Stock::Icons::instance->image_zdepth_16x16(), String("IMAGE ZDEPTH"));

	ToolButton* btn10 = Manage(new ToolButton);
	btn10->SetAction(BI::Stock::Icons::instance->imagefile_16x16(), String("IMAGEFILE"));

	hblock2->PushBack(btn6);
	hblock2->PushBack(btn7);
	hblock2->PushBack(btn8);
	hblock2->PushBack(btn9);
	hblock2->PushBack(btn10);
	hblock2->Resize(hblock2->GetPreferredSize());

	toolbar->PushBack(hblock);
	toolbar->PushBack(hblock2);

	events()->connect(m_tool_open->clicked(), this, &MainLayout::OnOpenClick);

	return toolbar;
}

BI::MenuBar* MainLayout::CreateMenuBar()
{
	MenuBar* menubar = Manage(new MenuBar);

	RefPtr<Menu> file_menu(new Menu);

    file_menu->SetRoundType(RoundAll);
    file_menu->AddAction(Stock::Icons::instance->check(), "MenuItem1", "Ctrl + 1");
    file_menu->AddAction("MenuItem2", "Ctrl + 1");
    file_menu->AddAction("MenuItem3", "Ctrl + 1");
    file_menu->AddAction("MenuItem4", "Ctrl + 1");
    file_menu->AddAction("MenuItem5");

	menubar->PushBack(String("File"), file_menu);

	RefPtr<Menu> edit_menu(new Menu);

	edit_menu->SetRoundType(RoundAll);
	edit_menu->AddAction(Stock::Icons::instance->check(), "MenuItem1", "Ctrl + 1");
	edit_menu->AddAction("MenuItem2", "Ctrl + 1");
	edit_menu->AddAction("MenuItem3", "Ctrl + 1");
	edit_menu->AddAction("MenuItem4", "Ctrl + 1");
	edit_menu->AddAction("MenuItem5");

	menubar->PushBack(String("Edit"), edit_menu);

	RefPtr<Menu> help_menu(new Menu);

	help_menu->SetRoundType(RoundAll);
	help_menu->AddAction(Stock::Icons::instance->check(), "MenuItem1", "Ctrl + 1");
	help_menu->AddAction("MenuItem2", "Ctrl + 1");
	help_menu->AddAction("MenuItem3", "Ctrl + 1");
	help_menu->AddAction("MenuItem4", "Ctrl + 1");
	help_menu->AddAction("MenuItem5");

	menubar->PushBack(String("Help"), help_menu);


	return menubar;
}

BI::ToolBox* MainLayout::CreateToolBox()
{
	ToolBox* toolbox = Manage(new ToolBox);

	Expander* expander1 = CreateTransformExpander();
	toolbox->PushBack(expander1);

	Expander* expander2 = CreateLightExpander();
	toolbox->PushBack(expander2);

	Expander* expander3 = CreateColorExpander();
	toolbox->PushBack(expander3);

	return toolbox;
}

BI::Expander* MainLayout::CreateTransformExpander()
{
	Expander* expander = Manage(new Expander("Transform"));

	Button* btn1 = Manage(new Button("Translate"));
	Button* btn2 = Manage(new Button("Rotate"));
	Button* btn3 = Manage(new Button("Scale"));

	VBlockLayout* vblock = Manage(new VBlockLayout);
	vblock->PushBack(btn1);
	vblock->PushBack(btn2);
	vblock->PushBack(btn3);

	expander->Setup(vblock);

	return expander;
}

BI::Expander* MainLayout::CreateLightExpander()
{
	Expander* expander = Manage(new Expander("Light"));

	NumericalSlider* ns1 = Manage(new NumericalSlider);
	NumericalSlider* ns2 = Manage(new NumericalSlider);
	NumericalSlider* ns3 = Manage(new NumericalSlider);

	VBlockLayout* vblock = Manage(new VBlockLayout);
	vblock->PushBack(ns1);
	vblock->PushBack(ns2);
	vblock->PushBack(ns3);

	expander->Setup(vblock);

	return expander;
}

BI::Expander* MainLayout::CreateColorExpander()
{
	Expander* expander = Manage(new Expander("Color"));

	ColorSelector* cs = Manage(new ColorSelector);

	expander->Setup(cs);

	return expander;
}

BI::Tab* MainLayout::CreateTab ()
{
	Tab* tab = Manage(new Tab);
	tab->SetMargin(0, 0, 0, 0);

    m_scene = Manage(new Viewport3D);
    m_image_view = Manage(new ImageView);

    tab->Add("3D View", m_scene);
    tab->Add("Image", m_image_view);

	return tab;
}

BI::ToolBar* MainLayout::CreateBottomBar ()
{
	ToolBar* toolbar = Manage(new ToolBar);
	toolbar->SetMargin(2, 2, 2, 2);

	HBox* box = Manage(new HBox);
	box->SetMargin(0, 0, 0, 0);
	box->SetSpace(-1);
	Label* label = Manage(new Label("Select 3D model or image file: "));
	m_file_input = Manage(new TextEntry);
	m_file_button = Manage(new FileButton);
	m_file_input->SetRoundType(RoundTopLeft | RoundBottomLeft);
	m_file_button->SetRoundType(RoundTopRight | RoundBottomRight);
	box->PushBack(label);
	box->PushBack(m_file_input);
	box->PushBack(m_file_button);

	m_btn_open = Manage(new Button("Open"));

	ComboBox* combo = Manage(new ComboBox);

	Label* info_label = Manage(new Label("Info: "));
	m_msg_label = Manage(new Label("Ready..."));
	m_msg_label->SetBackgroundColor(Color(0x8888EFFF));

	toolbar->PushBack(box);
	toolbar->PushBack(m_btn_open);
	toolbar->PushBack(combo);
	toolbar->PushBack(info_label);
	toolbar->PushBack(m_msg_label);

	events()->connect(m_file_button->file_selected(), this, &MainLayout::OnFileSelected);
	events()->connect(m_btn_open->clicked(), this, &MainLayout::OnOpenClick);

	return toolbar;
}

void MainLayout::OnFileSelected ()
{
	m_file_input->SetText(m_file_button->file());

	HBox* box = dynamic_cast<HBox*>(m_file_input->container());
	box->Resize(box->GetPreferredSize());
}
