/* BZOJ-3932: [CQOI2015]任务查询系统
 *   主席树 */
#include <cstdio>
#include <algorithm>

template<typename Type>
class memory_pool
{
	int used, remain;
	Type *data[1 << 16];
public:
	memory_pool() : remain(0), used(0) {}
	~memory_pool() 
	{
		for(int i = 0; i != used; ++i)
			delete[] data[i];
	}

	Type* get() 
	{
		if(!remain) data[used++] = new Type[remain = 1 << 16];
		return data[used - 1] + --remain;
	}
};

struct node_t
{
	int num;
	long long sum;
	node_t *l, *r;
};

memory_pool<node_t> mem;
node_t *nil;

node_t* modify(node_t *now, int head, int tail, int pos, int w)
{
	node_t *n = mem.get();
	*n = *now;
	if(w > 0) ++n->num;
	else --n->num;
	n->sum += w;
	if(!n->num) return nil;
	if(head == tail) return n;
	int m = (head + tail) >> 1;
	if(pos <= m) n->l = modify(n->l, head, m, pos, w);
	else n->r = modify(n->r, m + 1, tail, pos, w);
	return n;
}

long long ask(node_t *now, int head, int tail, int k)
{
	int m = (head + tail) >> 1;
	if(head == tail) return 1ll * m * std::min(k, now->num);
	if(now->num <= k) return now->sum;
	if(now->l->num >= k) return ask(now->l, head, m, k);
	return now->l->sum + ask(now->r, m + 1, tail, k - now->l->num);
}

void init()
{
	static node_t nil_base;
	nil = &nil_base;
	nil->l = nil->r = nil;
	nil->sum = nil->num = 0;
}

const int MaxN = 200010, MaxP = 10000010;
struct oper_t
{
	int pos, w;
	bool operator < (const oper_t& v) const
	{
		return pos < v.pos;
	}
} oper[MaxN];
node_t *root[MaxN];

int main()
{
	int n, m, tot = 0;
	std::scanf("%d %d", &n, &m);
	for(int i = 0; i != n; ++i)
	{
		int s, e, p;
		std::scanf("%d %d %d", &s, &e, &p);
		oper[tot].w = p;
		oper[tot++].pos = s;
		oper[tot].w = -p;
		oper[tot++].pos = e + 1;
	}

	std::sort(oper, oper + tot);
	
	init();
	root[0] = nil;
	int prev = 0;
	for(int i = 0; i != tot; ++i)
	{
		if(oper[i].pos != prev)
		{
			for(int j = prev + 1; j <= oper[i].pos; ++j)
				root[j] = root[prev];
			prev = oper[i].pos;
		}

		int w = oper[i].w;
		root[prev] = modify(root[prev], 1, MaxP, w < 0 ? -w : w, w);
	}

	for(int i = prev + 1; i <= m; ++i)
		root[i] = root[i - 1];

	long long lastans = 1;
	for(int i = 0; i != m; ++i)
	{
		int x, a, b, c;
		std::scanf("%d %d %d %d", &x, &a, &b, &c);
		int k = (a * lastans + b) % c + 1;
		lastans = ask(root[x], 1, MaxP, k);
		std::printf("%lld\n", lastans);
	}
	return 0;
}
