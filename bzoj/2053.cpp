/* BZOJ-2053: SRM199 ClosestPoints
 *   KD-Tree */
#include <cstdio>
#include <algorithm>

const int MaxN = 200010;

int kdtree_dimesion;
struct point_t
{
	long long p[3];
	long long& operator [](size_t z) { return p[z]; }
	const long long& operator [](size_t z) const { return p[z]; }
	bool operator < (const point_t& r) const
	{
		return p[kdtree_dimesion] < r[kdtree_dimesion];
	}
	bool operator == (const point_t& r) const
	{
		return p[0] == r[0] && p[1] == r[1] && p[2] == r[2];
	}
} pt[MaxN];

long long sqr(long long x) { return x * x; }
long long dist(const point_t& a, const point_t& b)
{
	long long v = 0;
	for(int i = 0; i != 3; ++i)
		v += sqr(a[i] - b[i]);
	return v;
}

bool cmp(const point_t& a, const point_t& b)
{
	if(a[0] == b[0])
	{
		if(a[1] == b[1]) return a[2] < b[2];
		return a[1] < b[1];
	}

	return a[0] < b[0];
}

struct node_t
{
	point_t p, min, max;
	node_t *l, *r;
} node[MaxN];
int used;

void kdtree_update(node_t *n)
{
	n->min = n->max = n->p;
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

node_t* kdtree_build(int l, int r, int d)
{
	if(r < l) return 0;
	int m = (l + r) >> 1;
	kdtree_dimesion = d;
	std::nth_element(pt + l, pt + m, pt + r + 1);
	node_t *n = node + used++;
	n->p = pt[m];
	n->l = kdtree_build(l, m - 1, (d + 1) % 3);
	n->r = kdtree_build(m + 1, r, (d + 1) % 3);
	kdtree_update(n);
	return n;
}

point_t asked;
long long kdtree_mindist(node_t *n)
{
	if(!n) return 0;
	long long ans = 0;
	for(int i = 0; i != 3; ++i)
	{
		if(asked[i] < n->min[i])
			ans += sqr(asked[i] - n->min[i]);
		if(asked[i] > n->max[i])
			ans += sqr(asked[i] - n->max[i]);
	}
	return ans;
}

long long ans = ~0llu >> 1, count;
void kdtree_ask(node_t *n)
{
	if(!n) return;
	long long d = dist(n->p, asked);
	if(d && d < ans) ans = d, count = 1;
	else if(d == ans) ++count;

	long long l = kdtree_mindist(n->l);
	long long r = kdtree_mindist(n->r);
	if(l < r)
	{
		if(l <= ans) kdtree_ask(n->l);
		if(r <= ans) kdtree_ask(n->r);
	} else {
		if(r <= ans) kdtree_ask(n->r);
		if(l <= ans) kdtree_ask(n->l);
	}
}

long long seed, range, n;
long long get()
{
	seed = (seed * 16807) % ((1ll << 31) - 1);
	return seed % (range << 1) - range;
}

int main()
{
	std::scanf("%lld %lld %lld", &n, &range, &seed);
	for(int i = 0; i != n; ++i)
	{
		for(int j = 0; j != 3; ++j)
			pt[i][j] = get();
	}

	std::sort(pt, pt + n, cmp);
	n = std::unique(pt, pt + n) - pt;
	node_t *root = kdtree_build(0, n - 1, 1);
	for(int i = 0; i != n; ++i)
	{
		asked = pt[i];
		kdtree_ask(root);
	}

	std::printf("%lld\n%lld", ans, count >> 1);
	return 0;
}
