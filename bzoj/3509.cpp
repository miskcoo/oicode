/* BZOJ-3509: [CodeChef] COUNTARI
 *   分块+FFT   */
#include <cstdio>
#include <cstring>
#include <cmath>
#include <algorithm>

const long long mod_v = 17ll * (1 << 27) + 1;
const int MaxN = 100010, MaxA = 1 << 16;
int max_v, A[MaxN], cntl[MaxA], cntr[MaxA];
long long X[MaxA], Y[MaxA], eps[MaxA], inv_eps[MaxA];

long long calc_block(int n, int block_size)
{
	long long ans = 0;

	for(int i = 0; i < n; ++i)
		++cntr[A[i]];

	for(int i = 0; i < n; i += block_size)
	{
		int r = std::min(n, i + block_size) - 1;
		for(int j = i; j <= r; ++j)
			--cntr[A[j]];
		for(int j = i; j <= r; ++j)
		{
			for(int k = j + 1; k <= r; ++k)
			{
				int z = 2 * A[j] - A[k];
				if(z >= 0) ans += cntl[z];
				int x = 2 * A[k] - A[j];
				if(x >= 0) ans += cntr[x];
			}

			++cntl[A[j]];
		}
	}
	return ans;
}

long long power(long long x, long long p)
{
	long long v = 1;
	while(p)
	{
		if(p & 1) v = x * v % mod_v;
		x = x * x % mod_v;
		p >>= 1;
	}

	return v;
}

void init_eps(int n)
{
	long long base = power(3, (mod_v - 1) / n);
	long long inv_base = power(base, mod_v - 2);
	eps[0] = 1, inv_eps[0] = 1;
	for(int i = 1; i < n; ++i)
	{
		eps[i] = eps[i - 1] * base % mod_v;
		inv_eps[i] = inv_eps[i - 1] * inv_base % mod_v;
	}
}

long long inc(long long x, long long d) 
{
	x += d; 
	return x >= mod_v ? x - mod_v : x; 
}

long long dec(long long x, long long d) 
{
	x -= d; 
	return x < 0 ? x + mod_v : x; 
}

void transform(int p, int n, long long *x, long long *w)
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
				long long z = x[j + m + k] * w[p / i * k] % mod_v;
				x[j + m + k] = dec(x[j + k], z);
				x[j + k] = inc(x[j + k], z);
			}
		}
	}
}

long long solve_other(int n, int block_size)
{
	int p = 1;
	while(p < max_v) p <<= 1;
	p <<= 1;
	init_eps(p);

	long long ans = 0, inv = power(p, mod_v - 2);
	for(int i = 0; i < n; i += block_size)
	{
		int m = 0;
		int r = std::min(n, i + block_size) - 1;
		for(int j = 0; j != i; ++j)
			++X[A[j]], m = std::max(m, A[j]);
		for(int j = r + 1; j < n; ++j)
			++Y[A[j]], m = std::max(m, A[j]);
		int len = 1;
		while(len < m) len <<= 1;
		len <<= 1;
		transform(p, len, X, eps);
		transform(p, len, Y, eps);
		for(int j = 0; j != len; ++j)
			X[j] = X[j] * Y[j] % mod_v;
		transform(p, len, X, inv_eps);
		for(int j = 0; j != len; ++j)
			X[j] = inc(X[j] * inv % mod_v, mod_v);
		for(int j = i; j <= r; ++j)
			ans += X[A[j] << 1];
		std::memset(X, 0, sizeof(long long) * len);
		std::memset(Y, 0, sizeof(long long) * len);
	}

	return ans;
}

int main()
{
	int n;
	std::scanf("%d", &n);
	for(int i = 0; i != n; ++i)
	{
		std::scanf("%d", A + i);
		if(A[i] > max_v) max_v = A[i];
	}

	int block_size = 8 * (int)sqrt(n);
	if(block_size > n) block_size = n;
	long long ans = 0;
	ans += calc_block(n, block_size);
	ans += solve_other(n, block_size);
	std::printf("%lld", ans);
	return 0;
}
