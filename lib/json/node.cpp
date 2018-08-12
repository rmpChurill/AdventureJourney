#include "json.hpp"

json::node::node(const std::string &name) : name(name)
{
}

json::node::node() : name("")
{
}

json::node::~node()
{
}

const std::string &json::node::get_name() const
{
    return this->name;
}

void json::node::set_name(const std::string &name)
{
    this->name.assign(name);
}

std::ostream &json::node::operator<<(std::ostream &stream) const
{
    stream << this->name;

    return stream;
}

json::formatted_printer &json::node::print_formatted(json::formatted_printer &p) const
{
    p.print(this->name);

    return p;
}