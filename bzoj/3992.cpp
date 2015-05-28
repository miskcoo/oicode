/* BZOJ-3992: [SDOI2015]序列统计
 *   FFT+原根 */
#include <cstdio>
#include <cmath>
#include <algorithm>

using std::swap;

typedef int value_t;
typedef long long calc_t;
const int MaxN = 1 << 15;
const value_t mod_base = 479, mod_exp = 21;
const value_t mod_v = (mod_base << mod_exp) + 1;
const value_t mod_primitive_root = 3;
int epsilon_num;
value_t eps[MaxN], inv_eps[MaxN];

value_t dec(value_t x, value_t v) { x -= v; return x < 0 ? x + mod_v : x; }
value_t inc(value_t x, value_t v) { x += v; return x >= mod_v ? x - mod_v : x; }
value_t pow(value_t x, value_t p, value_t m = mod_v)
{
	value_t v = 1;
	for(; p; p >>= 1, x = (calc_t)x * x % m)
		if(p & 1) v = (calc_t)x * v % m;
	return v;
}

void init_eps(int num)
{
	epsilon_num = num;
	value_t base = pow(mod_primitive_root, (mod_v - 1) / num);
	value_t inv_base = pow(base, mod_v - 2);
	eps[0] = inv_eps[0] = 1;
	for(int i = 1; i != num; ++i)
	{
		eps[i] = (calc_t)eps[i - 1] * base % mod_v;
		inv_eps[i] = (calc_t)inv_eps[i - 1] * inv_base % mod_v;
	}
}

void transform(int n, value_t *x, value_t *w = eps)
{
	for(int i = 0, j = 0; i != n; ++i)
	{
		if(i > j) swap(x[i], x[j]);
		for(int l = n >> 1; (j ^= l) < l; l >>= 1);
	}

	for(int i = 2; i <= n; i <<= 1)
	{
		int m = i >> 1, t = epsilon_num / i;
		for(int j = 0; j < n; j += i)
		{
			for(int p = 0, q = 0; p != m; ++p, q += t)
			{
				value_t z = (calc_t)x[j + m + p] * w[q] % mod_v;
				x[j + m + p] = dec(x[j + p], z);
				x[j + p] = inc(x[j + p], z);
			}
		}
	}
}

void inverse_transform(int n, value_t *x)
{
	transform(n, x, inv_eps);
	value_t inv = pow(n, mod_v - 2);
	for(int i = 0; i != n; ++i)
		x[i] = (calc_t)x[i] * inv % mod_v;
}

int m;
value_t A[MaxN], B[MaxN], C[MaxN];

void multiply(value_t* A, value_t* B)
{
	int p = 1;
	for(; p <= m << 1; p <<= 1);
	transform(p, A);
	if(A != B) transform(p, B);
	for(int i = 0; i != p; ++i)
		A[i] = (calc_t)A[i] * B[i] % mod_v;
	inverse_transform(p, A);
	if(A != B) inverse_transform(p, B);
	for(int i = m - 1; i != p; A[i++] = 0)
		A[i - (m - 1)] = inc(A[i - (m - 1)], A[i]);
}

void power(int p)
{
	B[0] = 1;
	while(p)
	{
		if(p & 1) multiply(B, A);
		if(p >> 1) multiply(A, A);
		p >>= 1;
	}
}

bool check(int x, int g)
{
	int z = x - 1, t = int(sqrt(z) + 1);
	for(int i = 2; i <= t; ++i)
	{
		if(z % i) continue;
		if(pow(g, (x - 1) / i, x) == 1)
			return false;
		while(z % i == 0) z /= i;
	}

	if(z != 1 && pow(g, (x - 1) / z, x) == 1)
		return false;
	return true;
}

int primitive_root(int x)
{
	for(int g = 2; ; ++g)
		if(check(x, g)) return g;
	return -1;
}

int ind[MaxN];

int main()
{
	int n, x, s;
	init_eps(1 << 15);
	std::scanf("%d %d %d %d", &n, &m, &x, &s);
	int g = primitive_root(m);
	ind[1] = 0;
	for(int i = 1, v = 1; i != m - 1; ++i)
	{
		v = (calc_t)v * g % m;
		ind[v] = i;
	}

	for(int i = 0; i != s; ++i)
	{
		int v;
		std::scanf("%d", &v);
		if(!v) continue;
		A[ind[v]] = 1;
	}

	power(n);
	printf("%d", B[ind[x]]);
	return 0;
}
