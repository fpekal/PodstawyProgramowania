#include <type_traits>
#include <iostream>
#include <Windows.h>

template<class T>
class list {

	// pojedynczy element listy
	struct node {
		T value;	// wartość elementu

		node* previous;	// poprzedni element
		node* next;		// następny element

		// forwarding-reference
		template<class U>
		node(U&& value) : value{ std::forward<U>(value) }, previous{ nullptr }, next{ nullptr } {}
	};	// struct node

	node* first;	// ostatni element listy
	node* last;		// pierwszy element listy

	size_t size;	// ilość elementów w liście

	template<class U>
	void first_element(U&& new_value) {	// funkcja pomocnicza do wkładania pierwszego elementu do listy
		node* new_node = new node{ std::forward<U>(new_value) };

		size = 1;

		first = new_node;
		last = new_node;
	}

public:
	list() : first{ nullptr }, last{ nullptr }, size{ 0 } {}
	~list() {
		// zniszcz wszystkie elementy listy
		node* next = nullptr;
		for (node* i = first; i != nullptr; i = next) {
			next = i->next;
			delete i;
		}
	}

	// włóż nowy element na koniec listy
	template<class U>
	void push_back(U&& new_value) {
		if (size == 0) {	// jeżeli lista pusta, użyj funkcji pomocniczej
			first_element(std::forward<U>(new_value));
		}
		else {	// jeżeli lista posiada już jakieś elementy
			node* new_node = new node{ std::forward<U>(new_value) };

			new_node->previous = last;
			last->next = new_node;
			last = new_node;

			++size;
		}
	}

	// włóż nowy element na początek listy
	template<class U>
	void push_front(U&& new_value) {
		if (size == 0) {
			first_element(new_value);
		}
		else {
			node* new_node = new node{ std::forward<U>(new_value) };

			new_node->next = first;
			first->previous = new_node;
			first = new_node;

			++size;
		}
	}

	// włóż nowy element w dowolnym miejscu listy
	template<class U>
	void insert(U&& new_value, size_t position) {
		if (size == 0) {
			first_element(std::forward<U>(new_value));
		}
		else if (position == 0) {
			push_front(std::forward<U>(new_value));
		}
		else if (position >= size) {
			push_back(std::forward<U>(new_value));
		}
		else {
			node* next = first;

			// przejdź do elementu, który będzie po nowym elemencie
			for (int i = 0; i < position; i++) {
				next = next->next;
			}

			node* previous = next->previous;

			// next = element, który będzie po nowym elemencie
			// previous = element, który będzie przed nowym elementem

			node* new_node = new node{ std::forward<U>(new_value) };

			previous->next = new_node;
			new_node->previous = previous;

			new_node->next = next;
			next->previous = new_node;

			++size;
		}
	}	// void insert

	void remove(size_t position) {
		if (size == 0) return;	// jeżeli lista jest pusta, to nic nie rób
		else if (size == 1) {	// jeżeli jest tylko jeden element w liście
			delete first;
			first = nullptr;
			last = nullptr;

			size = 0;
		}
		else if (position == 0) {	// jeżeli usuwany jest pierwszy element
			node* to_remove = first;
			first = first->next;
			first->previous = nullptr;

			delete to_remove;

			--size;
		}
		else if (position >= size - 1) {	// jeżeli usuwany jest ostatni element
			node* to_remove = last;
			last = last->previous;
			last->next = nullptr;

			delete to_remove;

			--size;
		}
		else {
			node* found = first;

			for (int i = 0; i < position; ++i) {	// jeżeli usuwany jest element w środku
				found = found->next;
			}

			found->previous->next = found->next;
			found->next->previous = found->previous;

			delete found;

			--size;
		}
	} // void remove

	class iterator {
		node* n;

		iterator() : n{ nullptr } {}

		friend class list<T>;
	public:
		iterator& operator++() {
			if (n != nullptr)
				n = n->next;

			return *this;
		}

		iterator operator++(int) {
			iterator ret = *this;
			if (n != nullptr)
				n = n->next;

			return ret;
		}

		iterator& operator--() {
			if (n != nullptr)
				n = n->previous;

			return *this;
		}

		iterator operator--(int) {
			iterator ret = *this;
			if (n != nullptr)
				n = n->previous;

			return ret;
		}

		bool operator==(const iterator& other) {
			return n == other.n;
		}

		bool operator!=(const iterator& other) {
			return !operator==(other);
		}

		T& operator*() {
			return n->value;
		}
	};	// class iterator

	iterator begin() const {
		iterator ret{};
		ret.n = first;
		return ret;
	}

	iterator end() const {
		return iterator{};
	}

	size_t length() const {
		return size;
	}
};	// class list

// Klasa wirtualna opisująca dowolny kształt
struct Shape {
	virtual float getArea() = 0;
	virtual float getCirc() = 0;

	virtual std::string getName() = 0;
};

struct Square : Shape {
	// bok
	float a;

	float getArea() {
		return a * a;
	}

	float getCirc() {
		return 4 * a;
	}

	std::string getName() {
		static const std::string name = "square";

		return name;
	}

	static Square* construct() {
		std::cout << "side length: ";
		float a;
		std::cin >> a;
		return new Square{ a };
	}

	Square(float a) : a{ a } {}
};	// struct Square

struct Circle : Shape {
	// promień
	float r;

	static const float PI;

	float getArea() {
		return r * r * PI;
	}

	float getCirc() {
		return 2 * r * PI;
	}

	std::string getName() {
		static const std::string name = "circle";

		return name;
	}

	static Circle* construct() {
		std::cout << "radius: ";
		float r;
		std::cin >> r;
		return new Circle{ r };
	}

	Circle(float r) : r{ r } {}
};	// struct Circle
const float Circle::PI = 3.1415926536f;

void displayShapesList(list<Shape*>& l, size_t cursor_position) {
	size_t iter = 0;
	for (auto& shape : l) {
		std::cout << (cursor_position == iter ? '>' : ' ') << shape->getName() << "   area: " << shape->getArea() << " circumference: " << shape->getCirc() << '\n';
		++iter;
	}
}

int main() {
	list<Shape*> listOfShapes;
	size_t iter = 0;					// indeks wybranego elementu
	
	while (true) {
		system("cls");		// czyszczenie ekranu, działa tylko pod windowsem
		displayShapesList(listOfShapes, iter);

		std::cout << "\nw - up, s - down, r - remove, c - create\n";

		std::string choice;
		std::cin >> choice;

		if (choice == "w") {
			if (iter > 0) {
				--iter;
			}
		}
		else if (choice == "s") {
			if (static_cast<long long>(iter) < static_cast<long long>(listOfShapes.length()) - 1) {
				++iter;
			}
		}
		else if (choice == "r") {
			if (listOfShapes.length() > 0) {
				listOfShapes.remove(iter);
				if (iter == listOfShapes.length() && iter != 0) --iter;
			}
		}
		else if (choice == "c") {
			std::cout << "s - square, c - circle\n";

			std::string shape;
			std::cin >> shape;

			if (shape == "s") listOfShapes.push_back(Square::construct());
			else if (shape == "c") listOfShapes.push_back(Circle::construct());
		}
	}
}