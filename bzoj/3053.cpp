/* BZOJ-3053: The Closest M Points
 *  KD-Tree   */
#include <cstdio>
#include <algorithm>
#include <queue>
#include <cstring>

const int MaxN = 50010, MaxK = 5;
struct point_t
{
	double p[MaxK];
} pt[MaxN];

struct node_t
{
	node_t *l, *r;
	point_t pt, min, max;
} node[MaxN];
int used, dim, k, m, n;

double sqr(double v) { return v * v; }

double get_dist(const point_t& a, const point_t& b)
{
	double v = 0.0;
	for(int i = 0; i != k; ++i)
		v += sqr(a.p[i] - b.p[i]);
	return v;
}

bool operator < (const point_t& a, const point_t& b)
{
	return a.p[dim] < b.p[dim];
}

void kdtree_update(node_t *n, node_t *z)
{
	if(!z) return;
	for(int i = 0; i != k; ++i)
	{
		n->min.p[i] = std::min(n->min.p[i], z->min.p[i]);
		n->max.p[i] = std::max(n->max.p[i], z->max.p[i]);
	}
}

node_t* kdtree_build(int l, int r, int d = 0)
{
	if(r < l) return 0;
	node_t *n = node + used++;
	int m = (l + r) >> 1;
	dim = d;
	std::nth_element(pt + l, pt + m, pt + r + 1);
	n->min = n->max = n->pt = pt[m];
	n->l = kdtree_build(l, m - 1, (d + 1) % k);
	n->r = kdtree_build(m + 1, r, (d + 1) % k);
	kdtree_update(n, n->l);
	kdtree_update(n, n->r);
	return n;
}

struct data_t
{
	double dist;
	point_t pt;
	bool operator < (const data_t& d) const
	{
		return dist < d.dist;
	}
};

point_t asked;
std::priority_queue<data_t> heap;

double kdtree_mindist(node_t* n)
{
	double d = 0.0;
	if(!n) return d;
	for(int i = 0; i != k; ++i)
	{
		if(asked.p[i] < n->min.p[i])
			d += sqr(n->min.p[i] - asked.p[i]);
		if(asked.p[i] > n->max.p[i])
			d += sqr(n->max.p[i] - asked.p[i]);
	}

	return d;
}

void kdtree_ask(node_t *n)
{
	if(!n) return;
	double d0 = kdtree_mindist(n->l);
	double d1 = kdtree_mindist(n->r);
	data_t d;
	d.pt = n->pt;
	d.dist = get_dist(n->pt, asked);
	heap.push(d);
	if(heap.size() > m) heap.pop();
	if(d0 < d1)
	{
		if(d0 < heap.top().dist || heap.size() < m)
			kdtree_ask(n->l);
		if(d1 < heap.top().dist || heap.size() < m)
			kdtree_ask(n->r);
	} else {
		if(d1 < heap.top().dist || heap.size() < m)
			kdtree_ask(n->r);
		if(d0 < heap.top().dist || heap.size() < m)
			kdtree_ask(n->l);
	}
}

void output()
{
	if(heap.empty()) return;
	point_t pt = heap.top().pt;
	heap.pop();
	output();
	std::printf("%d", (int)pt.p[0]);
	for(int i = 1; i != k; ++i)
		std::printf(" %d", (int)pt.p[i]);
	std::puts("");
}

int main()
{
	while(std::scanf("%d %d", &n, &k) != EOF)
	{
		for(int i = 0; i != n; ++i)
		{
			for(int j = 0; j != k; ++j)
				std::scanf("%lf", &pt[i].p[j]);
		}

		used = 0;
		node_t *root = kdtree_build(0, n - 1);

		int q;
		std::scanf("%d", &q);
		while(q --> 0)
		{
			for(int i = 0; i != k; ++i)
				std::scanf("%lf", &asked.p[i]);
			std::scanf("%d", &m);
			kdtree_ask(root);
			std::printf("the closest %d points are:\n", m);
			output();
		}
	}
	return 0;
}
