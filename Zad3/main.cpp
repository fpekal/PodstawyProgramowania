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

	void printVerbose() {
		bool first = true;
		for (auto& obj : addresses) {
			if (!first) {
				std::cout << "\n----------------------------------------\n";
			}
			first = false;
			std::cout <<
				  "First Name: " << obj.firstName <<
				"\nSurname:    " << obj.surname <<
				"\nZip-Code:   " << obj.zipCode <<
				"\nCity:       " << obj.city <<
				"\nStreet:     " << obj.street <<
				"\nNumber:     " << obj.number;
		}
	}

	void print() {
		for (auto& obj : addresses) {
			std::cout << obj.firstName << ' ' << obj.surname << '\n';
		}
	}

	void remove(std::list<Address>::const_iterator iter) {
		addresses.erase(iter);
	}

	void saveToFile() {
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
};



int main() {
	AddressDatabase db;

	//db.add("Jan", "Kowalski", "98-300", "Wielun", "Staszica", "13");
	//db.add("Andrzej", "Nowak", "98-300", "Wielun", "Wyszynskiego", "7/2");
	//db.saveToFile();

	db.loadFromFile();

	db.printVerbose();
}