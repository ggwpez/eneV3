#include "il.h"
#include "errors/warnings.h"
#include "errors/errors.hpp"

il::il(ProgramNode *code, std::wostringstream* ss)
{
	input = code;
	this->ss = ss;

	ss_code  = new std::wostringstream();
	ss_codeh = new std::wostringstream();
	ss_data  = new std::wostringstream();
	ss_bss   = new std::wostringstream();


	funtion_returns = std::stack<schar*>();
	registered_strings = std::unordered_map<schar*, std::wstring*>();
	str_c = brk_c = sml_c = grt_c = ssp_c = anym_c = if_c = while_c = 0;
}

il::~il()
{
	for (std::pair<schar*, std::wstring*> i : this->registered_strings)
		delete i.second;

	delete ss_code;
	delete ss_codeh;
	delete ss_data;
	delete ss_bss;
}

struct pusher
{
	pusher(int t, std::wstring obj);
	int t;
	std::wstring obj;
};

pusher::pusher(int t, std::wstring obj)
{
	this->t = t;
	this->obj = obj;
}

/*
 * push a
 * push b
 * push c
 *
 * pop eax
 * pop ecx
 * add ecx
 * push eax
 *
 * pop eax
 * pop ecx
 * add ecx
 * push eax
 *
 * a b c ++;
 */

bool rbx_free = true;
std::stack<pusher> locations;
void il::acum_push(std::wstring str)
{
	/*if (rbx_free)
	{
		*ss_code << L"mov " << rbx << L", " << str << std::endl;
		rbx_free = false;
		locations.push(pusher(1, str));
	}
	else
	{
		//eml(L"push " << str);
		locations.push(pusher(2, str));              //could be immediate push
	}*/
	*ss_code << L"push " << str << std::endl;
}

void il::acum_pop(std::wstring str)
{
	/*if (locations.top().t == 2)
	{
		*ss_code << L"mov " << str << L", " << locations.top().obj << std::endl;
	}
	else if (locations.top().t == 1)
	{
		*ss_code << L"mov " << str << L", " << rbx << std::endl;
		rbx_free = true;
	}
	else
		*ss_code << L"pop  " << str << std::endl;

	locations.pop();*/
	*ss_code << L"pop  " << str << std::endl;
}

void il::acum_non_immediate()
{
	/*if (locations.size() && locations.top().t == 2)
	{
		if (rbx_free)
		{
			*ss_code << L"mov " << rbx << L", " << locations.top().obj << std::endl;
			rbx_free = false;
			locations.top().t = 1;
		}
		else
		{
			*ss_code << L"push " << locations.top().obj << std::endl;
			locations.top().t = 0;
		}
	}*/
}

void il::generate()
{
	il::generate_output_init();
	initalize_streams();
	generate(input);
	il::generate_output_end();
	finalize_streams();
}

void il::generate_sf_enter(int size)
{
	std::wstring ebp = __BITS__ == 16 ? L"bp" : __BITS__ == 32 ? L"ebp" : L"rbp";
	std::wstring esp = __BITS__ == 16 ? L"sp" : __BITS__ == 32 ? L"esp" : L"rsp";

	eml(L"push " << ebp << std::endl <<
		L"mov " << ebp << L", " << esp << std::endl);
	if (size)
		eml(L"sub " << esp << L", " << size);
}

void il::generate_sf_leave(int size)
{
	std::wstring ebp = __BITS__ == 16 ? L"bp" : __BITS__ == 32 ? L"ebp" : L"rbp";
	std::wstring esp = __BITS__ == 16 ? L"sp" : __BITS__ == 32 ? L"esp" : L"rsp";

	if (size)
		eml(L"add " << esp << L", " << size << std::endl);
	eml(L"mov " << esp << L", " << ebp << std::endl <<
		L"pop " << ebp << std::endl <<
		L"ret ");
}

void il::generate_output_init()
{
	eml(std::endl << std::endl <<
		get_cc() << L"** generated by the ene compiler v3.1 " << std::endl <<
		get_cc() << L"** build on " << __DATE__ << L' ' << __TIME__ << std::endl << std::endl);
}

void il::generate_output_end()
{

}

std::wstring* il::generate_string_name(schar* content, bool& already_registered)
{
	auto it = this->registered_strings.find(content);

	if (it == registered_strings.end())
	{
		std::wstring tmp = std::wstring(L"__str_") +std::to_wstring(++str_c);
		std::wstring* to_add = new std::wstring(tmp);
		registered_strings[content] = to_add;

		already_registered = false;
		return to_add;
	}
	else
		return already_registered = true, (*it).second;
}

void il::generate_global(VariableNode *code)
{
	if (! code->type->t->size)
		WAR(war_t::INSTANCE_OF_VOID, code);
}

void il::generate(OperatorNode* code)
{
	switch (code->oper)
	{
		case op::ADD:
			generate_op_add(code);
			break;
		case op::SUB:
			generate_op_sub(code);
			break;
		case op::MUL:
			generate_op_mul(code);
			break;
		case op::DIV:
			generate_op_div(code);
			break;
		case op::DRF:
			generate_op_drf(code);
			break;
		case op::EQU:
			generate_op_equ(code);
			break;
		case op::SML:
			generate_op_sml(code);
			break;
		case op::GRT:
			generate_op_grt(code);
			break;
		case op::NOT:
			generate_op_not(code);
			break;
		case op::POP:
			generate_op_pop(code);
			break;
		case op::CPY:
			generate_op_cpy(code);
			break;
		case op::OR:
			generate_op_or(code);
			break;
		case op::AND:
			generate_op_and(code);
			break;
		default:
			ERR(err_t::GEN_IL);
			break;
	}
};
