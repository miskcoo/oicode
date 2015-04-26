/* BZOJ-2946: [Poi2000]公共串
 *   后缀自动机 */
#include <cstdio>
#include <algorithm>
#include <cstring>

const int MaxN = 4010, MaxAlpha = 26;
struct node_t
{
	int len, lcs, nlcs;
	node_t *fa, *ch[MaxAlpha];
} node[MaxN];
char str[MaxN];
int used, cnt[MaxN], sn[MaxN];
node_t *sam_head = node + used++, *sam_tail = sam_head;

void sam_extend(int x)
{
	node_t *p = sam_tail, *n = node + used++;
	sam_tail = n;
	n->len = n->lcs = p->len + 1;
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
			r->lcs = r->len = p->len + 1;
			q->fa = n->fa = r;
			for(; p && p->ch[x] == q; p = p->fa)
				p->ch[x] = r;
		}
	}
}

int main()
{
	int n;
	std::scanf("%d", &n);
	std::scanf("%s", str);
	for(int i = 0; str[i]; ++i)
		sam_extend(str[i] - 'a');

	int len = std::strlen(str);
	for(int i = 0; i != used; ++i) ++cnt[node[i].len];
	for(int i = 1; i <= len; ++i) cnt[i] += cnt[i - 1];
	for(int i = 0; i != used; ++i) sn[--cnt[node[i].len]] = i;
	
	for(int i = 1; i != n; ++i)
	{
		std::scanf("%s", str);
		int cnt = 0;
		node_t *n = sam_head;
		for(int j = 0; str[j]; ++j)
		{
			int x = str[j] - 'a';
			while(n && !n->ch[x])
				n = n->fa, cnt = n ? n->len : 0;
			if(!n) n = sam_head, cnt = 0;
			else n = n->ch[x], ++cnt;
			if(cnt > n->nlcs) n->nlcs = cnt;
		}

		for(int j = used - 1; j >= 0; --j)
		{
			node_t *n = node + sn[j];
			if(n->lcs > n->nlcs) n->lcs = n->nlcs;
			if(n->fa && n->fa->nlcs < n->nlcs)
				n->fa->nlcs = std::min(n->nlcs, n->fa->len);
			n->nlcs = 0;
		}
	}

	int ans = 0;
	for(int i = 0; i != used; ++i)
	{
		node_t *n = node + sn[i];
		if(n->lcs > ans) ans = n->lcs;
	}

	std::printf("%d", ans);
	return 0;
}
