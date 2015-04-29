/* BZOJ-2599: [IOI2011]Race
 *   点分治  */
#include <cstdio>

const int MaxN = 200010, MaxM = MaxN * 2, MaxK = 1000010;
int total, lazy_v, K, ans = ~0u >> 1;
int head[MaxN], point[MaxM], next[MaxM], weight[MaxM];
int que[MaxN], fa[MaxN], mark[MaxN], dist[MaxN], depth[MaxN];
int lazy[MaxK], record[MaxK], size[MaxN], max_size[MaxN];

void add_edge(int u, int v, int w)
{
	point[++total] = v;
	weight[total] = w;
	next[total] = head[u];
	head[u] = total;
}

int bfs_sort(int u)
{
	int qhead = 0, qtail = 0;
	que[qtail++] = u; fa[u] = 0;
	while(qhead != qtail)
	{
		int u = que[qhead++];
		size[u] = max_size[u] = 0;
		for(int k = head[u]; k; k = next[k])
		{
			int v = point[k];
			if(!mark[v] && fa[u] != v)
			{
				fa[v] = u;
				que[qtail++] = v;
			}
		}
	}

	return qtail;
}

void divide(int now)
{
	int tot = bfs_sort(now);
	for(int i = tot - 1; i >= 0; --i)
	{
		int u = que[i];
		size[u] += 1;
		size[fa[u]] += size[u];
		if(size[u] > max_size[fa[u]])
			max_size[fa[u]] = size[u];
	}

	int root, root_size = ~0u >> 1;
	for(int i = 0; i != tot; ++i)
	{
		int u = que[i];
		if(max_size[u] < size[now] - size[u])
			max_size[u] = size[now] - size[u];
		if(root_size > max_size[u])
		{
			root = u;
			root_size = max_size[u];
		}
	}

	mark[root] = 1;
	lazy[0] = ++lazy_v; 
	for(int k = head[root]; k; k = next[k])
	{
		int v = point[k];
		if(mark[v]) continue;
		int tot = bfs_sort(v);
		dist[v] = weight[k];
		depth[v] = 1;
		for(int i = 0; i != tot; ++i)
		{
			int u = que[i], d = dist[u];
			if(d <= K && lazy[K - d] == lazy_v && record[K - d] + depth[u] < ans)
				ans = record[K - dist[u]] + depth[u];
			for(int k = head[u]; k; k = next[k])
			{
				int v = point[k];
				if(!mark[v] && fa[u] != v)
				{
					depth[v] = depth[u] + 1;
					dist[v] = dist[u] + weight[k];
				}
			}
		}

		for(int i = 0; i != tot; ++i)
		{
			int u = que[i];
			if(dist[u] <= K)
			{
				int d = dist[u];
				if(lazy[d] != lazy_v)
				{
					record[d] = depth[u];
					lazy[d] = lazy_v;
				} else if(record[d] > depth[u]) {
					record[d] = depth[u];
				}
			}
		}
	}

	for(int k = head[root]; k; k = next[k])
	{
		int v = point[k];
		if(!mark[v]) divide(v);
	}
}

int main()
{
	int n;
	std::scanf("%d %d", &n, &K);
	for(int i = 1; i != n; ++i)
	{
		int u, v, w;
		std::scanf("%d %d %d", &u, &v, &w);
		add_edge(u + 1, v + 1, w);
		add_edge(v + 1, u + 1, w);
	}

	divide(1);
	if(ans > n) std::puts("-1");
	else std::printf("%d", ans);
	return 0;
}
