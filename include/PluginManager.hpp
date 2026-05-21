#pragma once

#include "LibraryExport.hpp"
#include "LibraryObject.hpp"

#include <functional>
#include <string>
#include <vector>

// Describes one optional UI action provided by a loaded plugin.
struct PluginAction {
    std::string label;
    std::function<void(const LibraryObjectList &)> handler;
};

// Holds metadata for a dynamically loaded plugin module.
struct LoadedPlugin {
    std::string path;
    std::string name;
    void *handle = nullptr;
    std::vector<PluginAction> actions;
};

// Singleton that loads plugin DLLs and collects plugin-specific UI actions.
class LIBRARY_API PluginManager {
public:
    static PluginManager &instance();

  // Load a single plugin file (.dll / .so). Returns false on failure.
    bool loadPlugin(const std::string &path);

  // Load every plugin module found in the given directory.
    int loadPluginsFromDirectory(const std::string &directory);

    const std::vector<LoadedPlugin> &loadedPlugins() const { return plugins_; }

  // Plugins call this from registerLibraryPlugin() to add menu actions.
    void registerAction(const std::string &pluginName, const std::string &label,
                        std::function<void(const LibraryObjectList &)> handler);

private:
    PluginManager() = default;
    bool resolveAndRegister(void *handle, const std::string &path);

    std::vector<LoadedPlugin> plugins_;
};
