#include <vector>
#include <cstdio>
#include <iostream>

#include "test.hpp"
#include "config.hpp"

int main(int argc, char **argv)
{
    test::test_collection tests;

    tests.push_back(test::test_run(new test::simple_test("simple tokenizing", &test::json_test::test_simple_tokenizing)));
    tests.push_back(test::test_run(new test::simple_test("basic color definitions", &test::color_test::test_basic_color_values)));
    tests.push_back(test::test_run(new test::simple_test("function event test", &test::event_test::test_function_events)));
    tests.push_back(test::test_run(new test::simple_test("listener event test", &test::event_test::test_listener_events)));
    tests.push_back(test::test_run(new test::simple_test("handler event test", &test::event_test::test_handler_events)));
    tests.push_back(test::test_run(new test::simple_test("member handler event test", &test::event_test::test_member_handler_events)));
    tests.push_back(test::test_run(new test::simple_test("rectangle test", &test::geometry_test::test_rectangle)));

    for (auto &testRun : tests)
    {
        testRun.execute();
        util::printr(testRun);
    }

    return 0;
}