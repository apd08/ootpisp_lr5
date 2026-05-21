#pragma once

// C linkage entry points exported by each plugin DLL.
// The host loads the module and calls registerLibraryPlugin().

#define PLUGIN_ENTRY_REGISTER "registerLibraryPlugin"
#define PLUGIN_ENTRY_NAME "getPluginName"

#if defined(_WIN32)
    #define PLUGIN_EXPORT extern "C" __declspec(dllexport)
#else
    #define PLUGIN_EXPORT extern "C" __attribute__((visibility("default")))
#endif

using PluginRegisterFn = void (*)();
using PluginNameFn = const char *(*)();
