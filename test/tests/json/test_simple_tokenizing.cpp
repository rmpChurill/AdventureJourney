#include <cstdio>

#include "test.hpp"
#include "json.hpp"

int test::json::test_simple_tokenizing()
{
    std::string textFromFile = util::read_file("test/tests/json/simple_test.json");
    std::string text = "{\"foo\": 3}";

    std::printf("Read text:\n%s", textFromFile.c_str());

    ::json::tokenizer t;

    auto tokens = t.tokenize(text);

    for (const auto &token : tokens)
    {
        util::printr(token);
    }

    ::json::node *n;
    ::json::parser p;
    ::json::formatted_printer fp;
    p.parse(text, &n);

    n->print_formatted(fp);
    std::printf("%s\n", fp.to_string().c_str());

    return 0;
}