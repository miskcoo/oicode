/* BZOJ-3456: 城市规划
 *   FFT+多项式求逆 */
#include <cstdio>
#include <algorithm>

using std::swap;
using std::fill;
using std::copy;
using std::reverse;
using std::reverse_copy;

typedef int value_t;
typedef long long calc_t;
const int MaxN = 1 << 18;
const value_t mod_base = 479, mod_exp = 21;
const value_t mod_v = (mod_base << mod_exp) + 1;
const value_t primitive_root = 3;
int epsilon_num;
value_t eps[MaxN], inv_eps[MaxN];

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

struct poly_t
{
	int deg;
	value_t x[MaxN];
	poly_t() : deg(0) { x[0] = 0; }
};

void polynomial_inverse(int n, const poly_t& A, poly_t& B)
{
	if(n == 1)
	{
		B.deg = 1;
		B.x[0] = pow(A.x[0], mod_v - 2);
		return;
	}

	static value_t X[MaxN];
	polynomial_inverse((n + 1) >> 1, A, B);

	int p = 1;
	for(; p < n << 1; p <<= 1);
	copy(A.x, A.x + n, X);
	fill(X + n, X + p, 0);
	transform(p, X);

	fill(B.x + B.deg, B.x + p, 0);
	transform(p, B.x);

	for(int i = 0; i != p; ++i)
		B.x[i] = (calc_t)B.x[i] * dec(2, (calc_t)X[i] * B.x[i] % mod_v) % mod_v;
	inverse_transform(p, B.x);
	B.deg = n;
}

poly_t A, B, C;
value_t inv[MaxN], inv_fac[MaxN], choose[MaxN];

int main()
{
	int n;
	std::scanf("%d", &n);
	int p = 1;
	for(; p < (n + 1) << 1; p <<= 1);
	init_eps(p);

	inv[1] = inv_fac[0] = 1;
	for(int i = 1; i <= n; ++i)
	{
		if(i != 1) inv[i] = -mod_v / i * (calc_t)inv[mod_v % i] % mod_v;
		if(inv[i] < 0) inv[i] += mod_v;
		inv_fac[i] = (calc_t)inv_fac[i - 1] * inv[i] % mod_v;
	}

	choose[0] = choose[1] = 1;
	for(int i = 2; i <= n; ++i)
		choose[i] = pow(2, (calc_t)i * (i - 1) / 2 % (mod_v - 1));

	A.deg = B.deg = n + 1;
	for(int i = 0; i <= n; ++i)
		A.x[i] = (calc_t)choose[i] * inv_fac[i] % mod_v;
	for(int i = 1; i <= n; ++i)
		B.x[i] = (calc_t)choose[i] * inv_fac[i - 1] % mod_v;
	polynomial_inverse(n + 1, A, C);
	fill(C.x + C.deg, C.x + p, 0);
	transform(p, C.x);
	transform(p, B.x);
	for(int i = 0; i <= p; ++i)
		C.x[i] = (calc_t)C.x[i] * B.x[i] % mod_v;
	inverse_transform(p, C.x);
	value_t ans = (calc_t)C.x[n] * pow(inv_fac[n - 1], mod_v - 2) % mod_v;
	if(ans < 0) ans += mod_v;
	std::printf("%d\n", ans);
	return 0;
}
