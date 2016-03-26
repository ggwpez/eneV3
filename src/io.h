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

    static std::string get_dir(std::string& file_name);
    static const schar* get_wc(const char* c);
    static const char*  get_c (const wchar_t* c);
    static void read_file(char* path, std::wostringstream& str);
    static schar* read_file(char* path);
};

#endif // IO_H
