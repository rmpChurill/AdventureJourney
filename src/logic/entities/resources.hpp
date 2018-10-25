#pragma once

#include <string>

#include "data/collection/array.hpp"
#include "entity_bases.hpp"

namespace logic
{
class resource : public entity_id_base<resource>
{
  private:
    std::string name;
};

class resource_set
{
  private:
    util::array<int> items;

  public:
    long get_amount_of(id_t) const;
    long get_amount_of(const resource &) const;

    void set_amount_of(id_t, long);
    void set_amount_of(const resource &, long);

    bool can_satisfy_requirements(const resource_set &);

    bool can_satisfy_requirement(id_t, long);
    bool can_satisfy_requirement(const resource &, long);

    resource_set operator+(const resource_set &other) const;
    resource_set operator-(const resource_set &other) const;
    resource_set operator/(const resource_set &other) const;
    resource_set operator*(const resource_set &other) const;
    resource_set &operator+=(const resource_set &other);
    resource_set &operator-=(const resource_set &other);
    resource_set &operator*=(const resource_set &other);
    resource_set &operator/=(const resource_set &other);
};
} // namespace logic