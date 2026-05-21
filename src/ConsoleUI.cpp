#include "ConsoleUI.hpp"

#include "ObjectFactory.hpp"
#include "PluginManager.hpp"

#include <functional>
#include <iostream>
#include <limits>

namespace {
int readChoice() {
    int choice = 0;
    while (true) {
        std::cout << "Choice: ";
        if (std::cin >> choice) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return choice;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid input. Try again.\n";
    }
}

std::string readLine(const std::string& prompt) {
    std::cout << prompt;
    std::string line;
    std::getline(std::cin, line);
    return line;
}
}  // namespace

void ConsoleUI::run() {
    std::cout << "=== Library Management System (Lab 4: Plugins) ===\n";
    std::cout << "Serialization variant: Text (3)\n\n";

    bool running = true;
    while (running) {
        printMenu();
        const int choice = readChoice();

        switch (choice) {
            case 1:
                listObjects();
                break;
            case 2:
                addObject();
                break;
            case 3:
                removeObject();
                break;
            case 4:
                editObject();
                break;
            case 5:
                saveToFile();
                break;
            case 6:
                loadFromFile();
                break;
            case 7:
                loadPluginFile();
                break;
            case 8:
                loadPluginsFromFolder();
                break;
            case 9:
                listLoadedPlugins();
                break;
            case 10:
                runPluginActions();
                break;
            case 0:
                running = false;
                break;
            default:
                std::cout << "Unknown command.\n";
                break;
        }
        std::cout << '\n';
    }
}

void ConsoleUI::printMenu() const {
    std::cout << "--- Menu ---\n"
              << "1. List objects\n"
              << "2. Add object\n"
              << "3. Remove object\n"
              << "4. Edit object\n"
              << "5. Save to file\n"
              << "6. Load from file\n"
              << "7. Load plugin (file path)\n"
              << "8. Load all plugins from folder\n"
              << "9. List loaded plugins\n"
              << "10. Plugin actions\n"
              << "0. Exit\n";
}

void ConsoleUI::listObjects() const {
    if (objects_.empty()) {
        std::cout << "List is empty.\n";
        return;
    }

    for (std::size_t i = 0; i < objects_.size(); ++i) {
        std::cout << '[' << i << "] " << objects_[i]->typeName() << '\n';
        objects_[i]->display();
        std::cout << "---\n";
    }
}

void ConsoleUI::addObject() {
    const auto types = ObjectFactory::instance().registeredTypes();
    if (types.empty()) {
        std::cout << "No types registered in factory.\n";
        return;
    }

    std::cout << "Select type:\n";
    for (std::size_t i = 0; i < types.size(); ++i) {
        std::cout << "  " << i << ". " << types[i] << '\n';
    }

    const int typeIndex = readChoice();
    if (typeIndex < 0 || static_cast<std::size_t>(typeIndex) >= types.size()) {
        std::cout << "Invalid type index.\n";
        return;
    }

    try {
        LibraryObjectPtr object = ObjectFactory::instance().create(types[typeIndex]);
        object->setId(nextId());
        std::cout << "Enter fields for new " << types[typeIndex] << ":\n";
        object->edit();
        objects_.push_back(std::move(object));
        std::cout << "Object added.\n";
    } catch (const std::exception& ex) {
        std::cout << "Error: " << ex.what() << '\n';
    }
}

void ConsoleUI::removeObject() {
    if (objects_.empty()) {
        std::cout << "List is empty.\n";
        return;
    }

    listObjects();
    const int index = readChoice();
    if (index < 0 || static_cast<std::size_t>(index) >= objects_.size()) {
        std::cout << "Invalid index.\n";
        return;
    }

    objects_.erase(objects_.begin() + index);
    std::cout << "Object removed.\n";
}

void ConsoleUI::editObject() {
    if (objects_.empty()) {
        std::cout << "List is empty.\n";
        return;
    }

    listObjects();
    const int index = readChoice();
    if (index < 0 || static_cast<std::size_t>(index) >= objects_.size()) {
        std::cout << "Invalid index.\n";
        return;
    }

    std::cout << "Editing " << objects_[index]->typeName() << ":\n";
    objects_[index]->edit();
    std::cout << "Object updated.\n";
}

void ConsoleUI::saveToFile() {
    const std::string path = readLine("File path [" + defaultFile_ + "]: ");
    const std::string filePath = path.empty() ? defaultFile_ : path;

    try {
        serializer_.save(objects_, filePath);
        std::cout << "Saved " << objects_.size() << " object(s) to " << filePath << '\n';
    } catch (const std::exception& ex) {
        std::cout << "Save failed: " << ex.what() << '\n';
    }
}

void ConsoleUI::loadFromFile() {
    const std::string path = readLine("File path [" + defaultFile_ + "]: ");
    const std::string filePath = path.empty() ? defaultFile_ : path;

    try {
        objects_ = serializer_.load(filePath);
        std::cout << "Loaded " << objects_.size() << " object(s) from " << filePath << '\n';
    } catch (const std::exception& ex) {
        std::cout << "Load failed: " << ex.what() << '\n';
    }
}

int ConsoleUI::nextId() const {
    int maxId = 0;
    for (const auto& object : objects_) {
        if (object->id() > maxId) {
            maxId = object->id();
        }
    }
    return maxId + 1;
}

void ConsoleUI::loadPluginFile() {
    const std::string path = readLine("Plugin file path: ");
    if (path.empty()) {
        std::cout << "Path is empty.\n";
        return;
    }
    PluginManager::instance().loadPlugin(path);
}

void ConsoleUI::loadPluginsFromFolder() {
    const std::string folder = readLine("Plugins folder [plugins]: ");
    const std::string dir = folder.empty() ? "plugins" : folder;
    const int count = PluginManager::instance().loadPluginsFromDirectory(dir);
    std::cout << "Loaded " << count << " plugin(s) from " << dir << '\n';
}

void ConsoleUI::listLoadedPlugins() const {
    const auto& plugins = PluginManager::instance().loadedPlugins();
    if (plugins.empty()) {
        std::cout << "No plugins loaded.\n";
        return;
    }

    for (std::size_t i = 0; i < plugins.size(); ++i) {
        std::cout << "  " << i << ". " << plugins[i].name << " (" << plugins[i].path
                  << "), actions: " << plugins[i].actions.size() << '\n';
    }
}

void ConsoleUI::runPluginActions() {
    const auto& plugins = PluginManager::instance().loadedPlugins();

    std::vector<std::pair<std::string, std::function<void(const LibraryObjectList&)>>> actions;
    for (const auto& plugin : plugins) {
        for (const auto& action : plugin.actions) {
            actions.emplace_back(plugin.name + ": " + action.label, action.handler);
        }
    }

    if (actions.empty()) {
        std::cout << "No plugin actions available. Load a plugin first.\n";
        return;
    }

    std::cout << "Select plugin action:\n";
    for (std::size_t i = 0; i < actions.size(); ++i) {
        std::cout << "  " << i << ". " << actions[i].first << '\n';
    }

    const int index = readChoice();
    if (index < 0 || static_cast<std::size_t>(index) >= actions.size()) {
        std::cout << "Invalid action index.\n";
        return;
    }

    actions[static_cast<std::size_t>(index)].second(objects_);
}
