#include "array_t.h"

array_t::array_t()
{
}

void array_t::print(std::wostream& out)
{
    out << L"<array_t <of=";
    this->of->print(out);
    out << L">>";
}
