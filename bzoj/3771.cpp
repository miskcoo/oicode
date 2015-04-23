/* BZOJ-3771: Triple
 *   母函数+FFT */
#include <cstdio>
#include <cmath>
#include <complex>
#include <algorithm>
#include <iostream>

typedef std::complex<double> complex_t;

void shuffle(int n, complex_t *x)
{
	for(int i = 0, j = 0; i != n; ++i)
	{
		if(i > j) std::swap(x[i], x[j]);
		for(int l = n >> 1; (j ^= l) < l; l >>= 1);
	}
}

void transform(int n, complex_t *x, complex_t *w)
{
	shuffle(n, x);
	for(int i = 2; i <= n; i <<= 1, ++w)
	{
		int m = i >> 1;
		complex_t *tmp = new complex_t[m];
		tmp[0] = 1;
		for(int p = 1; p != m; ++p)
			tmp[p] = tmp[p - 1] * *w;

		for(int p = 0; p < n; p += i)
		{
			for(int t = 0; t != m; ++t)
			{
				complex_t z = x[p + m + t] * tmp[t];
				x[p + m + t] = x[p + t] - z;
				x[p + t] += z;
			}
		}

		delete[] tmp;
	}
}

const int MaxL = 17, MaxN = (1 << MaxL) + 10;
complex_t eps[MaxL + 1], eps_inv[MaxL + 1];
complex_t A[MaxN], B[MaxN], C[MaxN];
void init_eps()
{
	double pi = std::acos(-1.0);
	double angle = 2.0 * pi / (1 << MaxL);
	eps[MaxL] = complex_t(std::cos(angle), std::sin(angle));
	for(int i = MaxL - 1; i >= 0; --i)
		eps[i] = eps[i + 1] * eps[i + 1];
	for(int i = 0; i <= MaxL; ++i)
		eps_inv[i] = 1.0 / eps[i];
}

int main()
{
	init_eps();
	int n, max_v = 0;
	std::scanf("%d", &n);
	for(int i = 0; i != n; ++i)
	{
		int v;
		std::scanf("%d", &v);
		A[v] += 1.0;
		B[v * 2] += 1.0;
		C[v * 3] += 1.0;
		if(v * 3 > max_v) max_v = v * 3;
	}

	int m = 1;
	while(m < max_v) m <<= 1;
	transform(m, A, eps + 1);
	transform(m, B, eps + 1);
	for(int i = 0; i != m; ++i)
		A[i] = A[i] * A[i] * A[i] / 6.0 + (A[i] * (A[i] - B[i]) - B[i]) / 2.0 + A[i];
	transform(m, A, eps_inv + 1);
	for(int i = 0; i != m; ++i)
		A[i] = A[i] / double(m) + C[i] / 3.0;
	for(int i = 0; i != m; ++i)
	{
		int x = int(A[i].real() + 0.5);
		if(x) std::printf("%d %d\n", i, x);
	}
	return 0;
}
