#pragma once

#include "LibraryObject.hpp"

#include <string>

// Text-format serializer (variant 3). Uses factory + virtual serialize/deserialize.
class TextSerializer {
public:
    static constexpr const char* kFormatHeader = "LIBRARY_TEXT_V1";

    void save(const LibraryObjectList& objects, const std::string& filePath) const;
    LibraryObjectList load(const std::string& filePath) const;
};
