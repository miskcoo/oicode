/* BZOJ-4003: [JLOI2015]城池攻占
 *   左偏树  */
#include <cstdio>
#include <algorithm>

const int MaxN = 300001;
struct node_t
{
	int len, count, lazy;
	long long add_v, mul_v, val;
	node_t *l, *r;
} node[MaxN];
int fa[MaxN], oper[MaxN], ans[MaxN];
long long val[MaxN], defend[MaxN];
node_t *nil = node, *root[MaxN];

void update(node_t *d, int lazy, long long add_v, long long mul_v)
{
	if(d == nil) return;
	d->val = d->val * mul_v + add_v;
	d->mul_v *= mul_v;
	d->add_v = d->add_v * mul_v + add_v;
	d->count += lazy;
	d->lazy += lazy;
}

void pushdown(node_t *n)
{
	if(n->mul_v != 1 || n->add_v != 0 || n->lazy != 0)
	{
		update(n->l, n->lazy, n->add_v, n->mul_v);
		update(n->r, n->lazy, n->add_v, n->mul_v);
		n->mul_v = 1, n->add_v = n->lazy = 0;
	}
}

node_t* merge(node_t *a, node_t *b)
{
	if(a == nil) return b;
	if(b == nil) return a;
	pushdown(a), pushdown(b);
	if(a->val > b->val) 
		std::swap(a, b);
	a->r = merge(a->r, b);
	if(a->r->len > a->l->len)
		std::swap(a->l, a->r);
	a->len = a->r->len + 1;
	return a;
}

void clear_lazy(node_t *n, int v)
{
	if(n == nil) return;
	n->count += v;
	clear_lazy(n->l, v + n->lazy);
	clear_lazy(n->r, v + n->lazy);
}

int main()
{
	nil->l = nil->r = nil;
	nil->mul_v = 1, nil->add_v = 0;
	nil->lazy = nil->count = 0;
	int n, m;
	std::scanf("%d %d", &n, &m);
	for(int i = 1; i <= n; ++i)
	{
		root[i] = nil;
		std::scanf("%lld", defend + i);
	}
	for(int i = 2; i <= n; ++i)
		std::scanf("%d %d %lld", fa + i, oper + i, val + i);
	for(int i = 1; i <= m; ++i)
	{
		int c;
		node[i] = *nil;
		std::scanf("%lld %d", &node[i].val, &c);
		root[c] = merge(root[c], node + i);
	}

	oper[1] = 1, val[1] = 0;
	for(int i = n; i; --i)
	{
		while(root[i] != nil && root[i]->val < defend[i])
		{
			pushdown(root[i]);
			root[i] = merge(root[i]->l, root[i]->r), ++ans[i];
		}

		if(!oper[i]) update(root[i], 1, val[i], 1);
		else update(root[i], 1, 0, val[i]);
		if(fa[i]) root[fa[i]] = merge(root[fa[i]], root[i]);
	}

	clear_lazy(root[1], 0);
	for(int i = 1; i <= n; ++i)
		std::printf("%d\n", ans[i]);
	for(int i = 1; i <= m; ++i)
		std::printf("%d\n", node[i].count);
	return 0;
}
