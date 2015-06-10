/* Codeforces 9D. How many trees?
 *   组合计数，DP 
 *   combinatorics, divide and conquer, dp */
#include <cstdio>

const int MaxN = 40;
long long f[MaxN][MaxN], prefix[MaxN];

int main()
{
	int n, h;
	std::scanf("%d %d", &n, &h);
	f[0][0] = 1;
	for(int i = 1; i <= n; ++i) 
	{ 
		for(int j = 0; j <= n; ++j) 
		{
			for(int k = 0; k != j; ++k) 
			{
//				for(int p = 0; p < i - 1; ++p)
//					f[i][j] += 2 * f[i - 1][k] * f[p][j - k - 1];
				f[i][j] += 2 * f[i - 1][k] * prefix[j - k - 1];
				f[i][j] += f[i - 1][k] * f[i - 1][j - k - 1];
			}
		}

		for(int j = 0; j <= n; ++j)
			prefix[j] += f[i - 1][j];
	}

	long long ans = 0;
	for(int i = h; i <= n; ++i)
		ans += f[i][n];
	std::printf("%I64d\n", ans);
	return 0;
}
