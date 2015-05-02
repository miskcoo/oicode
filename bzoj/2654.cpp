/* BZOJ-2654: tree
 *   二分+最小生成树 */
#include <cstdio>
#include <algorithm>

const int MaxM = 100010;
struct edge_t
{
	int u, v, w, c;
} tmp[MaxM], edge[MaxM];
int white_delta, weight_sum, white_count;
int fa[MaxM];

bool compare(const edge_t& a, const edge_t& b)
{
	if(a.w == b.w) return a.c < b.c;
	return a.w < b.w;
}

int find(int x)
{
	if(fa[x] == x) return x;
	return fa[x] = find(fa[x]);
}

void solve_mst(int n, int m)
{
	for(int i = 0; i <= n; ++i) fa[i] = i;
	for(int i = 0; i != m; ++i)
	{
		edge[i] = tmp[i];
		if(edge[i].c == 0) edge[i].w += white_delta;
	}
	std::sort(edge, edge + m, compare);
	white_count = weight_sum = 0;
	int connected = n;
	for(int i = 0; i != m; ++i)
	{
		int u = find(edge[i].u), v = find(edge[i].v);
		if(u != v)
		{
			fa[u] = v;
			weight_sum += edge[i].w;
			white_count += 1 - edge[i].c;
			if(--connected == 1) return;
		}
	}
}

int main()
{
	int n, m, k;
	std::scanf("%d %d %d", &n, &m, &k);
	for(int i = 0; i != m; ++i)
	{
		std::scanf("%d %d %d %d", &tmp[i].u, &tmp[i].v, &tmp[i].w, &tmp[i].c);
		++tmp[i].u, ++tmp[i].v;
	}
	int l = -110, r = 110, ans = ~0u >> 1;
	while(l <= r)
	{
		white_delta = (l + r) / 2;
		solve_mst(n, m);
		if(white_count < k) r = white_delta - 1;
		else l = white_delta + 1, ans = weight_sum - white_delta * k;
	}
	std::printf("%d", ans);
	return 0;
}
