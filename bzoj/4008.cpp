/* BZOJ-4008: [HNOI2015]亚瑟王
 *   概率DP */
#include <cstdio>
#include <cstring>

const int MaxN = 250;
double p[MaxN], d[MaxN];
double f[MaxN][MaxN], pw[MaxN][MaxN];

void solve()
{
	int n, r;
	std::scanf("%d %d", &n, &r);
	for(int i = 0; i != n; ++i)
		std::scanf("%lf %lf", p + i, d + i);

	for(int i = 0; i != n; ++i)
	{
		pw[i][0] = 1.0;
		for(int j = 1; j <= r; ++j)
			pw[i][j] = pw[i][j - 1] * (1.0 - p[i]);
		for(int j = 0; j <= r; ++j)
			f[i][j] = 0.0;
	}

	double ans = 0.0;
	f[0][r] = 1.0;
	for(int i = 0; i != n; ++i)
	{
		for(int j = r - i < 0 ? 0 : r - i; j <= r; ++j)
		{
			f[i + 1][j] += f[i][j] * pw[i][j];
			if(j >= 1)
			{
				f[i + 1][j - 1] += f[i][j] * (1 - pw[i][j]);
				ans += f[i][j] * (1 - pw[i][j]) * d[i];
			}
		}
	}

	std::printf("%.10lf\n", ans);
}

int main()
{
	int t;
	std::scanf("%d", &t);
	while(t --> 0) solve();
	return 0;
}
