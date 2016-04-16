#ifndef COMPILER_H
#define COMPILER_H

#include "io.h"
#include "errors/warnings.h"
#include "errors/errors.hpp"
#include "target.h"
#include "name_mng.h"
#include "gen/gen.h"

#include "cmp_alloc.h"
#include "cmp_args.h"

#include <string.h>



class compiler
{
public:
    compiler(cmp_args& args);
    ~compiler();

    int compile();
    void compile_file(std::string const& file_name, std::string& output_file_name, std::vector<std::string> *included_asm);

    std::string build_dir;

private:
    cmp_args* args;
    scope* sc;
    name_mng* names;
    std::vector<cmp_alloc*>* allocs;

    void post_as(std::string& i_file, std::string& o_file);
    void post_ld(std::vector<std::string>& i_file, std::string& o_file);
    void write_wstr(std::wostringstream &ss, std::string& file_name);
    void write_str (std::wostringstream& ss, std::string& file_name);
    void load_template(std::wostringstream& ss);
    void write_w_template(std::wostringstream& str, std::wofstream f);
    void write_template(std::wostringstream& str, std::ofstream f);
};

#endif // COMPILER_H
