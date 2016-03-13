#ifndef IO_H
#define IO_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "string_def.h"

class io
{
public:
    io();

    const schar* get_wc(const char* c);
    void read_file(char* path, std::wostringstream* str);
    schar* read_file(char* path);
};

#endif // IO_H
