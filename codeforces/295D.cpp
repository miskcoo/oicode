/* Codeforces 295D. Greg and Caves
 *   dp, combinatorics, */
#include <cstdio>

const long long mod_v = 1000000007;
const int MaxN = 2001;
long long f[MaxN][MaxN];

int main()
{
	int n, m;
	std::scanf("%d %d", &n, &m);
	for(int i = 1; i <= n; ++i)
	{
		long long s1 = 0, s2 = 0;
		for(int j = 2; j <= m; ++j)
		{
			s1 = (s1 + f[i - 1][j]) % mod_v;
			s2 = (s2 + j * f[i - 1][j]) % mod_v;
			f[i][j] = (1 + (j + 1) * s1 - s2) % mod_v;
		}
	}

	long long ans = 0;
	for(int i = 1; i <= n; ++i)
		for(int j = 2; j <= m; ++j)
			ans = (ans + (m - j + 1) * f[i][j] % mod_v * (f[n - i + 1][j] - f[n - i][j])) % mod_v;

	if(ans < 0) ans += mod_v;
	std::printf("%d\n", (int)ans);

	return 0;
}
