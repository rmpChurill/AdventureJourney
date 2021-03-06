#include <cstring>

#include "controls/containers/FrameContainer.hpp"
#include "Exception.hpp" 
#include "data/Io.hpp"

namespace
{
constexpr int BOTTOM_FRAME = static_cast<int>(terminal::FrameContainer::Orientation::Bottom);
constexpr int TOP_FRAME = static_cast<int>(terminal::FrameContainer::Orientation::Top);
constexpr int LEFT_FRAME = static_cast<int>(terminal::FrameContainer::Orientation::Left);
constexpr int RIGHT_FRAME = static_cast<int>(terminal::FrameContainer::Orientation::Right);
constexpr int CENTER_FRAME = static_cast<int>(terminal::FrameContainer::Orientation::Center);

struct OrderSorter
{
    int weights[5];

    OrderSorter(int *weights)
    {
        std::memcpy(weights, this->weights, 5 * sizeof(int));
    }

    int operator()(int i, int j)
    {
        if (weights[i] >= 0 && weights[j] >= 0)
        {
            return (weights[i] > weights[j]) - (weights[j] < weights[i]);
        }
        else if (weights[i] >= 0)
        {
            return 1;
        }
        else if (weights[j] >= 0)
        {
            return -1;
        }
        else
        {
            return (i > j) - (i < j);
        }
    }
};
} // namespace

terminal::FrameContainer::FrameContainer() : ContainerBase(),
                                             controls(),
                                             weights(),
                                             weightsFixed()
{
}

terminal::FrameContainer::~FrameContainer()
{
}

void terminal::FrameContainer::SetWeight(Orientation where, int value)
{
    auto id = static_cast<int>(where);

    if (value >= 0)
    {
        this->weightsFixed[id] = true;
        this->weights[id] = value;
    }
    else
    {
        this->weightsFixed[id] = false;
        this->weights[id] = 0;
    }
}

void terminal::FrameContainer::SetMaxSize(Orientation where, int value)
{
    auto id = static_cast<int>(where);

    this->maxSizes[id] = value;
    this->minSizes[id] = util::Min(value - 1, this->minSizes[id]);
}

void terminal::FrameContainer::SetMinSize(Orientation where, int value)
{
    auto id = static_cast<int>(where);

    this->minSizes[id] = value;
    this->maxSizes[id] = util::Max(value + 1, this->maxSizes[id]);
}

void terminal::FrameContainer::Add(ControlBase *item)
{
    for (int i = 0; i < 5; i++)
    {
        if (this->controls[i] == nullptr)
        {
            this->Add(static_cast<terminal::FrameContainer::Orientation>(i), item);

            break;
        }
    }
}

void terminal::FrameContainer::Add(Orientation where, ControlBase *item)
{
    auto id = -1;

    if (
        where == terminal::FrameContainer::Orientation::Left ||
        where == terminal::FrameContainer::Orientation::Right ||
        where == terminal::FrameContainer::Orientation::Top ||
        where == terminal::FrameContainer::Orientation::Bottom ||
        where == terminal::FrameContainer::Orientation::Center)
        id = static_cast<int>(where);
    else
        throw util::InvalidCaseException::MakeException(where);

    if (this->controls[id] != nullptr)
    {
        this->terminal::ContainerBase::Remove(this->controls[id]);
    }

    if (!this->weightsFixed[id])
    {
        int max = 0;

        for (int i = 0; i < 5; i++)
        {
            if (i != id && !this->weightsFixed[id] && this->weights[i] > max)
                max = this->weights[i];
        }

        this->weights[id] = max + 1;
    }

    this->controls[id] = item;
    this->terminal::ContainerBase::Add(item);

    this->Invalidate();
}

bool terminal::FrameContainer::Remove(ControlBase *control)
{
    for (int i = 0; i < 5; i++)
    {
        if (this->controls[i] == control)
        {
            this->controls[i] = nullptr;

            if (!this->weightsFixed[i])
                this->weights[i] = 0;

            return this->ContainerBase::Remove(control);
        }
    }

    return false;
}

void terminal::FrameContainer::RestoreLayout()
{
    this->ControlBase::RestoreLayout();

    int activeControls = 0;
    int order[5];

    for (size_t i = 0; i < 5; i++)
    {
        if (this->controls[i] != nullptr)
        {
            order[activeControls] = i;
            activeControls++;
        }
    }

    if (activeControls == 0)
        return;

    if (activeControls > 1)
        std::sort(order, order + activeControls, OrderSorter(this->weights));

    int id;
    Orientation where;
    ControlBase *control;
    auto centerArea = util::Rectangle();
    auto centerSet = false;
    int sizes[4] = {-1, -1, -1, -1};

    int yOff = 0;
    int xOff = 0;
    int maxW = 0;
    int maxH = 0;

    for (int i = 0; i < activeControls; i++)
    {
        id = order[i];
        control = this->controls[id];

        yOff = 0;
        xOff = 0;
        maxW = 0;
        maxH = 0;
        where = static_cast<Orientation>(id);

        if (where == Orientation::Left || where == Orientation::Right)
        {
            maxH = this->GetSize().GetHeight();

            if (sizes[BOTTOM_FRAME] != -1)
                maxH -= sizes[BOTTOM_FRAME];

            if (sizes[TOP_FRAME] != -1)
            {
                maxH -= sizes[TOP_FRAME];
                yOff = sizes[TOP_FRAME];
            }
        }

        if (where == Orientation::Top || where == Orientation::Bottom)
        {
            maxW = this->GetSize().GetWidth();

            if (sizes[RIGHT_FRAME] != -1)
                maxW -= sizes[RIGHT_FRAME];

            if (sizes[LEFT_FRAME] != -1)
            {
                maxW -= sizes[LEFT_FRAME];
                xOff = sizes[LEFT_FRAME];
            }
        }

        switch (where)
        {
        case Orientation::Left:
            if (centerSet)
                maxW = centerArea.GetX();
            else if (sizes[RIGHT_FRAME] >= 0)
                maxW = this->GetBounds().GetWidth() - sizes[RIGHT_FRAME];
            else
                maxW = this->GetBounds().GetWidth();

            if (this->minSizes[LEFT_FRAME] >= 0 && maxW < this->minSizes[LEFT_FRAME])
                maxW = this->minSizes[LEFT_FRAME];

            if (this->maxSizes[LEFT_FRAME] >= 0 && maxW > this->maxSizes[LEFT_FRAME])
                maxW = this->maxSizes[LEFT_FRAME];

            break;
        case Orientation::Right:
            if (centerSet)
                maxW = this->GetBounds().GetWidth() - centerArea.GetMaxX();
            else if (sizes[LEFT_FRAME] >= 0)
                maxW = this->GetBounds().GetWidth() - sizes[LEFT_FRAME];
            else
                maxW = this->GetBounds().GetWidth();

            if (this->minSizes[RIGHT_FRAME] >= 0 && maxW < this->minSizes[RIGHT_FRAME])
                maxW = this->minSizes[RIGHT_FRAME];

            if (this->maxSizes[RIGHT_FRAME] >= 0 && maxW > this->maxSizes[RIGHT_FRAME])
                maxW = this->maxSizes[RIGHT_FRAME];

            xOff = this->GetBounds().GetWidth() - maxW;

            break;
        case Orientation::Top:
            if (centerSet)
                maxH = centerArea.GetY();
            else if (sizes[BOTTOM_FRAME] >= 0)
                maxH = this->GetBounds().GetHeight() - sizes[BOTTOM_FRAME];
            else
                maxH = this->GetBounds().GetHeight();

            if (this->minSizes[TOP_FRAME] >= 0 && maxH < this->minSizes[TOP_FRAME])
                maxH = this->minSizes[TOP_FRAME];

            if (this->maxSizes[TOP_FRAME] >= 0 && maxH > this->maxSizes[TOP_FRAME])
                maxH = this->maxSizes[TOP_FRAME];

            break;
        case Orientation::Bottom:
            if (centerSet)
                maxH = this->GetBounds().GetHeight() - centerArea.GetMaxY();
            else if (sizes[TOP_FRAME] >= 0)
                maxH = this->GetBounds().GetHeight() - sizes[TOP_FRAME];
            else
                maxH = this->GetBounds().GetHeight();

            if (this->minSizes[BOTTOM_FRAME] >= 0 && maxH < this->minSizes[BOTTOM_FRAME])
                maxH = this->minSizes[BOTTOM_FRAME];

            if (this->maxSizes[BOTTOM_FRAME] >= 0 && maxH > this->maxSizes[BOTTOM_FRAME])
                maxH = this->maxSizes[BOTTOM_FRAME];

            yOff = this->GetBounds().GetHeight() - maxH;

            break;
        case Orientation::Center:
            maxW = this->GetBounds().GetWidth();
            maxH = this->GetBounds().GetHeight();

            if (sizes[LEFT_FRAME] >= 0)
            {
                xOff = sizes[LEFT_FRAME];
                maxW -= sizes[LEFT_FRAME];
            }
            else if (this->minSizes[LEFT_FRAME] >= 0)
            {
                xOff = this->minSizes[LEFT_FRAME];
                maxW -= this->minSizes[LEFT_FRAME];
            }

            if (sizes[RIGHT_FRAME] >= 0)
                maxW -= sizes[RIGHT_FRAME];
            else if (this->minSizes[RIGHT_FRAME] >= 0)
                maxW -= this->minSizes[RIGHT_FRAME];

            if (sizes[TOP_FRAME] >= 0)
            {
                yOff = sizes[TOP_FRAME];
                maxH -= sizes[TOP_FRAME];
            }
            else if (this->minSizes[TOP_FRAME] >= 0)
            {
                yOff = this->minSizes[TOP_FRAME];
                maxH -= this->minSizes[TOP_FRAME];
            }

            if (sizes[BOTTOM_FRAME] >= 0)
                maxH -= sizes[BOTTOM_FRAME];
            else if (this->minSizes[BOTTOM_FRAME] >= 0)
                maxH -= this->minSizes[BOTTOM_FRAME];

            break;
        default:
            throw util::InvalidCaseException();
        }

        util::dbg.WriteLine("FrameContainer [%]: Fitting [%] to % at x=%, y=%, w=%, h=%", this->GetName(), control->GetName(), where, xOff, yOff, maxW, maxH);

        control->SetLocation(xOff, yOff);
        control->ApplyAutoSize(util::Rectangle(xOff, yOff, maxW, maxH));
        control->RestoreLayout();

        if (where == Orientation::Center)
        {
            centerArea = control->GetBounds();
            centerSet = true;
        }
        else
        {
            if (where == Orientation::Left || where == Orientation::Right)
            {
                sizes[id] = control->GetBounds().GetWidth();
            }
            else if (where == Orientation::Top || where == Orientation::Bottom)
            {
                sizes[id] = control->GetBounds().GetHeight();
            }

            if (this->minSizes[id] >= 0 && sizes[id] < this->minSizes[id])
            {
                sizes[id] = this->minSizes[id];
            }

            if (this->maxSizes[id] >= 0 && sizes[id] > this->maxSizes[id])
            {
                sizes[id] = this->maxSizes[id];
            }
        }
    }
}

void terminal::FrameContainer::Render(Canvas &c)
{
    this->ContainerBase::Render(c);
}