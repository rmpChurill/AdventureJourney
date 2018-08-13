#include "test.hpp"

assert::assert_exception::assert_exception(const std::string &msg) : std::runtime_error("Assert Failed: " + msg)
{
}

void assert::is_null(const void *ptr)
{
    if (ptr != nullptr)
    {
        throw assert::assert_exception("Pointer is not null!");
    }
}

void assert::is_not_null(const void *ptr)
{
    if (ptr == nullptr)
    {
        throw assert::assert_exception("Pointer is null!");
    }
}

void assert::fail()
{
    throw assert_exception("Assert failed!");
}

void assert::is_true(bool x)
{
    if (!x)
    {
        throw assert_exception("Value is not true!");
    }
}

void assert::is_false(bool x)
{
    if (x)
    {
        throw assert_exception("Value is not false!");
    }
}