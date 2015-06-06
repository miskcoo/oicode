/* BZOJ-3684: 大朋友和多叉树
 *   FFT+多项式求逆、对数、指数+Lagrange反演 */
#include <cstdio>
#include <algorithm>

using std::swap;
using std::fill;
using std::copy;

typedef int value_t;
typedef long long calc_t;
const int MaxN = 1 << 20;
const value_t mod_base = 453, mod_exp = 21;
const value_t mod_v = (mod_base << mod_exp) + 1;
const value_t primitive_root = 7;
int epsilon_num;
value_t eps[MaxN], inv_eps[MaxN], inv[MaxN];

value_t dec(value_t x, value_t v) { x -= v; return x < 0 ? x + mod_v : x; }
value_t inc(value_t x, value_t v) { x += v; return x >= mod_v ? x - mod_v : x; }
value_t pow(value_t x, value_t p)
{
	value_t v = 1;
	for(; p; p >>= 1, x = (calc_t)x * x % mod_v)
		if(p & 1) v = (calc_t)x * v % mod_v;
	return v;
}

void init_eps(int num)
{
	epsilon_num = num;
	value_t base = pow(primitive_root, (mod_v - 1) / num);
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

void polynomial_inverse(int n, value_t *A, value_t *B)
{
	static value_t T[MaxN];
	if(n == 1)
	{
		B[0] = pow(A[0], mod_v - 2);
		return;
	}

	int half = (n + 1) >> 1;
	polynomial_inverse(half, A, B);

	int p = 1;
	for(; p < n << 1; p <<= 1);

	fill(B + half, B + p, 0);
	transform(p, B);

	copy(A, A + n, T);
	fill(T + n, T + p, 0);
	transform(p, T);

	for(int i = 0; i != p; ++i)
		B[i] = (calc_t)B[i] * dec(2, (calc_t)T[i] * B[i] % mod_v) % mod_v;
	inverse_transform(p, B);
}

void polynomial_logarithm(int n, value_t *A, value_t *B)
{
	static value_t T[MaxN];
	int p = 1;
	for(; p < n << 1; p <<= 1);

	polynomial_inverse(n, A, T);
	fill(T + n, T + p, 0);
	transform(p, T);

	// derivative
	copy(A, A + n, B);
	for(int i = 0; i < n - 1; ++i)
		B[i] = (calc_t)B[i + 1] * (i + 1) % mod_v;
	fill(B + n - 1, B + p, 0);
	transform(p, B);

	for(int i = 0; i != p; ++i)
		B[i] = (calc_t)B[i] * T[i] % mod_v;
	inverse_transform(p, B);

	// integral
	for(int i = n - 1; i; --i)
		B[i] = (calc_t)B[i - 1] * inv[i] % mod_v;
	B[0] = 0;
}

void polynomial_exponent(int n, value_t *A, value_t *B)
{
	static value_t T[MaxN];
	if(n == 1)
	{
		B[0] = 1;
		return;
	}

	int p = 1; 
	for(; p < n << 1; p <<= 1);

	int half = (n + 1) >> 1;
	polynomial_exponent(half, A, B);
	fill(B + half, B + p, 0);

	polynomial_logarithm(n, B, T);
	for(int i = 0; i != n; ++i)
		T[i] = dec(A[i], T[i]);
	T[0] = inc(T[0], 1);
	transform(p, T);
	transform(p, B);
	for(int i = 0; i != p; ++i)
		B[i] = (calc_t)B[i] * T[i] % mod_v;
	inverse_transform(p, B);
}

value_t A[MaxN], B[MaxN], C[MaxN];

int main()
{
	int s, m;
	std::scanf("%d %d", &s, &m);
	int p = 1;
	for(; p < (s + 1) << 1; p <<= 1);
	init_eps(p);
	inv[1] = 1;
	for(int i = 2; i != p; ++i)
		inv[i] = dec(0, (calc_t)mod_v / i * inv[mod_v % i] % mod_v);
	A[0] = 1;
	for(int i = 0; i != m; ++i)
	{
		int x;
		std::scanf("%d", &x);
		A[x - 1] = mod_v - 1;
	}

	polynomial_inverse(s + 1, A, B);
	polynomial_logarithm(s + 1, B, C);
	for(int i = 0; i <= s; ++i)
		C[i] = (calc_t)C[i] * s % mod_v;
	polynomial_exponent(s + 1, C, A);
	value_t ans = (calc_t)A[s - 1] * inv[s] % mod_v;
	std::printf("%d\n", ans);
	return 0;
}
