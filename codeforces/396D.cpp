/* Codeforces 396D. On Sum of Number of Inversions in Permutations
 *   数位DP，计数
 *   combinatorics */
#include <cstdio>

typedef long long calc_t;
const int inv4 = 250000002;
const int MaxN = 1000010, mod_v = 1000000007;
int n, P[MaxN], fac[MaxN], ta[MaxN], num[MaxN], rank[MaxN];

int ask(int x)
{
	int ans = 0;
	for(; x; x -= x & -x)
		ans += ta[x];
	return ans;
}

void modify(int x, int v = -1)
{
	for(; x <= n; x += x & -x) ta[x] += v;
}

calc_t get_perm(int n)
{
	return (calc_t)fac[n] * n % mod_v 
		* (n - 1) % mod_v 
		* inv4 % mod_v;
}

int main()
{
	std::scanf("%d", &n);

	fac[0] = 1;
	for(int i = 1; i <= n; ++i)
	{
		modify(i, 1);
		fac[i] = (calc_t)fac[i - 1] * i % mod_v;
		std::scanf("%d", P + i);
	}

	calc_t ans = 0;
	for(int i = 1; i <= n; ++i)
	{
		calc_t k = ask(P[i] - 1);
		calc_t comb2 = (k * (k - 1) >> 1) % mod_v;
		ans = (ans + comb2 * fac[n - i] + k * get_perm(n - i)) % mod_v;
		modify(P[i], -1);
		rank[i] = k;
	}

//	std::printf("%d\n", (int)ans);

	num[n] = 1;
	for(int i = n; i >= 2; --i)
		num[i - 1] = (num[i] + (calc_t)fac[n - i] * rank[i]) % mod_v;
	for(int i = 1; i <= n; ++i)
		ans = (ans + (calc_t)rank[i] * num[i]) % mod_v;

	std::printf("%d\n", (int)ans);
	return 0;
}
