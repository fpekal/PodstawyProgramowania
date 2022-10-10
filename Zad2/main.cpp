#include <iostream>
#include <vector>
#include <cstring>

class FunctionsContainer {
	struct Function {	// struktura przechowująca wskaźnik na funkcję i jej nazwę
		const char* name;
		int (*ptr)(int);

		Function(const char* name, int (*ptr)(int)) : name{ name }, ptr{ ptr } {}
	};

public:
	std::vector<Function> functions;

public:
	void add(const char* name, int (*ptr)(int)) {	// dodaj nową funkcję do wektora
		functions.emplace_back(name, ptr);
	}

	int execute(const char* name, int arg) {		// wykonaj funkcję o danej nazwie, a jak nie istnieje to zwróć 0
		for (auto& func : functions) {
			if (strcmp(func.name, name) == 0) {
				return func.ptr(arg);
			}
		}

		return 0;
	}
};	// class FunctionsContainer


//////////// PRZYKŁADOWE FUNKCJE
int mul2(int a) {
	return 2 * a;
}

int counter(int a) {
	static int i = 0;
	i += a;
	return i;
}
///////////


int main() {
	FunctionsContainer fc;	// Przechowuje funkcje w tablicy
	fc.add("mul2", mul2);	// Dodawanie funkcji mul2 i przypisywanie jej nazwy "mul2"
	fc.add("counter", counter);

	std::cout << "mul2 - mnozenie razy 2\ncounter - sumuje argumenty z roznych wywolan\n\n";

	while (true) {
		std::string func_name;
		int arg;
		std::cin >> func_name >> arg;

		std::cout << fc.execute(func_name.c_str(), arg) << '\n';
	}
}