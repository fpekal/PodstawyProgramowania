#include <iostream>
#include <list>
#include <compare>
#include <fstream>
#include <Windows.h>
#include <string>

class AddressDatabase {
public:
	struct Address {
		std::string firstName;
		std::string surname;

		std::string zipCode;
		std::string city;
		std::string street;
		std::string number;

		std::strong_ordering operator<=>(const Address& other) const {
			if (auto cmp = zipCode <=> other.zipCode; cmp != 0) return cmp;
			if (auto cmp = city <=> other.city; cmp != 0) return cmp;
			if (auto cmp = street <=> other.street; cmp != 0) return cmp;
			if (auto cmp = number <=> other.number; cmp != 0) return cmp;
			if (auto cmp = surname <=> other.surname; cmp != 0) return cmp;
			return firstName <=> other.firstName;
		}

		bool operator==(const Address& other) const {
			return (*this <=> other) == 0;
		}

		Address(const std::string& firstName, const std::string& surname, const std::string& zipCode, const std::string& city, const std::string& street, const std::string& number) :
			firstName{ firstName }, surname{ surname }, zipCode{ zipCode }, city{ city }, street{ street }, number{ number } {}

		Address() = default;
	};

	std::list<Address> addresses;

	void add(std::string firstName, std::string surname, std::string zipCode, std::string city, std::string street, std::string number) {
		addresses.emplace_back(firstName, surname, zipCode, city, street, number);
	}

	template<class T>
	void add(T&& other) {
		addresses.push_back(std::forward<T>(other));
	}

	void sort() {
		addresses.sort();
	}

	auto begin() {
		return addresses.begin();
	}
	
	auto end() {
		return addresses.end();
	}

	auto cbegin() const {
		return addresses.cbegin();
	}

	auto cend() const {
		return addresses.cend();
	}

	void printOne(std::list<Address>::const_iterator iter) const {
		std::cout <<
			"Imie:                  " << iter->firstName <<
			"\nNazwisko:              " << iter->surname <<
			"\nKod Pocztowy:          " << iter->zipCode <<
			"\nMiasto:                " << iter->city <<
			"\nUlica:                 " << iter->street <<
			"\nNumer domu/mieszkania: " << iter->number;
	}

	void printVerbose() const {
		bool first = true;
		size_t id = 1;
		for (auto iter = cbegin(); iter != cend(); ++iter) {
			if (!first) {
				std::cout << "\n----------------------------------------\n";
			}
			first = false;
			std::cout << "Wpis:                  " << id << '\n';
			printOne(iter);
			++id;
		}
	}

	void print() const {
		size_t id = 1;

		for (auto& obj : addresses) {
			std::cout << id << ' ' << obj.firstName << ' ' << obj.surname << '\n';
		}
	}

	void remove(std::list<Address>::const_iterator iter) {
		addresses.erase(iter);
	}

	void saveToFile() const {
		std::ofstream file{"db.csv.new"};

		bool first = true;

		for (auto& obj : addresses) {
			if (!first) {
				file << '\n';
			}
			first = false;

			file <<
				obj.firstName << ", " <<
				obj.surname << ", " <<
				obj.zipCode << ", " <<
				obj.city << ", " <<
				obj.street << ", " <<
				obj.number;
		}
		file.close();

		system("del db.csv.backup");
		system("ren db.csv db.csv.backup");
		system("ren db.csv.new db.csv");
	}

	void loadFromFile() {
		std::ifstream file{ "db.csv" };
		if (!file.is_open()) {
			std::cerr << "Cannot open file db.csv\n";
			return;
		}

		while (!file.eof()) {
			Address addr;

			std::getline(file, addr.firstName, ',');
			file.ignore(1);
			std::getline(file, addr.surname, ',');
			file.ignore(1);
			std::getline(file, addr.zipCode, ',');
			file.ignore(1);
			std::getline(file, addr.city, ',');
			file.ignore(1);
			std::getline(file, addr.street, ',');
			file.ignore(1);
			std::getline(file, addr.number, '\n');

			add(addr);
		}
	}

  static Address constructAddress() {
		AddressDatabase::Address newAddress;

		std::cout << "Imie: ";
		getline(std::cin, newAddress.firstName);
		std::cout << "Nazwisko: ";
		getline(std::cin, newAddress.surname);
		std::cout << "Kod pocztowy: ";
		getline(std::cin, newAddress.zipCode);
		std::cout << "Miejscowosc: ";
		getline(std::cin, newAddress.city);
		std::cout << "Ulica (- jezeli puste): ";
		getline(std::cin, newAddress.street);
		std::cout << "Numer domu/mieszkania: ";
		getline(std::cin, newAddress.number);

		return newAddress;
	}
};



int main() {
	AddressDatabase db;

	while (true) {
		std::cout << "\n\nload - zaladuj baze danych\nsave - zapisz baze danych\nnew - nowy wpis\ndelete - usun wpis\nmodify - zmodyfikuj wpis\nsort - posortuj alfabetycznie\nprint - wyswietl skrocone informacje\nprintv - wyswietl wszystkie dane\n";

		std::string command;
		std::cin >> command;

		if (command == "load") db.loadFromFile();
		else if (command == "save") db.saveToFile();
		else if (command == "new") {
			db.add(AddressDatabase::constructAddress());
		}
		else if (command == "modify") {
			std::cout << "Wpis: ";

			size_t entry;
			std::cin >> entry;

			auto iter = db.begin();
			std::advance(iter, entry - 1);

			std::cout << '\n';
			db.printOne(iter);

			std::cout << "\n\n";

			*iter = AddressDatabase::constructAddress();
		}
		else if (command == "delete") {
			std::cout << "Wpis: ";

			size_t entry;
			std::cin >> entry;

			auto iter = db.begin();
			std::advance(iter, entry - 1);

			db.remove(iter);
		}
		else if (command == "sort") {
			db.sort();
		}
		else if (command == "print") {
			std::cout << '\n';
			db.print();
		}
		else if (command == "printv") {
			std::cout << '\n';
			db.printVerbose();
		}
	}
}