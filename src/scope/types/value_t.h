#ifndef VALUE_T_H
#define VALUE_T_H

#include "itype.h"
#include "string_def.h"

extern schar* value_type_strings[];
extern schar* value_c_type_strings[];

enum class value_type : int
{
	I8 = 0,
	U8,
	I16,
	U16,
	I32,
	U32,
	I64,
	U64,
	F32,
	F64,
	size
};

class value_t : public itype
{
public:
	value_t(value_type t);

	void print(std::wostream& out) override;
	bool operator==(const itype& other) const override;

	value_type value;
};

#endif // VALUE_T_H
