#include <iostream>
#include <forward_list>

// Pojedynczy krążek to liczba oznaczająca jego rozmiar
typedef size_t HanoiPart;

class HanoiTower : private std::forward_list<HanoiPart> {
    // Dodaje krążek omijając testy wielkości krążków
    void forceAdd(HanoiPart part) {
        push_front(part);
        ++size;
    }

    // Usuwa ostatni krążek
    void forceRemove() {
        if (size == 0) return;
        pop_front();
        --size;
    }

    // Sprawdza czy można przenieść krążek z innej wieży
    bool checkSwap(const HanoiTower& other) const {
        return other.getFirst() < getFirst();
    }

    // Zwraca pierwszy krążek. Jeżeli nie ma żadnego krążka to zwraca 99999
    HanoiPart getFirst() const {
        if (size == 0) return 99999;
        return *cbegin();
    }

    size_t size;

public:
    typedef std::forward_list<HanoiPart>::iterator iterator;
    typedef std::forward_list<HanoiPart>::const_iterator const_iterator;

    const_iterator cbegin() const {
        return std::forward_list<HanoiPart>::cbegin();
    }

    const_iterator cend() const {
        return std::forward_list<HanoiPart>::cend();
    }

    iterator begin() {
        return std::forward_list<HanoiPart>::begin();
    }

    iterator end() {
        return std::forward_list<HanoiPart>::end();
    }

    // Przenosi krążek z jednej wieży na drugą
    bool swapOne(HanoiTower& other) {
        if (!checkSwap(other)) return false;

        forceAdd(other.getFirst());
        other.forceRemove();

        return true;
    }

    // Zwraca wielkość wieży
    size_t getSize() const {
        return size;
    }

    HanoiTower(size_t size) : HanoiTower{} {
        for (HanoiPart i = size; i > 0; --i) {
            forceAdd(i);
        }
    }

    HanoiTower() : size{ 0 } {};
};  // class HanoiTower

template<int parts_count>
class HanoiGame {
    HanoiTower a;
    HanoiTower b;
    HanoiTower c;
    
    // Zwraca wieżę na podstawie jej numeru
    HanoiTower& getTower(int i) {
        if (i == 1) return a;
        else if (i == 2) return b;
        return c;
    }

    // Przenosi kursor do pisania tekstu na ekranie, aby można było napisać tekst w dowolnym miejscu
    static void moveCursor(int x, int y) {
        std::cout << "\x1b[" << y << ";" << x << "f";
    }

    // Czyści cały ekran
    static void clearScreen() {
        std::cout << "\x1b[2J";
    }

    // Wyświetla jeden krążek w miejscu kursora
    static void printHanoiPart(HanoiPart part) {
        for (int i = 0; i < parts_count - part; ++i) {
            std::cout << ' ';
        }
        for (int i = 0; i < (part - 1)*2 + 1; ++i) {
            std::cout << '#';
        }
    }

    // Wyświetla całą wieżę
    void printTower(const HanoiTower& tower, int shift) {
        int i = 1;
        const int y_shift = parts_count - (int)tower.getSize();

        const int x_shift = shift * (parts_count * 2 + 2);

        for (auto iter = tower.cbegin(); iter != tower.cend(); ++iter, ++i) {
            moveCursor(x_shift + 1, i + y_shift);
            printHanoiPart(*iter);
        }

        moveCursor(x_shift + 1, parts_count + 1);
        for (int i = 0; i < parts_count * 2 - 1; ++i) {
            std::cout << '_';
        }
    }

public:
    // Wyświetla wszystkie wieże
    void print() {
        clearScreen();

        for (int i = 1; i <= 3; ++i) {
            printTower(getTower(i), i - 1);
        }

        moveCursor(1, parts_count + 3);
    }

    // Sterowanie grą
    void control() {
        std::cout << "> ";
        int first, second;
        std::cin >> first >> second;
        getTower(second).swapOne(getTower(first));
    }

    HanoiGame() : a{ parts_count }, b{}, c{} {}
};  // class HanoiGame

int main() {
    HanoiGame<5> game;

    while (true) {
        game.print();
        game.control();
    }
}