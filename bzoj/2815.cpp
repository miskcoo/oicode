/* BZOJ-2815: [ZJOI2012]灾难
 *  LCA，拓扑排序 */
#include <cstdio>
#include <cstring>
#include <algorithm>

const int MaxN = 66000, MaxE = 1000000;
int total;
int head[MaxN], next[MaxE], point[MaxE], depth[MaxN];
int dist[MaxN][17], deg[MaxN], que[MaxN], fa[MaxN];
int size[MaxN];

void add_edge(int u, int v)
{
	point[++total] = v;
	next[total] = head[u];
	++deg[v];
	head[u] = total;
}

int getlca(int u, int v)
{
	int du = depth[u], dv = depth[v];
	if(du < dv) std::swap(du, dv), std::swap(u, v);
	for(int i = 0, d = du - dv; d; ++i, d >>= 1)
		if(d & 1) u = dist[u][i];
		
	if(u == v) return u;
	
	for(int p = 16; u != v; p ? --p : 0)
	{
		if(dist[v][p] != dist[u][p] || p == 0)
		{
			u = dist[u][p];
			v = dist[v][p];
		}
	}
	
	return u;
}

void topology_sort(int n)
{
	int qhead = 0, qtail = 0;
	for(int i = 1; i <= n; ++i)
	{
		if(!deg[i])
		{
			que[qtail++] = i;
			depth[i] = 1;
		}
	}
	
	while(qhead != qtail)
	{
		int u = que[qhead++];
		dist[u][0] = fa[u];
		for(int i = 1; i <= 16; ++i)
			dist[u][i] = dist[dist[u][i - 1]][i - 1];
		
		for(int k = head[u]; k; k = next[k])
		{
			int v = point[k];
			if(fa[v] == 0) 
			{
				fa[v] = u;
			} else {
				fa[v] = getlca(fa[v], u);
				if(fa[v] == 0) fa[v] = -1;
			}
			
			if(--deg[v] == 0)
			{
				que[qtail++] = v;
				depth[v] = depth[fa[v]] + 1;
			}
		}
	}
}

void topology_sort2(int n)
{
	int qhead = 0, qtail = 0;
	for(int i = 1; i <= n; ++i)
	{
		if(!deg[i])
		{
			que[qtail++] = i;
			size[i] = 1;
		}
	}
	
	while(qhead != qtail)
	{
		int u = que[qhead++];
		
		for(int k = head[u]; k; k = next[k])
		{
			int v = point[k];
			size[v] += size[u];
			
			if(--deg[v] == 0)
			{
				que[qtail++] = v;
				++size[v];
			}
		}
	}
}

int main()
{
	int n;
	std::scanf("%d", &n);
	for(int i = 1; i <= n; ++i)
	{
		int v;
		while(std::scanf("%d", &v), v)
			add_edge(v, i);
	}
	
	topology_sort(n);
	std::memset(head, 0, sizeof(head));
	std::memset(deg, 0, sizeof(deg));
	
	for(int i = 1; i <= n; ++i)
		if(fa[i] > 0) add_edge(i, fa[i]);
	
	topology_sort2(n);
	for(int i = 1; i <= n; ++i)
		std::printf("%d\n", size[i] - 1);
	return 0;
}
