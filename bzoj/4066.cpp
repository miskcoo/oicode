/* BZOJ-4066: 简单题
 *   KD-Tree+重量平衡 */
#include <cstdio>
#include <algorithm>
#include <cstring>

struct point_t
{
	int val, p[2];
};

const int MaxN = 200010;
struct node_t
{
	char d;
	int v, sum, size;
	point_t p, min, max;
	node_t *l, *r;
} node[MaxN];
int used;

void kdtree_update(node_t *now, node_t *z)
{
	if(!now || !z) return;
	for(int i = 0; i != 2; ++i)
	{
		now->min.p[i] = std::min(now->min.p[i], z->min.p[i]);
		now->max.p[i] = std::max(now->max.p[i], z->max.p[i]);
	}
}

double alpha = 0.60;
point_t pt, opt[MaxN];
node_t *mem[MaxN];
int mem_used, dim;
bool operator < (const point_t& a, const point_t& b)
{
	return a.p[dim] < b.p[dim];
}

node_t* kdtree_build(int l, int r, int d = 0)
{
	if(r < l) return 0;
	int m = (l + r) >> 1;
	dim = d;
	std::nth_element(opt + l, opt + m, opt + r + 1);
	node_t *n = mem[mem_used++];
	n->p = n->min = n->max = opt[m]; 
	n->size = r - l + 1; n->d = d;
	n->l = kdtree_build(l, m - 1, d ^ 1);
	n->r = kdtree_build(m + 1, r, d ^ 1);
	n->sum = n->v= opt[m].val;
	if(n->l) n->sum += n->l->sum;
	if(n->r) n->sum += n->r->sum;
	kdtree_update(n, n->l);
	kdtree_update(n, n->r);
	return n;
}

void kdtree_flatten(node_t *n)
{
	if(!n) return;
	opt[mem_used] = n->p;
	mem[mem_used++] = n;
	kdtree_flatten(n->l);
	kdtree_flatten(n->r);
}

void kdtree_rebuild(node_t *n)
{
	mem_used = 0;
	kdtree_flatten(n);
	opt[mem_used] = pt;
	mem[mem_used++] = node + used++;
	int size = mem_used;
	mem_used = 0;
	kdtree_build(0, size - 1, n->d);
}

void kdtree_insert(node_t *&now, int dim = 0)
{
	if(!now)
	{
		now = node + used++;
		now->size = 1; now->d = dim;
		now->v = now->sum = pt.val;
		now->l = now->r = 0;
		now->p = now->min = now->max = pt;
	} else {
		if((now->l && now->size * alpha < now->l->size)
			|| (now->r && now->size * alpha < now->r->size))
		{
			kdtree_rebuild(now);
		} else if(pt.p[dim] <= now->p.p[dim])
		{
			++now->size;
			now->sum += pt.val;
			kdtree_insert(now->l, dim ^ 1);
			kdtree_update(now, now->l);
		} else {
			++now->size;
			now->sum += pt.val;
			kdtree_insert(now->r, dim ^ 1);
			kdtree_update(now, now->r);
		}

	}
}

int x1, x2, y1, y2, lastans;
bool kdtree_check(const point_t& p)
{
	return x1 <= p.p[0] && p.p[0] <= x2 && y1 <= p.p[1] && p.p[1] <= y2;
}

bool kdtree_check1(node_t *z)
{
	if(!z) return false;
	int cnt = 0;
	cnt += kdtree_check(z->min);
	cnt += kdtree_check(z->max);
	return cnt == 2;
}

bool kdtree_check2(node_t *z)
{
	if(!z) return false;
	return x1 <= z->max.p[0] && x2 >= z->min.p[0]
		&& y1 <= z->max.p[1] && y2 >= z->min.p[1];
}

void kdtree_ask(node_t *n)
{
	if(!n) return;
	if(kdtree_check(n->p))
		lastans += n->v;
	if(kdtree_check1(n->l)) lastans += n->l->sum;
	else if(kdtree_check2(n->l)) kdtree_ask(n->l);

	if(kdtree_check1(n->r)) lastans += n->r->sum;
	else if(kdtree_check2(n->r)) kdtree_ask(n->r);
}

int main()
{
	std::scanf("%*d");
	int op;
	node_t *root = 0;
	while(std::scanf("%d", &op), op != 3)
	{
		if(op == 1)
		{
			int x, y, a;
			std::scanf("%d %d %d", &x, &y, &a);
			pt.p[0] = x ^ lastans;
			pt.p[1] = y ^ lastans;
			pt.val = a ^ lastans;
			kdtree_insert(root);
		} else {
			std::scanf("%d %d %d %d", &x1, &y1, &x2, &y2);
			x1 ^= lastans; x2 ^= lastans;
			y1 ^= lastans; y2 ^= lastans;
			lastans = 0;
			kdtree_ask(root);
			std::printf("%d\n", lastans);
		}
	}
	return 0;
}
