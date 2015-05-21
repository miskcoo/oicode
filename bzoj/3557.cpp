/* BZOJ-3557: [Ctsc2014]随机数
 *   多项式乘、除、求模及GCD  */
#include <cstdio>
#include <algorithm>

using std::swap;
using std::fill;
using std::copy;
using std::reverse;
using std::reverse_copy;

const int MaxL = 21;
typedef int value_t;
typedef long long calc_t;
typedef long long power_t;
const value_t mod_base = 479, mod_exp = 21;
const value_t mod_v = (mod_base << mod_exp) + 1;
const value_t primitive_root = 3;

int fft_tot, type;
value_t eps[1 << MaxL], inv_eps[1 << MaxL];

void mod2(value_t& x)
{
	if(x >= (mod_v >> 1))
		x = (mod_v - x) & 1;
	else x &= 1;
}

value_t pow(value_t x, power_t p)
{
	value_t v = 1;
	for(; p; p >>= 1, x = (calc_t)x * x % mod_v)
		if(p & 1) v = (calc_t)x * v % mod_v;
	return v;
}

value_t inc(value_t x, value_t delta) 
{
	x += delta;
	return x >= mod_v ? x - mod_v : x;
}

value_t dec(value_t x, value_t delta)
{
	x -= delta;
	return x < 0 ? x + mod_v : x;
}

void init_eps(int tot)
{
	fft_tot = tot;
	value_t base = pow(primitive_root, (mod_v - 1) / tot);
	value_t inv_base = pow(base, mod_v - 2);
	eps[0] = inv_eps[0] = 1;
	for(int i = 1; i != tot; ++i)
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

	for(int i = 2, t = 1; i <= n; t = i, i <<= 1)
	{
		int r = fft_tot / i;
		for(int p = 0; p < n; p += i)
		{
			for(int l = 0, s = 0; l != t; ++l, s += r)
			{
				value_t z = (calc_t)x[p + l + t] * w[s] % mod_v;
				x[p + l + t] = dec(x[p + l], z);
				x[p + l] = inc(x[p + l], z);
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
//	for(int i = 0; i != n; ++i) mod2(x[i]);
}

// A(x)B(x) = 1 (mod x^n)
void polynomial_inverse(int n, value_t *A, value_t *B)
{
	if(n == 1)
	{
		B[0] = pow(A[0], mod_v - 2);
		return;
	}

	static value_t tmp[1 << MaxL];
	int p = 1;
	while(p < n << 1) p <<= 1;

	int half = (n + 1) >> 1;
	polynomial_inverse(half, A, B);
	fill(B + half, B + p, 0);
	transform(p, B);

	copy(A, A + n, tmp);
	fill(tmp + n, tmp + p, 0);
	transform(p, tmp);

	for(int i = 0; i != p; ++i)
		tmp[i] = (calc_t)B[i] * tmp[i] % mod_v;
	if(n > 100000)
	{
		inverse_transform(p, tmp);
		for(int i = 0; i != p; ++i) mod2(tmp[i]);
		transform(p, tmp);
	}
	for(int i = 0; i != p; ++i)
		B[i] = dec(2, (calc_t)B[i] * tmp[i] % mod_v);
	inverse_transform(p, B);
	for(int i = 0; i != n; ++i) mod2(B[i]);
	fill(B + n, B + p, 0);
}

// A(x) = B(x)D(x) + R(x)
void polynomial_division(int n, int m, value_t *A, value_t *B, value_t *D, value_t *R)
{
	static bool mod_solved = false;
	static value_t Z[1 << MaxL];
	static value_t A0[1 << MaxL], B0[1 << MaxL];

	int p = 1, t = n - m + 1;
	while(p < t << 1) p <<= 1;

	if(!mod_solved || type)
	{
		if(!type) mod_solved = true;
		fill(A0, A0 + p, 0);
		reverse_copy(B, B + m, A0);
		polynomial_inverse(t, A0, Z);
		fill(Z + t, Z + p, 0);
		for(int i = 0; i != t; ++i) mod2(Z[i]);
		transform(p, Z);
	}

	reverse_copy(A, A + n, A0);
	fill(A0 + t, A0 + p, 0);
	transform(p, A0);

	for(int i = 0; i != p; ++i)
		A0[i] = (calc_t)A0[i] * Z[i] % mod_v;
	inverse_transform(p, A0);
	reverse(A0, A0 + t);
	for(int i = 0; i != t; ++i) mod2(A0[i]);
	if(D) copy(A0, A0 + t, D);
	
	if(!R) return;
	p = 1;
	if(type) while(p < m + n) p <<= 1;
	else while(p < m << 1) p <<= 1;
	if(t < p) fill(A0 + t, A0 + p, 0);
	transform(p, A0);

	copy(B, B + m, B0);
	fill(B0 + m, B0 + p, 0);
	transform(p, B0);

	for(int i = 0; i != p; ++i)
		A0[i] = (calc_t)A0[i] * B0[i] % mod_v;

	inverse_transform(p, A0);
	for(int i = 0; i != m; ++i)
	{
		R[i] = dec(A[i], A0[i]);
		mod2(R[i]);
	}

	fill(R + m, R + p, 0);
}

int mod_len;
value_t MOD[1 << MaxL], M0[1 << MaxL];
void multiply(int n, value_t *A, value_t *B, value_t *C)
{
	static value_t B0[1 << MaxL];
	int p = 1;
	while(p < n) p <<= 1;
	if(A == B)
	{
		int pos = p - 1;
		for(; pos && !A[pos]; --pos);
		for(p = 1; p <= pos << 1; p <<= 1);

		copy(A, A + p, C);
		transform(p, C);
		for(int i = 0; i != p; ++i)
			C[i] = (calc_t)C[i] * C[i] % mod_v;
		inverse_transform(p, C);
	} else {
		int pos1 = p - 1, pos2 = p - 1;
		for(; pos1 && !A[pos1]; --pos1);
		for(; pos2 && !B[pos2]; --pos2);
		for(p = 1; p <= pos1 + pos2; p <<= 1);

		copy(B, B + p, B0);
		copy(A, A + p, C);
		transform(p, C);
		transform(p, B0);
		for(int i = 0; i != p; ++i)
			C[i] = (calc_t)C[i] * B0[i] % mod_v;
		inverse_transform(p, C);
	}

	for(int i = 0; i != p; ++i) mod2(C[i]);
	if(p < n) fill(C + p, C + n, 0);
	bool flag = false;
	for(int i = mod_len - 1; i < p; ++i)
	{
		if(C[i]) 
		{
			flag = true;
			break;
		}
	}

	if(flag) polynomial_division(n, mod_len, C, MOD, 0, C);
}

// AX + BY = 1
typedef std::pair<int, int> len_t;
len_t polynomial_exgcd(int n, int m, value_t *A, value_t *B, value_t *X, value_t *Y, value_t *R)
{
	while(n && !A[n - 1]) --n;
	while(m && !B[m - 1]) --m;
	if(!m)
	{
		X[0] = 1, Y[0] = 0;
		return len_t(1, 1);
	}

	if(n < m) 
	{
		len_t len = polynomial_exgcd(m, n, B, A, Y, X, R);
		std::swap(len.first, len.second);
		return len;
	}

	int p = 1;
	while(p < (n - m + 1) + m) p <<= 1;
	value_t *D = new value_t[p << 1];

	polynomial_division(n, m, A, B, D, R);
	len_t l = polynomial_exgcd(m, m, B, R, X, Y, A);

	while(p < (n - m + 1) + l.second) p <<= 1;

	len_t ret_len;
	copy(X, X + l.first, A);
	fill(A + l.first, A + p, 0);
	copy(Y, Y + l.second, X);
	ret_len.first = l.second;

	fill(D + (n - m + 1), D + p, 0);
	transform(p, D);
	fill(Y + l.second, Y + p, 0);
	transform(p, Y);

	for(int i = 0; i != p; ++i)
		Y[i] = (calc_t)D[i] * Y[i] % mod_v;
	delete[] D;

	inverse_transform(p, Y);
	for(int i = 0; i != p; ++i)
	{
		Y[i] = dec(A[i], Y[i]);
		mod2(Y[i]);
	}

	int len = p;
	while(len && !Y[len - 1]) --len;
	ret_len.second = len;
	return ret_len;
}

value_t temp[5][1 << MaxL];
void solve0(int m)
{
	power_t k;
	std::scanf("%lld", &k);
	mod_len = m + 1;

	int p = 1;
	while(p < mod_len << 1) p <<= 1;
	init_eps(p);

	value_t *V = temp[0], *B = temp[1];
	int flag = 1, lenv = 0, lenb = 1;

	while(k)
	{
		if(!flag)
		{
			if(k & 1) multiply(mod_len << 1, B, V, V);
			if(k >> 1) multiply(mod_len << 1, B, B, B);
		} else {
			if(k & 1) lenv += lenb;
			lenb <<= 1;

			if(lenb > m >> 1 || lenv > m >> 1)
			{
				flag = 0;
				B[lenb] = 1;
				V[lenv] = 1;
			}
		} 

		k >>= 1;
	}

	if(flag) V[lenv] = 1;
	multiply(mod_len << 1, V, M0, M0);
	for(int i = 0; i != m; ++i)
		std::printf("%d", M0[i]);
}

void solve1(int m)
{
	int l;
	std::scanf("%d", &l);
	value_t *Mk = temp[0], *K = temp[1];
	value_t *X = temp[2], *Y = temp[3];
	mod_len = m + 1;

	int p = 1;
	while(p < std::max(2 << l, m << 1)) p <<= 1;
	init_eps(p << 1);

	copy(MOD, MOD + p, K);
	copy(M0, M0 + p, Mk);
	len_t len = polynomial_exgcd(m, m + 1, Mk, K, X, Y, temp[4]);

	for(int i = 0; i != m; ++i) std::scanf("%d", Mk + i);
	fill(Mk + m, Mk + p, 0);

	multiply(len.first + m, Mk, X, Mk);

	type = 0;
	for(int i = 0; i != m - l; ++i)
		multiply(mod_len << 1, Mk, Mk, Mk);

	multiply(mod_len << 1, M0, Mk, M0);
	for(int i = 0; i != m; ++i)
		std::printf("%d", M0[i]);
}

int main()
{
	int m;
	std::scanf("%d", &m);
	for(int i = 0; i != m; ++i)
		std::scanf("%d", MOD + i);
	MOD[m] = 1;
	for(int i = 0; i != m; ++i)
		std::scanf("%d", M0 + i);
	std::scanf("%d", &type);
	if(type == 0) solve0(m);
	else solve1(m);
	return 0;
}
