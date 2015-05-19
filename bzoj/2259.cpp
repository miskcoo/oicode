/* BZOJ-2259: [Oibh]新型计算机 
 *   DP+堆 */
#include <cstdio>
#include <queue>
#include <functional>
#include <algorithm>

const int MaxN = 1000010;
int total;
int head[MaxN], point[MaxN], next[MaxN];
int f[MaxN], val[MaxN];

void add_edge(int u, int v)
{
	point[++total] = v;
	next[total] = head[u];
	head[u] = total;
}

struct data_t
{
	int dtime, val;
	bool operator < (const data_t& v) const
	{
		return val < v.val;
	}
};

struct node_t
{
	node_t *l, *r;
	int len;
	data_t v;
} node[MaxN];
int used;
node_t *nil = node + used++;

node_t* merge(node_t* a, node_t* b)
{
	if(a == nil) return b;
	if(b == nil) return a;
	if(b->v < a->v) std::swap(a, b);
	a->r = merge(a->r, b);
	if(a->l->len < a->r->len) std::swap(a->l, a->r);
	a->len = a->r->len + 1;
	return a;
}

int main()
{
	nil->l = nil->r = nil;
	int n;
	std::scanf("%d", &n);
	for(int i = 1; i <= n; ++i)
		std::scanf("%d", val + i);

	node_t *root = nil;
	int part2 = 1000000001;
	for(int i = 1; i <= n; ++i)
	{
		int dtime = val[i] + i;
		if(dtime <= i)
		{
			part2 = std::min(part2, f[i - 1] - dtime);
		} else {
			data_t d;
			d.dtime = dtime;
			d.val = f[i - 1] + dtime;
			node_t *z = node + used++;
			*z = *nil;
			z->v = d;
			root = merge(root, z);
			if(dtime <= n) add_edge(dtime, i);
		}

		for(int k = head[i]; k; k = next[k])
		{
			int j = point[k];
			part2 = std::min(part2, f[j - 1] - (val[j] + j));
		}

		f[i] = part2 + i;
		while(root != nil)
		{
			if(root->v.dtime < i) root = merge(root->l, root->r);
			else {
				f[i] = std::min(f[i], root->v.val - i);
				break;
			}
		}
	}

	std::printf("%d", f[n]);
	return 0;
}
