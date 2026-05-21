#include "ComicBook.hpp"

#include "ObjectFactory.hpp"
#include "PluginApi.hpp"
#include "PluginManager.hpp"

#include <iostream>
#include <memory>

namespace {

std::string readLine(const std::string &prompt) {
    std::cout << prompt;
    std::string line;
    std::getline(std::cin, line);
    return line;
}

// Plugin UI action: list ComicBook objects grouped by illustrator.
void listComicsByIllustrator(const LibraryObjectList &objects) {
    bool found = false;
    for (const auto &object : objects) {
        if (object->typeName() != "ComicBook") {
            continue;
        }
        const auto *comic = dynamic_cast<const ComicBook *>(object.get());
        if (comic == nullptr) {
            continue;
        }
        found = true;
        std::cout << "  [" << comic->id() << "] " << comic->title()
                  << " — illustrator: " << comic->illustrator()
                  << ", series: " << comic->series() << '\n';
    }
    if (!found) {
        std::cout << "No ComicBook objects in the list.\n";
    }
}

}  // namespace

void ComicBook::display() const {
    Book::display();
    std::cout << "  Illustrator: " << illustrator_ << '\n'
              << "  Series: " << series_ << '\n';
}

void ComicBook::edit() {
    Book::edit();
    illustrator_ = readLine("Illustrator: ");
    series_ = readLine("Series: ");
}

void ComicBook::serialize(std::ostream &out) const {
    out << "TYPE " << typeName() << '\n';
    serializeBook(out);
    out << "illustrator " << illustrator_ << '\n'
        << "series " << series_ << '\n';
}

void ComicBook::deserialize(std::istream &in) {
    deserializeBook(in);
    std::string key;
    in >> key >> std::ws;
    std::getline(in, illustrator_);
    in >> key >> std::ws;
    std::getline(in, series_);
}

// Plugin entry point: register type and UI action in the host application.
PLUGIN_EXPORT void registerLibraryPlugin() {
    ObjectFactory::instance().registerType(
        "ComicBook", []() { return std::make_unique<ComicBook>(); });

    PluginManager::instance().registerAction(
        "Comic Plugin", "List ComicBooks by illustrator",
        [](const LibraryObjectList &objects) { listComicsByIllustrator(objects); });
}

PLUGIN_EXPORT const char *getPluginName() { return "Comic Plugin"; }
