/* BZOJ-2194: 快速傅立叶之二
 *   FFT */
#include <cstdio>
#include <cmath>
#include <complex>
#include <algorithm>

const int MaxL = 18, MaxN = 1 << MaxL;
typedef std::complex<double> complex_t;
complex_t eps[MaxL], eps_inv[MaxL], A[MaxN], B[MaxN];

void init_eps()
{
	double pi = acos(-1), angle = 2.0 * pi / (1 << MaxL);
	eps[MaxL - 1] = complex_t(cos(angle), sin(angle));
	for(int i = MaxL - 2; i >= 0; --i)
		eps[i] = eps[i + 1] * eps[i + 1];
	for(int i = 0; i != MaxL; ++i)
		eps_inv[i] = conj(eps[i]);
}

void transform(int n, complex_t *z, complex_t *w)
{
	for(int i = 0, j = 0; i != n; ++i)
	{
		if(i > j) std::swap(z[i], z[j]);
		for(int l = n >> 1; (j ^= l) < l; l >>= 1);
	}

	for(int i = 2; i <= n; i <<= 1, ++w)
	{
		int m = i >> 1;
		complex_t *e = new complex_t[m];
		e[0] = 1.0;
		for(int j = 1; j != m; ++j)
			e[j] = e[j - 1] * *w;
		for(int j = 0; j < n; j += i)
		{
			for(int p = 0; p != m; ++p)
			{
				complex_t x = z[j + m + p] * e[p];
				z[j + m + p] = z[j + p] - x;
				z[j + p] += x;
			}
		}
		delete[] e;
	}
}

int main()
{
	int n;
	std::scanf("%d", &n);
	init_eps();
	for(int i = 0; i != n; ++i)
	{
		double a, b;
		std::scanf("%lf %lf", &a, &b);
		A[i] = a, B[i] = b;
	}

	int p = 1;
	while(p < n) p <<= 1;
	p <<= 1;
	std::reverse(A, A + n);
	transform(p, A, eps);
	transform(p, B, eps);
	for(int i = 0; i != p; ++i)
		A[i] *= B[i];
	transform(p, A, eps_inv);
	for(int i = 0; i != n; ++i)
		std::printf("%d\n", int(A[n - i - 1].real() / p + 0.5));
	return 0;
}
