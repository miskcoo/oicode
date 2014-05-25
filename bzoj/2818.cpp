/*
 * BZOJ-2818 Gcd
 *  数论，线性筛法，Mobius 函数
 *  同 BZOJ-2820
 */
#include <cstdio>
#include <algorithm>
#include <cstring>

int total = 0;
int prime[664579];
char not_prime[10000001];
int miu[10000001];
int g[10000001];
int* sum = miu;

void linear_sieves(int N)
{
	miu[1] = 1;
	g[1] = 0;
	for(int i = 2; i <= N; ++i)
	{
		if(!not_prime[i])
		{
			prime[total++] = i;
			miu[i] = -1;
			g[i] = 1;
		}

		for(int j = 0; j != total; ++j)
		{
			int t = prime[j] * i;
			if(t > N) break;
			not_prime[t] = 1;
			if(i % prime[j])
			{
				miu[t] = -miu[i];
				g[t] = miu[i] - g[i];
			} else {
				miu[t] = 0;
				g[t] = miu[i];
				break;
			}
		}
	}

	sum[0] = 0;
	for(int i = 1; i <= N; ++i)
		sum[i] = sum[i - 1] + g[i];
}

int main()
{
	int N;
	std::scanf("%d", &N);
	linear_sieves(N);
	long long ans = 0;
	for(int l = 1; l <= N; )
	{
		int v = N / l;
		int r = N / v;
		ans += (long long)v * v * (sum[r] - sum[l - 1]);
		l = r + 1;
	}
	std::printf("%lld\n", ans);
	return 0;
}
