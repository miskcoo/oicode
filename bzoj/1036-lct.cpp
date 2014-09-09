#include <cstdio>
#include <cstring>
#include <algorithm>

template<int VSize>
class link_cut_tree
{
	int fa[VSize + 1];
	int son[VSize + 1][2];
	int val[VSize + 1];
	int max[VSize + 1];
	int sum[VSize + 1];
	bool rev[VSize + 1];

	int edge[VSize + 1][2];
public:
	void solve()
	{
		int n;
		std::scanf("%d", &n);
		for(int i = 1; i != n; ++i)
			std::scanf("%d %d", edge[i], edge[i] + 1);
		max[0] = val[0] = -100000000;
		for(int i = 1; i <= n; ++i)
		{
			int v;
			std::scanf("%d", &v);
			max[i] = sum[i] = val[i] = v;
		}

		for(int i = 1; i != n; ++i)
			link(edge[i][0], edge[i][1]);

		int q;
		std::scanf("%d", &q);
		while(q--)
		{
			char oper[7];
			int a, b;
			std::scanf("%s %d %d", oper, &a, &b);
			make_root(a);
			if(oper[1] == 'H')
			{
				val[a] = b;
				pushup(a);
			} else {
				access(b);
				splay(b);
				if(oper[1] == 'M')
					std::printf("%d\n", max[b]);
				else std::printf("%d\n", sum[b]);
			}
		}
	}

	bool not_root(int u)
	{
		int f = fa[u];
		return son[f][0] == u || son[f][1] == u;
	}

	void pushup(int u)
	{
		int sl = son[u][0], sr = son[u][1];
		max[u] = std::max(max[sl], max[sr]);
		max[u] = std::max(max[u], val[u]);
		sum[u] = sum[sl] + sum[sr] + val[u];
	}

	void pushdown(int u)
	{
		if(!rev[u]) return;
		rev[u] = 0;
		int sl = son[u][0], sr = son[u][1];
		std::swap(son[u][0], son[u][1]);
		rev[sl] ^= 1, rev[sr] ^= 1;
	}

	void rotate(int u)
	{
		int f = fa[u];
		bool p = son[f][0] == u;
		fa[u] = fa[f];
		if(not_root(f))
			son[fa[u]][son[fa[u]][0] != f] = u;

		if(!!(son[f][!p] = son[u][p]))
			fa[son[u][p]] = f;
		fa[son[u][p] = f] = u;
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
		clear_mark(u);
		for(; not_root(u); rotate(u))
		{
			int f = fa[u];
			if(not_root(f))
				rotate(((son[f][0] == u) ^ (son[fa[f]][0] == f)) ? u : f);
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
		son[v][0] = fa[u] = 0;
	}
};

link_cut_tree<30010> lct;

int main()
{
	lct.solve();
	return 0;
}
