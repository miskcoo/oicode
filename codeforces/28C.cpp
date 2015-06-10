/* Codeforces 28C. Bath Queue
 *   组合计数，DP
 *   combinatorics, dp, probabilities */
#include <cstdio>
#include <algorithm>

const int MaxN = 55;
double C[MaxN << 1][MaxN << 1];
double f[MaxN][MaxN][MaxN];

double pow(double x, int p)
{
	double v = 1.0;
	for(; p; p >>= 1, x *= x)
		if(p & 1) v *= x;
	return v;
}

void init_comb(int n, int m)
{
	for(int i = 0; i <= n + m; ++i)
	{
		C[i][0] = C[i][i] = 1;
		for(int j = 1; j < i; ++j)
			C[i][j] = C[i - 1][j] + C[i - 1][j - 1];
	}
}

int main()
{
	int n, m;
	std::scanf("%d %d", &n, &m);
	init_comb(n, m);

	f[0][0][0] = 1;
	for(int i = 1; i <= m; ++i)
	{
		int basins;
		std::scanf("%d", &basins);
		f[i][0][0] = 1;

		for(int j = 1; j <= n; ++j)
		{
			for(int l = 0; l <= j; ++l)
			{
				int b = basins * l, a = basins * (l - 1) + 1;
				for(int k = 0; k <= l; ++k)
				{
					for(int p = std::max(a, 0); p <= b && p <= j; ++p)
						f[i][j][l] += f[i - 1][j - p][k] * C[n - j + p][p];
				}

				for(int p = 0; p < a && p <= j; ++p)
					f[i][j][l] += f[i - 1][j - p][l] * C[n - j + p][p];
			}
		}
	}

	double ans = 0;
	for(int i = 1; i <= n; ++i)
		ans += f[m][n][i] * i;
	std::printf("%.14lf\n", ans / pow(m, n));
	return 0;
}
