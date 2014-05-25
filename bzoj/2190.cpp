/*
 * BZOJ-2190 [SDOI2008]仪仗队
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

int calc(int N)
{
	int count = 0;
	for(int l = 1; l <= N; )
	{
		int v = N / l;
		int r = N / v;
		count += v * v * (sum[r] - sum[l - 1]);
		l = r + 1;
	}
	return count;
}

int main()
{
	int N;
	std::scanf("%d", &N);
	linear_sieves(N - 1);
	std::printf("%d", 2 + calc(N - 1));
	return 0;
}
