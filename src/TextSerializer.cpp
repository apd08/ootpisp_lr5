#include "TextSerializer.hpp"

#include "ObjectFactory.hpp"

#include <fstream>
#include <stdexcept>

void TextSerializer::save(const LibraryObjectList& objects, const std::string& filePath) const {
    std::ofstream out(filePath);
    if (!out) {
        throw std::runtime_error("Cannot open file for writing: " + filePath);
    }

    out << kFormatHeader << '\n';
    out << "COUNT " << objects.size() << '\n';

    for (const auto& object : objects) {
        out << "BEGIN_OBJECT\n";
        object->serialize(out);
        out << "END_OBJECT\n";
    }
}

LibraryObjectList TextSerializer::load(const std::string& filePath) const {
    std::ifstream in(filePath);
    if (!in) {
        throw std::runtime_error("Cannot open file for reading: " + filePath);
    }

    std::string header;
    std::getline(in, header);
    if (header != kFormatHeader) {
        throw std::runtime_error("Invalid file format or version.");
    }

    std::string key;
    std::size_t count = 0;
    in >> key >> count;

    LibraryObjectList objects;
    objects.reserve(count);

    for (std::size_t i = 0; i < count; ++i) {
        std::string marker;
        in >> marker;
        if (marker != "BEGIN_OBJECT") {
            throw std::runtime_error("Expected BEGIN_OBJECT marker.");
        }

        std::string typeKey;
        std::string typeName;
        in >> typeKey >> typeName;
        if (typeKey != "TYPE") {
            throw std::runtime_error("Expected TYPE line inside object block.");
        }

        LibraryObjectPtr object = ObjectFactory::instance().create(typeName);
        object->deserialize(in);

        in >> marker;
        if (marker != "END_OBJECT") {
            throw std::runtime_error("Expected END_OBJECT marker.");
        }

        objects.push_back(std::move(object));
    }

    return objects;
}
