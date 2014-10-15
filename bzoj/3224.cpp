/* BZOJ-3224: Tyvj 1728 普通平衡树
 *  Splay */
#include <cstdio>

template<int Size>
class splay_tree
{
	int total;
	int fa[Size];
	int son[Size][2];
	int size[Size];
	int value[Size];
	int count[Size];
	int root;
public:
	void solve()
	{
		value[++total] = -99999999;
		count[total] = 1;
		root = total;

		int N;
		std::scanf("%d", &N);
		for(int i = 0; i != N; ++i)
		{
			int opt, x;
			std::scanf("%d %d", &opt, &x);
			switch(opt)
			{
			case 1:
				insert(x);
				break;
			case 2:
				remove(x);
				break;
			case 3:
				std::printf("%d\n", find_rank(x) - 1);
				break;
			case 4:
				std::printf("%d\n", find_kth(x + 1));
				break;
			case 5:
				opt = find_prev(x);
				splay(opt);
				std::printf("%d\n", value[opt]);
				break;
			case 6:
				opt = find_next(x);
				splay(opt);
				std::printf("%d\n", value[opt]);
				break;
			}
		}
	}

	void pushup(int u)
	{
		size[u] = size[son[u][0]] + size[son[u][1]] + count[u];
	}

	bool not_root(int u)
	{
		int f = fa[u];
		return son[f][0] == u || son[f][1] == u;
	}

	void rotate(int u)
	{
		int f = fa[u];
		bool p = son[f][0] == u;
		fa[u] = fa[f];
		if(not_root(f))
			son[fa[f]][son[fa[f]][0] != f] = u;

		if(!!(son[f][!p] = son[u][p]))
			fa[son[u][p]] = f;
		fa[son[u][p] = f] = u;
		pushup(f);
	}

	void splay(int u)
	{
		if(!u) return;
		for(; not_root(u); rotate(u))
		{
			int f = fa[u];
			if(not_root(f))
				rotate(((son[f][0] == u) ^ (son[fa[f]][0] == f)) ? u : f);
		}

		pushup(u);
		root = u;
	}

	int find_prev(int v)
	{
		int ans = 0;
		int u = root;
		while(u)
		{
			if(value[u] < v)
			{
				ans = u;
				u = son[u][1];
			} else u = son[u][0];
		}

		return ans;
	}

	int find_next(int v)
	{
		int ans = 0;
		int u = root;
		while(u)
		{
			if(value[u] > v)
			{
				ans = u;
				u = son[u][0];
			} else u = son[u][1];
		}

		return ans;
	}

	int find_kth(int k)
	{
		int u = root;
		while(u)
		{
			if(size[son[u][0]] < k)
			{
				k -= size[son[u][0]] + count[u];
				if(k <= 0) break;
				u = son[u][1];
			} else {
				u = son[u][0];
			}
		}

		splay(u);
		return value[u];
	}

	int find_rank(int v)
	{
		int u = root;
		int rank = 1;
		while(u)
		{
			if(value[u] == v)
			{
				rank += size[son[u][0]];
				break;
			} else if(value[u] < v) {
				rank += size[son[u][0]] + count[u];
				u = son[u][1];
			} else {
				u = son[u][0];
			}
		}

		splay(u);
		return rank;
	}

	void insert(int v)
	{
		int now = find_prev(v + 1);
		if(value[now] == v)
		{
			++count[now];
			splay(now);
		} else {
			int prev = find_prev(v);
			splay(prev);
			son[++total][1] = son[prev][1];
			fa[son[prev][1]] = total;
			son[prev][1] = total;
			fa[total] = prev;

			count[total] = 1;
			value[total] = v;
			splay(total);
		}
	}

	void remove(int v)
	{
		int prev = find_prev(v + 1);
		splay(prev);
		if(count[prev] > 1)
		{
			--count[prev];
		} else {
			root = son[prev][0];
			fa[root] = 0;
			int u = root, p = 0;
			while(u)
			{
				p = u;
				u = son[u][1];
			}

			son[p][1] = son[prev][1];
			fa[son[prev][1]] = p;
			splay(p);
		}
	}
};

splay_tree<100010> s;

int main()
{
	s.solve();
	return 0;
}
