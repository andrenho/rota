#include <stdexcept>
#include "interpreter/interpreter.hh"

void test(std::string const& code, rotavm::Value expected)
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
    }
}

int main()
{
    test("500\n", 500);
    test("500 + 30\n", 530);
    test("500 - 30\n", 470);
}