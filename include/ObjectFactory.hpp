#pragma once

#include "LibraryExport.hpp"
#include "LibraryObject.hpp"

#include <functional>
#include <map>
#include <string>
#include <vector>

// Singleton factory: creates objects by type name using registered creators.
class LIBRARY_API ObjectFactory {
public:
    using Creator = std::function<LibraryObjectPtr()>;

    static ObjectFactory& instance();

    void registerType(const std::string& typeName, Creator creator);
    LibraryObjectPtr create(const std::string& typeName) const;
    std::vector<std::string> registeredTypes() const;

private:
    ObjectFactory() = default;

    std::map<std::string, Creator> creators_;
};
