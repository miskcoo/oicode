/* Codeforces 698C. LRU
 *  bitmasks, dp, probabilities */
#include <cstdio>
#include <cmath>

int n, k;
double p[22], f[1 << 21], ans[22];
const double eps = 1.e-9;

int main()
{
	int non_zero = 0;
	std::scanf("%d %d", &n, &k);
	for(int i = 0; i != n; ++i)
	{
		std::scanf("%lf", p + i);
		if(p[i] > eps) ++non_zero;
	}

	if(k > non_zero) k = non_zero;

	f[0] = 1;
	int tot = 1 << n;

	for(int s = 1; s != tot; ++s)
	{
		int sz = 0;
		double sum = 0;

		for(int i = 0; i != n; ++i)
		{
			if(s & (1 << i))
			{
				sum += p[i];
				++sz;
			}
		}

		if(sz > k) continue;
		for(int i = 0; i != n; ++i)
		{
			if((s & (1 << i)) && p[i] > eps)
				f[s] += f[s ^ (1 << i)] * p[i] / (1 - sum + p[i]);
		}

		if(sz == k)
		{
			for(int i = 0; i != n; ++i)
				if(s & (1 << i))
					ans[i] += f[s];
		}
	}

	for(int i = 0; i != n; ++i)
		std::printf("%.9lf ", ans[i]);

	return 0;
}
