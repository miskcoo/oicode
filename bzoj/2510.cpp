/* BZOJ-2510: 弱题
 *   循环矩阵乘法+快速幂 */
#include <cstdio>

const int MaxN = 1000;
int msize;
struct matrix_t
{
	double m[MaxN];

	matrix_t operator * (const matrix_t& r) const
	{
		matrix_t z;
		for(int j = 0; j != msize; ++j)
		{
			double v = 0;
			for(int k = 0; k != msize; ++k)
				v += m[k] * r.m[(j - k + msize) % msize];
			z.m[j] = v;
		} 

		return z;
	}
} one, m0;

matrix_t pow(matrix_t x, int p)
{
	matrix_t v = one;
	while(p)
	{
		if(p & 1) v = x * v;
		if(p >> 1) x = x * x;
		p >>= 1;
	}

	return v;
}

double ans[MaxN];

int main()
{
	one.m[0] = 1;

	int n, m, k;
	std::scanf("%d %d %d", &n, &m, &k);
	msize = n;
	m0.m[0] = 1.0 - 1.0 / m;
	m0.m[n - 1] = 1.0 / m;
	m0 = pow(m0, k);

	for(int i = 0; i != n; ++i)
	{
		int x;
		std::scanf("%d", &x);
		for(int j = 0; j != n; ++j)
			ans[(j + i) % n] += x * m0.m[(n - j) % n];
	}

	for(int i = 0; i != n; ++i)
		std::printf("%.3lf\n", ans[i]);
	return 0;
}

