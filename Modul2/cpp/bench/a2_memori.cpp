// A2: pemakaian memori LinkedList vs std::vector pada tag dari my-catalog.csv
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <new>
#include <string>
#include <vector>
#include "pinjamlab/structures/linked_list.hpp"

using namespace pinjamlab::structures;

static std::size_t g_calls = 0, g_bytes = 0;
void* operator new(std::size_t n) {
    ++g_calls;
    g_bytes += n;
    if (void* p = std::malloc(n)) return p;
    throw std::bad_alloc();
}
void operator delete(void* p) noexcept { std::free(p); }
void operator delete(void* p, std::size_t) noexcept { std::free(p); }

int main() {
    std::vector<std::string> tags;
    {
        std::ifstream in("sample-data/my-catalog.csv");
        std::string line;
        std::getline(in, line);
        while (std::getline(in, line))
            if (!line.empty()) tags.push_back(line.substr(0, line.find(',')));
    }
    std::cout << "baris dibaca = " << tags.size() << "\n";
    std::cout << "sizeof(std::string)             = " << sizeof(std::string) << "\n";
    std::cout << "sizeof(LinkedList<string>::Node) = "
              << sizeof(LinkedList<std::string>::Node) << "\n\n";

    g_calls = g_bytes = 0;
    {
        LinkedList<std::string> l;
        for (const auto& t : tags) l.pushBack(t);
        std::cout << "LinkedList: elemen=" << l.size() << " panggilan new=" << g_calls
                  << " byte diminta=" << g_bytes << "\n";
    }
    g_calls = g_bytes = 0;
    {
        std::vector<std::string> v;
        for (const auto& t : tags) v.push_back(t);
        std::cout << "vector    : elemen=" << v.size() << " panggilan new=" << g_calls
                  << " byte diminta=" << g_bytes << " (capacity=" << v.capacity() << ")\n";
    }
    g_calls = g_bytes = 0;
    {
        std::vector<std::string> v;
        v.reserve(tags.size());
        for (const auto& t : tags) v.push_back(t);
        std::cout << "vector+reserve: panggilan new=" << g_calls << " byte diminta=" << g_bytes << "\n";
    }
}