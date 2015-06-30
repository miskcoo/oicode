/* Codeforces 552E. Vanya and Brackets
 *   brute force, dp, expression parsing, greedy, implementation */
#include <string>
#include <algorithm>
#include <cctype>
#include <iostream>

int pos[17];
std::string str, expr;
std::string::iterator now;

long long factor1();
long long atom()
{
	long long ans = 0;
	if(*now == '(')
	{
		++now;
		ans = factor1();
		++now;
	} else {
		ans = *now++ - '0';
	}

	return ans;
}

long long factor2()
{
	long long v = atom();
	while(now != expr.end() && *now == '*')
	{
		++now;
		v *= atom();
	}

	return v;
}

long long factor1()
{
	long long v = factor2();
	while(now != expr.end() && *now == '+')
	{
		++now;
		v += factor2();
	}

	return v;
}

long long eval()
{
	now = expr.begin();
	return factor1();
}

int main()
{
	int tot = 0;
	std::cin >> str;
	for(size_t i = 0; i != str.length(); ++i)
		if(str[i] == '*') pos[tot++] = i;

	expr = str;
	long long ans = eval();
	for(int i = 0; i != tot; ++i)
	{
		expr = "(" + str;
		expr.insert(expr.begin() + pos[i] + 1, ')');
		ans = std::max(ans, eval());
	}

	for(int i = 0; i != tot; ++i)
	{
		expr = str + ")";
		expr.insert(expr.begin() + pos[i] + 1, '(');
		ans = std::max(ans, eval());
	}

	for(int i = 0; i != tot; ++i)
	{
		for(int j = i + 1; j != tot; ++j)
		{
			expr = str;
			expr.insert(expr.begin() + pos[j], ')');
			expr.insert(expr.begin() + pos[i] + 1, '(');
			ans = std::max(ans, eval());
		}
	}

	std::cout << ans << std::endl;
	return 0;
}
