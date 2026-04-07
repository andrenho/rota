#include <stdexcept>
#include "interpreter/interpreter.hh"

template <typename T>
static void test(std::string const& code, T const& v_expected)
{
    rotavm::Value expected(v_expected);

    printf("-----------------------------\n");

    printf("%s", code.c_str());

    RotaInterpreter itp;
    try {
        itp.run(code);

        if (itp.peek_vm() != expected) {
            char buf[255]; snprintf(buf, sizeof(buf), "Expected: %s, found %s", expected.debug().c_str(), itp.peek_vm().debug().c_str());
            throw std::runtime_error(buf);
        }

        printf("  --> \e[0;32mok (%s)\e[0m\n", itp.peek_vm().debug().c_str());

    } catch (std::exception& e) {
        printf("  --> \e[0;31m%s\e[0m\n", e.what());
        printf("      \e[0;31m"); itp.print_stack(); printf("\e[0m");
        exit(1);
    }
}

int main()
{
    // arithmetic

    test("500\n", 500);
    test("500 + 30\n", 530);
    test("1.2f + 4\n", 5.2f);
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
    test("5 // 4\n", 1);

    // logic
    test("false\n", 0);
    test("true\n", -1);
    test("1 != 0\n", -1);
    test("1 == 0\n", 0);
    test("1 != 1\n", 0);
    test("1 == 1\n", -1);
    test("2 >= 1\n", -1);
    test("1 >= 2\n", 0);
    test("1 >= 1\n", -1);
    test("1 > 1\n", 0);
    test("-1 && -1\n", -1);
    test("0 && -1\n", 0);
    test("0 && 0\n", 0);
    test("-1 || -1\n", -1);
    test("500 || 0\n", -1);
    test("0 || -1\n", -1);
    test("0 || 0\n", 0);
    test("2 > 1 && 1 > 2\n", 0);
    test("2 > 1 || 1 > 2\n", -1);
    test("!1\n", 0);
    test("!0\n", -1);
    test("!(2 > 1 || 1 > 2)\n", 0);
}