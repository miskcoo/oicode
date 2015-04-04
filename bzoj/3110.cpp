/* BZOJ-3110: [Zjoi2013]K大数查询
 *  线段树套线段树（权值+区间） */
#include <cstdio>

template<typename Type>
class memory_pool
{
	Type* data[1 << 16];
	int used, remain;
public:
	memory_pool() : used(0), remain(1 << 16) {}
	~memory_pool() 
	{
		for(int i = 0; i != used; ++i)
			delete[] data[i];
	}

	Type* fetch() 
	{
		if(remain + 1 < 1 << 16) 
			return data[used - 1] + ++remain;
		data[used++] = new Type[1 << 16];
		return data[used - 1] + (remain = 0);
	}
};

struct node_t
{
	int w, lazy;
	node_t *l, *r;
};

const int MaxN = 50010 * 4;
memory_pool<node_t> mem;
node_t *nil, *root[MaxN];
int n, m;

node_t* make_node()
{
	node_t* n = mem.fetch();
	n->l = n->r = nil;
	n->w = n->lazy = 0;
	return n;
}

node_t* modify(node_t* n, int head, int tail, int a, int b)
{
	if(n == nil) n = make_node();

	if(head == a && tail == b)
	{
		n->w += b - a + 1;
		++n->lazy;
		return n;
	}

	int m = (head + tail) >> 1;
	if(b <= m) n->l = modify(n->l, head, m, a, b);
	else if(m < a) n->r = modify(n->r, m + 1, tail, a, b);
	else {
		n->l = modify(n->l, head, m, a, m);
		n->r = modify(n->r, m + 1, tail, m + 1, b);
	}

	n->w = n->l->w + n->r->w + (tail - head + 1) * n->lazy;
	return n;
}

int query(node_t* n, int head, int tail, int a, int b)
{
	if(n == nil) return 0;
	if(head == a && tail == b)
		return n->w;
	int m = (head + tail) >> 1;
	int base = (b - a + 1) * n->lazy;
	if(b <= m) return query(n->l, head, m, a, b) + base;
	else if(m < a) return query(n->r, m + 1, tail, a, b) + base;
	return base + query(n->l, head, m, a, m)
		+ query(n->r, m + 1, tail, m + 1, b);
}

void insert(int a, int b, int c)
{
	int l = 1, r = n, now = 1;
	while(l < r)
	{
		int m = (l + r) >> 1;
		root[now] = modify(root[now], 1, n, a, b);
		if(c <= m) r = m, now <<= 1;
		else l = m + 1, now = (now << 1) + 1;
	}

	root[now] = modify(root[now], 1, n, a, b);
}

int ask(int a, int b, int k)
{
	int l = 1, r = n, now = 1;
	while(l < r)
	{
		int m = (l + r) >> 1;
		int t = query(root[now << 1], 1, n, a, b);
		if(k <= t) r = m, now <<= 1;
		else l = m + 1, now = (now << 1) + 1, k -= t;
	}

	return l;
}

int main()
{
	nil = mem.fetch();
	nil->l = nil->r = nil;
	nil->w = nil->lazy = 0;

	std::scanf("%d %d", &n, &m);
	for(int i = 0; i <= n << 2; ++i)
		root[i] = nil;
	for(int i = 0; i != m; ++i)
	{
		int op, a, b, c;
		std::scanf("%d %d %d %d", &op, &a, &b, &c);
		if(op == 1) insert(a, b, n - c + 1);
		else std::printf("%d\n", n - ask(a, b, c) + 1);
	}
	return 0;
}
