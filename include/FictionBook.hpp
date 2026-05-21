#pragma once

#include "Book.hpp"

class LIBRARY_API FictionBook : public Book {
public:
    std::string typeName() const override { return "FictionBook"; }
    void display() const override;
    void edit() override;
    void serialize(std::ostream& out) const override;
    void deserialize(std::istream& in) override;

    const std::string& genre() const { return genre_; }
    void setGenre(const std::string& genre) { genre_ = genre; }

private:
    std::string genre_;
};
