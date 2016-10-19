#ifndef NASM_OPTIMIZER_HPP
#define NASM_OPTIMIZER_HPP

#include <string>
#include <vector>
#include <ostream>
#include <istream>
#include <sstream>

std::vector<std::wstring> split_string(const std::wstring& str,
									  const std::wstring& delimiter)
{
	std::vector<std::wstring> strings;

	std::string::size_type pos = 0;
	std::string::size_type prev = 0;
	while ((pos = str.find(delimiter, prev)) != std::wstring::npos)
	{
		strings.push_back(str.substr(prev, pos - prev));
		prev = pos + 1;
	}

	// To get the last substring (or only, if delimiter is not found)
	strings.push_back(str.substr(prev));

	return strings;
}

class nasm_optimizer
{
public:
	nasm_optimizer(std::wstring code)
		: lines(split_string(code, L"\n"))
	{ }

	std::wstring optimize(unsigned level);

private:
	void optimize_h(bool& changed);

	std::vector<std::wstring> lines;
};

#endif // OPTIMIZER_HPP
