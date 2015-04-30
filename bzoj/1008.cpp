/* BZOJ-1008: [HNOI2008]越狱
 *   快速幂  */
#include <cstdio>

const long long mod_v = 100003;
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

int main()
{
	long long n, m;
	std::scanf("%lld %lld", &m, &n);
	long long ans = power(m, n) - m * power(m - 1, n - 1);
	ans = (ans % mod_v + mod_v) % mod_v;
	std::printf("%lld", ans);
	return 0;
}
