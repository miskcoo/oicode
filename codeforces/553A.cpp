/* Codeforces 553A. Kyoya and Colored Balls
 *   combinatorics, dp, math, number theory */
#include <cstdio>
#include <iostream>

const int mod_v = 1000000007;
const int MaxN = 2001;
long long C[MaxN][MaxN];

void init_comb(int n = 2000)
{
	for(int i = 0; i <= n; ++i)
	{
		C[i][0] = C[i][i] = 1;
		for(int j = 1; j < i; ++j)
			C[i][j] = (C[i - 1][j] + C[i - 1][j - 1]) % mod_v;
	}
}

int main()
{
	init_comb();

	int k;
	std::cin >> k;

	long long ans = 1, sum = 0;
	for(int i = 1; i <= k; ++i)
	{
		int v;
		std::cin >> v;
		int a = sum + 1, b = v - 1;
		ans = ans * C[a + b - 1][a - 1] % mod_v;
		sum += v;
	}

	std::cout << ans;
	return 0;
}
