/* UOJ-#3. 【NOI2014】魔法森林 
 *   LCT */
#include <cstdio>
#include <algorithm>

const int MaxV = 150010, MaxE = 100010;
int fa[MaxV], son[MaxV][2], rev[MaxV], max[MaxV], val[MaxV];

struct edge_t
{
	int w1, w2;
	int from, to;

	friend bool operator < (const edge_t& a, const edge_t& b)
	{
		return a.w1 < b.w1;
	}
} edge[MaxE];

bool not_root(int u)
{
	int f = fa[u];
	return son[f][0] == u || son[f][1] == u;
}

void pushdown(int u)
{
	if(!rev[u]) return;
	std::swap(son[u][0], son[u][1]);
	rev[u] = 0;
	rev[son[u][0]] ^= 1;
	rev[son[u][1]] ^= 1;
}

void pushup(int u)
{
	max[u] = u;
	int l = son[u][0], r = son[u][1];
	if(val[max[u]] < val[max[l]])
		max[u] = max[l];
	if(val[max[u]] < val[max[r]])
		max[u] = max[r];
}

void rotate(int u)
{
	int f = fa[u];
	int p = son[f][0] == u;

	fa[u] = fa[f];
	if(not_root(f))
		son[fa[f]][son[fa[f]][1] == f] = u;

	son[f][!p] = son[u][p];
	if(son[u][p]) fa[son[u][p]] = f;
	son[fa[f] = u][p] = f;
	pushup(f);
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
			rotate((son[f][0] == u) ^ (son[fa[f]][0] == f) ? u : f);
	}

	pushup(u);
}

void access(int u)
{
	int last = 0;
	while(u)
	{
		splay(u);
		son[u][1] = last;
		last = u;
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

	son[v][0] = fa[u] = 0;
}

int mfa[MaxV];
int find(int v)
{
	if(!mfa[v] || mfa[v] == v)
		return v;
	return mfa[v] = find(mfa[v]);
}

int main()
{
	val[0] = -2000000000;
	int N, M;
	std::scanf("%d %d", &N, &M);
	for(int i = 0; i != M; ++i)
	{
		std::scanf("%d %d", &edge[i].from, &edge[i].to);
		std::scanf("%d %d", &edge[i].w1, &edge[i].w2);
	}

	int ans = ~0u >> 1;
	std::sort(edge, edge + M);
	for(int i = 0; i != M; ++i)
	{
		int u = edge[i].from, v = edge[i].to;
		int n = i + N + 1;
		val[n] = edge[i].w2;
		if(find(u) != find(v))
		{
			link(u, n);
			link(v, n);
			mfa[find(v)] = find(u);
		} else {
			make_root(u);
			access(v);
			splay(v);
			int m = max[v];
			if(val[m] > edge[i].w2)
			{
				cut(m, edge[m - N - 1].from);
				cut(m, edge[m - N - 1].to);
				link(u, n);
				link(v, n);
			}
		}

		if(find(1) == find(N))
		{
			make_root(1);
			access(N);
			splay(N);
			ans = std::min(ans, val[max[N]] + edge[i].w1);
		}
	}

	if(find(1) == find(N))
		std::printf("%d", ans);
	else std::printf("-1");
	return 0;
}

