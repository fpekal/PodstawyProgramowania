#include <iostream>
#include <unordered_map>
#include <array>

// Słownik do zamieniania znaków na inty. `ctoi['A'] == 10`
const std::unordered_map<char, int> ctoi {
    {'0', 0},
    {'1', 1},
    {'2', 2},
    {'3', 3},
    {'4', 4},
    {'5', 5},
    {'6', 6},
    {'7', 7},
    {'8', 8},
    {'9', 9},
    {'A', 10},
    {'B', 11},
    {'C', 12},
    {'D', 13},
    {'E', 14},
    {'F', 15},
    {'G', 16},
    {'H', 17},
    {'I', 18},
    {'J', 19},
    {'K', 20},
    {'L', 21},
    {'M', 22},
    {'N', 23},
    {'O', 24},
    {'P', 25},
    {'Q', 26},
    {'R', 27},
    {'S', 28},
    {'T', 29},
    {'U', 30},
    {'V', 31},
    {'W', 32},
    {'X', 33},
    {'Y', 34},
    {'Z', 35}
};

// Tablica do zamieniania int'ów na znaki. `itoc[10] == 'A'`
constexpr std::array<char, 36> itoc{
    '0', '1', '2', '3', '4',
    '5', '6', '7', '8', '9',
    'A', 'B', 'C', 'D', 'E',
    'F', 'G', 'H', 'I', 'J',
    'K', 'L', 'M', 'N', 'O',
    'P', 'Q', 'R', 'S', 'T',
    'U', 'V', 'W', 'X', 'Y',
    'Z'
};


// Przekonwertuj const char* na int'a
size_t convertToInt(const char* str, size_t size, size_t base) {
    if (size == 1) return ctoi.at(*str);

    return convertToInt(str + 1, size - 1, base) + ctoi.at(*str) * std::pow(base, size - 1);
}

// Przekonwertuj std::string na int'a
size_t convertToInt(const std::string& str, size_t base) {
    return convertToInt(str.c_str(), str.length(), base);
}

// Przekonwertuj int'a na std::string
std::string convertToStr(size_t val, size_t base) {
    char right = itoc[val % base];
    val /= base;

    if (val == 0) return std::string{ right };
    return convertToStr(val, base) + right;
}

int main() {
    while (true) {
        size_t inBase, outBase;
        std::string in;

        std::cout << "System na wejsciu: ";
        std::cin >> inBase;
        std::cout << "Wejscie: ";
        std::cin >> in;
        std::cout << "System na wyjsciu: ";
        std::cin >> outBase;
        std::cout << "Wyjscie: " << convertToStr(convertToInt(in, inBase), outBase) << "\n\n";
    }
}