#include "ObjectFactory.hpp"

#include <stdexcept>

ObjectFactory& ObjectFactory::instance() {
    static ObjectFactory factory;
    return factory;
}

void ObjectFactory::registerType(const std::string& typeName, Creator creator) {
    creators_[typeName] = std::move(creator);
}

LibraryObjectPtr ObjectFactory::create(const std::string& typeName) const {
    const auto it = creators_.find(typeName);
    if (it == creators_.end()) {
        throw std::runtime_error("Unknown object type: " + typeName);
    }
    return it->second();
}

std::vector<std::string> ObjectFactory::registeredTypes() const {
    std::vector<std::string> names;
    names.reserve(creators_.size());
    for (const auto& entry : creators_) {
        names.push_back(entry.first);
    }
    return names;
}
