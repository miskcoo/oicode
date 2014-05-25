/*
 * BZOJ-2956 模积和
 *  数论-分块
 */
#include <cstdio>
#include <algorithm>

const long long mod_const = 19940417;

long long get_mod_sum(int n, int m)
{
	long long s = 0;
	if(n > m) n = m;
	for(int l = 1; l <= n; )
	{
		long long v = m / l;
		long long r = m / v;
		if(r > n) r = n;
		s = (s + (l + r) * (r - l + 1) / 2 
			% mod_const * v) % mod_const;
		l = r + 1;
	}

	return s;
}

long long get_square_sum(long long n)
{
	long long t = n * (n + 1) % mod_const 
		* (2 * n + 1) % mod_const;
	return t * 3323403 % mod_const;
}

long long get_mod_remain(int n, int m)
{
	long long s = 0;
	int last = std::min(n, m);
	for(int l = 1; l <= last; )
	{
		long long v1 = n / l, v2 = m / l;
		long long r = std::min(n / v1, m / v2);
		if(r > last) r = last;
		s += (get_square_sum(r) - get_square_sum(l - 1))
			* v1 % mod_const * v2 % mod_const;
		s %= mod_const;
		l = r + 1;
	}

	return s;
}

int main()
{
	long long N, M;
	std::scanf("%lld %lld", &N, &M);
	if(N < M) std::swap(N, M);
	long long tt = get_mod_sum(M, M);
	long long sn = (N * N % mod_const - get_mod_sum(N, N)) % mod_const;
	long long sm = (M * M % mod_const - tt) % mod_const;
	long long s1 = (N * tt) % mod_const;
	long long s2 = (M * get_mod_sum(M, N)) % mod_const;
	long long st = (M * N % mod_const * M % mod_const
		+ get_mod_remain(N, M)) % mod_const;
	long long ans = (sn * sm % mod_const + s1 + s2 - st) % mod_const;
	if(ans < 0) ans += mod_const;
	std::printf("%lld\n", ans);
	return 0;
}
