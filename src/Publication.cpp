#include "Publication.hpp"

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

void Publication::display() const {
    LibraryObject::display();
    std::cout << "  Publisher: " << publisher_ << '\n'
              << "  Pages: " << pages_ << '\n';
}

void Publication::edit() {
    LibraryObject::edit();
    publisher_ = readLine("Publisher: ");
    pages_ = readInt("Pages: ");
}

void Publication::serializePublication(std::ostream& out) const {
    serializeBase(out);
    out << "publisher " << publisher_ << '\n'
        << "pages " << pages_ << '\n';
}

void Publication::deserializePublication(std::istream& in) {
    deserializeBase(in);
    std::string key;
    in >> key >> std::ws;
    std::getline(in, publisher_);
    in >> key >> pages_;
}
