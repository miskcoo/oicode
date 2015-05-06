/* BZOJ-1682: [Usaco2005 Mar]Out of Hay 干草危机
 *   最小瓶颈生成树 */
#include <cstdio>
#include <algorithm>

const int MaxN = 2010, MaxM = 10010;
struct edge_t
{
	int u, v, w;
	bool operator < (const edge_t& e) const
	{
		return w < e.w;
	}
} e[MaxM];

int fa[MaxN], size[MaxN];

int find(int v)
{
	if(fa[v] == v) return v;
	return fa[v] = find(fa[v]);
}

int main()
{
	int n, m;
	std::scanf("%d %d", &n, &m);
	for(int i = 1; i <= n; ++i)
		fa[i] = i, size[i] = 1;
	for(int i = 0; i != m; ++i)
		std::scanf("%d %d %d", &e[i].u, &e[i].v, &e[i].w);
	std::sort(e, e + m);

	int unconnected = n;
	for(int i = 0; i != m; ++i)
	{
		int u = find(e[i].u), v = find(e[i].v);
		if(u != v)
		{
			if(size[u] > size[v])
				std::swap(u, v);
			fa[u] = fa[v];
			size[v] += size[u];
			if(--unconnected == 1)
			{
				std::printf("%d", e[i].w);
				break;
			}
		}
	}
	return 0;
}
