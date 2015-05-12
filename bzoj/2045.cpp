/* BZOJ-2045: 双亲数 
 *   Mobius 反演 */
#include <cstdio>
#include <algorithm>

const int MaxN = 1000010;
char not_prime[MaxN];
int prime[MaxN], miu[MaxN];

void linear_sieve(int n)
{
	miu[1] = 1;
	int tot = 0;
	for(int i = 2; i <= n; ++i)
	{
		if(!not_prime[i])
		{
			prime[tot++] = i;
			miu[i] = -1;
		}

		for(int j = 0; j != tot; ++j)
		{
			long long t = 1ll * i * prime[j];
			if(t > n) break;
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
}

int main()
{
	int a, b, d;
	std::scanf("%d %d %d", &a, &b, &d);
	a /= d, b /= d;
	int n = std::min(a, b);
	linear_sieve(n);
	for(int i = 2; i <= n; ++i)
		miu[i] += miu[i - 1];
	long long ans = 0;
	for(int l = 1; l <= n; )
	{
		int r = std::min(a / (a / l), b / (b / l));
		ans += 1ll * (a / l) * (b / l) * (miu[r] - miu[l - 1]);
		l = r + 1;
	}
	std::printf("%lld\n", ans);
	return 0;
}
