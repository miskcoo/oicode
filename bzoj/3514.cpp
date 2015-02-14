/* BZOJ-3514: Codechef MARCH14 GERALD07加强版
 *  LCT + 函数式线段树 */
#include <cstdio>
#include <algorithm>

const int MaxN = 400010, MaxM = 5000010;
int fa[MaxN], son[MaxN][2], rev[MaxN], from[MaxN], point[MaxN];
int mfa[MaxN], ntr[MaxN], min_v[MaxN], val[MaxN];
int root[MaxN], used;
struct node_t
{
	int son[2], val;
} node[MaxM];

/* union-find set */
int find(int u)
{
	if(!mfa[u] || mfa[u] == u)
		return u;
	return mfa[u] = find(mfa[u]);
}

/* splay tree */
bool not_root(int u)
{
	int f = fa[u];
	return son[f][0] == u || son[f][1] == u;
}

void pushdown(int u)
{
	if(!rev[u]) return;
	rev[u] = 0;
	rev[son[u][0]] ^= 1; 
	rev[son[u][1]] ^= 1;
	std::swap(son[u][0], son[u][1]);
}

void clear_mark(int u)
{
	if(not_root(u))
		clear_mark(fa[u]);
	pushdown(u);
}

void pushup(int u)
{
	min_v[u] = std::min(min_v[son[u][0]], min_v[son[u][1]]);
	min_v[u] = std::min(min_v[u], val[u]);
}

void rotate(int u)
{
	int f = fa[u];
	int p = son[f][0] == u;

	if(not_root(f))
		son[fa[f]][son[fa[f]][1] == f] = u;
	fa[u] = fa[f];
	son[f][!p] = son[u][p];
	if(son[u][p]) fa[son[u][p]] = f;
	fa[son[u][p] = f] = u;
	pushup(f);
}

void splay(int u)
{
	for(clear_mark(u); not_root(u); rotate(u))
	{
		int f = fa[u];
		if(not_root(f))
			rotate(((son[f][0] == u) ^ (son[fa[f]][0] == f)) ? u : f);
	}

	pushup(u);
}

/* link-cut tree */
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
	son[v][0] = fa[u] = 0;
}

/* functional segment tree */
int insert_element(int prev, int head, int tail, int p)
{
	int now = ++used;
	node[now] = node[prev];
	++node[now].val;
	if(head == tail) return now;

	int m = (head + tail) >> 1;
	if(p <= m) node[now].son[0] = insert_element(node[prev].son[0], head, m, p);
	else node[now].son[1] = insert_element(node[prev].son[1], m + 1, tail, p);

	node[now].val = node[node[now].son[0]].val + node[node[now].son[1]].val;
	return now;
}

int ask(int l, int r, int head, int tail, int v)
{
	if(tail <= v) return node[r].val - node[l].val;

	int m = (head + tail) >> 1;
	if(v <= m) return ask(node[l].son[0], node[r].son[0], head, m, v);
	return ask(node[l].son[1], node[r].son[1], m + 1, tail, v)
		+ ask(node[l].son[0], node[r].son[0], head, m, v);
}

int main()
{
	int n, m, k, type;
	std::scanf("%d %d %d %d", &n, &m, &k, &type);
	for(int i = 0; i <= n; ++i)
		val[i] = min_v[i] = ~0u >> 1;
	for(int i = 1; i <= m; ++i)
	{
		int u, v;
		std::scanf("%d %d", &u, &v);
		if(u == v) 
		{
			ntr[i] = i;
			continue;
		}
		min_v[i + n] = val[i + n] = i;
		from[i] = u, point[i] = v;

		int mu = find(u), mv = find(v);
		if(mu != mv)
		{
			mfa[mu] = mv;
			ntr[i] = 0;
		} else {
			make_root(u);
			access(v);
			splay(v);

			int node = min_v[v];
			cut(n + node, from[node]);
			cut(n + node, point[node]);
			ntr[i] = node;
		}

		link(u, i + n);
		link(v, i + n);
	}

	for(int i = 1; i <= m; ++i)
		root[i] = insert_element(root[i - 1], 0, m, ntr[i]);

	int lastans = 0;
	for(int i = 0; i != k; ++i)
	{
		int l, r;
		std::scanf("%d %d", &l, &r);
		if(type) 
		{
			l ^= lastans;
			r ^= lastans;
		}

		if(l == 1 && r == m)
			lastans = 1;
		else lastans = n - ask(root[l - 1], root[r], 0, m, l - 1);
		std::printf("%d\n", lastans);
	}

	return 0;
}
