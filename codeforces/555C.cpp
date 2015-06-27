/* Codeforces 555C. Case of Chocolate 
 *    线段树
 *    data structures */
#include <algorithm>
#include <set>
#include <cstdio>

template<typename Type>
class memory_pool
{
	int used, remain;
	Type* data[1 << 16];
public:
	memory_pool() : used(0), remain(0) {}
	~memory_pool() 
	{
		for(int i = 0; i != used; ++i)
			delete[] data[i];
	}

	Type* get() 
	{
		if(!remain) data[used++] = new Type[remain = 1 << 16];
		return data[used - 1] + --remain;
	}
};

int n;
class seg_tree
{
	struct node_t
	{
		int val; //, id;
		node_t *l, *r;
	};

	node_t* root;
	memory_pool<node_t> mem;
//	int time;

public:
	seg_tree()
	{
		root = mem.get();
		root->val = 0;
	}

	int ask(int pos)
	{
		node_t* now = root;
		int l = 1, r = n + 1;
		int ans = root->val;
		while(now && l < r)
		{
			int m = (l + r) >> 1;
			ans = std::max(ans, now->val);
			if(pos <= m) r = m, now = now->l;
			else l = m + 1, now = now->r;
		}

		if(now) ans = std::max(ans, now->val);

		return ans;
	}

	node_t* modify(node_t* now, int head, int tail, int a, int b, int v)
	{
		if(!now) now = mem.get(), now->l = now->r = 0, now->val = -1;
		if(head == a && tail == b)
		{
			now->val = std::max(now->val, v);
			return now;
		}

		int m = (head + tail) >> 1;
		if(b <= m) return now->l = modify(now->l, head, m, a, b, v), now;
		if(m < a) return now->r = modify(now->r, m + 1, tail, a, b, v), now;
		now->l = modify(now->l, head, m, a, m, v);
		now->r = modify(now->r, m + 1, tail, m + 1, b, v);
		return now;
	}

	void set(int l, int r, int v)
	{
		modify(root, 1, n + 1, l, r, v);
//		std::printf("set (%d, %d) = %d\n", l, r, v);
	}
} s1, s2;

std::set<int> s0;

int main()
{
	int q;
	std::scanf("%d %d", &n, &q);
	while(q --> 0)
	{
		char dir[2];
		int x, y, ans;
		std::scanf("%d %d %s", &x, &y, dir);
		if(s0.insert(x).second == false)
		{
			ans = 0;
		} else if(*dir == 'U') {
			ans = y - s1.ask(x);
			if(ans) s2.set(x, x + ans - 1, x);
		} else {
			ans = x - s2.ask(x);
			if(ans) s1.set(x - ans + 1, x, y);
		}

		std::printf("%d\n", ans);
	}
	return 0;
}
