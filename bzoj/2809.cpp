/* BZOJ-2809: [Apio2012]dispatching
 *   左偏树（可合并堆） */
#include <cstdio>
#include <algorithm>

const int MaxN = 100010;
struct node_t
{
	int v, h;
	node_t *l, *r;
} node[MaxN];
node_t *root[MaxN], *nil = node;
int B[MaxN], L[MaxN], size[MaxN];
long long sum[MaxN];

node_t* merge(node_t *a, node_t *b)
{
	if(a == nil) return b;
	if(b == nil) return a;
	if(a->v < b->v) std::swap(a, b);
	a->r = merge(a->r, b);
	a->h = a->r->h + 1;
	if(a->l->h < a->r->h) std::swap(a->l, a->r);
	return a;
}

int main()
{
	int n, m;
	nil->l = nil->r = nil;
	std::scanf("%d %d", &n, &m);
	for(int i = 1; i <= n; ++i)
	{
		std::scanf("%d %d %d", B + i, &node[i].v, L + i);
		size[i] = 1;
		sum[i] = node[i].v;
		root[i] = node + i;
		root[i]->l = root[i]->r = nil;
	}

	long long ans = 0;
	for(int i = n; i; --i)
	{
		while(sum[i] > m)
		{
			--size[i];
			sum[i] -= root[i]->v;
			root[i] = merge(root[i]->l, root[i]->r);
		}

		ans = std::max(ans, (long long)size[i] * L[i]);
		if(!B[i]) continue;
		root[B[i]] = merge(root[B[i]], root[i]);
		size[B[i]] += size[i];
		sum[B[i]] += sum[i];
	}

	std::printf("%lld", ans);
	return 0;
}
