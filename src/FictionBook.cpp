#include "FictionBook.hpp"

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

void FictionBook::display() const {
    Book::display();
    std::cout << "  Genre: " << genre_ << '\n';
}

void FictionBook::edit() {
    Book::edit();
    genre_ = readLine("Genre: ");
}

void FictionBook::serialize(std::ostream& out) const {
    out << "TYPE " << typeName() << '\n';
    serializeBook(out);
    out << "genre " << genre_ << '\n';
}

void FictionBook::deserialize(std::istream& in) {
    deserializeBook(in);
    std::string key;
    in >> key >> std::ws;
    std::getline(in, genre_);
}

static TypeRegistrar<FictionBook> fictionBookRegistrar("FictionBook");
