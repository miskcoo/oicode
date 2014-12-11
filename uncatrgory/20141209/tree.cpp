#include <cstdio>
#include <algorithm>

const unsigned mod_v = 51061;
const int MaxN = 100010;
int fa[MaxN], son[MaxN][2], rev[MaxN], size[MaxN];
unsigned plus[MaxN], multi[MaxN], sum[MaxN], val[MaxN];

bool not_root(int u)
{
	int f = fa[u];
	return son[f][0] == u || son[f][1] == u;
}

void update_node(int u, unsigned add_v, unsigned mul_v)
{
	val[u] = (val[u] * mul_v + add_v) % mod_v;
	sum[u] = (sum[u] * mul_v + add_v * size[u]) % mod_v;
	plus[u] = (plus[u] * mul_v + add_v) % mod_v;
	multi[u] = multi[u] * mul_v % mod_v;
}

void pushup(int u)
{
	size[u] = size[son[u][0]] + size[son[u][1]] + 1;
	sum[u] = sum[son[u][0]] + sum[son[u][1]] + val[u];
	while(sum[u] >= mod_v) sum[u] -= mod_v;
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

	if(multi[u] != 1 || plus[u] != 0)
	{
		update_node(son[u][0], plus[u], multi[u]);
		update_node(son[u][1], plus[u], multi[u]);
		plus[u] = 0, multi[u] = 1;
	}
}

void clear_mark(int u)
{
	if(not_root(u))
		clear_mark(fa[u]);
	pushdown(u);
}

void rotate(int u)
{
	int f = fa[u];
	int p = son[f][0] == u;

	fa[u] = fa[f];
	if(not_root(f))
		son[fa[f]][son[fa[f]][1] == f] = u;
	son[f][!p] = son[u][p];
	if(son[u][p])
		fa[son[u][p]] = f;
	son[fa[f] = u][p] = f;

	pushup(f);
}

void splay(int u)
{
	for(clear_mark(u); not_root(u); rotate(u))
	{
		int f = fa[u];
		if(!not_root(f))
			continue;
		rotate((son[f][0] == u) ^ (son[fa[f]][0] == f) ? u : f);
	}

	pushup(u);
}

void access(int u)
{
	int pre_link = 0;
	while(u)
	{
		splay(u);
		son[u][1] = pre_link;
		pre_link = u;
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

int main()
{
	std::freopen("tree.in", "r", stdin);
	std::freopen("tree.out", "w", stdout);
	int n, q;
	std::scanf("%d %d", &n, &q);
	for(int i = 1; i <= n; ++i)
		multi[i] = val[i] = sum[i] = 1;
	for(int i = 1; i != n; ++i)
	{
		int u, v;
		std::scanf("%d %d", &u, &v);
		link(u, v);
	}

	for(int i = 0; i != q; ++i)
	{
		char oper[3];
		std::scanf("%s", oper);
		if(*oper == '+' || *oper == '*')
		{
			int u, v;
			unsigned c;
			std::scanf("%d %d %u", &u, &v, &c);
			make_root(u);
			access(v);
			splay(v);
			if(*oper == '+')
				update_node(v, c, 1);
			else update_node(v, 0, c);
		} else if(*oper == '-') {
			int u1, v1, u2, v2;
			std::scanf("%d %d %d %d", &u1, &v1, &u2, &v2);
			cut(u1, v1);
			link(u2, v2);
		} else {
			int u, v;
			std::scanf("%d %d", &u, &v);
			make_root(u);
			access(v);
			splay(v);
			std::printf("%u\n", sum[v]);
		}
	}
	return 0;
}
