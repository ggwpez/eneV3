#include "io.h"
#include "errors/errors.hpp"

io::io()
{
}

const schar* io::get_wc(const char* c)
{
    const size_t cSize = mbstowcs(NULL, c, 0)+1;
    wchar_t* wc = new wchar_t[cSize];
    mbstowcs (wc, c, cSize);

    return wc;
}

void io::read_file(char* path, std::wostringstream& str)
{
    schar* tem = read_file(path);
    if (!tem)
        return;

    str << tem;
    free(tem);
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

    buf = malloc(l);
    fread(buf, 1, l, f);
    buf[l -1] = 0;

    fclose(f);

    wchar_t* ret = get_wc(buf);
    free(buf);

    return ret;
}
