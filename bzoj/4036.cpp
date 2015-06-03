/* BZOJ-4036: [HAOI2015]Set
 *   集合多项式+Mobius反演 */
#include <cstdio>
#include <cmath>

const double eps = 1.0e-8;
const int MaxN = 20;
double p[1 << MaxN];

int main()
{
	int n, m, sum = 0;
	std::scanf("%d", &n);
	m = 1 << n;
	for(int i = 0; i != m; ++i)
	{
		std::scanf("%lf", p + i);
		if(p[i] > eps) sum |= i;
	}

	if(sum != m - 1) 
	{
		std::puts("INF");
		return 0;
	}

	for(int i = 1; i < m; i <<= 1)
		for(int j = 0; j != m; ++j)
			if(i & j) p[j] += p[i ^ j];

	for(int i = 0; i != m; ++i)
	{
		if(fabs(p[i] - 1.0) < eps) p[i] = 0;
		else p[i] = 1.0 / (p[i] - 1.0);
	}

	for(int i = 1; i < m; i <<= 1)
		for(int j = 0; j != m; ++j)
			if(i & j) p[j] -= p[i ^ j];

	std::printf("%.10lf", p[m - 1]);
	return 0;
}
