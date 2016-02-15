#include <cstdio>

const int MaxN = 100001;
const int mod_v = 1000000007;
long long A[MaxN], inv[MaxN];

int main()
{
	int t, x, n;
	long long m;
	std::scanf("%d", &t);

	inv[1] = 1;
	for(int i = 2; i != MaxN; ++i)
		inv[i] = -(mod_v / i) * inv[mod_v % i] % mod_v;

	while(t --> 0)
	{
		std::scanf("%d %d %lld", &n, &x, &m);
		for(int i = 1; i <= n; ++i)
			std::scanf("%lld", A + i);

		m %= mod_v;

		long long ans = 0, c = 1;
		for(int i = 0; i != x; ++i)
		{
			ans = (ans + c * (A[x - i] % mod_v)) % mod_v;
			c = c * (m + i) % mod_v * inv[i + 1] % mod_v;
		}

		std::printf("%lld\n", (ans + mod_v) % mod_v);
	}
	return 0;
}
