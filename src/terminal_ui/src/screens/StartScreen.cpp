#include "Screens.hpp"

tui::StartScreen::StartScreen(ScreenCollection *screens)
{
    this->screens = screens;

    this->titleText = new terminal::TextView("Welcome to\nGalaxy At War\n\n[press any key to continue]");
    this->titleText->SetAutoSizeMode(terminal::AutoSizeMode::Fill);
    this->titleText->SetBorderEnabled(true);
    this->titleText->SetCenterVertical(true);
    this->titleText->SetCenterHorizontal(true);
    this->titleText->SetPadding(0.3f, 0.f, 0.3f, 0.f);
    this->titleText->SetName("Start::TitleText");
    this->titleText->SetBorderEnabled(true);
    this->titleText->SetBorder(this->screens->border1);

    this->Add(titleText);
    this->OnKey().Register(this, &StartScreen::HandleKey);
    this->SetName("StartScreen");
}

tui::StartScreen::~StartScreen()
{
}

void tui::StartScreen::HandleKey(terminal::KeyEventArgs &args)
{
    args.handled = true;
    this->screens->menu->Show();
}