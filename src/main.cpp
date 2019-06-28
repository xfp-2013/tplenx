#include <stdio.h>

#include <pantor/inja.hpp>

using namespace	inja;
using namespace	std;
using json	= nlohmann::json;

extern char	**environ;
FILE*	inn	= stdin;
FILE*	out	= stdout;


int _read(char* p, size_t n) {
	return	fread((void*)p, 1, n, stdin);
}
int _write(char* p, size_t n) {
	return	fwrite((void*)p, 1, n, stdout);
}

template<typename sfun>
void _split_string(const std::string &str, char delimiter, sfun f) {
	size_t	frm	= 0;
	bool	r_	= true;
	for (std::size_t i=0; i<str.size(); ++i) {
		if (str[i] == delimiter) {
			r_	= f(str, frm, i);
			frm	= i + 1;
			if (r_ == false)
				break;
		}
	}
	if (r_ == true && frm <= str.size())
		f(str, frm, str.size());
}


Environment	env;

int main(int argc, char*argv[]) {

	json	js;
	string	inn;

	{
		istreambuf_iterator<char> begin(cin), end;
		inn	= string(begin, end);
	}
	{
		char** env	= environ;
		while (*env) {
			_split_string(*env++, '=', [&js](const string &s, size_t from, size_t to) -> bool {
				js[s.substr(from, to - from)]	= s.substr(to+1);
				return	false;
			});
		}
	}
	{
		cout	<< env.render(inn.c_str(), js);
	}

	return	0;
}
