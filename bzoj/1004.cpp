/* BZOJ-1004: [HNOI2008]Cards
 *   Polya计数+Burnside引理+背包 */
#include <cstdio>
#include <cstring>

int mod_v, sr, sb, sg, n;
int f[25][25][25];
int sw[65], id[65], size[65], sum[65];

void exgcd(int a, int b, int& x, int& y)
{
	if(b == 0)
	{
		x = 1, y = 0;
	} else {
		exgcd(b, a % b, x, y);
		int t = y;
		y = x - a / b * y;
		x = t;
	}
}

int calc()
{
	int tot = 0;
	std::memset(id, 0, sizeof(id));
	for(int i = 1; i <= n; ++i)
	{
		if(sw[i])
		{
			size[++tot] = -1;
			for(int j = sw[i], t; j; t = sw[j], sw[j] = 0, j = t)
				++size[tot];
			sum[tot] = sum[tot - 1] + size[tot];
			id[sum[tot]] = tot;
		}
	}

	std::memset(f, 0, sizeof(f));
	f[0][0][0] = 1;
	for(int r = 0; r <= sr; ++r)
	{
		for(int g = 0; g <= sg; ++g)
		{
			for(int b = 0; b <= sb; ++b)
			{
				if(!id[r + g + b]) continue;
				int v = size[id[r + g + b]];
				if(r - v >= 0) f[r][g][b] += f[r - v][g][b];
				if(g - v >= 0) f[r][g][b] += f[r][g - v][b];
				if(b - v >= 0) f[r][g][b] += f[r][g][b - v];
				f[r][g][b] %= mod_v;
			}
		}
	}

	return f[sr][sg][sb];
}

int main()
{
	int m;
	std::scanf("%d %d %d %d %d", &sr, &sb, &sg, &m, &mod_v);
	n = sr + sb + sg;
	int ans = 0;
	for(int i = 0; i != m; ++i)
	{
		for(int j = 1; j <= n; ++j)
			std::scanf("%d", sw + j);
		ans += calc();
	}

	for(int i = 1; i <= n; ++i)
		sw[i] = i;
	ans += calc();
	int k, inv;
	exgcd(m + 1, mod_v, inv, k);
	if(inv < 0) inv += mod_v;
	std::printf("%d\n", ans % mod_v * inv % mod_v);
	return 0;
}
