/* BZOJ-4056: [Ctsc2015]shallot
 *   可持久化Treap+KD-Tree */
#include <cstdio>
#include <cstdlib>
#include <algorithm>

const int MaxN = 100001;

struct point_t
{
	int p[2];
	int& operator [](size_t z) { return p[z]; }
	const int& operator [](size_t z) const { return p[z]; }
} pt[MaxN << 1];

struct node_t
{
	int size, id, lmost, rmost;
	point_t min, max;
	node_t *l, *r;
} node[MaxN * 60];
int used, ptnum;
node_t *root[MaxN];

void pushup(node_t *n)
{
	n->size = 1;
	n->min = n->max = pt[n->id];
	n->lmost = n->rmost = n->id;
	if(n->l)
	{
		n->size += n->l->size;
		for(int i = 0; i != 2; ++i)
		{
			n->min[i] = std::min(n->min[i], n->l->min[i]);
			n->max[i] = std::max(n->max[i], n->l->max[i]);
			n->lmost = n->l->lmost;
		}
	}

	if(n->r) 
	{
		n->size += n->r->size;
		for(int i = 0; i != 2; ++i)
		{
			n->min[i] = std::min(n->min[i], n->r->min[i]);
			n->max[i] = std::max(n->max[i], n->r->max[i]);
			n->rmost = n->r->rmost;
		}
	}
}

node_t* merge(node_t *a, node_t *b)
{
	if(!a) return b;
	if(!b) return a;
	node_t *n = node + used++;
	if(std::rand() % (a->size + b->size) < a->size)
	{
		*n = *a;
		n->r = merge(n->r, b);
	} else {
		*n = *b;
		n->l = merge(a, n->l);
	}

	pushup(n);
	return n;
}

node_t* split_left(node_t *n, int size)
{
	if(!n || !size) return 0;
	int sz = n->l ? n->l->size + 1 : 1;
	if(size < sz)
	{
		return split_left(n->l, size);
	} else {
		node_t *z = node + used++;
		*z = *n;
		z->r = split_left(n->r, size - sz);
		pushup(z);
		return z;
	}
	
}

node_t* split_right(node_t *n, int size)
{
	if(!n || !size) return 0;
	int sz = n->r ? n->r->size + 1 : 1;
	if(size < sz)
	{
		return split_right(n->r, size);
	} else {
		node_t *z = node + used++;
		*z = *n;
		z->l = split_right(n->l, size - sz);
		pushup(z);
		return z;
	}
}

long long A, B, C, ans;
bool check(long long a, long long b)
{
	return a >= 0 && b <= 0 || a <= 0 && b >= 0;
}

long long eval(long long x, long long y)
{
	return A * x + B * y + C;
}

bool check_line(int a, int b)
{
	return check(eval(pt[a][0], pt[a][1]), eval(pt[b][0], pt[b][1]));
}

void ask(node_t *n)
{
	if(!n) return;
	if(n->l) ans += check_line(n->l->rmost, n->id);
	if(n->r) ans += check_line(n->r->lmost, n->id);
	long long d[4];
	d[0] = eval(n->min[0], n->min[1]);
	d[1] = eval(n->max[0], n->max[1]);
	d[2] = eval(n->min[0], n->max[1]);
	d[3] = eval(n->max[0], n->min[1]);
	int cnt = check(d[0], d[1]) + check(d[1], d[2]) 
		+ check(d[2], d[3]) + check(d[3], d[0]);
	if(cnt) ask(n->l), ask(n->r);
}

int main()
{
	int n, m, encoded;
	std::scanf("%d %d %d", &n, &m, &encoded);
	for(int i = 1; i <= n; ++i)
	{
		std::scanf("%d %d", &pt[i][0], &pt[i][1]);
		node_t *n = node + used++;
		n->id = i; pushup(n);
		root[0] = merge(root[0], n);
	}
	ptnum = n;

	for(int i = 1; i <= m; ++i)
	{
		char op[2];
		std::scanf("%s", op);
		if(*op == 'H')
		{
			int t, x0, y0, x, y;
			std::scanf("%d %d %d %d %d", &t, &x0, &y0, &x, &y);
			if(encoded) x0 ^= ans, y0 ^= ans, x ^= ans, y ^= ans;
			if(x == 0)
			{
				A = 1;
				B = 0;
				C = -x0;
			} else {
				A = y;
				B = -x;
				C = 1LL * y0 * x - 1LL * x0 * y;
			}

			ans = 0;
			ask(root[t]);
			root[i] = root[t];
			std::printf("%lld\n", ans);
		} else {
			int t, a, x, y;
			std::scanf("%d %d %d %d", &t, &a, &x, &y);
			if(encoded) x ^= ans, y ^= ans;
			node_t *l = split_left(root[t], a);
			node_t *r = split_right(root[t], root[t]->size - a);
			node_t *z = node + used++;
			z->id = ++ptnum;
			pt[ptnum][0] = x, pt[ptnum][1] = y;
			pushup(z);
			root[i] = merge(l, merge(z, r));
		}
	}
	return 0;
}
