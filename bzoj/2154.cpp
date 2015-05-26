/* BZOJ-2154: Crash的数字表格
 *   Mobius反演 */
#include <cstdio>
#include <algorithm>

typedef int value_t;
typedef long long calc_t;
const value_t mod_v = 20101009;
const int MaxN = 10000010;

int tot;
value_t prime[MaxN], miu[MaxN];
char not_prime[MaxN];

void linear_sieve(int n)
{
	miu[1] = 1;
	for(int i = 2; i <= n; ++i)
	{
		if(!not_prime[i])
		{
			prime[tot++] = i;
			miu[i] = -1;
		} 

		for(int j = 0; j != tot; ++j)
		{
			calc_t z = (calc_t)prime[j] * i;
			if(z > n) break;
			not_prime[z] = 1;
			if(i % prime[j])
			{
				miu[z] = -miu[i];
			} else {
				miu[z] = 0;
				break;
			}
		}
	}

	for(int i = 2; i <= n; ++i)
		miu[i] = (miu[i - 1] + (calc_t)miu[i] * i % mod_v * i) % mod_v;
}

value_t sum(value_t n, value_t m)
{
	return ((calc_t)n * (n + 1) / 2 % mod_v)
		* ((calc_t)m * (m + 1) / 2 % mod_v) % mod_v;
}

value_t F(value_t n, value_t m)
{
	value_t ans = 0;
	for(value_t l = 1, r; l <= n; l = r + 1)
	{
		r = std::min(n / (n / l), m / (m / l));
		if(r > n) r = n;
		ans = (ans + (calc_t)(miu[r] - miu[l - 1]) * sum(n / l, m / l)) % mod_v;
	}

	return ans;
}

int main()
{
	int n, m;
	std::scanf("%d %d", &n, &m);
	if(n > m) std::swap(n, m);
	linear_sieve(n);
	value_t ans = 0;
	for(int l = 1, r; l <= n; l = r + 1)
	{
		r = std::min(n / (n / l), m / (m / l));
		if(r > n) r = n;
		ans = (ans + (calc_t)(l + r) * (r - l + 1) / 2 % mod_v * F(n / l, m / l)) % mod_v;
	}

	if(ans < 0) ans += mod_v;
	std::printf("%lld\n", ans);
	return 0;
}
