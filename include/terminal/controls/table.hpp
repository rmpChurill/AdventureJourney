#pragma once

#include "terminal/controls/control_base.hpp"

namespace terminal
{
class table : public control_base
{
  private:
    std::vector<std::string> content;
    int columns;
    int rows;

  public:
    table();
    table(int rows, int cols);
    
    virtual void handle_focus_aquired();
    virtual void handle_focus_lost();
    virtual void handle_key(key_input &);
    virtual void handle_mouse(mouse_input &);
    virtual void handle_add_to_control(control_base *);

    virtual void render(const util::rectangle &, canvas &);
};
} // namespace terminal