#include "nasm_optimizer.hpp"

#include <stack>
#include <numeric>

std::wstring nasm_optimizer::optimize(unsigned level)
{
	bool changed(level);
	while (changed)
		optimize_h(changed);

	std::wostringstream output;
	for (std::wstring const& s : lines)
		output << s;

	return output.str();
}

bool starts_with(std::wstring const& a, std::wstring const& b)
{
	return a.compare(0, b.length(), b);//(a.substr(0, b.length()) == b);
}

std::stack<std::wstring> stack;
void dump_stack(std::vector<std::wstring>& output)
{
	while (! stack.empty())
		output.push_back(stack.top()), stack.pop();
}

void nasm_optimizer::optimize_h(bool& changed)
{
	std::vector<std::wstring> output;

	for (std::size_t i = 0; i < lines.size(); ++i)
	{
		std::wstring& line = lines[i];

		if (starts_with(line, L"push "))
		{
			stack.push(line.substr(0, 5));
		}
		else if (starts_with(line, L"pop "))
		{
			std::wstring dest = line.substr(4);
			output.push_back(L"mov " +dest + stack.top());
			stack.pop();
		}
		else if (starts_with(line, L"mov"))
		{

		}
		else if (starts_with(line, L"call") || starts_with(line, L"j"))
		{
			dump_stack(output);
			output.push_back(line);
		}
		else
			output.push_back(line);
	}

	lines = output;		// move
}
