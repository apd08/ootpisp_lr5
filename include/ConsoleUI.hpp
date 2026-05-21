#pragma once

#include "LibraryObject.hpp"
#include "TextSerializer.hpp"

#include <string>

// Console menu for managing a list of library objects.
class ConsoleUI {
public:
    void run();

private:
    void printMenu() const;
    void listObjects() const;
    void addObject();
    void removeObject();
    void editObject();
    void saveToFile();
    void loadFromFile();
    void loadPluginFile();
    void loadPluginsFromFolder();
    void listLoadedPlugins() const;
    void runPluginActions();

    int nextId() const;

    LibraryObjectList objects_;
    TextSerializer serializer_;
    std::string defaultFile_ = "library_data.txt";
};
