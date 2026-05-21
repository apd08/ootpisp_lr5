#include "PluginManager.hpp"

#include "PluginApi.hpp"

#include <filesystem>
#include <iostream>

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#else
#include <dlfcn.h>
#endif

namespace {

// Returns true if a plugin with the same path is already loaded.
bool isAlreadyLoaded(const std::vector<LoadedPlugin> &plugins, const std::string &path) {
    const std::string normalized =
        std::filesystem::path(path).lexically_normal().string();
    for (const auto &plugin : plugins) {
        if (std::filesystem::path(plugin.path).lexically_normal().string() == normalized) {
            return true;
        }
    }
    return false;
}

#ifdef _WIN32
void *loadModule(const std::string &path) {
    return static_cast<void *>(LoadLibraryA(path.c_str()));
}

void *getSymbol(void *handle, const char *name) {
    return reinterpret_cast<void *>(GetProcAddress(static_cast<HMODULE>(handle), name));
}

std::string moduleError() {
    const DWORD code = GetLastError();
    if (code == 0) {
        return "unknown error";
    }
    return "Win32 error " + std::to_string(code);
}

void unloadModule(void *handle) {
    if (handle != nullptr) {
        FreeLibrary(static_cast<HMODULE>(handle));
    }
}
#else
void *loadModule(const std::string &path) {
    return dlopen(path.c_str(), RTLD_NOW);
}

void *getSymbol(void *handle, const char *name) { return dlsym(handle, name); }

std::string moduleError() {
    const char *msg = dlerror();
    return (msg != nullptr) ? msg : "unknown error";
}

void unloadModule(void *handle) {
    if (handle != nullptr) {
        dlclose(handle);
    }
}
#endif

}  // namespace

PluginManager &PluginManager::instance() {
    static PluginManager manager;
    return manager;
}

bool PluginManager::loadPlugin(const std::string &path) {
    if (isAlreadyLoaded(plugins_, path)) {
        std::cout << "Plugin already loaded: " << path << '\n';
        return true;
    }

    void *const handle = loadModule(path);
    if (handle == nullptr) {
        std::cout << "Failed to load plugin \"" << path << "\": " << moduleError() << '\n';
        return false;
    }

    if (!resolveAndRegister(handle, path)) {
        unloadModule(handle);
        return false;
    }

    return true;
}

int PluginManager::loadPluginsFromDirectory(const std::string &directory) {
    namespace fs = std::filesystem;

    if (!fs::exists(directory) || !fs::is_directory(directory)) {
        std::cout << "Plugin directory not found: " << directory << '\n';
        return 0;
    }

    int loadedCount = 0;
    for (const auto &entry : fs::directory_iterator(directory)) {
        if (!entry.is_regular_file()) {
            continue;
        }

#ifdef _WIN32
        const std::string ext = entry.path().extension().string();
        if (ext != ".dll") {
            continue;
        }
#else
        const std::string ext = entry.path().extension().string();
        if (ext != ".so" && ext != ".dylib") {
            continue;
        }
#endif

        if (loadPlugin(entry.path().string())) {
            ++loadedCount;
        }
    }

    return loadedCount;
}

void PluginManager::registerAction(
    const std::string & /*pluginName*/, const std::string &label,
    std::function<void(const LibraryObjectList &)> handler) {
    if (plugins_.empty()) {
        return;
    }
    plugins_.back().actions.push_back(PluginAction{label, std::move(handler)});
}

bool PluginManager::resolveAndRegister(void *handle, const std::string &path) {
    auto *const registerFn =
        reinterpret_cast<PluginRegisterFn>(getSymbol(handle, PLUGIN_ENTRY_REGISTER));
    if (registerFn == nullptr) {
        std::cout << "Plugin \"" << path << "\" does not export " << PLUGIN_ENTRY_REGISTER
                  << ".\n";
        return false;
    }

    LoadedPlugin plugin;
    plugin.path = path;
    plugin.handle = handle;
    plugin.name = path;

    auto *const nameFn = reinterpret_cast<PluginNameFn>(getSymbol(handle, PLUGIN_ENTRY_NAME));
    plugins_.push_back(std::move(plugin));

    if (nameFn != nullptr) {
        plugins_.back().name = nameFn();
    }

    // Registration may add factory types and plugin-specific UI actions.
    registerFn();

    std::cout << "Loaded plugin: " << plugins_.back().name << " (" << path << ")\n";
    return true;
}
