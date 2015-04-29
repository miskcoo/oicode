/* BZOJ-1468: Tree
 *   点分治  */
#include <cstdio>
#include <algorithm>

const int MaxN = 40010, MaxM = MaxN << 1;
int total, K;
int head[MaxN], point[MaxM], next[MaxM], weight[MaxM];
int que[MaxN], mark[MaxN], dist[MaxN], fa[MaxN];
int size[MaxN], max_size[MaxN], record[MaxN];

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
	fa[u] = 0, que[qtail++] = u;
	while(qhead != qtail)
	{
		int u = que[qhead++];
		size[u] = max_size[u] = 0;
		for(int k = head[u]; k; k = next[k])
		{
			int v = point[k];
			if(fa[u] != v && !mark[v])
			{
				fa[v] = u;
				que[qtail++] = v;
			}
		}
	}

	return qtail;
}

int calc(int u, int d = 0)
{
	int tot = bfs_sort(u);
	dist[u] = d;
	for(int i = 0; i != tot; ++i)
	{
		int u = que[i];
		record[i] = dist[u];
		for(int k = head[u]; k; k = next[k])
		{
			int v = point[k];
			if(!mark[v] && fa[u] != v)
				dist[v] = dist[u] + weight[k];
		}
	}

	std::sort(record, record + tot);
	int ans = 0, r = tot - 1;
	for(int l = 0; l < r; ++l)
	{
		while(l < r && record[l] + record[r] > K) --r;
		ans += r - l;
	}

	return ans;
}

int divide(int now)
{
	int tot = bfs_sort(now);
	for(int i = tot - 1; i >= 0; --i)
	{
		int u = que[i];
		size[u] += 1;
		size[fa[u]] += size[u];
		if(max_size[fa[u]] < size[u])
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
	int ans = calc(root);
	for(int k = head[root]; k; k = next[k])
	{
		int v = point[k];
		if(!mark[v]) ans -= calc(v, weight[k]);
	}

	for(int k = head[root]; k; k = next[k])
	{
		int v = point[k];
		if(!mark[v]) ans += divide(v);
	}

	return ans;
}

int main()
{
	int n;
	std::scanf("%d", &n);
	for(int i = 1; i != n; ++i)
	{
		int u, v, w;
		std::scanf("%d %d %d", &u, &v, &w);
		add_edge(u, v, w);
		add_edge(v, u, w);
	}
	std::scanf("%d", &K);
	int ans = divide(1);
	std::printf("%d", ans);
	return 0;
}
