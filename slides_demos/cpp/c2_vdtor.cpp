// Week 2 slide demo 2 -- the missing `virtual` on a base destructor.
//
// Two hierarchies that differ by ONE keyword. Both are deleted through a base
// pointer. Watch which destructors announce themselves.
//
// Build & run (from code/Module1):
//   g++ -std=c++20 -Wall -Wextra slides_demos/cpp/c2_vdtor.cpp -o bin/c2_vdtor
//   ./bin/c2_vdtor

#include <iostream>
#include <memory>
#include <string>

// ---------------------------------------------------------------- broken ---
struct BadBase {
    ~BadBase() { std::cout << "  ~BadBase\n"; }          // NOT virtual
    virtual int dailyFine() const { return 0; }
};

struct BadLaptop : BadBase {
    std::string* serial = new std::string("SN-0001");    // owns heap memory
    ~BadLaptop() {
        std::cout << "  ~BadLaptop (frees serial)\n";
        delete serial;
    }
    int dailyFine() const override { return 50000; }
};

// ----------------------------------------------------------------- fixed ---
struct GoodBase {
    virtual ~GoodBase() { std::cout << "  ~GoodBase\n"; }  // virtual
    virtual int dailyFine() const { return 0; }
};

struct GoodLaptop : GoodBase {
    std::string* serial = new std::string("SN-0002");
    ~GoodLaptop() override {
        std::cout << "  ~GoodLaptop (frees serial)\n";
        delete serial;
    }
    int dailyFine() const override { return 50000; }
};

int main() {
    std::cout << "A. base destructor NOT virtual\n";
    {
        BadBase* p = new BadLaptop();
        std::cout << "  dailyFine() -> " << p->dailyFine()
                  << "   (dispatch still works)\n";
        delete p;
    }

    std::cout << "\nB. base destructor virtual\n";
    {
        GoodBase* p = new GoodLaptop();
        std::cout << "  dailyFine() -> " << p->dailyFine() << '\n';
        delete p;
    }

    std::cout << "\nC. the same fix, spelled unique_ptr\n";
    {
        std::unique_ptr<GoodBase> p = std::make_unique<GoodLaptop>();
        std::cout << "  leaving the scope now\n";
    }

    std::cout << "\nIn A the derived destructor never ran, so `serial` was\n"
                 "never freed -- and dailyFine() dispatched correctly the whole\n"
                 "time, which is why nothing looks wrong from the outside.\n";
    return 0;
}
