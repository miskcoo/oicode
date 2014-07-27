/* 
 * BZOJ-3667 Rabin-Miller算法
 *  Miller-Rabin、Pollard-rho、模乘算法
 */
#include <cstdio>
#include <cstdlib>

long long mod_mul(long long a, long long b, long long p)
{
	if(a == 0 || b == 0) return 0;
	long long ret = (a * b - (long long)((long double)a / p * b + 1.0e-8) * p) % p;
	if(ret < 0) return ret + p;
	return ret;
}

long long mod_pow(long long a, long long n, long long p)
{
	long long v = 1;
	while(n)
	{
		if(n & 1) v = mod_mul(v, a, p);
		a = mod_mul(a, a, p);
		n >>= 1;
	}

	return v;
}

bool witness(long long v, long long p)
{
	long long q = p - 1;
	int k = 0; 
	while(!(q & 1))
		++k, q >>= 1;
	long long t = mod_pow(v, q, p);
	if(t == 1 || t == p - 1)
		return false;
	while(k--)
	{
		t = mod_mul(t, t, p);
		if(t == p - 1)
			return false;
	}
	return true;
}

bool miller_rabin(long long n)
{
	if(n < 2) return false;
	if(n == 2) return true;
	if(!(n & 1)) return false;

	for(int i = 0; i != 20; ++i)
	{
		long long v = std::rand() % (n - 1) + 1;
		if(witness(v, n))
			return false;
	}

	return true;
}

long long gcd(long long a, long long b)
{
	return b ? gcd(b, a % b) : a;
}

long long pollard_rho(long long n, long long c)
{
	long long i = 1, k = 2;
	long long x = std::rand() % (n - 1) + 1;
	long long y = x;
	for(;;)
	{
		++i;
		x = mod_mul(x, x, n) + c;
		if(x >= n || x < 0) x -= n;
		long long d = gcd(n, y - x);
		if(d < 0) d = -d;
		if(1 != d && d != n) return d;
		if(y == x) return n;
		if(i == k) y = x, k <<= 1;
	}
}

long long factor[100];
int total;

void frac(long long n)
{
	if(miller_rabin(n))
	{
		factor[total++] = n;
		return;
	}

	long long p = n;
	while(p >= n) p = pollard_rho(p, std::rand() % (n - 1) + 1);
	frac(p);
	frac(n / p);
}

int main()
{
	int T;
	std::scanf("%d", &T);
	while(T--)
	{
		long long n;
		std::scanf("%lld", &n);
		total = 0;
		frac(n);
		if(total == 1)
		{
			std::puts("Prime");
			continue;
		}

		long long max = 0;
		for(int i = 0; i != total; ++i)
			if(max < factor[i]) max = factor[i];
		std::printf("%lld\n", max);
	}
	return 0;
}
