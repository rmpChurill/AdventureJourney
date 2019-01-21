#include <algorithm>

#include "Terminal.hpp"

using util::Point;
using util::Rectangle;

terminal::TerminalWindow::TerminalWindow() : loop(true),
                                             escapeKey(0),
                                             hasEscapeKey(false),
                                             thread(),
                                             hasThread(false)
{
}

terminal::TerminalWindow::~TerminalWindow()
{
}

void terminal::TerminalWindow::Start(int escapeKey)
{
    this->escapeKey = escapeKey;
    this->hasEscapeKey = true;

    this->Start();
}

void terminal::TerminalWindow::StartAsync()
{
}

void terminal::TerminalWindow::Start()
{
    auto view = TerminalView::GetInstance();

    this->Render();

    while (this->loop)
    {
        auto key = view->ReadKey();

        if (terminal::IsMouse(key))
        {
            MEVENT mouseEvent;

            if (getmouse(&mouseEvent) == OK)
            {
                MouseInput input;
                input.cx = mouseEvent.x;
                input.cy = mouseEvent.y;
                input.handled = false;
                input.action = static_cast<MouseAction>(mouseEvent.bstate);

                this->HandleMouse(input);
            }
        }
        else
        {
            KeyInput input;
            input.handled = false;
            input.key = key;

            this->HandleKey(input);
        }

        this->Render();
    }
}

void terminal::TerminalWindow::HandleKey(KeyInput &input)
{
    if (this->hasEscapeKey && input.key == this->escapeKey)
    {
        this->loop = false;
    }
}

void terminal::TerminalWindow::Quit()
{
    this->loop = false;
}

void terminal::TerminalWindow::Render()
{
    auto view = TerminalView::GetInstance();
    auto canvas = terminal::Canvas(view);

    canvas.Clear();

    this->ContainerBase::Render(canvas);

    view->Flush();
}