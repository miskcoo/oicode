/* Codeforces 26D. Tickets
 *   组合计数（Relection Principle），DP 
 *   combinatorics, probabilities */
#include <cstdio>
#include <algorithm>

//const int MaxN = 1001;
//double f[MaxN][MaxN];

int main()
{
	int n, m, k;
	std::scanf("%d %d %d", &n, &m, &k);
//	f[0][0] = 1;
	/*
	for(int i = 0; i <= n; ++i)
	{
		for(int j = (i == 0); j <= std::min(m, i + k); ++j)
		{
			if(j) f[i][j] += (m - j + 1.0) / (n + m - i - j + 1.0) * f[i][j - 1];
			if(i && i - 1 + k >= j)
				f[i][j] += (n - i + 1.0) / (n + m - i - j + 1.0) * f[i - 1][j];
		}
	} 
	std::printf("%lf\n", f[n][m]);
	*/

	/*
	for(int p = 1; p <= n + m; ++p)
	{
		// j = p - i, i + k >= j --> 2i + k >= p --> i >= (p - k) / 2
		double d = 1.0 / (n + m - p + 1);
		for(int i = std::max((p - k + 1) >> 1, 0); i <= std::min(p, n); ++i)
			f[p][i] += (m - (p - i) + 1.0) * d * f[p - 1][i];

		for(int i = std::max((p - k + 2) >> 1, 1); i <= std::min(p, n); ++i)
			f[p][i] += (n - i + 1.0) * d * f[p - 1][i - 1];
	}

	std::printf("%lf\n", f[n + m][n]); */

	if(m > n + k) std::puts("0");
	else if(m <= k) std::puts("1");
	else {
		double ans = 1.0;
		for(int i = 0; i <= k; ++i)
			ans *= (m - i) / (n + i + 1.0);
		std::printf("%lf\n", 1.0 - ans);
	}
	return 0;
}
