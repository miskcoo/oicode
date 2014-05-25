/*
 * BZOJ-3223 文艺平衡树
 *  Splay 
 */
#include <cstdio>
#include <algorithm>

template<int NodeSize>
class splay
{
	int son[NodeSize + 1][2];
	int fa[NodeSize + 1];
	int value[NodeSize + 1];
	int csz[NodeSize + 1];
	int lazy[NodeSize + 1];
	int root;
	int size;
private:
	void push_up(int x)
	{
		csz[x] = csz[son[x][0]]
			+ csz[son[x][1]] + 1;
	}

	void push_down(int x)
	{
		if(lazy[x] == 0) 
			return;
		std::swap(son[x][0], son[x][1]);
		lazy[x] = 0;
		lazy[son[x][0]] ^= 1;
		lazy[son[x][1]] ^= 1;
	}

	void rotate(int x)
	{
		int y = fa[x];
		int sx = son[y][1] == x;
		int sy = son[fa[y]][1] == y;

		son[fa[x] = fa[y]][sy] = x;
		fa[son[y][sx] = son[x][sx ^ 1]] = y;
		son[fa[y] = x][sx ^ 1] = y;

		push_up(y);
		push_up(x);
	}

	void splay_node(int x, int to)
	{
		while(fa[x] != to)
		{
			int y = fa[x];
			if(fa[y] == to)
			{
				rotate(x);
			} else {
				int sx = son[y][1] == x;
				int sy = son[fa[y]][1] == y;
				rotate(sx == sy ? y : x);
				rotate(x);
			}
		}

		son[0][0] = son[0][1] = fa[0]
			= csz[0] = lazy[0] = 0;
		if(to == 0) 
			root = x;
	}

public:
	void insert(int v)
	{
		int x = root;
		int y = 0;
		while(x) 
		{
			y = x;
			x = son[x][1];
		}

		value[++size] = v;
		fa[size] = y;
		son[y][1] = size;
		csz[size] = 1;
		lazy[size] = 0;
		son[size][0] = son[size][1] = 0;
		splay_node(size, 0);
	}

	int find(int k)
	{
		int x = root;
		while(x)
		{
			push_down(x);
			if(csz[son[x][0]] + 1 == k)
				break;
			if(k > csz[son[x][0]])
			{
				k -= csz[son[x][0]] + 1;
				x = son[x][1];
			} else {
				x = son[x][0];
			}
		}

		if(x) splay_node(x, 0);
		return x;
	}

	void reverse(int a, int b)
	{
		int pa = find(a - 1);
		root = son[pa][1];
		fa[root] = 0;
		son[pa][1] = 0;
		push_up(pa);
		int pb = find(b + 1 - (a - 1));
		lazy[son[pb][0]] = 1;

		int x = root;
		int y = 0;
		while(x)
		{
			push_down(y = x);
			x = son[x][0];
		}
		
		son[y][0] = pa;
		fa[pa] = y;
		splay_node(y, 0);
	}

	int get_root() const { return root; }

	void print(int now)
	{
		if(now == 0)
			return;
		push_down(now);
		print(son[now][0]);
		if(value[now] != 0 && value[now] + 1 != size)
			std::printf("%d ", value[now]);
		print(son[now][1]);
	}
};

splay<100010> sp;

int main()
{
	int N, M;
	std::scanf("%d %d", &N, &M);
	for(int i = 0; i <= N + 1; ++i)
		sp.insert(i);

	for(int i = 0; i != M; ++i)
	{
		int a, b;
		std::scanf("%d %d", &a, &b);
		if(a == b) continue;
		sp.reverse(a + 1, b + 1);
	}

	sp.print(sp.get_root());
	return 0;
}
