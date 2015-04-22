/* BZOJ-3997: [TJOI2015]组合数学
 *  DP */
#include <cstdio>
#include <algorithm>

const int MaxN = 1001;
long long data[MaxN][MaxN], f[MaxN][MaxN];

void solve()
{
	int n, m;
	std::scanf("%d %d", &n, &m);
	for(int i = 1; i <= n; ++i)
		for(int j = 1; j <= m; ++j)
			std::scanf("%lld", data[i] + j);

	for(int i = 1; i <= n; ++i)
	{
		for(int j = m; j; --j)
		{
			long long t = std::max(data[i][j] + f[i - 1][j + 1], f[i - 1][j]);
			f[i][j] = std::max(t, f[i][j + 1]);
		}
	}

	std::printf("%lld\n", f[n][1]);
}

int main()
{
	int t;
	std::scanf("%d", &t);
	while(t --> 0)
		solve();
	return 0;
}
