/* BZOJ-3223: Tyvj 1729 文艺平衡树 
 *  Splay */
#include <cstdio>
#include <algorithm>

const int MaxN = 100005;
int N, M;
int root = 1, total;
int rev[MaxN], son[MaxN][2], fa[MaxN], size[MaxN], value[MaxN];

bool not_root(int u)
{
	int f = fa[u];
	return son[f][1] == u || son[f][0] == u;
}

void pushup(int u)
{
	size[u] = size[son[u][0]] + size[son[u][1]] + 1;
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
	fa[son[u][p] = f] = u;

	pushup(f);
}

void pushdown(int u)
{
	if(rev[u])
	{
		rev[u] = 0;
		std::swap(son[u][0], son[u][1]);
		rev[son[u][0]] ^= 1;
		rev[son[u][1]] ^= 1;
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
			rotate((son[f][0] == u) ^ (son[fa[f]][0] == f) ? u : f);
	}

	pushup(u);
	root = u;
}

int find_kth(int k)
{
	int u = root;
	while(u)
	{
		pushdown(u);
		if(k > size[son[u][0]])
		{
			k -= size[son[u][0]] + 1;
			if(k == 0) return u;
			u = son[u][1];
		} else {
			u = son[u][0];
		}
	}

	return -1;
}

void print(int now)
{
	pushdown(now);
	if(son[now][0]) print(son[now][0]);
	if(value[now]) std::printf("%d ", value[now]);
	if(son[now][1]) print(son[now][1]);
}

int main()
{
	std::scanf("%d %d", &N, &M);
	for(int i = 0; i <= N; ++i)
	{
		size[++total] = N - i + 2;
		value[total] = i;
		son[total][1] = total + 1;
		fa[total] = total - 1;
	}

	++total;
	fa[total] = total - 1;
	size[total] = 1;

	for(int i = 0; i != M; ++i)
	{
		int a, b;
		std::scanf("%d %d", &a, &b);

		if(a == b) continue;

		int ppa = find_kth(a); // prev
		splay(ppa);
		int pa = son[ppa][1];
		fa[root = pa] = 0;

		int nnb = find_kth(b - size[son[ppa][0]] + 1); // next
		splay(nnb);
		rev[son[nnb][0]] ^= 1;

		son[fa[root] = ppa][1] = root;
		root = ppa;
	}

	print(root);
	return 0;
}
