/* Codeforces 559B. Equivalent Strings
 *  divide and conquer, hashing, sortings, strings */
#include <string>
#include <iostream>

std::string calc(const std::string& str)
{
	int len = str.length();
	if(len & 1) return str;
	len >>= 1;
	std::string left(str.begin(), str.begin() + len);
	std::string right(str.begin() + len, str.end());
	left = calc(left), right = calc(right);
	if(left < right) return left + right;
	else return right + left;
}

int main()
{
	std::string A, B;
	std::cin >> A >> B;
	if(calc(A) == calc(B)) std::cout << "YES";
	else std::cout << "NO";
	return 0;
}
