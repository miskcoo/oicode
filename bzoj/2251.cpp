/* BZOJ-2251: [2010Beijing Wc]外星联络
 *   后缀自动机 */
#include <cstdio>

const int MaxN = 6010, MaxAlpha = 2;
struct node_t
{
	int len, w;
	node_t *fa, *ch[MaxAlpha];
} node[MaxN];
char str[MaxN];
int used, cnt[MaxN], sn[MaxN];
node_t *sam_head = node + used++, *sam_tail = sam_head;

void sam_extend(int x)
{
	node_t *p = sam_tail, *n = node + used++;
	sam_tail = n; n->w = 1;
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
			*r = *q; r->w = 0;
			r->len = p->len + 1;
			q->fa = n->fa = r;
			for(; p && p->ch[x] == q; p = p->fa)
				p->ch[x] = r;
		}
	}
}

void dfs(node_t *now)
{
	if(!now) return;
	if(now->w > 1 && now != sam_head)
		std::printf("%d\n", now->w);
	dfs(now->ch[0]);
	dfs(now->ch[1]);
}

int main()
{
	int n;
	std::scanf("%d", &n);
	std::scanf("%s", str);
	for(int i = 0; str[i]; ++i)
		sam_extend(str[i] - '0');
	for(int i = 0; i != used; ++i) ++cnt[node[i].len];
	for(int i = 1; i <= n; ++i) cnt[i] += cnt[i - 1];
	for(int i = 0; i != used; ++i) sn[--cnt[node[i].len]] = i;
	for(int i = used - 1; i >= 0; --i)
	{
		node_t *n = node + sn[i];
		if(n->fa) n->fa->w += n->w;
	}
	dfs(sam_head);
	return 0;
}
