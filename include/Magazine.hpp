#pragma once

#include "Publication.hpp"

class LIBRARY_API Magazine : public Publication {
public:
    std::string typeName() const override { return "Magazine"; }
    void display() const override;
    void edit() override;
    void serialize(std::ostream& out) const override;
    void deserialize(std::istream& in) override;

    int issueNumber() const { return issueNumber_; }
    void setIssueNumber(int issue) { issueNumber_ = issue; }

    const std::string& frequency() const { return frequency_; }
    void setFrequency(const std::string& frequency) { frequency_ = frequency; }

protected:
    void serializeMagazine(std::ostream& out) const;
    void deserializeMagazine(std::istream& in);

    int issueNumber_ = 0;
    std::string frequency_;
};
