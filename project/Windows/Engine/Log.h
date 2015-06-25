#ifndef LOG_H
#define LOG_H

//## **************************************************************************
//## LogError
//##
//## Used to report an error.
//## 
//## Input:
//##   pError - null terminated string to log.
//## **************************************************************************
void LogError(char* pError);

//## **************************************************************************
//## LogWarning
//##
//## Used to report an warning.
//## 
//## Input:
//##   pWarning - null terminated string to log.
//## **************************************************************************
void LogWarning(char* pWarning);

//## **************************************************************************
//## LogDebug
//##
//## Used to report an a debug statement.
//## 
//## Input:
//##   pDebug - null terminated string to log.
//## **************************************************************************
void LogDebug(char* pDebug);

#endif
