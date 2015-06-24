/* BZOJ-2177: 曼哈顿最小生成树 */
#include <cstdio>
#include <algorithm>

const int MaxN = 100010, MaxL = 18;

struct info_t
{
	int val, pos;

	info_t(int val = ~0u >> 1, int pos = -1) : val(val), pos(pos) {}

	info_t operator + (const info_t& r) const
	{
		info_t info;
		if(val < r.val) info = *this;
		else info = r;
		return info;
	}
};

struct tree_array
{
	int size;
	info_t* ta;

	~tree_array() { if(ta) delete[] ta; }

	void reset(int sz)
	{
		if(ta) delete[] ta;
		size = sz;
		ta = new info_t[size + 10];
	}

	void modify(int x, info_t v)
	{
		for(++x; x; x -= x & -x)
			ta[x] = ta[x] + v;
	}

	int ask(int x)
	{
		info_t v;
		for(++x; x <= size; x += x & -x)
			v = v + ta[x];
		return v.pos;
	}
} ta;

struct point_t
{
	int x, y, id, k;
	bool operator < (const point_t& r) const
	{
		if(x == r.x) return y < r.y;
		return x < r.x;
	}
} pt[MaxN], pt0[MaxN];

int abs(int v) { return v < 0 ? -v : v; }

struct edge_t
{
	int a, b, w;
	bool operator < (const edge_t& r) const
	{
		return w < r.w;
	}
} edge[MaxN << 2];
int etot = 0;

struct data_t
{
	int pos, val;
	bool operator < (const data_t& r) const
	{
		return val < r.val;
	}
} tmp[MaxN];

void calc_edge(int n)
{
	int tot = 0;
	for(int i = 1; i <= n; ++i)
	{
		tmp[tot].pos = i;
		tmp[tot++].val = pt[i].y - pt[i].x;
	}

	std::sort(tmp, tmp + tot);
	int cnt = 0, prev = -1;
	for(int i = 0; i != tot; ++i)
	{
		if(prev != tmp[i].val)
			++cnt, prev = tmp[i].val;
		int id = tmp[i].pos;
		pt[id].k = cnt;
	}

	std::sort(pt + 1, pt + n + 1);
	ta.reset(cnt + 1);
	for(int i = n; i; --i)
	{
		int pos = ta.ask(pt[i].k);
		if(pos >= 0)
		{
			edge[etot].a = pt[i].id;
			edge[etot].b = pt[pos].id;
			edge[etot].w = abs(pt[i].x - pt[pos].x)
						 + abs(pt[i].y - pt[pos].y);
			++etot;
		}

		info_t info(pt[i].x + pt[i].y, i);
		ta.modify(pt[i].k, info);
	}
}

int gf[MaxN];
int find(int v)
{
	if(gf[v] == v) return v;
	return gf[v] = find(gf[v]);
}

long long calc(int n)
{
	std::copy(pt0 + 1, pt0 + n + 1, pt + 1);
	calc_edge(n);

	std::copy(pt0 + 1, pt0 + n + 1, pt + 1);
	for(int i = 1; i <= n; ++i)
		pt[i].y = -pt[i].y;
	calc_edge(n);

	std::copy(pt0 + 1, pt0 + n + 1, pt + 1);
	for(int i = 1; i <= n; ++i)
		std::swap(pt[i].x, pt[i].y);
	calc_edge(n);

	std::copy(pt0 + 1, pt0 + n + 1, pt + 1);
	for(int i = 1; i <= n; ++i)
	{
		std::swap(pt[i].x, pt[i].y);
		pt[i].y = -pt[i].y;
	}
	calc_edge(n);

	std::sort(edge, edge + etot);
	for(int i = 1; i <= n; ++i) gf[i] = i;

	long long ans = 0;
	for(int i = 0; i != etot; ++i)
	{
		int u = find(edge[i].a), v = find(edge[i].b);
		if(u != v)
		{
			ans += edge[i].w;
			gf[u] = gf[v];
		}
	}

	return ans;
}

int main()
{
	int n;
	std::scanf("%d", &n);
	for(int i = 1; i <= n; ++i)
	{
		pt0[i].id = i;
		std::scanf("%d %d", &pt0[i].x, &pt0[i].y);
	}

	std::printf("%lld", calc(n));
	return 0;
}
