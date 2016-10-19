#include "value_t.h"

int    value_stack_sizes[]  = {    1,     1,      2,      2,      4,      4,      4,      8,	   4,      8 };
schar* value_type_strings[] = { L"i8", L"u8", L"i16", L"u16", L"i32", L"u32", L"i64", L"u64", L"f32", L"f64" };

value_t::value_t(value_type t) : itype(value_stack_sizes[(int)t])
{
	this->value = t;
}

void value_t::print(std::wostream& out)
{
	out << value_type_strings[(int)this->value];
}

bool value_t::operator==(const itype& other) const
{
	if (value_t const* p = dynamic_cast<value_t const*>(&other))
		return this->value == p->value;
	else
		return false;
}
