/* Codeforces 543D. Road Improvement
 *   神题！树DP
 *   dp, trees */
#include <cstdio>
#include <algorithm>

const long long mod_v = 1000000007;
const int MaxN = 200010;
int fa[MaxN], facd[MaxN];
long long d[MaxN], u[MaxN];

long long pow(long long x, int p)
{
	long long v = 1;
	for(; p; p >>= 1, x = x * x % mod_v)
		if(p & 1) v = x * v % mod_v;
	return v;
}

long long inv(long long x)
{
	return pow(x, mod_v - 2);
}

long long get(int i)
{
	if(facd[i]) return 0;
	return d[i];
}

int main()
{
	int n;
	std::scanf("%d", &n);
	for(int i = 2; i <= n; ++i)
		std::scanf("%d", fa + i);

	std::fill(d, d + n + 1, 1);
	for(int i = n; i >= 2; --i)
	{
		if(get(i) + 1 == mod_v) ++facd[fa[i]];
		else d[fa[i]] = d[fa[i]] * (get(i) + 1) % mod_v;
	}

	u[1] = 1;
	for(int i = 2; i <= n; ++i)
	{
		int p = fa[i];
		if(facd[p] > 1 || facd[p] == 1 && get(i) + 1 != mod_v) u[i] = 1;
		else if(facd[p] == 1) u[i] = (u[p] * d[p] + 1) % mod_v;
		else u[i] = (u[p] * d[p] % mod_v * inv(get(i) + 1) + 1) % mod_v;
	}

	for(int i = 1; i <= n; ++i)
		std::printf("%d ", int(get(i) * u[i] % mod_v));
	return 0;
}
