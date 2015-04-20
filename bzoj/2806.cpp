/* BZOJ-2806: [Ctsc2012]Cheat
 *  后缀自动机+单调队列DP */
#include <cstdio>
#include <algorithm>
#include <cstring>

const int MaxN = 1100000, MaxAlpha = 3;
struct node_t
{
	int len;
	node_t *fa, *ch[MaxAlpha];
} node[MaxN << 1];
char str[MaxN];
int used = 1, d[MaxN], f[MaxN], que[MaxN];
node_t *sam_head = node, *sam_tail = sam_head;

void sam_extend(int x)
{
	node_t *p = sam_tail, *n = node + used++;
	sam_tail = n;
	n->len = p->len + 1;
	for(; p && !p->ch[x]; p = p->fa)
		p->ch[x] = n;
	if(!p)
	{
		n->fa = sam_head;
	} else {
		if(p->len + 1 == p->ch[x]->len)
		{
			n->fa = p->ch[x];
		} else {
			node_t *q = p->ch[x], *r = node + used++;
			*r = *q;
			r->len = p->len + 1;
			q->fa = n->fa = r;
			for(; p && p->ch[x] == q; p = p->fa)
				p->ch[x] = r;
		}
	}
}

bool check(int n, int l)
{
	f[0] = 0;
	int qhead = 0, qtail = 0;
	for(int i = 1; i <= n; ++i)
	{
		if(i - l >= 0)
		{
			int p = i - l;
			while(qhead != qtail && f[que[qtail - 1]] - que[qtail - 1] <= f[p] - p) --qtail;
			que[qtail++] = p;
		}

		while(qhead != qtail && que[qhead] < i - d[i]) ++qhead;
		f[i] = 0;
		if(qhead != qtail)
			f[i] = f[que[qhead]] - que[qhead] + i;
		if(f[i - 1] > f[i]) f[i] = f[i - 1];
	}

	return f[n] * 10 >= n * 9;
}

void solve_case()
{
	std::scanf("%s", str);
	int len = std::strlen(str);
	node_t *now = sam_head;
	int cur = 0;
	for(int i = 0; str[i]; ++i)
	{
		int x = str[i] - '0';
		while(!now->ch[x]) 
		{
			now = now->fa;
			cur = now->len;
		}

		now = now->ch[x];
		d[i + 1] = ++cur;
	}

	int l = 0, r = len;
	while(l < r)
	{
		int m = (l + r + 1) >> 1;
		if(check(len, m)) l = m;
		else r = m - 1;
	}

	std::printf("%d\n", l);
}

int main()
{
	int n, m;
	std::scanf("%d %d", &n, &m);
	for(int i = 0; i != m; ++i)
	{
		std::scanf("%s", str);
		for(int j = 0; str[j]; ++j)
			sam_extend(str[j] - '0');
		sam_extend(2);
	}

	for(int i = 0; i != n; ++i)
		solve_case();
	return 0;
}
