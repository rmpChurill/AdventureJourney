#include "controls/containers/ContainerBase.hpp"
#include "geometry/Rectangle.hpp"
#include "data/Io.hpp"

terminal::ContainerBase::ContainerBase() : ControlBase(),
                                           controls(0),
                                           focusedControlIndex(-1)
{
    this->SwitchFocus(-1);
}

terminal::ContainerBase::~ContainerBase()
{
    while (!this->controls.empty())
    {
        delete this->controls.back();
        this->controls.pop_back();
    }
}

void terminal::ContainerBase::HandleChildZIndexChanged(ControlBase *)
{
}

void terminal::ContainerBase::HandleChildTabIndexChanged(ControlBase *)
{
}

void terminal::ContainerBase::HandleFocusAquired()
{
}

void terminal::ContainerBase::HandleFocusLost()
{
}

void terminal::ContainerBase::HandleKey(KeyInput &input)
{
    this->ControlBase::HandleKey(input);

    if (input.handled)
        return;

    if (this->focusedControlIndex >= 0)
        this->controls[focusedControlIndex]->HandleKey(input);

    if (input.handled)
        return;

    if (!this->controls.empty() && input.key == '\t')
    {
        auto nextControl = (this->focusedControlIndex + 1) % this->controls.size();

        if (nextControl != this->focusedControlIndex)
            this->SwitchFocus(nextControl);

        input.handled = true;
    }
}

void terminal::ContainerBase::HandleMouse(MouseInput &input)
{
    this->ControlBase::HandleMouse(input);

    if (input.handled)
        return;

    auto click = util::Point(input.x, input.y);

    input.x -= this->GetContentBounds().GetX();
    input.y -= this->GetContentBounds().GetY();

    if (this->focusedControlIndex >= 0 &&
        this->controls[this->focusedControlIndex]->Contains(input.x, input.y))
    {
        this->controls[focusedControlIndex]->HandleMouse(input);
    }
    else
    {
        bool controlHit = false;

        for (size_t i = 0; i < this->controls.size(); i++)
        {
            const auto &control = this->controls[i];

            if (control->Contains(input.x, input.y))
            {
                this->SwitchFocus(i);
                control->HandleMouse(input);
                controlHit = true;
                break;
            }
        }

        if (!controlHit)
        {
            this->SwitchFocus(-1);
            input.handled = true;
        }
    }
}

void terminal::ContainerBase::HandleZIndexChanged(int)
{
}

void terminal::ContainerBase::HandleTabIndexChanged(int)
{
}

void terminal::ContainerBase::Render(Canvas &canvas)
{
    this->terminal::ControlBase::Render(canvas);

    auto subCanvas = canvas.GetSubCanvas(this->GetContentBounds());

    for (auto i = this->controls.begin(), end = this->controls.end(); i != end; i++)
    {
        (*i)->Render(subCanvas);
    }
}

void terminal::ContainerBase::Add(terminal::ControlBase *control)
{
    this->controls.push_back(control);
    this->Invalidate();

    if (this->controls.size() == 1)
    {
        this->focusedControlIndex = 0;
        control->HandleFocusAquired();
    }

    control->HandleAddToControl(this);
}

bool terminal::ContainerBase::Remove(terminal::ControlBase *control)
{
    auto pos = std::find(this->controls.begin(), this->controls.end(), control);

    if (pos != this->controls.end())
    {
        this->controls.erase(pos);
        this->Invalidate();

        return true;
    }

    return false;
}

void terminal::ContainerBase::SwitchFocus(int next)
{
    if (this->focusedControlIndex > 0)
    {
        this->controls[this->focusedControlIndex]->HandleFocusLost();
    }

    util::dbg.WriteLine("ContainerBase [%]: switching focus from % (%) to % (%)",
                        this->GetName(),
                        this->focusedControlIndex,
                        this->focusedControlIndex > 0 ? this->controls[this->focusedControlIndex]->GetName() : "-",
                        next,
                        next > 0 ? this->controls[next]->GetName() : "-");

    this->focusedControlIndex = next;
    terminal::View::GetInstance()->SetCursorMode(terminal::CursorMode::Invisible);

    if (this->focusedControlIndex > 0)
    {
        this->controls[this->focusedControlIndex]->HandleFocusAquired();
    }
}

const terminal::ControlBase *terminal::ContainerBase::GetFocusedControl() const
{
    if (this->focusedControlIndex == -1)
        return nullptr;

    return this->controls[this->focusedControlIndex];
}

terminal::ControlBase *terminal::ContainerBase::GetFocusedControl()
{
    if (this->focusedControlIndex == -1)
        return nullptr;

    return this->controls[this->focusedControlIndex];
}

size_t terminal::ContainerBase::GetFocusedControlIndex() const
{
    return this->focusedControlIndex;
}

bool terminal::ContainerBase::RequestFocus(ControlBase *control)
{
    size_t i;

    for (i = 0; i < this->controls.size(); i++)
        if (this->controls[i] == control)
            break;

    if (i < this->controls.size())
    {
        this->SwitchFocus(i);

        return true;
    }

    return false;
}

std::vector<terminal::ControlBase *> &terminal::ContainerBase::GetControls()
{
    return this->controls;
}

const std::vector<terminal::ControlBase *> &terminal::ContainerBase::GetControls() const
{
    return this->controls;
}

void terminal::ContainerBase::RestoreLayout()
{
    this->ControlBase::RestoreLayout();

    for (auto control : this->controls)
    {
        control->ApplyAutoSize(this->GetContentBounds());
        control->RestoreLayout();
    }
}