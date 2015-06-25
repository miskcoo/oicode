/* BZOJ-4009: [HNOI2015]接水果
 *   DFS序，二维线段树 */
#include <cstdio>
#include <algorithm>

const int MaxL = 17, MaxN = 40010, MaxM = MaxN << 1;
int n, p, q, total, dfn_index;
int head[MaxN], point[MaxM], next[MaxM], fa[MaxL][MaxN];
int enter[MaxN], leave[MaxN], depth[MaxN];

void add_edge(int u, int v)
{
	point[++total] = v;
	next[total] = head[u];
	head[u] = total;
}

void dfs(int u)
{
	for(int i = 1; i != MaxL; ++i)
		fa[i][u] = fa[i - 1][fa[i - 1][u]];
	enter[u] = ++dfn_index;
	for(int k = head[u]; k; k = next[k])
	{
		if(point[k] != fa[0][u])
		{
			fa[0][point[k]] = u;
			depth[point[k]] = depth[u] + 1;
			dfs(point[k]);
		}
	}

	leave[u] = dfn_index;
}

struct event_t
{
	int op, x, y0, y1, w;
	event_t(int a = 0, int b = 0, int c = 0, int d = 0, int e = 0)
		: op(a), x(b), y0(c), y1(d), w(e) {}

	bool operator < (const event_t& r) const
	{
		return x < r.x;
	}
} event[MaxN << 3];
int etot = 0;

void read_tree()
{
	for(int i = 1; i != n; ++i)
	{
		int u, v;
		std::scanf("%d %d", &u, &v);
		add_edge(u, v);
		add_edge(v, u);
	}

	dfs(1);
}

void add_event(int x0, int x1, int y0, int y1, int w)
{
	if(x1 + 1 < x0 || y1 < y0) return;
	event[etot++] = event_t(1, x0, y0, y1, w);
	event[etot++] = event_t(-1, x1 + 1, y0, y1, w);
}

struct plate_t
{
	int a, b, c;
	bool operator < (const plate_t& r) const
	{
		return c < r.c;
	}
} plate[MaxN];
int val_map[MaxN];

void read_plate()
{
	for(int i = 0; i != p; ++i)
		std::scanf("%d %d %d", &plate[i].a, &plate[i].b, &plate[i].c);
	std::sort(plate, plate + p);

	int cnt = 0, prev = -1;
	for(int i = 0; i != p; ++i)
	{
		if(prev != plate[i].c)
			prev = plate[i].c, val_map[++cnt] = prev;
		plate[i].c = cnt;
	}

	for(int i = 0; i != p; ++i)
	{
		int a = plate[i].a, b = plate[i].b, c = plate[i].c;
		if(depth[a] > depth[b]) std::swap(a, b);
		if(enter[a] <= enter[b] && leave[a] >= leave[b])
		{
			int diff = depth[b] - depth[a] - 1;
			int k = b;
			for(int i = 0; diff; diff >>= 1, ++i)
				if(diff & 1) k = fa[i][k];
			add_event(1, enter[k] - 1, enter[b], leave[b], c);
			add_event(leave[k] + 1, n, enter[b], leave[b], c);
			add_event(enter[b], leave[b], 1, enter[k] - 1, c);
			add_event(enter[b], leave[b], leave[k] + 1, n, c);
		} else {
			if(enter[a] > enter[b]) std::swap(a, b);
			add_event(enter[a], leave[a], enter[b], leave[b], c);
			add_event(enter[b], leave[b], enter[a], leave[a], c);
		}
	}

	std::sort(event, event + etot);
}

struct fruit_t
{
	int u, v, k, id;
	bool operator < (const fruit_t& r) const
	{
		return u < r.u;
	}
} fruit[MaxN << 1];

void read_fruit()
{
	for(int i = 0; i != q; ++i)
	{
		int u, v, k;
		std::scanf("%d %d %d", &u, &v, &k);
		if(enter[v] < enter[u]) std::swap(u, v);
		fruit[i].id = i;
		fruit[i].k = k;
		fruit[i].u = enter[u];
		fruit[i].v = enter[v];
	}

	std::sort(fruit, fruit + q);
}

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

struct seg_t
{
	int w;
	seg_t *l, *r;
	seg_t() : l(0), r(0), w(0) {}
};

seg_t *ta[MaxN], *seg[MaxN];
memory_pool<seg_t> mem;

seg_t* seg_modify(seg_t* now, int head, int tail, int pos, int w)
{
	if(!now) now = mem.get();
	now->w += w;
	if(head == tail) return now;

	int m = (head + tail) >> 1;
	if(pos <= m) now->l = seg_modify(now->l, head, m, pos, w);
	else now->r = seg_modify(now->r, m + 1, tail, pos, w);
	return now;
}

void modify(int l, int r, int pos, int w)
{
	for(int x = r + 1; x <= n; x += x & -x)
		ta[x] = seg_modify(ta[x], 1, p, pos, -w);
	for(int x = l; x <= n; x += x & -x)
		ta[x] = seg_modify(ta[x], 1, p, pos, w);
}

int ask_kth(int b, int k)
{
	int s = 0;
	for(int x = b; x; x -= x & -x)
		seg[s++] = ta[x];

	int l = 1, r = p;
	while(l < r)
	{
		int cnt = 0;
		for(int i = 0; i != s; ++i)
			if(seg[i] && seg[i]->l) cnt += seg[i]->l->w;

		int m = (l + r) >> 1;
		if(k <= cnt)
		{
			for(int i = 0; i != s; ++i)
				if(seg[i]) seg[i] = seg[i]->l;
			r = m;
		} else {
			for(int i = 0; i != s; ++i)
				if(seg[i]) seg[i] = seg[i]->r;
			k -= cnt;
			l = m + 1;
		}
	}

	return val_map[l];
}

int ans[MaxN];
void solve()
{
	int epos = 0;
	for(int i = 0; i != q; ++i)
	{
		while(epos != etot && event[epos].x <= fruit[i].u)
		{
			modify(event[epos].y0, event[epos].y1, event[epos].w, event[epos].op);
			++epos;
		}

		ans[fruit[i].id] = ask_kth(fruit[i].v, fruit[i].k);
	}

	for(int i = 0; i != q; ++i)
		std::printf("%d\n", ans[i]);
}

int main()
{
	std::scanf("%d %d %d", &n, &p, &q);
	read_tree();
	read_plate();
	read_fruit();
	solve();
	return 0;
}
