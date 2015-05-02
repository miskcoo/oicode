/* BZOJ-2850: 巧克力王国
 *   KD-Tree
 *   注意减枝的时候子树没有东西就不要递归 */
#include <algorithm>
#include <cstdio>

const int MaxN = 50010;
struct point_t
{
	int d[2], v;
	int& operator [](size_t z) { return d[z]; }
	const int& operator [](size_t z) const { return d[z]; }
} pt[MaxN];

struct node_t
{
	node_t *l, *r;
	int v;
	long long sum;
	point_t p, min, max;
} node[MaxN];

int dim, used;
bool cmp(const point_t& a, const point_t& b)
{
	return a[dim] < b[dim];
}

void kdtree_update(node_t* n)
{
	n->min = n->max = n->p;
	n->v = n->sum = n->p.v;
	for(int i = 0; i != 2; ++i)
	{
		if(n->l) 
		{
			n->min[i] = std::min(n->min[i], n->l->min[i]);
			n->max[i] = std::max(n->max[i], n->l->max[i]);
		}

		if(n->r) 
		{
			n->min[i] = std::min(n->min[i], n->r->min[i]);
			n->max[i] = std::max(n->max[i], n->r->max[i]);
		}
	}

	if(n->l) n->sum += n->l->sum;
	if(n->r) n->sum += n->r->sum;
}

node_t* kdtree_build(int l, int r, int d = 0)
{
	if(l > r) return 0;
	node_t *now = node + used++;
	int m = (l + r) >> 1;
	dim = d;
	std::nth_element(pt + l, pt + m, pt + r + 1, cmp);
	now->p = pt[m];
	now->l = kdtree_build(l, m - 1, d ^ 1);
	now->r = kdtree_build(m + 1, r, d ^ 1);
	kdtree_update(now);
	return now;
}

long long A, B, C;

bool kdtree_check(int x, int y)
{
	return A * x + B * y < C;
}

int kdtree_check(node_t* n)
{
	if(!n) return 0;
	int tot = 0;
	tot += kdtree_check(n->min[0], n->min[1]);
	tot += kdtree_check(n->max[0], n->min[1]);
	tot += kdtree_check(n->min[0], n->max[1]);
	tot += kdtree_check(n->max[0], n->max[1]);
	return tot;
}

long long kdtree_ask(node_t* now)
{
	if(!now) return 0;
	long long ans = 0;
	if(kdtree_check(now->p[0], now->p[1]))
		ans += now->v;
	int l = kdtree_check(now->l);
	int r = kdtree_check(now->r);
	if(l == 4) ans += now->l->sum;
	else if(l) ans += kdtree_ask(now->l);
	if(r == 4) ans += now->r->sum;
	else if(r) ans += kdtree_ask(now->r);
	return ans;
}

int main()
{
	int n, m;
	std::scanf("%d %d", &n, &m);
	for(int i = 0; i != n; ++i)
		std::scanf("%d %d %d", pt[i].d, pt[i].d + 1, &pt[i].v);
	node_t *root = kdtree_build(0, n - 1);
	for(int i = 0; i != m; ++i)
	{
		std::scanf("%lld %lld %lld", &A, &B, &C);
		std::printf("%lld\n", kdtree_ask(root));
	}
	return 0;
}
