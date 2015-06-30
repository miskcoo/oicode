/* Codeforces 554B. Ohana Cleans Up
 *   brute force, strings */
#include <bitset>
#include <algorithm>
#include <iostream>

std::bitset<100> row[100];

int calc(int n, int r)
{
	int count = 0;
	for(int i = 0; i != n; ++i)
	{
		if((row[i] ^ row[r]).count() == 0)
			++count;
	}

	return count;
}

int main()
{
	int n;
	std::cin >> n;
	for(int i = 0; i != n; ++i)
		std::cin >> row[i];

	int ans = 0;
	for(int i = 0; i != n; ++i)
		ans = std::max(ans, calc(n, i));
	std::cout << ans << std::endl;
	return 0;
}
