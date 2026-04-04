#ifndef FORTUNA_6_VM_HH
#define FORTUNA_6_VM_HH

#include <cstdint>
#include <cstddef>

#define STACK_SZ 512
#define RAM_SZ (448 * 1024)

using Val32 = int32_t;

struct VMDef {
};

class VM {
public:
    explicit VM(VMDef&& vmdef);

    void set_ram(uint8_t const* data, size_t sz);
    void run_until_halt();

    [[nodiscard]] uint32_t PC() const { return PC_; }

    [[nodiscard]] Val32    peek() const;
    [[nodiscard]] uint16_t stack_sz() const { return stack_idx_; };

private:
    Val32 pop();
    void  push(Val32 v);
    Val32 nextVal();

    VMDef    vm_def_;

    Val32    stack_[STACK_SZ] { 0 };    // stack
    uint16_t stack_idx_ = 0;

    uint8_t  ram_[RAM_SZ] {0 };         // code
    uint32_t PC_ = 0;

    void (*oper[256])(VM& vm) {};       // pointer to function operators
};

#endif //FORTUNA_6_VM_HH
