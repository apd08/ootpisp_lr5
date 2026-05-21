#pragma once

#include "Book.hpp"

class LIBRARY_API Textbook : public Book {
public:
    std::string typeName() const override { return "Textbook"; }
    void display() const override;
    void edit() override;
    void serialize(std::ostream& out) const override;
    void deserialize(std::istream& in) override;

    const std::string& subject() const { return subject_; }
    void setSubject(const std::string& subject) { subject_ = subject; }

    int edition() const { return edition_; }
    void setEdition(int edition) { edition_ = edition; }

private:
    std::string subject_;
    int edition_ = 1;
};
