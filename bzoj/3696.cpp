/* BZOJ-3696: 化合物
 *   树DP+FWT */
#include <cstdio>

const int MaxN = 100010, MaxH = 512;
int total;
int head[MaxN], point[MaxN], next[MaxN];
long long f[MaxN][MaxH], ans[MaxH];

void add_edge(int u, int v)
{
	point[++total] = v;
	next[total] = head[u];
	head[u] = total;
}

void fwt(int p, long long* f)
{
	for(int i = 1; i < p; i <<= 1)
	{
		for(int j = 0; j != p; ++j)
		{
			if(i & j)
			{
				long long a = f[i ^ j], b = f[j];
				f[i ^ j] = a + b;
				f[j] = a - b;
			}
		}
	}
}

void ifwt(int p, long long* f)
{
	int l = 0;
	for(; 1 << l < p; ++l);
	fwt(p, f);
	for(int i = 0; i != p; ++i)
		f[i] >>= l;
}

long long tmp[MaxH];
int dfs(int u, int fa)
{
	int max_depth = 0;
	f[u][0] = 1;
	for(int k = head[u]; k; k = next[k])
	{
		int v = point[k];
		if(v != fa)
		{
			int dep0 = dfs(v, u);
			if(dep0 > max_depth)
				max_depth = dep0;
		}
	}

	int p = 1;
	for(; p <= max_depth; p <<= 1);
	fwt(p, f[u]);
	for(int k = head[u]; k; k = next[k])
	{
		int v = point[k];
		if(v != fa)
		{
			for(int i = p - 1; i; --i)
				f[v][i] = f[v][i - 1];
			f[v][0] = 0;
			fwt(p, f[v]);
			for(int i = 0; i != p; ++i)
			{
				tmp[i] = f[u][i] * f[v][i];
				f[u][i] += f[v][i];
			}
			ifwt(p, tmp);
			for(int i = 0; i != p; ++i)
				ans[i] += tmp[i];
		}
	}

	ifwt(p, f[u]);

	return max_depth + 1;
}

int main()
{
	int n;
	std::scanf("%d", &n);
	for(int i = 2; i <= n; ++i)
	{
		int fa;
		std::scanf("%d", &fa);
		add_edge(fa, i);
	}

	dfs(1, 0);
	int num = 511;
	while(!ans[num]) --num;
	for(int i = 0; i <= num; ++i)
		printf("%lld\n", ans[i]);
	return 0;
}
