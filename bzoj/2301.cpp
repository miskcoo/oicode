/*
 * BZOJ-2301 [HAOI2011]Problem b
 *  数论，莫比乌斯反演
 */
#include <cstdio>
#include <algorithm>

int total;
int prime[5134];
char not_prime[50001];
int miu[50001];
int sum[50001];

void linear_sieves(int N)
{
	miu[1] = 1;
	for(int i = 2; i <= N; ++i)
	{
		if(!not_prime[i])
		{
			prime[total++] = i;
			miu[i] = -1;
		}

		for(int j = 0; j != total; ++j)
		{
			int t = prime[j] * i;
			if(t > N) break;
			not_prime[t] = 1;
			if(i % prime[j])
			{
				miu[t] = -miu[i];
			} else {
				miu[t] = 0;
				break;
			}
		}
	}

	sum[0] = 0;
	for(int i = 1; i <= N; ++i)
		sum[i] = sum[i - 1] + miu[i];
}

int calc(int N, int M)
{
	int count = 0;
	int last = std::min(N, M);
	for(int l = 1; l <= last; )
	{
		int v1 = N / l;
		int v2 = M / l;
		int r = std::min(N / v1, M / v2);
		count += v1 * v2 * (sum[r] - sum[l - 1]);
		l = r + 1;
	}
	return count;
}

int main()
{
	linear_sieves(50000);
	int T;
	std::scanf("%d", &T);
	while(T--)
	{
		int a, b, c, d, k;
		std::scanf("%d %d %d %d %d", &a, &b, &c, &d, &k);
		std::printf("%d\n", calc(b / k, d / k)
			+ calc((a - 1) / k, (c - 1) / k)
			- calc((a - 1) / k, d / k) 
			- calc((c - 1) / k, b / k));
	}
	return 0;
}
