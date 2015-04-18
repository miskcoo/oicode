/* SPOJ-1812 Longest Common Substring II 
 *  后缀自动机 */
#include <cstdio>
#include <cstring>

const int MaxN = 400001, MaxAlpha = 26;
struct node_t
{
	int len, lcs, nlcs;
	node_t *f, *ch[MaxAlpha];
} node[MaxN << 1];

char str[MaxN];
int used, cnt[MaxN];
node_t *sn[MaxN << 1];
node_t *sam_head = node + used++, *sam_tail = sam_head;

void sam_extend(int x)
{
	node_t *p = sam_tail, *n = node + used++;
	sam_tail = n;
	n->lcs = n->len = p->len + 1;
	for(; p && !p->ch[x]; p = p->f)
		p->ch[x] = n;
	if(!p)
	{
		n->f = sam_head;
	} else {
		if(p->len + 1 == p->ch[x]->len)
		{
			n->f = p->ch[x];
		} else {
			node_t *q = p->ch[x], *r = node + used++;
			*r = *q;
			r->lcs = r->len = p->len + 1;
			n->f = q->f = r;
			for(; p && p->ch[x] == q; p = p->f)
				p->ch[x] = r;
		}
	}
}

int sort_node()
{
	int max = 0;
	for(int i = 0; i != used; ++i)
	{
		++cnt[node[i].len];
		if(node[i].len > max)
			max = node[i].len;
	}

	for(int i = 1; i <= max; ++i)
		cnt[i] += cnt[i - 1];
	int num = cnt[max];

	for(int i = 0; i != used; ++i)
		sn[--cnt[node[i].len]] = node + i;
	return num;
}

int main()
{
	std::scanf("%s", str);
	for(int i = 0; str[i]; ++i)
		sam_extend(str[i] - 'a');

	int tot = sort_node();
	while(std::scanf("%s", str) != EOF)
	{
		int len = 0;
		node_t *now = sam_head;
		for(int i = 0; str[i]; ++i)
		{
			int x = str[i] - 'a';
			if(now->ch[x])
			{
				++len;
				now = now->ch[x];
			} else {
				while(now && !now->ch[x]) now = now->f;
				if(!now) len = 0, now = sam_head;
				else len = now->len + 1, now = now->ch[x];
			}

			if(len > now->nlcs)
				now->nlcs = len;
		}

		for(int i = tot - 1; i >= 0; --i)
		{
			node_t *p = sn[i];
			if(p->lcs > p->nlcs)
				p->lcs = p->nlcs;
			if(p->f && p->f->nlcs < p->nlcs)
				p->f->nlcs = p->nlcs;
			p->nlcs = 0;
		}
	}

	int ans = 0;
	for(int i = 0; i != tot; ++i)
	{
		if(ans < sn[i]->lcs)
			ans = sn[i]->lcs;
	}

	std::printf("%d", ans);
	return 0;
}
