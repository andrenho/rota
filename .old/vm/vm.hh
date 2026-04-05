#ifndef FORTUNA_6_VM_HH
#define FORTUNA_6_VM_HH

#include <cstdint>
#include <cstddef>
#include "../common/types.hh"

#define STACK_SZ 512
#define RAM_SZ (448 * 1024)

struct VMDef {
};

class VM {
public:
    explicit VM(VMDef&& vmdef);

    void set_ram(uint8_t const* data, size_t sz);
    void run_until_halt();

    [[nodiscard]] uint32_t PC() const { return PC_; }

    [[nodiscard]] types::Value peek() const;
    [[nodiscard]] uint16_t     stack_sz() const { return stack_idx_; };

private:
    types::Value pop();
    void         push(types::Value v);
    types::Value nextVal();

    template<typename Op> void binary_op(VM& vm, Op op);

    VMDef    vm_def_;

    types::Value stack_[STACK_SZ] { 0 };    // stack
    uint16_t     stack_idx_ = 0;

    uint8_t  ram_[RAM_SZ] {0 };         // code
    uint32_t PC_ = 0;

    void (*oper[256])(VM& vm) {};       // pointer to function operators
};

#endif //FORTUNA_6_VM_HH
