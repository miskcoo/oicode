/* Codeforces 621A. Wet Shark and Odd and Even
 *   implementation */
#include <cstdio>
#include <iostream>

int main()
{
	int n, min_odd = ~0u >> 1;
	long long sum = 0;
	std::cin >> n;
	for(int i = 0; i != n; ++i)
	{
		int v;
		std::cin >> v;
		if((v & 1) && v < min_odd)
			min_odd = v;
		sum += v;
	}

	if(sum & 1) sum -= min_odd;
	std::cout << sum << std::endl;
	return 0;
}
