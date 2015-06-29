/* Codeforces 553B. Kyoya and Permutation
 *   binary search, combinatorics,
 *   constructive algorithms, 
 *   implementation, math   */
#include <iostream>

const int MaxN = 51;
long long fib[MaxN];

int main()
{
	int n;
	long long k;
	std::cin >> n >> k;
	fib[0] = 0, fib[1] = 1;
	for(int i = 2; i <= n; ++i)
		fib[i] = fib[i - 1] + fib[i - 2];

	--k;
	int base = 0;
	while(n > 0)
	{
		if(k < fib[n])
		{
			std::cout << base + 1 << " ";
			n -= 1, base += 1;
		} else {
			k -= fib[n];
			std::cout << base + 2 << " " << base + 1 << " ";
			n -= 2, base += 2;
		}
	}

	return 0;
}
