// Week 2 slide demo 3 -- RAII: the destructor runs on the exception path too.
//
// This is the whole argument for RAII in one program. The manual-cleanup
// version leaks the moment anything throws between the new and the delete;
// the RAII version cannot, because releasing is a destructor's job and the
// stack unwinder runs destructors whether you leave normally or by throwing.
//
// Build & run (from code/Module1):
//   g++ -std=c++20 -Wall -Wextra slides_demos/cpp/c3_raii.cpp -o bin/c3_raii
//   ./bin/c3_raii

#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

struct Handle {
    std::string name;
    explicit Handle(std::string n) : name(std::move(n)) {
        std::cout << "    + " << name << " acquired\n";
    }
    ~Handle() { std::cout << "    - " << name << " released\n"; }
};

void validate(int days) {
    if (days < 0) throw std::invalid_argument("overdueDays cannot be negative");
}

// ---- manual: correct on the happy path, leaks on the throwing one ----------
void manual(int days) {
    Handle* h = new Handle("manual");
    validate(days);              // <- if this throws, the delete below never runs
    delete h;
}

// ---- RAII: the release is the destructor, so it cannot be skipped ----------
void raii(int days) {
    auto h = std::make_unique<Handle>("raii");
    validate(days);              // <- unwinding still destroys h
}

int main() {
    std::cout << "1. manual cleanup, no exception\n";
    manual(3);

    std::cout << "\n2. manual cleanup, validate() throws\n";
    try { manual(-1); } catch (const std::exception& e) {
        std::cout << "    caught: " << e.what() << "   <- no 'released' line above\n";
    }

    std::cout << "\n3. RAII, validate() throws\n";
    try { raii(-1); } catch (const std::exception& e) {
        std::cout << "    caught: " << e.what() << '\n';
    }

    std::cout << "\nRead case 2 against case 3. Same failure, same message,\n"
                 "and only one of them still owns the handle.\n";
    return 0;
}
