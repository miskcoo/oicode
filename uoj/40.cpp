/* UOJ-#40. 【清华集训2014】卡常数
 *   KD-Tree */
#include <cstdio>
#include <algorithm>
#include <cmath>

const int MaxN = 1 << 17;
double lastans = 0.1, encode_a, encode_b;
double decode(double y, double l = -350, double r = 350)
{
	while(r - l > 1.0e-8)
	{
		double m = (l + r) * 0.5;
		double x = m * lastans + 1.0;
		if(encode_a * x - encode_b * sin(x) < y)
			l = m;
		else r = m;
	}

	return (l + r) * 0.5;
}

const double eps = 1.0e-5, inf = 1.0e100;
double sqr(double v) { return v * v; }
double abs(double v) { return v < 0 ? -v : v; }
double cmp(double a, double b) { return abs(a - b) < eps; }

int kdtree_dimesion;
struct point_t
{
	int id;
	double p[3];
	double& operator [](size_t z) { return p[z]; }
	const double& operator [](size_t z) const { return p[z]; }
	bool operator < (const point_t& r) const 
	{
		return p[kdtree_dimesion] < r[kdtree_dimesion];
	}
} pt[MaxN];

double dist(const point_t& a, const point_t& b)
{
	double ans = 0.0;
	for(int i = 0; i != 3; ++i)
		ans += sqr(a[i] - b[i]);
	return ans;
}

struct node_t
{
	int id, dim, mark;
	point_t p, min, max;
	node_t *l, *r, *fa;
} node[MaxN];
node_t *map[MaxN];
int used;

void kdtree_update(node_t *n, point_t p)
{
	for(int i = 0; i != 3; ++i)
	{
		n->min[i] = std::min(n->min[i], p[i]);
		n->max[i] = std::max(n->max[i], p[i]);
	}
}

void kdtree_update(node_t *n)
{
	if(!n->mark) n->min = n->max = n->p;
	else for(int i = 0; i != 3; ++i) 
		n->min[i] = inf, n->max[i] = -inf;

	if(n->l) for(int i = 0; i != 3; ++i)
	{
		n->min[i] = std::min(n->min[i], n->l->min[i]);
		n->max[i] = std::max(n->max[i], n->l->max[i]);
	}

	if(n->r) for(int i = 0; i != 3; ++i)
	{
		n->min[i] = std::min(n->min[i], n->r->min[i]);
		n->max[i] = std::max(n->max[i], n->r->max[i]);
	}
}

node_t *kdtree_build(int l, int r, int d = 0)
{
	if(r < l) return 0;
	int m = (l + r) >> 1;
	kdtree_dimesion = d;
	std::nth_element(pt + l, pt + m, pt + r + 1);
	node_t *n = node + used++;
	map[pt[m].id] = n; n->id = pt[m].id;
	n->p = pt[m]; n->dim = d;
	n->l = kdtree_build(l, m - 1, (d + 1) % 3);
	n->r = kdtree_build(m + 1, r, (d + 1) % 3);
	if(n->l) n->l->fa = n;
	if(n->r) n->r->fa = n;
	kdtree_update(n);
	return n;
}

double asked_radius;
point_t asked;
double kdtree_mindist(node_t *n)
{
	if(!n) return 0;
	double res = 0.0;
	for(int i = 0; i != 3; ++i)
	{
		if(asked[i] < n->min[i])
			res += sqr(asked[i] - n->min[i]);
		else if(asked[i] > n->max[i])
			res += sqr(asked[i] - n->max[i]);
	}

	return res;
}

double kdtree_maxdist(node_t *n)
{
	if(!n) return 0;
	double res = 0.0;
	for(int i = 0; i != 3; ++i)
		res += std::max(sqr(asked[i] - n->min[i]), sqr(asked[i] - n->max[i]));

	return res;
}

int kdtree_ask(node_t *n)
{
	if(!n) return 0;
	if(cmp(dist(n->p, asked), asked_radius))
		return n->id;

	double minl = kdtree_mindist(n->l) - eps;
	double minr = kdtree_mindist(n->r) - eps;
	double maxl = kdtree_maxdist(n->l) + eps;
	double maxr = kdtree_maxdist(n->r) + eps;

	int ans = 0;
	if(minl <= asked_radius && asked_radius <= maxl)
		ans = kdtree_ask(n->l);
	if(!ans && minr <= asked_radius && asked_radius <= maxr)
		ans = kdtree_ask(n->r);
	return ans;
}

void read_pt(point_t& p)
{
	std::scanf("%lf %lf %lf", &p[0], &p[1], &p[2]);
}

void decode_pt(point_t& p)
{
	for(int i = 0; i != 3; ++i)
		p[i] = decode(p[i]);
}

int main()
{
	int n, m;
	std::scanf("%d %d", &n, &m);
	std::scanf("%lf %lf", &encode_a, &encode_b);
	for(int i = 1; i <= n; ++i) read_pt(pt[i]), pt[i].id = i;
	node_t *root = kdtree_build(1, n);
	while(m --> 0)
	{
		int op;
		std::scanf("%d", &op);
		if(op == 0)
		{
			double encode_id;
			point_t p;
			std::scanf("%lf", &encode_id);
			read_pt(p);
			decode_pt(p);
			int id = int(decode(encode_id, 1, n) + 0.5);

			node_t *now = map[id], *z = root, *prev;
			for(now->mark = 1; now; now = now->fa)
				kdtree_update(now);
			now = map[id] = node + used++;
			now->min = now->max = now->p = p; now->id = id;

			while(z)
			{
				prev = z;
				kdtree_update(z, p);
				kdtree_dimesion = z->dim;
				if(p < z->p) z = z->l;
				else z = z->r;
			}

			now->dim = (prev->dim + 1) % 3;
			kdtree_dimesion = prev->dim;
			if(p < prev->p) prev->l = now;
			else prev->r = now;
			now->fa = prev;
		} else {
			read_pt(asked);
			decode_pt(asked);
			std::scanf("%lf", &asked_radius);
			asked_radius = sqr(decode(asked_radius));
			lastans = kdtree_ask(root);
			std::printf("%d\n", int(lastans + 0.5));
		}
	}
	return 0;
}

