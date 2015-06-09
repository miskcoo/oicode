/* BZOJ-2169: 连边
 *   组合计数 */
#include <cstdio>

const int MaxN = 1010, mod_v = 10007;
int f[MaxN][MaxN], deg[MaxN];

void inc(int& x, int v) { x += v % mod_v; if(x >= mod_v) x -= mod_v; }
void dec(int& x, int v) { x -= v % mod_v; if(x < 0) x += mod_v; }

int pow(int x, int p)
{
	int v = 1;
	for(; p; p >>= 1, x = x * x % mod_v)
		if(p & 1) v = x * v % mod_v;
	return v;
}

int main()
{
	int n, m, k;
	std::scanf("%d %d %d", &n, &m, &k);
	for(int i = 0; i != m; ++i)
	{
		int u, v;
		std::scanf("%d %d", &u, &v);
		++deg[u], ++deg[v];
	}

	int odd = 0;
	for(int i = 1; i <= n; ++i)
		odd += deg[i] & 1;

	int etot = n * (n - 1) >> 1;
	f[0][n - odd] = 1;
	for(int i = 0; i <= k; ++i)
	{
		for(int j = 0; j <= n; ++j)
		{
		//	printf("%d ", f[i][j]);
		//	if(!f[i][j]) continue;
			if(i >= 2) dec(f[i][j], f[i - 2][j] * ((etot - i + 2) * (i - 1) % mod_v));
			inc(f[i + 1][j], f[i][j] * (j * (n - j) % mod_v));
			if(j >= 2) inc(f[i + 1][j - 2], f[i][j] * (j * (j - 1) / 2 % mod_v));
			if(j + 2 <= n) inc(f[i + 1][j + 2], f[i][j] * ((n - j) * (n - j - 1) / 2 % mod_v));
		}
//		printf("\n");
	}

	int fac = 1;
	for(int i = 2; i <= k; ++i)
		fac = fac * i % mod_v;
	int inv_fac = pow(fac, mod_v - 2);

	printf("%d", f[k][n] * inv_fac % mod_v);
	return 0;
}
