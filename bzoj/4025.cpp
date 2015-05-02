/* BZOJ-4025: 二分图
 *   LCT维护奇环 */
#include <cstdio>
#include <vector>
#include <algorithm>

const int MaxN = 300010, MaxT = 100010;
int n, m, t, ans;
int fa[MaxN], son[MaxN][2];
int rev[MaxN], size[MaxN], time[MaxN], val[MaxN];
int incircle[MaxN], outside[MaxN];
struct edge_t
{
	int u, v, l, r;
} edge[MaxN];

struct graph_t
{
	int total;
	int head[MaxT], point[MaxN], next[MaxN];

	void add(int u, int v)
	{
		point[++total] = v;
		next[total] = head[u];
		head[u] = total;
	}
} edge_insert, edge_delete;

int min(int a, int b)
{
	if(edge[a].r < edge[b].r)
		return a;
	return b;
}

bool not_root(int u)
{
	int f = fa[u];
	return son[f][0] == u || son[f][1] == u;
}

void pushup(int u)
{
	val[u] = min(val[son[u][0]], val[son[u][1]]);
	val[u] = min(val[u], time[u]);
	size[u] = size[son[u][0]] + size[son[u][1]] + 1;
}

void rotate(int u)
{
	int f = fa[u];
	int p = son[f][0] == u;
	son[f][!p] = son[u][p];
	if(son[u][p]) fa[son[u][p]] = f;
	if(not_root(f)) son[fa[f]][son[fa[f]][1] == f] = u;
	fa[u] = fa[f];
	son[fa[f] = u][p] = f;
	pushup(f);
}

void pushdown(int u)
{
	if(rev[u])
	{
		std::swap(son[u][0], son[u][1]);
		rev[son[u][0]] ^= 1;
		rev[son[u][1]] ^= 1;
		rev[u] = 0;
	}
}

void clear_mark(int u)
{
	if(not_root(u))
		clear_mark(fa[u]);
	pushdown(u);
}

void splay(int u)
{
	for(clear_mark(u); not_root(u); rotate(u))
	{
		int f = fa[u];
		if(not_root(f))
			rotate((son[fa[f]][0] == f) ^ (son[f][0] == u) ? u : f);
	}

	pushup(u);
}

void access(int u)
{
	int prev = 0;
	while(u)
	{
		splay(u);
		son[u][1] = prev;
		prev = u;
		u = fa[u];
	}
}

void make_root(int u)
{
	access(u);
	splay(u);
	rev[u] ^= 1;
}

void link(int u, int v)
{
	make_root(u);
	fa[u] = v;
}

void cut(int u, int v)
{
	make_root(u);
	access(v);
	splay(v);
	fa[u] = son[v][0] = 0;
}

int find(int u)
{
	while(not_root(u))
		u = fa[u];
	return u;
}

void insert_edge(int i)
{
	int u = edge[i].u, v = edge[i].v;
	make_root(u);
	access(v);
	if(find(u) != find(v))
	{
		link(i + n, u);
		link(i + n, v);
	} else {
		splay(v);
		int m = val[v];
		if(edge[i].r > edge[m].r)
		{
			cut(edge[m].u, m + n);
			cut(m + n, edge[m].v);
			link(i + n, u);
			link(i + n, v);
		} else m = i;

		u = edge[m].u, v = edge[m].v;
		make_root(u);
		access(v);
		splay(v);
		if(((size[v] + 1) >> 1) & 1)
			incircle[m] = 1;
		else incircle[m] = 0;
		ans += incircle[m];
		outside[m] = 1;
	}
}

void delete_edge(int i)
{
	if(outside[i])
	{
		outside[i] = 0;
		ans -= incircle[i];
	} else {
		cut(edge[i].u, i + n);
		cut(i + n, edge[i].v);
	}
}

int main()
{
	edge[0].r = ~0u >> 1;
	std::scanf("%d %d %d", &n, &m, &t);
	for(int i = 1; i <= m; ++i)
		std::scanf("%d %d %d %d", &edge[i].u, &edge[i].v, &edge[i].l, &edge[i].r);
	for(int i = 1; i <= m; ++i)
	{
		edge_insert.add(edge[i].l, i);
		edge_delete.add(edge[i].r, i);
		val[i + n] = time[i + n] = i;
	}

	for(int i = 1; i <= n + m; ++i)
		size[i] = 1;
	for(int i = 0; i < t; ++i)
	{
		for(int k = edge_insert.head[i]; k; k = edge_insert.next[k])
			insert_edge(edge_insert.point[k]);
		for(int k = edge_delete.head[i]; k; k = edge_delete.next[k])
			delete_edge(edge_delete.point[k]);
		std::puts(ans ? "No" : "Yes");
	}
	return 0;
}
