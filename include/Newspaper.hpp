#pragma once

#include "Magazine.hpp"

class LIBRARY_API Newspaper : public Magazine {
public:
    std::string typeName() const override { return "Newspaper"; }
    void display() const override;
    void edit() override;
    void serialize(std::ostream& out) const override;
    void deserialize(std::istream& in) override;

    const std::string& issueDate() const { return issueDate_; }
    void setIssueDate(const std::string& date) { issueDate_ = date; }

    const std::string& section() const { return section_; }
    void setSection(const std::string& section) { section_ = section; }

private:
    std::string issueDate_;
    std::string section_;
};
