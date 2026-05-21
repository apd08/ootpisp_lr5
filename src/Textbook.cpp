#include "Textbook.hpp"

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

void Textbook::display() const {
    Book::display();
    std::cout << "  Subject: " << subject_ << '\n'
              << "  Edition: " << edition_ << '\n';
}

void Textbook::edit() {
    Book::edit();
    subject_ = readLine("Subject: ");
    edition_ = readInt("Edition: ");
}

void Textbook::serialize(std::ostream& out) const {
    out << "TYPE " << typeName() << '\n';
    serializeBook(out);
    out << "subject " << subject_ << '\n'
        << "edition " << edition_ << '\n';
}

void Textbook::deserialize(std::istream& in) {
    deserializeBook(in);
    std::string key;
    in >> key >> std::ws;
    std::getline(in, subject_);
    in >> key >> edition_;
}

static TypeRegistrar<Textbook> textbookRegistrar("Textbook");
