#include <cstdio>
#include <algorithm>

const int MaxN = 503;
int n;
long long mod_v;
long long f[MaxN], g[MaxN], t[MaxN];
long long p[MaxN], arti[MaxN];

long long pow(long long x, long long p)
{
	long long v = 1;
	while(p)
	{
		if(p & 1) v = v * x % mod_v;
		x = x * x % mod_v;
		p >>= 1;
	}

	return v;
}

void calc(int a, long long* f, long long* g)
{
	if(a == 0)
	{
		f[0] = 1;
		for(int i = 1; i <= n; ++i)
			f[i] = 0;
	} else {
		int half = a >> 1;
		calc(a >> 1, g, f);

		p[0] = 1;
		for(int i = 1; i <= n; ++i)
			p[i] = p[i - 1] * half % mod_v;

		for(int i = 0; i <= std::min(half, n); ++i)
		{
			t[i] = 0;
			long long comb = 1;
			for(int j = 0; j <= i; ++j)
			{
				t[i] = (t[i] + p[j] * comb % mod_v * g[i - j]) % mod_v;
				comb = comb * (half - i + j + 1) % mod_v * arti[j + 1] % mod_v;
			}
		}

		for(int i = half + 1; i <= n; ++i)
			t[i] = 0;

		f[0] = 1;
		for(int i = 1; i <= n; ++i)
		{
			f[i] = 0;
			for(int j = 0; j <= i; ++j)
			{
				f[i] += g[j] * t[i - j] % mod_v;
				if(f[i] >= mod_v) f[i] -= mod_v;
			}
		}

		if(a & 1)
		{
			for(int i = n; i >= 1; --i)
				f[i] = (a * f[i - 1] + f[i]) % mod_v;
		}
	}
}

int main()
{
	std::freopen("calc.in", "r", stdin);
	std::freopen("calc.out", "w", stdout);
	int a;
	std::scanf("%d %d %lld", &a, &n, &mod_v);

	arti[0] = 1;
	for(int i = 1; i != MaxN; ++i)
		arti[i] = pow(i, mod_v - 2);

	calc(a, f, g);
	long long ans = f[n];
	for(int i = 2; i <= n; ++i)
		ans = ans * i % mod_v;
	std::printf("%lld", ans);
	return 0;
}
