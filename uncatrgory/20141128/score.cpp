#include <cstdio>
#include <cstring>
#include <algorithm>

const int MaxV = 10010, MaxE = MaxV * 2, MaxK = 101;
int n, k, total;
int head[MaxV], next[MaxE], point[MaxE];
int f[MaxV][MaxK], g[MaxK], weight[MaxV];

void add_edge(int u, int v)
{
	next[++total] = head[u];
	point[total] = v;
	head[u] = total;
}

void dfs(int u, int fa)
{
	using std::max;
	using std::min;
	f[u][0] = weight[u];

	for(int p = head[u]; p; p = next[p])
	{
		int v = point[p];
		if(v == fa) continue;
		dfs(v, u);
		std::memcpy(g, f[u], sizeof(g));
		for(int i = 0; i <= k; ++i)
		{
			for(int j = k - i; j <= k; ++j)
			{
				int t = min(i, min(j + 1, k));
				f[u][t] = max(f[u][t], g[i] + f[v][j]);
			}
		}
	}
}

int main()
{
	std::freopen("score.in", "r", stdin);
	std::freopen("score.out", "w", stdout);
	std::scanf("%d %d", &n, &k);
	for(int i = 1; i <= n; ++i)
		std::scanf("%d", weight + i);
	for(int i = 1; i != n; ++i)
	{
		int u, v;
		std::scanf("%d %d", &u, &v);
		add_edge(u, v);
		add_edge(v, u);
	}

	dfs(1, 0);
	int ans = 0;
	for(int i = 0; i <= k; ++i)
		ans = std::max(ans, f[1][i]);
	std::printf("%d", ans);
	return 0;
}
