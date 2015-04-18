/* SPOJ-2 Prime Generator
 *   线性筛 */
#include <cstdio>

const int MaxN = 100001;
char mark_v = 1;
char not_prime[MaxN];
int prime[MaxN];

int linear_sieve(int n)
{
	int tot = 0;
	for(int i = 2; i <= n; ++i)
	{
		if(!not_prime[i])
			prime[tot++] = i;
		for(int j = 0; j != tot; ++j)
		{
			int t = i * prime[j];
			if(t > n) break;
			not_prime[t] = 1;
			if(i % prime[j] == 0) break;
		}
	}

	return tot;
}

void solve(int tot)
{
	long long n, m;
	std::scanf("%lld %lld", &n, &m);

	for(int i = 0; i != tot; ++i)
	{
		long long v = prime[i];
		long long j = (n + v - 1) / v * v;
		if(j == v) j += v;
		for(; j <= m; j += v)
			not_prime[j - n] = mark_v;
	}

	for(int i = 0, d = m - n; i <= d; ++i)
	{
		if(not_prime[i] != mark_v && n + i != 1)
			std::printf("%lld\n", n + i);
	}

	std::printf("\n");
}

int main()
{
	int tot = linear_sieve(31623);
	int t;
	std::scanf("%d", &t);
	while(t --> 0)
	{
		++mark_v;
		solve(tot);
	}
	return 0;
}
