#include "Book.hpp"

#include "TypeRegistrar.hpp"

#include <iostream>

namespace {
std::string readLine(const std::string& prompt) {
    std::cout << prompt;
    std::string line;
    std::getline(std::cin, line);
    return line;
}
}  // namespace

void Book::display() const {
    Publication::display();
    std::cout << "  Author: " << author_ << '\n'
              << "  ISBN: " << isbn_ << '\n';
}

void Book::edit() {
    Publication::edit();
    author_ = readLine("Author: ");
    isbn_ = readLine("ISBN: ");
}

void Book::serialize(std::ostream& out) const {
    out << "TYPE " << typeName() << '\n';
    serializeBook(out);
}

void Book::deserialize(std::istream& in) {
    deserializeBook(in);
}

void Book::serializeBook(std::ostream& out) const {
    serializePublication(out);
    out << "author " << author_ << '\n'
        << "isbn " << isbn_ << '\n';
}

void Book::deserializeBook(std::istream& in) {
    deserializePublication(in);
    std::string key;
    in >> key >> std::ws;
    std::getline(in, author_);
    in >> key >> std::ws;
    std::getline(in, isbn_);
}

// Registers Book in the global factory (no switch/case in serializer).
static TypeRegistrar<Book> bookRegistrar("Book");
