/* Codeforces 37D. Lesson Timetable
 *   组合计数，DP
 *   combinatorics, dp, math */
#include <cstdio>
#include <algorithm>

typedef long long calc_t;
const int MaxN = 110, MaxS = 1010;
const int mod_v = 1000000007;
int sum[MaxN], C[MaxS][MaxS], f[MaxN][MaxS];

int inc(int x, int v) { x += v; if(x >= mod_v) x -= mod_v; return x; }
void inc0(int& x, int v) { x = inc(x, v); }

int main()
{
	int n;
	std::scanf("%d", &n);
	for(int i = 1; i <= n; ++i)
	{
		std::scanf("%d", sum + i);
		sum[i] += sum[i - 1];
	}

	for(int i = 0; i <= sum[n]; ++i)
	{
		C[i][0] = C[i][i] = 1;
		for(int j = 1; j < i; ++j)
			C[i][j] = inc(C[i - 1][j - 1], C[i - 1][j]);
	}

	f[0][0] = 1;
	for(int i = 0; i != n; ++i)
	{
		int y;
		std::scanf("%d", &y);
		for(int j = 0; j <= sum[i]; ++j)
		{
			int tot = sum[i + 1] - j;
			for(int k = 0; k <= std::min(tot, y); ++k)
				inc0(f[i + 1][j + k], (calc_t)f[i][j] * C[tot][k] % mod_v);
		}
	}

	int ans = f[n][sum[n]];
	for(int i = 1; i <= n; ++i)
		ans = (calc_t)ans * C[sum[n] - sum[i - 1]][sum[i] - sum[i - 1]] % mod_v;
	std::printf("%d\n", ans);
	return 0;
}
