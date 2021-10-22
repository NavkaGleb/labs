#ifndef OS_LAB1_PLATFORM_DETECTION_HPP_
#define OS_LAB1_PLATFORM_DETECTION_HPP_

#if defined(_WIN32) || defined(_WIN64)
#define OS_LAB1_PLATFORM_WINDOWS
#error "Windows64 are not supported!"
#elif defined(__APPLE__) || defined(__MACH__)
#define OS_LAB1_PLATFORM_MACOS
#error "MacOS is not supported!"
#elif defined(__ANDROID__)
#define OS_LAB1_PLATFORM_ANDROID
#error "Android is not supported!"
#elif defined(__linux__)
#define OS_LAB1_PLATFORM_LINUX
#pragma message("Linux platform!")
#else
#error "Unknown platform!"
#endif

#endif // OS_LAB1_PLATFORM_DETECTION_HPP_
