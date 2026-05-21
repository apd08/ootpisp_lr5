#include "LibraryObject.hpp"

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

void LibraryObject::display() const {
    std::cout << "  ID: " << id_ << '\n'
              << "  Title: " << title_ << '\n'
              << "  Year: " << year_ << '\n';
}

void LibraryObject::edit() {
    title_ = readLine("Title: ");
    year_ = readInt("Year: ");
}

void LibraryObject::serializeBase(std::ostream& out) const {
    out << "id " << id_ << '\n'
        << "title " << title_ << '\n'
        << "year " << year_ << '\n';
}

void LibraryObject::deserializeBase(std::istream& in) {
    std::string key;
    in >> key >> id_;
    in >> key >> std::ws;
    std::getline(in, title_);
    in >> key >> year_;
}
