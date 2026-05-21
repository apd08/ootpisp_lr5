#pragma once

#include "LibraryExport.hpp"

#include <iostream>
#include <memory>
#include <string>
#include <vector>

// Abstract root of the library object hierarchy.
class LIBRARY_API LibraryObject {
public:
    virtual ~LibraryObject() = default;

    virtual std::string typeName() const = 0;
    virtual void display() const;
    virtual void edit();

    // Persist object-specific fields (type line is written by serializer).
    virtual void serialize(std::ostream& out) const = 0;
    virtual void deserialize(std::istream& in) = 0;

    int id() const { return id_; }
    void setId(int id) { id_ = id; }

    const std::string& title() const { return title_; }
    void setTitle(const std::string& title) { title_ = title; }

    int year() const { return year_; }
    void setYear(int year) { year_ = year; }

protected:
    void serializeBase(std::ostream& out) const;
    void deserializeBase(std::istream& in);

    int id_ = 0;
    std::string title_;
    int year_ = 0;
};

using LibraryObjectPtr = std::unique_ptr<LibraryObject>;
using LibraryObjectList = std::vector<LibraryObjectPtr>;
