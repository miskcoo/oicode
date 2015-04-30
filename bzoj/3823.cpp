/* BZOJ-3823: 定情信物
 *  线性筛逆元 */
#include <cstdio>

const int MaxN = 10000010;
int inv[MaxN << 1];

long long power(long long x, long long p, long long m)
{
	long long v = 1;
	while(p)
	{
		if(p & 1) v = x * v % m;
		x = x * x % m;
		p >>= 1;
	}
	return v;
}

void calc_inv(int n, long long p)
{
	inv[1] = 1;
	for(int i = 2; i <= n; ++i)
		inv[i] = -p / i * inv[p % i] % p;
}

int main()
{
	int n;
	long long p;
	std::scanf("%d %lld", &n, &p);
	calc_inv(n << 1, p);
	long long now = power(2, n, p);
	long long ans = now;
	int tot = 0;
	for(int i = 1; i <= n; ++i)
	{
		int z = n - i + 1;
		while(z % p == 0) ++tot, z /= p;
		now = now * z % p;
		z = i << 1;
		while(z % p == 0) --tot, z /= p;
		now = now * inv[z % p] % p;
		if(now < 0) now += p;
		ans ^= tot ? 0 : now;
	}

	std::printf("%lld", ans);
	return 0;
}
