/* Codeforces 554A. Kyoya and Photobooks
 *   brute force, math */
#include <string>
#include <iostream>
#include <unordered_set>

int main()
{
	std::unordered_set<std::string> s0;
	std::string str;
	std::cin >> str;

	for(char c = 'a'; c <= 'z'; ++c)
	{
		for(size_t i = 0; i <= str.size(); ++i)
		{
			std::string str0 = str;
			str0.insert(str0.begin() + i, c);
			s0.insert(str0);
		}
	}

	std::cout << s0.size();
	return 0;
}
