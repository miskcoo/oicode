/* BZOJ-1787: [Ahoi2008]Meet 紧急集合
 *  倍增LCA */
#include <cstdio>
#include <algorithm>

const int MaxN = 500010, MaxL = 20;
int total, head[MaxN], point[MaxN << 1], next[MaxN << 1];
int que[MaxN], dist[MaxL][MaxN], depth[MaxN];

void add_edge(int u, int v)
{
	point[++total] = v;
	next[total] = head[u];
	head[u] = total;
}

void make_father(int n)
{
	int qhead = 0, qtail = 0;
	que[qtail++] = 1;
	while(qhead != qtail)
	{
		int u = que[qhead++];
		for(int k = head[u]; k; k = next[k])
		{
			int v = point[k];
			if(dist[0][v] || v == 1) continue;
			dist[0][v] = u;
			depth[v] = depth[u] + 1;
			que[qtail++] = v;
		}
	}
}

void make_lca(int n)
{
	for(int i = 1; i != MaxL; ++i)
	{
		for(int j = 1; j <= n; ++j)
			dist[i][j] = dist[i - 1][dist[i - 1][j]];
	}
}

int get_lca(int u, int v)
{
	if(depth[u] < depth[v])
		std::swap(u, v);
	int diff = depth[u] - depth[v];
	for(int k = 0; diff; ++k, diff >>= 1)
		if(diff & 1) u = dist[k][u];

	for(int p = MaxL - 1; u != v; p ? --p : 0)
	{
		if(dist[p][u] != dist[p][v] || p == 0)
		{
			u = dist[p][u];
			v = dist[p][v];
		}
	}

	return u;
}

int main()
{
	int n, m;
	std::scanf("%d %d", &n, &m);
	for(int i = 1; i != n; ++i)
	{
		int u, v;
		std::scanf("%d %d", &u, &v);
		add_edge(u, v);
		add_edge(v, u);
	}

	make_father(n);
	make_lca(n);

	for(int i = 0; i != m; ++i)
	{
		int a, b, c;
		std::scanf("%d %d %d", &a, &b, &c);
		int ab = get_lca(a, b);
		int ac = get_lca(a, c);
		int bc = get_lca(b, c);
		int abc = get_lca(ab, c);

		int d = depth[a] + depth[b] + depth[c] - depth[abc] * 2;
		int d1 = d - depth[ab];
		int d2 = d - depth[ac];
		int d3 = d - depth[bc];

		if(d1 < d2 && d1 < d2)
			std::printf("%d %d\n", ab, d1);
		else if(d2 < d1 && d2 < d3)
			std::printf("%d %d\n", ac, d2);
		else std::printf("%d %d\n", bc, d3);
	}
	return 0;
}
