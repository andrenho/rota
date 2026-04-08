#include <stdexcept>
#include "vm/compiler/compiler.hh"
#include "vm/rotavm.hh"

static bool debug;

template <typename T>
static void test(std::string const& code, T const& v_expected)
{
    rotavm::Value expected(v_expected);

    printf("-----------------------------\n");
    printf("%s", code.c_str());

    rotavm::RotaVM vm;
    try {
        vm.set_executable_memory(rotavm::compile(code), true);

        if (debug) {
            printf("%s\n", vm.debug_executable_memory().c_str());
            printf("%s\n", vm.debug_executable().c_str());
        }

        vm.run_until_halt();

        if (vm.last_value() != expected) {
            char buf[255]; snprintf(buf, sizeof(buf), "Expected: %s, found %s", expected.debug().c_str(),
                    vm.last_value().debug().c_str());
            throw std::runtime_error(buf);
        }

        printf("  --> \e[0;32mok (%s)\e[0m\n", vm.last_value().debug().c_str());

    } catch (std::exception& e) {
        printf("  --> \e[0;31m%s\e[0m\n", e.what());
        printf("      \e[0;31m%s\e[0m\n", vm.debug_stack().c_str());
        exit(1);
    }
}

int main(int argc, char* argv[])
{
    debug = (argc == 2 && std::string(argv[1]) == "-d");

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

    // nil

    test("nil\n", rotavm::Value());
    test("nil && true\n", 0);
    test("nil || true\n", -1);
}