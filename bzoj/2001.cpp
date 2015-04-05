/* BZOJ-2001: [Hnoi2010]City 城市建设
 *  CDQ分治缩图 */
#include <cstdio>
#include <algorithm>

const int inf = ~0u >> 1;
const int MaxN = 20010, MaxM = 50010, MaxQ = MaxM, MaxL = 16;

struct edge_t
{
	int u, v, w, id;
	friend bool operator < (const edge_t& a, const edge_t& b)
	{
		return a.w < b.w;
	}
} edge[MaxL][MaxM], d[MaxM], t[MaxM];
struct ask_t
{
	int x, y;
} ques[MaxQ];
int fa[MaxM], size[MaxM], map[MaxM], weight[MaxM];
long long ans[MaxQ];

int find(int x)
{
	if(fa[x] == x)
		return x;
	return fa[x] = find(fa[x]);
}

void merge(int x, int y)
{
	x = find(x), y = find(y);
	if(size[x] > size[y]) std::swap(x, y);
	size[y] += size[x], fa[x] = fa[y];
}

void reset(int n, const edge_t* e)
{
	for(int i = 0; i != n; ++i)
	{
		fa[e[i].u] = e[i].u;
		fa[e[i].v] = e[i].v;
		size[e[i].u] = size[e[i].v] = 1;
	}
}

long long contraction(int& n)
{
	int tmp = 0;

	std::sort(d, d + n);
	for(int i = 0; i != n; ++i)
	{
		if(find(d[i].u) != find(d[i].v))
		{
			merge(d[i].u, d[i].v);
			t[tmp++] = d[i];
		}
	}

	reset(tmp, t);
	long long v = 0;
	for(int i = 0; i != tmp; ++i)
	{
		if(t[i].w != -inf && find(t[i].u) != find(t[i].v))
		{
			merge(t[i].u, t[i].v);
			v += t[i].w;
		}
	}

	tmp = 0;
	for(int i = 0; i != n; ++i)
	{
		if(find(d[i].u) != find(d[i].v))
		{
			t[tmp] = d[i];
			t[tmp].u = fa[d[i].u];
			t[tmp].v = fa[d[i].v];
			map[d[i].id] = tmp++;
		}
	}

	reset(n, d); n = tmp;
	for(int i = 0; i != tmp; ++i)
		d[i] = t[i];
	return v;
}

void reduction(int& n)
{
	int tmp = 0;
	std::sort(d, d + n);
	for(int i = 0; i != n; ++i)
	{
		if(find(d[i].u) != find(d[i].v))
		{
			merge(d[i].u, d[i].v);
			t[tmp] = d[i];
			map[d[i].id] = tmp++;
		} else if(d[i].w == inf) {
			t[tmp] = d[i];
			map[d[i].id] = tmp++;
		}
	}

	reset(n, d); n = tmp;
	for(int i = 0; i != tmp; ++i)
		d[i] = t[i];
}

void divide(int now, int n, int l, int r, long long v)
{
	if(l == r) weight[ques[l].x] = ques[l].y;
	for(int i = 0; i != n; ++i)
	{
		edge[now][i].w = weight[edge[now][i].id];
		d[i] = edge[now][i];
		map[d[i].id] = i;
	}

	if(l == r)
	{
		ans[l] = v;
		std::sort(d, d + n);
		for(int i = 0; i != n; ++i)
		{
			if(find(d[i].u) != find(d[i].v))
			{
				merge(d[i].u, d[i].v);
				ans[l] += d[i].w;
			}
		}
		return reset(n, d);
	}

	for(int i = l; i <= r; ++i)
		d[map[ques[i].x]].w = -inf;
	v += contraction(n);

	for(int i = l; i <= r; ++i)
		d[map[ques[i].x]].w = inf;
	reduction(n);

	for(int i = 0; i != n; ++i)
		edge[now + 1][i] = d[i];

	int m = (l + r) >> 1;
	divide(now + 1, n, l, m, v);
	divide(now + 1, n, m + 1, r, v);
}

int main()
{
	int n, m, q;
	std::scanf("%d %d %d", &n, &m, &q);

	for(int i = 0; i <= n; ++i)
		fa[i] = i, size[i] = 1;

	for(int i = 0; i != m; ++i)
	{
		int u, v, w;
		std::scanf("%d %d %d", &u, &v, &w);
		edge[0][i].u = u, edge[0][i].v = v;
		edge[0][i].w = w, edge[0][i].id = i;
		weight[i] = w;
	}

	for(int i = 0; i != q; ++i)
	{
		std::scanf("%d %d", &ques[i].x, &ques[i].y);
		--ques[i].x;
	}

	divide(0, m, 0, q - 1, 0);
	for(int i = 0; i != q; ++i)
		std::printf("%lld\n", ans[i]);
	return 0;
}
