/* BZOJ-4161: Shlw loves matrixI
 *   Caylay-Hamilton定理，线性递推 */
#include <cstdio>
#include <algorithm>

typedef long long calc_t;
const int mod_v = 1000000007;
const int MaxN = 4010;
int n, base[MaxN], poly0[MaxN], poly1[MaxN], init_v[MaxN];

void subsititute(int *x, int pos)
{
	calc_t v = x[pos];
	if(!v) return;
	for(int i = 0; i != n; ++i)
		x[pos - i - 1] = (x[pos - i - 1] + base[i] * v) % mod_v;
}

void poly_mul(int *dest, int *x, int* y)
{
	static int tmp[MaxN];
	std::fill(tmp, tmp + 2 * n, 0);
	for(int i = 0; i != n; ++i)
		for(int j = 0; j != n; ++j)
			tmp[i + j] = (tmp[i + j] + (calc_t)x[i] * y[j]) % mod_v;
	for(int i = 2 * (n - 1); i >= n; --i)
		subsititute(tmp, i);
	std::copy(tmp, tmp + n, dest);
}

void poly_shift(int *x, int z)
{
	for(int i = 2 * (n - 1); i >= z; --i)
		x[i] = x[i - z];
	for(int i = 0; i != z; ++i) x[i] = 0;
	for(int i = 2 * (n - 1); i >= n; --i)
		subsititute(x, i);
}

void poly_pow(int *v, int *x, int p)
{
	std::fill(v, v + n, 0);
	v[0] = 1;
	while(p)
	{
		if(p & 1) poly_mul(v, x, v);
		if(p >> 1) poly_mul(x, x, x);
		p >>= 1;
	}
}

int main()
{
	int p;
	std::scanf("%d %d", &p, &n);
	for(int i = 0; i != n; ++i)
	{
		std::scanf("%d", base + i);
		base[i] = (base[i] % mod_v + mod_v) % mod_v;
	}

	for(int i = 0; i != n; ++i)
	{
		std::scanf("%d", init_v + i);
		init_v[i] = (init_v[i] % mod_v + mod_v) % mod_v;
	}

	if(p <= n) 
	{
		std::printf("%d\n", init_v[p - 1]);
	} else {
		std::reverse_copy(base, base + n, poly1);
		poly_pow(poly0, poly1, p / n);
		poly_shift(poly0, p % n);
		int ans = 0;
		for(int i = 0; i != n; ++i)
			ans = (ans + (calc_t)init_v[i] * poly0[i]) % mod_v;

		std::printf("%d\n", ans);
	}

	return 0;
}
