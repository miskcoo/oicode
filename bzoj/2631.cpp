/* BZOJ-2631: tree
 *  LCT */
#include <cstdio>
#include <algorithm>

const int MaxV = 200001;
const unsigned mod_v = 51061;
int fa[MaxV], son[MaxV][2], rev[MaxV], size[MaxV];
unsigned multi[MaxV], add[MaxV], val[MaxV], sum[MaxV];

bool not_root(int u)
{
	int f = fa[u];
	return son[f][0] == u || son[f][1] == u;
}

void change_node(int u, unsigned mul_v, unsigned add_v)
{
	val[u] = (val[u] * mul_v + add_v) % mod_v;
	sum[u] = (sum[u] * mul_v + add_v * size[u]) % mod_v;
	multi[u] = multi[u] * mul_v % mod_v;
	add[u] = (add[u] * mul_v + add_v) % mod_v;
}

void pushdown(int u)
{
	if(rev[u]) 
	{
		rev[u] = 0;
		rev[son[u][0]] ^= 1;
		rev[son[u][1]] ^= 1;
		std::swap(son[u][0], son[u][1]);
	}

	if(multi[u] != 1 || add[u] != 0)
	{
		change_node(son[u][0], multi[u], add[u]);
		change_node(son[u][1], multi[u], add[u]);
		multi[u] = 1, add[u] = 0;
	}
}

void pushup(int u)
{
	size[u] = size[son[u][0]] + size[son[u][1]] + 1;
	sum[u] = sum[son[u][0]] + sum[son[u][1]] + val[u];
	while(sum[u] >= mod_v) sum[u] -= mod_v;
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

int main()
{
	int n, q;
	std::scanf("%d %d", &n, &q);
	for(int i = 1; i != n; ++i)
	{
		int u, v;
		std::scanf("%d %d", &u, &v);
		multi[u] = sum[u] = val[u] = 1;
		multi[v] = sum[v] = val[v] = 1;
		link(u, v);
	}

	for(int i = 0; i != q; ++i)
	{
		char op[2];
		int u, v;
		std::scanf("%s %d %d", op, &u, &v);
		if(*op == '+' || *op == '*')
		{
			unsigned c;
			std::scanf("%u", &c);
			make_root(u);
			access(v);
			splay(v);

			if(*op == '+') change_node(v, 1, c);
			else change_node(v, c, 0);
		} else if(*op == '-') {
			int p, q;
			std::scanf("%d %d", &p, &q);
			cut(u, v);
			link(p, q);
		} else {
			make_root(u);
			access(v);
			splay(v);
			std::printf("%u\n", sum[v]);
		}
	}
	return 0;
}
