/* BZOJ-1076: [SCOI2008]奖励关
 *  概率DP */
#include <cstdio>
#include <algorithm>

int p[20], value[20], cond[20];
double dp[2][65536];
double* f = dp[0], *g = dp[1];

int main()
{
	for(int i = 0; i != 20; ++i)
		p[i] = 1 << i;
	int N, K;
	std::scanf("%d %d", &N, &K);
	for(int i = 0; i != K; ++i)
	{
		int c;
		std::scanf("%d", value + i);
		std::scanf("%d", &c);
		while(c)
		{
			cond[i] |= p[c - 1];
			std::scanf("%d", &c);
		}
	}

	for(int i = N - 1; i >= 0; --i)
	{
		for(int k = 0; k != p[K]; ++k)
		{
			f[k] = 0.0;
			for(int j = 0; j != K; ++j)
			{
				if((cond[j] & k) == cond[j])
				{
					f[k] += std::max(g[k], 
						g[k | p[j]] + value[j]);
				} else f[k] += g[k];
			}

			f[k] /= K;
		}
		std::swap(f, g);
	}

	std::printf("%.6lf", g[0]);
	return 0;
}
