#include <stdexcept>
#include "interpreter/interpreter.hh"

static void test(std::string const& code, rotavm::Value const& expected)
{
    printf("-----------------------------\n");

    printf("%s", code.c_str());

    try {
        RotaInterpreter itp;
        itp.run(code);

        if (itp.peek_vm() != expected) {
            char buf[255]; snprintf(buf, sizeof(buf), "Expected: %s, found %s", expected.debug().c_str(), itp.peek_vm().debug().c_str());
            throw std::runtime_error(buf);
        }

        printf("  --> \e[0;32mok (%s)\e[0m\n", itp.peek_vm().debug().c_str());

    } catch (std::exception& e) {
        printf("  --> \e[0;31m%s\e[0m\n", e.what());
        exit(1);
    }
}

int main()
{
    // arithmetic

    test("500\n", rotavm::Value(500));
    test("500 + 30\n", rotavm::Value(530));
    test("1.2f + 4\n", rotavm::Value(5.2f));
    test("500 - 30\n", rotavm::Value(470));
    test("65 / 5\n", rotavm::Value(13.f));
    test("1 + 4 * 3\n", rotavm::Value(13));
    test("(1 + 4) * 3\n", rotavm::Value(15));
    test("1.2f\n", rotavm::Value(1.2f));
    test("1.2f + 4\n", rotavm::Value(5.2f));
    test("-500\n", rotavm::Value(-500));
    test("500 + -30\n", rotavm::Value(470));
    test("500 - -30\n", rotavm::Value(530));
    test("5 % 3\n", rotavm::Value(2));
    test("5 ^ 3\n", rotavm::Value(125));
    test("2 / 4\n", rotavm::Value(0.5f));
    test("5 // 4\n", rotavm::Value(1));
}