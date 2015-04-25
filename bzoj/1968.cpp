/* BZOJ-1968: [Ahoi2005]COMMON 约数研究
 *   欧拉筛（线性筛） */
#include <cstdio>

const int MaxN = 1000010;
char not_prime[MaxN];
int prime[MaxN], sigma[MaxN], cnt[MaxN];

void euler_sieve(int n)
{
	int tot = 0;
	for(int i = 2; i <= n; ++i)
	{
		if(!not_prime[i])
		{
			cnt[i] = 1;
			sigma[i] = 2;
			prime[tot++] = i;
		}

		for(int j = 0; j != tot; ++j)
		{
			long long t = (long long)prime[j] * i;
			if(t > n) break;
			not_prime[t] = 1;
			if(i % prime[j])
			{
				cnt[t] = 1;
				sigma[t] = sigma[i] << 1;
			} else {
				cnt[t] = cnt[i] + 1;
				sigma[t] = sigma[i] / cnt[t] * (cnt[t] + 1);
				break;
			}
		}
	}
}

int main()
{
	int n;
	std::scanf("%d", &n);
	euler_sieve(n);
	long long ans = 1;
	for(int i = 2; i <= n; ++i)
		ans += sigma[i];
	std::printf("%lld", ans);
	return 0;
}
