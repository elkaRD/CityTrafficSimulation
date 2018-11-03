///   EN: Project for OOP subject at Warsaw University of Technology
///       City traffic simulation
///
///   PL: Projekt PROI (Programowanie obiektowe) PW WEiTI 18L
///       Symulacja ruchu miejskiego
///
///   Copyright (C) Robert Dudzinski 2018
///
///   File: ExceptionClass.cpp


#include "ExceptionClass.h"

ExceptionClass::ExceptionClass(const char *msg) throw()
{
    exceptionMsg = msg;
}

ExceptionClass::ExceptionClass(std::string msg) throw()
{
    exceptionMsg = msg.c_str();
}

const char *ExceptionClass::what() const throw()
{
    return exceptionMsg;
}
