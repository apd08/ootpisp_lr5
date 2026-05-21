#pragma once

#include "Publication.hpp"

class LIBRARY_API Book : public Publication {
public:
    std::string typeName() const override { return "Book"; }
    void display() const override;
    void edit() override;
    void serialize(std::ostream& out) const override;
    void deserialize(std::istream& in) override;

    const std::string& author() const { return author_; }
    void setAuthor(const std::string& author) { author_ = author; }

    const std::string& isbn() const { return isbn_; }
    void setIsbn(const std::string& isbn) { isbn_ = isbn; }

protected:
    void serializeBook(std::ostream& out) const;
    void deserializeBook(std::istream& in);

    std::string author_;
    std::string isbn_;
};
