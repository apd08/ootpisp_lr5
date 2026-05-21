#include "Magazine.hpp"

#include "TypeRegistrar.hpp"

#include <iostream>
#include <limits>

namespace {
int readInt(const std::string& prompt) {
    int value = 0;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid input. Try again.\n";
    }
}

std::string readLine(const std::string& prompt) {
    std::cout << prompt;
    std::string line;
    std::getline(std::cin, line);
    return line;
}
}  // namespace

void Magazine::display() const {
    Publication::display();
    std::cout << "  Issue: " << issueNumber_ << '\n'
              << "  Frequency: " << frequency_ << '\n';
}

void Magazine::edit() {
    Publication::edit();
    issueNumber_ = readInt("Issue number: ");
    frequency_ = readLine("Frequency (monthly/weekly): ");
}

void Magazine::serialize(std::ostream& out) const {
    out << "TYPE " << typeName() << '\n';
    serializeMagazine(out);
}

void Magazine::deserialize(std::istream& in) {
    deserializeMagazine(in);
}

void Magazine::serializeMagazine(std::ostream& out) const {
    serializePublication(out);
    out << "issue " << issueNumber_ << '\n'
        << "frequency " << frequency_ << '\n';
}

void Magazine::deserializeMagazine(std::istream& in) {
    deserializePublication(in);
    std::string key;
    in >> key >> issueNumber_;
    in >> key >> std::ws;
    std::getline(in, frequency_);
}

static TypeRegistrar<Magazine> magazineRegistrar("Magazine");
