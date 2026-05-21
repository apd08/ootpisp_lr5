#pragma once

// Export/import macros for the shared library_core DLL.
#if defined(_WIN32)
    #if defined(LIBRARY_CORE_BUILDING)
        #define LIBRARY_API __declspec(dllexport)
    #else
        #define LIBRARY_API __declspec(dllimport)
    #endif
#else
    #if defined(LIBRARY_CORE_BUILDING)
        #define LIBRARY_API __attribute__((visibility("default")))
    #else
        #define LIBRARY_API
    #endif
#endif
