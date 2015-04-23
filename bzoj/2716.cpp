/* BZOJ-2716: [Violet 3]天使玩偶
 *  KD-Tree   */
#include <cstdio>
#include <algorithm>

const int MaxN = 1200010;
struct point_t
{
	int x, y;
} pt[MaxN];

int abs(int x) { return x < 0 ? -x : x; }
int dist(const point_t& a, const point_t& b)
{
	return abs(a.x - b.x) + abs(a.y - b.y);
}

struct point_comparer
{
	int d;
	point_comparer(int d) : d(d) {}
	bool operator ()(const point_t& a, const point_t& b)
	{
		return d ? a.x < b.x : a.y < b.y;
	}
};

struct node_t
{
	point_t pt;
	node_t *ch[2];
	int l, r, u, d;

	void set(const point_t& p)
	{
		pt = p;
		l = r = p.x;
		u = d = p.y;
	}

	void update(int c)
	{
		l = std::min(l, ch[c]->l);
		d = std::min(d, ch[c]->d);
		r = std::max(r, ch[c]->r);
		u = std::max(u, ch[c]->u);
	}

	int mindist(const point_t& p)
	{
		int v = 0;
		if(p.x < l) v += l - p.x;
		else if(p.x > r) v += p.x - r;
		if(p.y < d) v += d - p.y;
		else if(p.y > u) v += p.y - u;
		return v;
	}
} node[MaxN];
int used, inf = ~0u >> 1;
node_t *nil;

void kdtree_init()
{
	nil = node + used++;
	nil->ch[0] = nil->ch[1] = nil;
}

node_t* kdtree_build(int l, int r, int d)
{
	if(l > r) return nil;
	int m = (l + r) >> 1;
	std::nth_element(pt + l, pt + m, pt + r + 1, point_comparer(d));
	node_t *n = node + used++;
	n->set(pt[m]);
	n->ch[0] = kdtree_build(l, m - 1, d ^ 1);
	n->ch[1] = kdtree_build(m + 1, r, d ^ 1);
	if(n->ch[0] != nil) n->update(0);
	if(n->ch[1] != nil) n->update(1);
	return n;
}

node_t* kdtree_insert(node_t *now, const point_t& pt, int d)
{
	if(now == nil)
	{
		now = node + used++;
		now->set(pt);
		now->ch[0] = now->ch[1] = nil;
		return now;
	}

	int z = point_comparer(d)(now->pt, pt) ^ 1;
	now->ch[z] = kdtree_insert(now->ch[z], pt, d ^ 1);
	now->update(z);
	return now;
}

int ans;
void kdtree_ask(node_t *now, const point_t& pt)
{
	ans = std::min(ans, dist(now->pt, pt));
	int dis[2] = { now->ch[0] == nil ? inf : now->ch[0]->mindist(pt),
		now->ch[1] == nil ? inf : now->ch[1]->mindist(pt) };
	int d = dis[0] > dis[1];
	if(now->ch[d] && ans > dis[d]) 
		kdtree_ask(now->ch[d], pt);
	if(now->ch[d ^ 1] && ans > dis[d ^ 1])
		kdtree_ask(now->ch[d ^ 1], pt);
}

int main()
{
	int n, m;
	std::scanf("%d %d", &n, &m);
	for(int i = 0; i != n; ++i)
		std::scanf("%d %d", &pt[i].x, &pt[i].y);
	kdtree_init();
	node_t *root = kdtree_build(0, n - 1, 1);
	for(int i = 0; i != m; ++i)
	{
		int t;
		point_t p;
		std::scanf("%d %d %d", &t, &p.x, &p.y);
		if(t == 2)
		{
			ans = inf;
			kdtree_ask(root, p);
			std::printf("%d\n", ans);
		} else {
			root = kdtree_insert(root, p, 1);
		}
	}
	return 0;
}
