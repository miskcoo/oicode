/* Codeforces 396A. On Number of Decompositions into Multipliers
 *   combinatorics, number theory */
#include <cstdio>
#include <cmath>
#include <map>

typedef long long calc_t;
const int mod_v = 1000000007;

int pow(int x, int p)
{
	int v = 1;
	for(; p; p >>= 1, x = (calc_t)x * x % mod_v)
		if(p & 1) v = (calc_t)x * v % mod_v;
	return v;
}

int comb(int n, int m)
{
	int ans = 1;
	for(int i = 0; i != m; ++i)
		ans = (calc_t)ans * (n - i) % mod_v * pow(i + 1, mod_v - 2) % mod_v;
	return ans;
}

int main()
{
	int n;
	std::scanf("%d", &n);
	std::map<int, int> fact;
	for(int i = 0; i != n; ++i)
	{
		int v, t;
		std::scanf("%d", &v);
		t = sqrt(v + 1.0);
		for(int j = 2; v != 1 && j <= t; ++j)
		{
			while(v % j == 0)
			{
				++fact[j];
				v /= j;
			}
		}

		if(v != 1) ++fact[v];
	}

	int ans = 1;
	for(auto x : fact)
		ans = (calc_t)ans * comb(x.second + n - 1, n - 1) % mod_v;
	std::printf("%d\n", ans);
	return 0;
}
