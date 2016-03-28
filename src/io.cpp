#include "io.h"
#include "errors/errors.hpp"

io::io()
{
}

std::string io::get_dir(std::string& file_name)
{
    std::string directory;
    const size_t last_slash_idx = file_name.rfind('/');
    if (std::string::npos != last_slash_idx)
    {
        directory = file_name.substr(0, last_slash_idx) +std::string("/");
    }

    return directory;
}

const schar* io::get_wc(const char* c)
{
    const size_t cSize = mbstowcs(NULL, c, 0) +sizeof(char);
    wchar_t* wc = new wchar_t[cSize];
    mbstowcs (wc, c, cSize);

    return wc;
}

const char* io::get_c(const wchar_t* c)
{
    const size_t cSize = wcslen(c) +sizeof(wchar_t);
    char* wc = new char[cSize];
    wcstombs(wc, c, cSize);

    return wc;
}

void io::read_file(char* path, std::wostringstream& str)
{
    schar* tem = read_file(path);
    if (!tem)
        return;

    str << tem;
    delete[] tem;
}

schar* io::read_file(char* path)
{
    setlocale( LC_ALL, "" );

    FILE* f = fopen(path, "r");
    if (!f)
        ERR(err_t::IO_FILE_NOT_FOUND, path);

    size_t l;
    char* buf;

    fseek(f, 0, SEEK_END);
    l = ftell(f) +1;
    rewind(f);

    buf = new char[l];
    fread(buf, 1, l, f);
    buf[l -1] = 0;

    fclose(f);

    wchar_t* ret = get_wc(buf);
    delete[] buf;

    return ret;
}
