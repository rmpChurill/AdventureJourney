#include "terminal.hpp"

int terminal::control_base::z_index_sorter::operator()(const control_base &a, const control_base &b)
{
    return a.z_index - b.z_index;
}

int terminal::control_base::tab_index_sorter::operator()(const control_base &a, const control_base &b)
{
    return a.tab_index - b.tab_index;
}

terminal::control_base::control_base() : bounds(0, 0, 0, 0),
                                         parent(nullptr),
                                         z_index(0),
                                         has_focus(false),
                                         visible(true)
{
}

terminal::control_base::~control_base()
{
}

terminal::control_base *terminal::control_base::get_parent()
{
    return this->parent;
}

const terminal::control_base *terminal::control_base::get_parent() const
{
    return this->parent;
}

bool terminal::control_base::has_parent() const
{
    return this->parent != nullptr;
}

const util::rectangle &terminal::control_base::get_bounds() const
{
    return this->bounds;
}

void terminal::control_base::set_bounds(const util::rectangle &bounds)
{
    this->bounds = bounds;
}

void terminal::control_base::set_z_index(int zIndex)
{
    this->z_index = zIndex;
}

int terminal::control_base::get_z_index() const
{
    return this->z_index;
}

std::string &terminal::control_base::get_text()
{
    return this->text;
}

const std::string &terminal::control_base::get_text() const
{
    return this->text;
}

void terminal::control_base::set_text(const std::string &text)
{
    this->text = text;
}

bool terminal::control_base::is_visible() const
{
    return this->visible;
}

void terminal::control_base::set_visibility(bool visible)
{
    this->visible = visible;
}

int terminal::control_base::get_tab_index() const
{
    return this->tab_index;
}

void terminal::control_base::set_tab_index(int tabIndex)
{
    this->tab_index = tabIndex;
}

const util::dimension &terminal::control_base::get_min_size() const
{
    return this->minimum_size;
}

const util::dimension &terminal::control_base::get_max_size() const
{
    return this->maximum_size;
}

void terminal::control_base::set_min_size(const util::dimension &size)
{
    this->minimum_size = size;
}

void terminal::control_base::set_max_size(const util::dimension &size)
{
    this->maximum_size = size;
}

void terminal::control_base::handle_focus_aquired()
{
}

void terminal::control_base::handle_focus_lost()
{
}

void terminal::control_base::handle_key(key_input &)
{
}

void terminal::control_base::handle_mouse(mouse_input &)
{
}

void terminal::control_base::handle_add_to_control(container_base *)
{
}

void terminal::control_base::render(terminal::canvas &)
{
}

void terminal::control_base::handle_tab_index_changed(int oldTabIndex)
{
}

void terminal::control_base::handle_z_index_changed(int oldZIndex)
{
}