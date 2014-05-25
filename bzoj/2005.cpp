/*
 * BZOJ-2005 [Noi2010]能量采集
 *  数论，欧拉函数
 */
#include <cstdio>
#include <cstring>
#include <algorithm>

int total;
int prime[100001];
char not_prime[200001];
int phi[200001];
long long phi_sum[200001];

void linear_sieve(int N)
{
	phi[1] = 1;
	for(int i = 2; i <= N; ++i)
	{
		if(!not_prime[i])
		{
			prime[total++] = i;
			phi[i] = i - 1;
		}

		for(int j = 0; j != total; ++j)
		{
			int t = prime[j] * i;
			if(t > N) break;
			not_prime[t] = 1;
			if(i % prime[j])
			{
				phi[t] = phi[i] * (prime[j] - 1);
			} else {
				phi[t] = phi[i] * prime[j];
				break;
			}
		}
	}

	phi_sum[0] = 0;
	for(int i = 1; i <= N; ++i)
		phi_sum[i] = phi_sum[i - 1] + phi[i];
}

int main()
{
	int N, M;
	std::scanf("%d %d", &N, &M);
	long long sum = 0;
	int last = std::min(N, M);
	linear_sieve(last);
	for(int l = 1; l <= last; )
	{
		int v1 = N / l, v2 = M / l;
		int r = std::min(N / v1, M / v2);
		sum += (long long)v1 * v2 * (phi_sum[r] - phi_sum[l - 1]);
		l = r + 1;
	}

	std::printf("%lld", sum * 2 - (long long)N * M);
	return 0;
}
