/* BZOJ-1827: [Usaco2010 Mar]gather 奶牛大集会
     树型DP */
#include <cstdio>
#include <cstring>
#include <algorithm>

const int MaxN = 100010, MaxE = 200010;
int total;
int que[MaxN], deg[MaxN], fa[MaxN], len[MaxN];
int head[MaxN], point[MaxE], next[MaxE], weight[MaxE];
long long down[MaxN], son[MaxN], up[MaxN];

void add_edge(int u, int v, int w)
{
	point[++total] = v;
	weight[total] = w;
	next[total] = head[u];
	head[u] = total;
	++deg[v];
}

void topology_sort(int n)
{
	int qhead = 0, qtail = 0;
	for(int u = 1; u <= n; ++u)
	{
		if(deg[u] == 1)
		{
			deg[u] = 0;
			que[qtail++] = u;
		}
	}

	while(qhead != qtail)
	{
		int u = que[qhead++];
		deg[u] = 0;
		for(int k = head[u]; k; k = next[k])
		{
			int v = point[k];
			if(!deg[v]) continue;
			fa[u] = v;
			len[u] = weight[k];
			son[v] += son[u];
			down[v] += son[u] * weight[k] + down[u];

			if(--deg[v] == 1)
				que[qtail++] = v;

			break;
		}
	}
}

void solve_up(int n)
{
	int r = que[n - 1];
	for(int i = n - 1; i; --i)
	{
		int u = que[i - 1];
		up[u] = up[fa[u]] + (son[r] - son[u]) * len[u]
			+ down[fa[u]] - down[u] - len[u] * son[u];
	}
}

int main()
{
	int n;
	std::scanf("%d", &n);
	for(int i = 1; i <= n; ++i)
		std::scanf("%lld", son + i);
	for(int i = 1; i != n; ++i)
	{
		int u, v, w;
		std::scanf("%d %d %d", &u, &v, &w);
		add_edge(u, v, w);
		add_edge(v, u, w);
	}

	topology_sort(n);
	solve_up(n);

	long long ans = ~0llu >> 1;
	for(int i = 1; i <= n; ++i)
		ans = std::min(ans, up[i] + down[i]);
	std::printf("%lld", ans);
	return 0;
}
