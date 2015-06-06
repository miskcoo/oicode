/* BZOJ-2318: Spoj4060 game with probability Problem
 *   概率+等比数列 */
#include <algorithm>
#include <cstdio>
#include <cstring>

double f[500], g[500];
int main()
{
	int t;
	std::scanf("%d", &t);
	f[0] = 0, g[0] = 1;

	while(t --> 0)
	{
		int n;
		double np, nq;
		std::scanf("%d %lf %lf", &n, &np, &nq);
		if(n > 400) n = 400;

		for(int i = 1; i <= n; ++i)
		{
			double p = np, q = nq;
			if(f[i - 1] > g[i - 1])
				p = 1 - p, q = 1 - q;
			double r = 1 - (1 - p) * (1 - q);
			f[i] = (p * g[i - 1] + (1 - p) * q * f[i - 1]) / r;
			g[i] = (q * f[i - 1] + (1 - q) * p * g[i - 1]) / r;
		}

		std::printf("%.6lf\n", f[n]);
	}
	return 0;
}
