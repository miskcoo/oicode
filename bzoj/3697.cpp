/* BZOJ-3697: 采药人的路径
 *   树分治 */
#include <cstdio>

const int MaxN = 100010, MaxM = 200010;
int total, root, root_size, base, max_depth;
int head[MaxN], point[MaxM], weight[MaxM], next[MaxM];
int size[MaxN], max_size[MaxN], mark[MaxN];
int depth[MaxN], dist[MaxN], sum[MaxM];
long long ans, f[MaxM][2], g[MaxM][2];

void add_edge(int u, int v, int w)
{
	point[++total] = v;
	weight[total] = w;
	next[total] = head[u];
	head[u] = total;
}

void dfs_size(int u, int fa)
{
	max_size[u] = size[u] = 1;
	for(int k = head[u]; k; k = next[k])
	{
		int v = point[k];
		if(!mark[v] && v != fa)
		{
			dfs_size(v, u);
			size[u] += size[v];
			if(size[v] > max_size[u])
				max_size[u] = size[v];
		}
	}
}

void dfs_root(int u, int fa, int r)
{
	if(max_size[u] < size[r] - size[u])
		max_size[u] = size[r] - size[u];
	if(root_size > max_size[u])
	{
		root = u;
		root_size = max_size[u];
	}

	for(int k = head[u]; k; k = next[k])
	{
		int v = point[k];
		if(!mark[v] && v != fa)
			dfs_root(v, u, r);
	}
}

void dfs(int u, int fa)
{
	int z = dist[u] + base;
	if(sum[z]) ++f[z][1];
	else ++f[z][0];

	++sum[z];
	for(int k = head[u]; k; k = next[k])
	{
		int v = point[k];
		if(!mark[v] && v != fa)
		{
			dist[v] = dist[u] + weight[k];
			depth[v] = depth[u] + 1;
			if(depth[v] > max_depth)
				max_depth = depth[v];
			dfs(v, u);
		}
	}

	--sum[z];
}

void divide(int now)
{
	root_size = ~0u >> 1;
	dfs_size(now, 0);
	dfs_root(now, 0, now);
	mark[root] = 1;
	g[base][0] = 1;

	int now_depth = 1;
	for(int k = head[root]; k; k = next[k])
	{
		int v = point[k];
		if(mark[v]) continue;
		max_depth = 1;
		depth[v] = 1; dist[v] = weight[k];
		dfs(v, root);
		if(max_depth > now_depth)
			now_depth = max_depth;
		ans += f[base][0] * (g[base][0] - 1);
		for(int i = -max_depth; i <= max_depth; ++i)
		{
			ans += g[base + i][0] * f[base - i][1]
				+  g[base + i][1] * f[base - i][0]
				+  g[base + i][1] * f[base - i][1];
		}
		for(int i = base - max_depth; i <= base + max_depth; ++i)
		{
			g[i][0] += f[i][0];
			g[i][1] += f[i][1];
			f[i][0] = f[i][1] = 0;
		}
	}

	for(int i = base - now_depth; i <= base + now_depth; ++i)
		g[i][0] = g[i][1] = 0;

	for(int k = head[root]; k; k = next[k])
	{
		if(!mark[point[k]])
			divide(point[k]);
	}
}

int main()
{
	int n;
	std::scanf("%d", &n);
	base = n;
	for(int i = 1; i != n; ++i)
	{
		int u, v, w;
		std::scanf("%d %d %d", &u, &v, &w);
		if(!w) w = -1;
		add_edge(u, v, w);
		add_edge(v, u, w);
	}

	divide(1);
	std::printf("%lld", ans);
	return 0;
}
