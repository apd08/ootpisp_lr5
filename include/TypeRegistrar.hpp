#pragma once

#include "ObjectFactory.hpp"

#include <memory>
#include <string>

// Registers a concrete type at static initialization time.
// Adding a new class only requires a static TypeRegistrar in its .cpp file.
template <typename T>
class TypeRegistrar {
public:
    explicit TypeRegistrar(const std::string& typeName) {
        ObjectFactory::instance().registerType(
            typeName, []() { return std::make_unique<T>(); });
    }
};
