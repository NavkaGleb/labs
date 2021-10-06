#ifndef OS_LAB1_PLATFORM_DETECTION_H_
#define OS_LAB1_PLATFORM_DETECTION_H_

#ifdef _WIN32
  #ifdef _WIN64
    #define OS_LAB1_PLATFORM_WINDOWS
    #error "Windows64 are not supported!"
  #else
    #define OS_LAB1_PLATFORM_WINDOWS32
    #error "Windows32 are not supported!"
  #endif
#elif defined(__APPLE__) || defined(__MACH__)
  #include <TargetConditionals.h>

  #if TARGET_IPHONE_SIMULATOR == 1
    #error "IOS simulator is not supported!"
  #elif TARGET_OS_IPHONE == 1
    #define OS_LAB1_PLATFORM_IOS
    #error "IOS is not supported!"
  #elif TARGET_OS_MAC == 1
    #define OS_LAB1_PLATFORM_MACOS
    #error "MacOS is not supported!"
  #else
    #error "Unknown Apple platform!"
  #endif
#elif defined(__ANDROID__)
  #define OS_LAB1_PLATFORM_ANDROID
  #error "Android is not supported!"
#elif defined(__linux__)
  #define OS_LAB1_PLATFORM_LINUX
#else
  #error "Unknown platform!"
#endif

#endif // OS_LAB1_PLATFORM_DETECTION_H_
