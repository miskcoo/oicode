/* BZOJ-3000: Big Number
 *  Stirling 近似 */
#include <cstdio>
#include <cmath>

int main()
{
	long long k, n;
	while(std::scanf("%lld %lld", &n, &k) != EOF)
	{
		long double ans = 0;
		if(n <= 1)
		{
			std::puts("1");
		} else if(n <= 10000) {
			for(int i = 2; i <= n; ++i)
				ans += log((long double)i);
			ans /= log((long double)k);
			std::printf("%lld\n", (long long)ceil(ans + 1.0e-10));
		} else { 
			long double z = n, w = k;
			ans = (0.5L * log(2.0L * acos(-1.0L) * z) + z * log(z) - z) / log(w);
			std::printf("%lld\n", (long long)(ans + 1.0));
		}

	}
	return 0;
}
