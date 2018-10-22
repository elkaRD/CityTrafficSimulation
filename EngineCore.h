///   Projekt PROI 18L
///   Symulator ruchu miejskiego
///
///   Copyright (C) Robert Dudzinski 2018
///
///   File: EngineCore.h


#ifndef ENGINECORE_H
#define ENGINECORE_H

#if WIN32
#include "EngineCoreWindows.h"
#else
#include "EngineCoreLinux.h"
#endif // WIN32

#endif // ENGINECORE_H
