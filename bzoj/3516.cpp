/*
 * BZOJ-3516 国王奇遇记加强版
 *  数学-组合
 */ 
#include <cstdio>

const long long mod_const = 1000000007LL;
long long comb[1001][1001];
long long f[1001];

void init_combination(int n)
{
	comb[0][0] = 1;
	for(int i = 1; i <= n; ++i)
	{
		comb[i][0] = 1;
		comb[i][i] = 1;
		for(int j = 1; j != i; ++j)
		{
			comb[i][j] = comb[i - 1][j] + comb[i - 1][j - 1];
			comb[i][j] %= mod_const;
		}
	}
}

long long power(long long base, int p)
{
	long long v = 1;
	while(p)
	{
		if(p & 1) v = v * base % mod_const;
		base = base * base % mod_const;
		p >>= 1;
	}
	return v;
}

long long calc_sum(int n, int m)
{
	long long invert = power(m - 1, mod_const - 2);
	f[0] = ((power(m, n + 1) - 1) * invert - 1) % mod_const;
	if(f[0] < 0) f[0] += mod_const;
	for(int i = 1; i <= m; ++i)
	{
		long long t = 0;
		for(int j = 0; j != i; ++j)
		{
			int sign = ((i ^ j) & 1) ? -1 : 1;
			t = (t + comb[i][j] * sign * f[j]) % mod_const;
		}

		f[i] = (t + power(n, i) * power(m, n + 1)) % mod_const;
		f[i] = f[i] * invert % mod_const;
		if(f[i] < 0) f[i] += mod_const;
	}
	return f[m];
}

int main()
{
	int N, M;
	std::scanf("%d %d", &N, &M);
	init_combination(M);
	if(M == 1) std::printf("%lld", (long long)(N + 1) * N / 2 % mod_const);
	else std::printf("%lld", calc_sum(N, M));
	return 0;
}
