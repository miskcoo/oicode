/* BZOJ-2626: JZPFAR
 *   KD-Tree   */
#include <cstdio>
#include <queue>
#include <algorithm>

const int MaxN = 100010;
struct point_t
{
	int p[2], id;
} pt[MaxN];

struct node_t
{
	point_t p, min, max;
	node_t *l, *r;
} node[MaxN];

int dim, used;
bool cmp(const point_t& a, const point_t& b)
{
	return a.p[dim] < b.p[dim];
}

void kdtree_update(node_t *n, node_t* t)
{
	n->min.p[0] = std::min(t->min.p[0], n->min.p[0]);
	n->max.p[0] = std::max(t->max.p[0], n->max.p[0]);
	n->min.p[1] = std::min(t->min.p[1], n->min.p[1]);
	n->max.p[1] = std::max(t->max.p[1], n->max.p[1]);
}

node_t* kdtree_build(int l, int r, int d)
{
	if(r < l) return 0;
	node_t *n = node + used++;
	dim = d;
	int m = (l + r) >> 1;
	std::nth_element(pt + l, pt + m, pt + r + 1, cmp);
	n->min = n->max = n->p = pt[m];
	n->l = kdtree_build(l, m - 1, d ^ 1);
	n->r = kdtree_build(m + 1, r, d ^ 1);
	if(n->l) kdtree_update(n, n->l);
	if(n->r) kdtree_update(n, n->r);
	return n;
}

point_t asked_pt;

long long get_dist(const point_t& a, const point_t& b)
{
	return 1ll * (a.p[0] - b.p[0]) * (a.p[0] - b.p[0])
		+ 1ll * (a.p[1] - b.p[1]) * (a.p[1] - b.p[1]);
}

struct info_t
{
	point_t pt;
	long long dist;

	info_t(const point_t& p) : pt(p) 
	{
		dist = get_dist(pt, asked_pt);
	}

	bool operator < (const info_t& a) const
	{
		if(dist == a.dist) return pt.id < a.pt.id;
		return dist > a.dist;
	}
};

long long kdtree_maxdist(node_t *n)
{
	if(!n) return 0;
	long long d = 0;
	d += std::max(1ll * (n->max.p[0] - asked_pt.p[0]) * (n->max.p[0] - asked_pt.p[0]),
		1ll * (n->min.p[0] - asked_pt.p[0]) * (n->min.p[0] - asked_pt.p[0]));
	d += std::max(1ll * (n->max.p[1] - asked_pt.p[1]) * (n->max.p[1] - asked_pt.p[1]),
		1ll * (n->min.p[1] - asked_pt.p[1]) * (n->min.p[1] - asked_pt.p[1]));
	return d;
}

int k;
std::priority_queue<info_t> heap;

bool kdtree_updatable(long long d)
{
	return heap.size() != k || d >= heap.top().dist;
}

void kdtree_ask(node_t *n)
{
	if(!n) return;
	long long d1 = kdtree_maxdist(n->l);
	long long d2 = kdtree_maxdist(n->r);
	heap.push(n->p);
	if(heap.size() > k) heap.pop();
	if(d1 > d2)
	{
		if(kdtree_updatable(d1))
			kdtree_ask(n->l);
		if(kdtree_updatable(d2))
			kdtree_ask(n->r);
	} else {
		if(kdtree_updatable(d2))
			kdtree_ask(n->r);
		if(kdtree_updatable(d1))
			kdtree_ask(n->l);
	}
}

int main()
{
	int n;
	std::scanf("%d", &n);
	for(int i = 0; i != n; ++i)
	{
		pt[i].id = i + 1;
		std::scanf("%d %d", pt[i].p, pt[i].p + 1);
	}
	node_t *root = kdtree_build(0, n - 1, 0);
	int m;
	std::scanf("%d", &m);
	for(int i = 0; i != m; ++i)
	{
		std::scanf("%d %d %d", asked_pt.p, asked_pt.p + 1, &k);
		while(!heap.empty()) heap.pop();
		kdtree_ask(root);
		std::printf("%d\n", heap.top().pt.id);
	}
	return 0;
}
