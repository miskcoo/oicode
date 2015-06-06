/* BZOJ-3625: [Codeforces Round #250]小朋友和二叉树
 *   FFT+多项式开方 */
#include <cstdio>
#include <algorithm>

using std::swap;
using std::fill;
using std::copy;

typedef int value_t;
typedef long long calc_t;
const int MaxN = 1 << 19;
const value_t mod_base = 119, mod_exp = 23;
const value_t mod_v = (mod_base << mod_exp) + 1;
const value_t primitive_root = 3;
int epsilon_num;
value_t eps[MaxN], inv_eps[MaxN], inv2;

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

void polynomial_sqrt(int n, value_t *A, value_t *B)
{
	static value_t T[MaxN];
	if(n == 1)
	{
		B[0] = 1; // sqrt A[0], here is 1
		return;
	}

	int p = 1;
	for(; p < n << 1; p <<= 1);

	int half = (n + 1) >> 1;
	polynomial_sqrt(half, A, B);
	fill(B + half, B + n, 0);
	polynomial_inverse(n, B, T);
	fill(T + n, T + p, 0);
	transform(p, T);

	fill(B + half, B + p, 0);
	transform(p >> 1, B);
	for(int i = 0; i != p >> 1; ++i)
		B[i] = (calc_t)B[i] * B[i] % mod_v;
	inverse_transform(p >> 1, B);
	for(int i = 0; i != n; ++i)
		B[i] = (calc_t)inc(A[i], B[i]) * inv2 % mod_v;
	transform(p, B);
	for(int i = 0; i != p; ++i)
		B[i] = (calc_t)B[i] * T[i] % mod_v;
	inverse_transform(p, B);

}

value_t tmp[MaxN];
value_t A[MaxN], B[MaxN], C[MaxN], T[MaxN];

int main()
{
	int n, m;
	std::scanf("%d %d", &n, &m);
	int min_v = ~0u >> 1;
	for(int i = 0; i != n; ++i)
	{
		std::scanf("%d", tmp + i);
		if(min_v > tmp[i]) min_v = tmp[i];
	}

	inv2 = mod_v - mod_v / 2;

	int p = 1;
	for(; p < (m + min_v + 1) << 1; p <<= 1);
	init_eps(p);

	A[0] = 1;
	for(int i = 0; i != n; ++i)
	{
		int x = tmp[i];
		T[x - min_v] = 2;
		A[x] = mod_v - 4;
	}

	polynomial_inverse(m + min_v + 1, T, C);
	polynomial_sqrt(m + min_v + 1, A, B);
	B[0] = dec(1, B[0]);
	for(int i = 1; i <= m + min_v; ++i)
		B[i] = mod_v - B[i];
	for(int i = 0; i <= m; ++i)
		B[i] = B[i + min_v];
	fill(B + m + 1, B + p, 0);
	fill(C + m + 1, C + p, 0);
	transform(p, B);
	transform(p, C);
	for(int i = 0; i != p; ++i)
		B[i] = (calc_t)B[i] * C[i] % mod_v;
	inverse_transform(p, B);
	for(int i = 1; i <= m; ++i)
		std::printf("%d\n", B[i]);
	return 0;
}
