/* BZOJ-4195: [Noi2015]程序自动分析 */
#include <cstdio>
#include <cstring>
#include <algorithm>

const int MaxN = 200100;

struct mset
{
	int fa[MaxN];

	void reset(int n)
	{
		for(int i = 0; i <= n; ++i)
			fa[i] = i;
	}

	int find(int v)
	{
		if(fa[v] == v) return v;
		return fa[v] = find(fa[v]);
	}

	void merge(int u, int v)
	{
		u = find(u), v = find(v);
		if(u != v) fa[u] = v;
	}
} ms;

struct cond_t
{
	int a, b, t;
} cond[MaxN];

struct data_t
{
	int id, pos, val;
	bool operator < (const data_t& r) const
	{
		return val < r.val;
	}
} data[MaxN];

struct graph_t
{
	int total;
	int head[MaxN], next[MaxN << 1], point[MaxN << 1];
	
	void reset()
	{
		total = 0;
		std::memset(head, 0, sizeof(head));
	}

	void add_edge(int u, int v)
	{
		point[++total] = v;
		next[total] = head[u];
		head[u] = total;
	}

} gp;

int n;

bool solve()
{
	int tot = 0;
	std::scanf("%d", &n);
	for(int i = 0; i != n; ++i)
	{
		std::scanf("%d %d %d", &cond[i].a, &cond[i].b, &cond[i].t);
		data[tot].id = i;
		data[tot].pos = 0;
		data[tot++].val = cond[i].a;

		data[tot].id = i;
		data[tot].pos = 1;
		data[tot++].val = cond[i].b;
	}

	std::sort(data, data + tot);
	int cnt = 0, prev = -1;
	for(int i = 0; i != tot; ++i)
	{
		if(prev != data[i].val)
			prev = data[i].val, ++cnt;
		int id = data[i].id;
		if(data[i].pos) cond[id].b = cnt;
		else cond[id].a = cnt;
	}

	gp.reset();
	ms.reset(cnt);
	for(int i = 0; i != n; ++i)
		if(cond[i].t) ms.merge(cond[i].a, cond[i].b);

	// check mutex
	for(int i = 0; i != n; ++i)
	{
		if(!cond[i].t)
		{
			int a = ms.find(cond[i].a);
			int b = ms.find(cond[i].b);
			if(a == b) return false;
			gp.add_edge(a, b);
			gp.add_edge(b, a);
//			fprintf(stderr, "%d, %d\n", a, b);
		}
	}
	return true;
}

int main()
{
	int t;
	std::scanf("%d", &t);
	for(int i = 0; i != t; ++i)
		std::puts(solve() ? "YES" : "NO");
	return 0;
}
