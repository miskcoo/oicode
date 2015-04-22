/* BZOJ-3676: [Apio2014]回文串 
 *   回文自动机 */
#include <cstdio>

const int MaxN = 300100, MaxAlpha = 26;
struct node_t
{
	int len, w;
	node_t *fail, *ch[MaxAlpha];
} node[MaxN];

int used;
char str[MaxN];
node_t *pam_odd, *pam_even, *pam_tail;

void pam_init()
{
	pam_odd = node + used++;
	pam_even = node + used++;
	pam_tail = pam_odd;
	pam_even->fail = pam_odd->fail = pam_odd;
	pam_even->len = 0, pam_odd->len = -1;
}

void pam_extend(const char* str, int x)
{
	node_t *p = pam_tail;
	while(str[x - p->len - 1] != str[x]) p = p->fail;
	if(!p->ch[str[x] - 'a'])
	{
		node_t *n = node + used++;
		n->len = p->len + 2;
		node_t *q = p->fail;
		while(str[x - q->len - 1] != str[x]) q = q->fail;
		n->fail = q->ch[str[x] - 'a'];
		if(!n->fail) n->fail = pam_even;
		p->ch[str[x] - 'a'] = n;
	} 

	pam_tail = p->ch[str[x] - 'a'];
	++pam_tail->w;
}

int main()
{
	std::scanf("%s", str);
	pam_init();
	for(int i = 0; str[i]; ++i)
		pam_extend(str, i);

	long long ans = 0;
	for(int i = used - 1; i > 1; --i)
	{
		node_t *n = node + i;
		n->fail->w += n->w;
		long long d = (long long)n->w * n->len;
		if(d > ans) ans = d;
	}

	std::printf("%lld", ans);
	return 0;
}
