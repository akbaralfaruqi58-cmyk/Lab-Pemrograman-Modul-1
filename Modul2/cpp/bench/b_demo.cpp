// Cacat B: menghapus dari std::vector sambil mengiterasinya (Listing 6, modul)
#include <iostream>
#include <string>
#include <vector>

using Tags = std::vector<std::string>;

void removeReturned_BROKEN(Tags& tags) {
    for (auto it = tags.begin(); it != tags.end(); ++it)
        if (it->starts_with("LP-"))
            tags.erase(it);                       // UB: 'it' tidak valid setelah erase
}

void removeReturned_FIXED(Tags& tags) {           // perbaikan 1: pakai iterator hasil erase
    for (auto it = tags.begin(); it != tags.end(); )
        it = it->starts_with("LP-") ? tags.erase(it) : it + 1;
}

void removeReturned_IDIOMATIC(Tags& tags) {       // perbaikan 2: C++20
    std::erase_if(tags, [](const std::string& t) { return t.starts_with("LP-"); });
}

static void show(const char* name, const Tags& t) {
    std::cout << name << " (" << t.size() << "):";
    for (const auto& s : t) std::cout << ' ' << s;
    std::cout << '\n';
}

int main() {
    // Data A: tag LP- tidak berdampingan -> BROKEN kebetulan benar ("lulus test")
    const Tags a = {"AR-1", "LP-1", "AR-2", "LP-2", "AR-3"};
    // Data B: dua LP- berdampingan dan satu di ujung -> BROKEN salah, tanpa pesan error
    const Tags b = {"AR-1", "LP-1", "LP-2", "AR-2", "LP-3", "LP-4"};

    for (const auto* data : {&a, &b}) {
        std::cout << (data == &a ? "== Data A ==\n" : "== Data B ==\n");
        Tags x = *data, y = *data, z = *data;
        removeReturned_BROKEN(x);     show("BROKEN    ", x);
        removeReturned_FIXED(y);      show("FIXED     ", y);
        removeReturned_IDIOMATIC(z);  show("IDIOMATIC ", z);
    }
}