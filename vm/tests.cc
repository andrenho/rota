#include <stdexcept>
#include "interpreter/interpreter.hh"

static void test(std::string const& code, rotavm::Value expected)
{
    printf("-----------------------------\n");

    printf("%s", code.c_str());

    try {
        RotaInterpreter itp;
        itp.run(code);

        if (itp.peek_vm() != expected) {
            char buf[255]; snprintf(buf, sizeof(buf), "Expected: %s, found %s", std::to_string(expected).c_str(), std::to_string(itp.peek_vm()).c_str());
            throw std::runtime_error(buf);
        }

        printf("  --> \e[0;32mok (%s)\e[0m\n", std::to_string(itp.peek_vm()).c_str());

    } catch (std::exception& e) {
        printf("  --> \e[0;31m%s\e[0m\n", e.what());
        exit(1);
    }
}

int main()
{
    // arithmetic

    test("500\n", 500);
    test("500 + 30\n", 530);
    test("500 - 30\n", 470);
    test("65 / 5\n", 13.f);
    test("1 + 4 * 3\n", 13);
    test("(1 + 4) * 3\n", 15);
    test("1.2f\n", 1.2f);
    test("1.2f + 4\n", 5.2f);
    test("-500\n", -500);
    test("500 + -30\n", 470);
    test("500 - -30\n", 530);
    test("5 % 3\n", 2);
    test("5 ^ 3\n", 125);
    test("2 / 4\n", 0.5f);
    test("2 // 4\n", 0);
}