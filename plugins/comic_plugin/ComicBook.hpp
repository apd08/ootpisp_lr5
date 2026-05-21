#pragma once

#include "Book.hpp"

// Plugin-provided type: a book with comic-specific metadata.
class ComicBook : public Book {
public:
    std::string typeName() const override { return "ComicBook"; }
    void display() const override;
    void edit() override;
    void serialize(std::ostream &out) const override;
    void deserialize(std::istream &in) override;

    const std::string &illustrator() const { return illustrator_; }
    void setIllustrator(const std::string &illustrator) { illustrator_ = illustrator; }

    const std::string &series() const { return series_; }
    void setSeries(const std::string &series) { series_ = series; }

private:
    std::string illustrator_;
    std::string series_;
};
