/* BZOJ-2049: [Sdoi2008]Cave 洞穴勘测
 *   LCT维护联通性 */
#include <cstdio>
#include <algorithm>

const int MaxN = 10010;
int fa[MaxN], son[MaxN][2], rev[MaxN];

bool not_root(int u)
{
	int f = fa[u];
	return son[f][0] == u || son[f][1] == u;
}

void rotate(int u)
{
	int f = fa[u];
	int p = son[f][0] == u;

	fa[u] = fa[f];
	if(not_root(f)) son[fa[f]][son[fa[f]][1] == f] = u;
	if(son[u][p]) fa[son[u][p]] = f;
	son[f][!p] = son[u][p];
	fa[son[u][p] = f] = u;
}

void pushdown(int u)
{
	if(rev[u])
	{
		rev[u] ^= 1;
		rev[son[u][0]] ^= 1;
		rev[son[u][1]] ^= 1;
		std::swap(son[u][0], son[u][1]);
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
	son[v][0] = fa[u] = 0;
}

int main()
{
	int n, m;
	std::scanf("%d %d", &n, &m);
	for(int i = 0; i != m; ++i)
	{
		char str[10];
		int a, b;
		std::scanf("%s %d %d", str, &a, &b);
		switch(*str)
		{
		case 'Q':
			make_root(a);
			access(b);
			splay(b);
			while(a && b != a) 
				a = fa[a];
			if(b == a) std::puts("Yes");
			else std::puts("No");
			break;
		case 'C':
			link(a, b);
			break;
		case 'D':
			cut(a, b);
			break;
		}
	}
	return 0;
}
