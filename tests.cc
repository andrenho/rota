#include <stdexcept>
#include "vm/compiler/compiler.hh"
#include "vm/rotavm.hh"

static bool debug;

template <typename T>
static void test(std::string const& code, T const& v_expected)
{
    rotavm::Value expected(v_expected);

    printf("-----------------------------\n");
    printf("%s\n", code.c_str());

    rotavm::RotaVM vm;
    try {
        auto executable = rotavm::compile(code);
        vm.set_executable(executable);

        if (debug)
            printf("%s\n", executable.debug().c_str());

        vm.run_until_halt();

        rotavm::Value v = vm.last_value(), e = expected;
        if (v.type() != e.type() || (v.i() != e.i() && v.f() != e.f())) {
            char buf[255]; snprintf(buf, sizeof(buf), "Expected: %s, found %s", expected.debug().c_str(),
                    vm.last_value().debug().c_str());
            throw std::runtime_error(buf);
        }

        if (vm.stack_sz() != 0)
            throw std::runtime_error("Queue not empty (size " + std::to_string(vm.stack_sz()) + ")");

        printf("  --> \e[0;32mok (%s)\e[0m\n", vm.last_value().debug().c_str());

    } catch (std::exception& e) {
        printf("  --> \e[0;31m%s\e[0m\n", e.what());
        printf("      \e[0;31m%s\e[0m\n", vm.debug_stack().c_str());
        exit(1);
    }
}

static void test_fail(std::string const& code)
{
    printf("-----------------------------\n");
    printf("%s\n", code.c_str());

    rotavm::RotaVM vm;
    try {
        auto executable = rotavm::compile(code);
        vm.set_executable(executable);

        if (debug)
            printf("%s\n", executable.debug().c_str());

        vm.run_until_halt();

    } catch (std::exception& e) {
        printf("  --> \e[0;32mok (%s)\e[0m\n", e.what());
        return;
    }

    printf("  --> \e[0;31mTest was supposed to fail\e[0m\n");
}

int main(int argc, char* argv[])
{
    debug = (argc == 2 && std::string(argv[1]) == "-d");

    // arithmetic

    test("500;", 500);
    test("500 + 30;", 530);
    test("1.2f + 4;", 5.2f);
    test("500 - 30;", 470);
    test("65 / 5;", 13.f);
    test("1 + 4 * 3;", 13);
    test("(1 + 4) * 3;", 15);
    test("1.2f;", 1.2f);
    test("1.2f + 4;", 5.2f);
    test("-500;", -500);
    test("500 + -30;", 470);
    test("500 - -30;", 530);
    test("5 % 3;", 2);
    test("5 ^ 3;", 125);
    test("2 / 4;", 0.5f);
    test("5 // 4;", 1);

    test_fail("3 4");

    // logic

    test("false;", 0);
    test("true;", -1);
    test("1 != 0;", -1);
    test("1 == 0;", 0);
    test("1 != 1;", 0);
    test("1 == 1;", -1);
    test("2 >= 1;", -1);
    test("1 >= 2;", 0);
    test("1 >= 1;", -1);
    test("1 > 1;", 0);
    test("-1 && -1;", -1);
    test("0 && -1;", 0);
    test("0 && 0;", 0);
    test("-1 || -1;", -1);
    test("500 || 0;", -1);
    test("0 ||\n -1;", -1);
    test("0 || 0;", 0);
    test("2 > 1 && 1 > 2;", 0);
    test("2 > 1 || 1 > 2;", -1);
    test("!1;", 0);
    test("!0;", -1);
    test("!(2 > 1 || 1 > 2);", 0);

    // nil

    test("nil;", rotavm::Value());
    test("nil && true;", 0);
    test("nil || true;", -1);
    test("500;;;", 500);

    // multiple expressions

    test("42; 3 + 4;", 7);

    // functions

    test("func() {\n return 42;\n }();\n", 42);

    // local variables

    test("a = 42; a;", 42);
    test("a = 42; b = 12; a;", 42);
    test("a = 42; b = 12; b;", 12);

    // local variables on scopes

    test("a = 42; { b = 10; }; a;", 42);
    test_fail("a = 42; { b = 10; }; b;");

    // local variables on functions

    // function parameters
}