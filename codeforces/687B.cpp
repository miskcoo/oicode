#include <cstdio>
#include <algorithm>

const int MaxN = 500, MaxV = 1000010;

int n, k, tot;
int prime[MaxV], val[MaxV], fac[MaxV], num[MaxV], lcm[MaxV], C[MaxV];
char not_prime[MaxV];

void linear_sieve(int n)
{
	fac[1] = -1;
	for(int i = 2; i <= n; ++i)
	{
		if(!not_prime[i])
		{
			fac[i] = tot;
			val[tot] = -1;
			prime[tot++] = i;
		}

		for(int j = 0; j != tot; ++j)
		{
			long long t = 1ll * prime[j] * i;
			if(t > n) break;
			not_prime[t] = 1;
			fac[t] = j;
			if(i % prime[j] == 0) break;
		}
	}
}

int count(int x, int p)
{
	int cnt = 0;
	while(x % p == 0)
	{
		x /= p; ++cnt;
	}

	return cnt;
}

bool check(int f, int num)
{
	int p = prime[f], cnt = 0;
	for(int i = 0; i != n; ++i)
	{
		int z = count(C[i], p);
		cnt = std::max(z, cnt);
	}

	return cnt >= num;
}

int main()
{
	linear_sieve(1000000);
	std::scanf("%d %d", &n, &k);

	for(int i = 0; i != n; ++i)
		std::scanf("%d", C + i);

	bool flag = true;
	while(k != 1)
	{
		int cnt = 0, f = fac[k];
		while(fac[k] == f)
		{
			++cnt;
			k /= prime[f];
		}

		if(!check(f, cnt))
		{
			flag = false;
			break;
		}
	}

	std::puts(flag ? "Yes" : "No");
	return 0;
}
