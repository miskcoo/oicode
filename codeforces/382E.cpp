/* Codeforces 382E. Ksenia and Combinatorics
 *   组合计数，树DP，DP套DP 
 *   combinatorics, dp */
#include <iostream>

typedef long long calc_t;
const int MaxN = 52, mod_v = 1000000007;
calc_t f[MaxN][MaxN >> 1][2], C[MaxN][MaxN];

void init_comb(int n)
{
	for(int i = 0; i <= n; ++i)
	{
		C[i][i] = C[i][0] = 1;
		for(int j = 1; j < i; ++j)
		{
			C[i][j] = C[i - 1][j] + C[i - 1][j - 1];
			if(C[i][j] >= mod_v) C[i][j] -= mod_v;
		}
	}
}

int main()
{
	int n0, k0;
	std::cin >> n0 >> k0;
	if(k0 << 1 > n0)
	{
		std::cout << "0\n";
		return 0;
	}

	init_comb(n0);
	f[1][0][0] = 1; f[0][0][1] = 1;
	for(int n = 2; n <= n0; ++n)
	{
		for(int k = 1; k <= n >> 1; ++k)
		{
			for(int g = 0; g <= 1; ++g)
			{
				calc_t& val = f[n][k][g];
				for(int n1 = 0, n2 = n - 1; n1 <= n2; ++n1, --n2)
				{
					calc_t ways;
					if(n1 != n2) ways = C[n - 1][n1];
					else ways = C[n - 2][n1 - 1];
					if(n1) ways = ways * n1 * n2 % mod_v;
					else ways = ways * n2 % mod_v;

					for(int k1 = 0; k1 + g <= k; ++k1)
					{
						int k2 = k - k1 - g;
						if(g == 0) 
						{
							val += f[n1][k1][1] * f[n2][k2][1] % mod_v * ways % mod_v;
						} else {
							val += (f[n1][k1][1] * f[n2][k2][0]
								 + f[n1][k1][0] * f[n2][k2][1]
								 + f[n1][k1][0] * f[n2][k2][0]) % mod_v * ways % mod_v;
						}
					}

					val %= mod_v;
				}
			}
		}
	}

	std::cout << (f[n0][k0][0] + f[n0][k0][1]) % mod_v << std::endl;
	return 0;
}
