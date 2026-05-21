#pragma once

#include "LibraryObject.hpp"

// Intermediate abstract class for published materials.
class LIBRARY_API Publication : public LibraryObject {
public:
    void display() const override;
    void edit() override;

    const std::string& publisher() const { return publisher_; }
    void setPublisher(const std::string& publisher) { publisher_ = publisher; }

    int pages() const { return pages_; }
    void setPages(int pages) { pages_ = pages; }

protected:
    void serializePublication(std::ostream& out) const;
    void deserializePublication(std::istream& in);

    std::string publisher_;
    int pages_ = 0;
};
