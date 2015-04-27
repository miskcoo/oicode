/* BZOJ-3527: [Zjoi2014]力
 *   FFT */
#include <cstdio>
#include <complex>
#include <cmath>
#include <algorithm>
#include <iostream>

const int MaxL = 18, MaxN = 1 << MaxL;
typedef std::complex<double> complex_t;
complex_t f[MaxN], g[MaxN];
complex_t eps[MaxN], inv_eps[MaxN];

void init_eps(int p)
{
	double pi = acos(-1);
	double angle = 2.0 * pi / p;
	for(int i = 0; i != p; ++i)
		eps[i] = complex_t(cos(2.0 * pi * i / p), sin(2.0 * pi * i / p));
	for(int i = 0; i != p; ++i)
		inv_eps[i] = conj(eps[i]);
}

void transform(int n, complex_t *x, complex_t *w)
{
	for(int i = 0, j = 0; i != n; ++i)
	{
		if(i > j) std::swap(x[i], x[j]);
		for(int l = n >> 1; (j ^= l) < l; l >>= 1);
	}

	for(int i = 2; i <= n; i <<= 1)
	{
		int m = i >> 1;
		for(int j = 0; j < n; j += i)
		{
			for(int k = 0; k != m; ++k)
			{
				complex_t z = x[j + m + k] * w[n / i * k];
				x[j + m + k] = x[j + k] - z;
				x[j + k] += z;
			}
		}
	}
}

int main()
{
	int n;
	std::scanf("%d", &n);
	int l = 0, p = 1;
	while(p < n) ++l, p <<= 1;
	++l, p <<= 1;

	for(int i = 0; i != p; ++i)
		f[i] = g[i] = 0.0;

	for(int i = 0; i != n; ++i)
	{
		double x;
		std::scanf("%lf", &x);
		f[i] = x;
	}

	for(int i = 0; i + 1 < n; ++i)
	{
		g[i] = 1.0 / ((n - i - 1.0) * (n - i - 1.0));
		g[2 * n - i - 2] = -g[i];
	}

	init_eps(p);
	std::reverse(g, g + p);

	transform(p, f, eps);
	transform(p, g, eps);
	for(int i = 0; i != p; ++i)
		f[i] *= g[i];
	transform(p, f, inv_eps);
	for(int i = p - n; i != p; ++i)
		std::printf("%.3lf\n", f[i].real() / p);
	return 0;
}
