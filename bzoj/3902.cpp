/* BZOJ-3902: 三向投影
 *   计数+容斥 */
#include <cstdio>
#include <algorithm>

const long long mod_v = 1000000009;
const int MaxN = 55;
int n, m;
long long ans = 1;
int A[MaxN], B[MaxN];
long long C[MaxN][MaxN];
long long dp[MaxN][MaxN];

long long power(long long x, long long p)
{
	long long v = 1;
	while(p)
	{
		if(p & 1) v = x * v % mod_v;
		x = x * x % mod_v;
		p >>= 1;
	}
	return v;
}

int value;
long long f(int a, int b, int p, int q)
{
	if(!a && !b) return 1;
	if(dp[a][b] >= 0) return dp[a][b];
	long long now = power(value + 1, p * q - (a - p) * (b - q));
	for(int i = 0; i <= a; ++i)
	{
		for(int j = (i == 0); j <= b; ++j)
		{
			now -= C[a][i] * C[b][j] % mod_v * power(value, p * q - (p - i) * (q - j)) % mod_v * f(a - i, b - j, p - i, q - j) % mod_v;
			now %= mod_v;
		}
	}
	if(now < 0) now += mod_v;
	return dp[a][b] = now;
}

long long solve(int a, int b)
{
	if(a < 0 && b < 0) return 1;
	int va = a >= 0 ? A[a] : 0;
	int vb = b >= 0 ? B[b] : 0;
	int v = std::max(va, vb);
	int w = 0, h = 0;
	while(a >= 0 && A[a] == v) ++w, --a;
	while(b >= 0 && B[b] == v) ++h, --b;
	value = v;
	for(int i = 0; i <= n; ++i)
		for(int j = 0; j <= m; ++j)
			dp[i][j] = -1;
	long long ans = f(w, h, n - a - 1, m - b - 1);
	return ans * solve(a, b) % mod_v;
}

int main()
{
	std::scanf("%d %d", &n, &m);
	for(int i = 0; i != n; ++i)
		std::scanf("%d", A + i);
	for(int i = 0; i != m; ++i)
		std::scanf("%d", B + i);
	std::sort(A, A + n);
	std::sort(B, B + m); 

	for(int i = 0; i <= 50; ++i)
	{
		C[i][i] = C[i][0] = 1;
		for(int j = 1; j < i; ++j)
			C[i][j] = (C[i - 1][j - 1] + C[i - 1][j]) % mod_v;
	}

	std::printf("%lld", solve(n - 1, m - 1));
	return 0;
}
