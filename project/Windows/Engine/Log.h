#ifndef LOG_H
#define LOG_H

#if defined(ANDROID)
#include <android/log.h>
#define LogDebug(...) ((void)__android_log_print(ANDROID_LOG_INFO, "Vakz", __VA_ARGS__))
#define LogWarning(...) ((void)__android_log_print(ANDROID_LOG_WARN, "Vakz", __VA_ARGS__))
#define LogError(...) ((void)__android_log_print(ANDROID_LOG_WARN, "Vakz", __VA_ARGS__))
#else

//## **************************************************************************
//## LogError
//##
//## Used to report an error.
//## 
//## Input:
//##   pError - null terminated string to log.
//## **************************************************************************
void LogError(const char* pError);

//## **************************************************************************
//## LogWarning
//##
//## Used to report an warning.
//## 
//## Input:
//##   pWarning - null terminated string to log.
//## **************************************************************************
void LogWarning(const char* pWarning);

//## **************************************************************************
//## LogDebug
//##
//## Used to report an a debug statement.
//## 
//## Input:
//##   pDebug - null terminated string to log.
//## **************************************************************************
void LogDebug(const char* pDebug);

#endif
#endif
