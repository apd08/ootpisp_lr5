#include "Newspaper.hpp"

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

void Newspaper::display() const {
    Magazine::display();
    std::cout << "  Issue date: " << issueDate_ << '\n'
              << "  Section: " << section_ << '\n';
}

void Newspaper::edit() {
    Magazine::edit();
    issueDate_ = readLine("Issue date (YYYY-MM-DD): ");
    section_ = readLine("Section: ");
}

void Newspaper::serialize(std::ostream& out) const {
    out << "TYPE " << typeName() << '\n';
    serializeMagazine(out);
    out << "issueDate " << issueDate_ << '\n'
        << "section " << section_ << '\n';
}

void Newspaper::deserialize(std::istream& in) {
    deserializeMagazine(in);
    std::string key;
    in >> key >> std::ws;
    std::getline(in, issueDate_);
    in >> key >> std::ws;
    std::getline(in, section_);
}

static TypeRegistrar<Newspaper> newspaperRegistrar("Newspaper");
