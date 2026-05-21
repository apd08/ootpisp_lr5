#include "ConsoleUI.hpp"
#include "PluginManager.hpp"

#include <iostream>

int main(int argc, char *argv[]) {
    PluginManager &pluginManager = PluginManager::instance();

    // Optional command-line argument: path to a single plugin module.
    if (argc > 1) {
        for (int i = 1; i < argc; ++i) {
            pluginManager.loadPlugin(argv[i]);
        }
    }

    // Auto-load every plugin from the default folder (no main-app recompile needed).
    const int autoLoaded = pluginManager.loadPluginsFromDirectory("plugins");
    if (autoLoaded > 0) {
        std::cout << "Auto-loaded " << autoLoaded << " plugin(s) from ./plugins\n\n";
    }

    ConsoleUI ui;
    ui.run();
    return 0;
}
