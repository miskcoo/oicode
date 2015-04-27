/* BZOJ-3237: [Ahoi2013]连通图
 *   CDQ分治图重构 */
#include <cstdio>
#include <algorithm>

const int MaxL = 18, MaxN = 100010, MaxM = 200010, MaxQ = 100010;
int map[MaxM], mark[MaxM], mark_v;
struct edge_t
{
	int id, u, v, mark;
} edge[MaxL][MaxM], d[MaxM], t[MaxM];

struct ques_t
{
	int ans, num, d[4];
} ques[MaxQ];

struct mset
{
	int fa[MaxN];

	void reset(int n, edge_t* e)
	{
		for(int i = 0; i != n; ++i)
		{
			fa[e[i].u] = 0;
			fa[e[i].v] = 0;
		}
	}

	int find(int u)
	{
		if(!fa[u] || fa[u] == u) 
			return fa[u] = u;
		return fa[u] = find(fa[u]);
	}
} ms;

int contraction(int& n)
{
	int connect = 0;
	for(int i = 0; i != n; ++i)
	{
		int u = ms.find(d[i].u), v = ms.find(d[i].v);
		if(!d[i].mark && u != v)
		{
			ms.fa[u] = ms.fa[v];
			++connect;
		}
	}

	int tot = 0;
	for(int i = 0; i != n; ++i)
	{
		int u = ms.find(d[i].u), v = ms.find(d[i].v);
		if(u != v || d[i].mark)
		{
			t[tot].u = u, t[tot].v = v;
			t[tot].id = d[i].id;
			t[tot++].mark = 0;
		}
	}

	ms.reset(n, d); n = tot;
	return connect;
}

void reduction(int &n, int l, int r)
{
	++mark_v;
	int tot = 0;
	for(int i = 0; i != n; ++i)
	{
		if(t[i].u == t[i].v)
			mark[t[i].id] = mark_v;
		else d[tot++] = t[i];
	}

	n = tot;

	for(int i = l; i <= r; ++i)
	{
		int num = ques[i].num;
		for(int j = 0; j != num; ++j)
		{
			if(mark[ques[i].d[j]] == mark_v)
				std::swap(ques[i].d[j--], ques[i].d[--num]);
		}

		ques[i].num = num;
	}
}

void divide(int now, int n, int l, int r, int connect)
{
	for(int i = 0; i != n; ++i)
	{
		d[i] = edge[now][i];
		map[d[i].id] = i;
	}

	for(int i = l; i <= r; ++i)
	{
		for(int j = 0; j != ques[i].num; ++j)
			d[map[ques[i].d[j]]].mark = 1;
	}

	if(l == r)
	{
		for(int i = 0; i != n; ++i)
		{
			int u = ms.find(d[i].u), v = ms.find(d[i].v);
			if(u != v && !d[i].mark) 
			{
				ms.fa[u] = ms.fa[v];
				--connect;
			}
		}

		ms.reset(n, d);
		ques[l].ans = connect;
		return;
	} 

	connect -= contraction(n);
	reduction(n, l, r);

	for(int i = 0; i != n; ++i)
		edge[now + 1][i] = d[i];

	int m = (l + r) >> 1;
	divide(now + 1, n, l, m, connect);
	divide(now + 1, n, m + 1, r, connect);
}

int main()
{
	int n, m;
	std::scanf("%d %d", &n, &m);
	for(int i = 0; i != m; ++i)
	{
		edge[0][i].id = i;
		std::scanf("%d %d", &edge[0][i].u, &edge[0][i].v);
	}
	int q;
	std::scanf("%d", &q);
	for(int i = 0; i != q; ++i)
	{
		std::scanf("%d", &ques[i].num);
		for(int j = 0; j != ques[i].num; ++j)
		{
			std::scanf("%d", ques[i].d + j);
			--ques[i].d[j];
		}
	}

	divide(0, m, 0, q - 1, n);
	for(int i = 0; i != q; ++i)
		std::puts(ques[i].ans != 1 ? "Disconnected" : "Connected");
	return 0;
}
