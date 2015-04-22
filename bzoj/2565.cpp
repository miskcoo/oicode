/* BZOJ-2565: 最长双回文串
 *   回文自动机 */
#include <cstdio>
#include <cstring>
#include <algorithm>

const int MaxN = 100010, MaxAlpha = 26;
struct node_t
{
	int len;
	node_t *fail, *ch[MaxAlpha];
} node[MaxN << 1];

char str[MaxN];
int used, prev[MaxN], next[MaxN];
node_t *pam_odd, *pam_even, *pam_tail;
void pam_init()
{
	pam_odd = node + used++;
	pam_even = node + used++;
	pam_tail = pam_odd;
	pam_odd->fail = pam_even->fail = pam_odd;
	pam_odd->len = -1, pam_even->len = 0;
}

int pam_extend(const char *str, int n)
{
	int x = str[n] - 'a';
	node_t *p = pam_tail;
	while(str[n - p->len - 1] != str[n]) p = p->fail;
	if(!p->ch[x])
	{
		node_t *z = node + used++;
		z->len = p->len + 2;
		node_t *q = p->fail;
		while(str[n - q->len - 1] != str[n]) q = q->fail;
		z->fail = q->ch[x] ? q->ch[x] : pam_even;
		p->ch[x] = z;
	} 

	pam_tail = p->ch[x];
	return pam_tail->len;
}

int main()
{
	std::scanf("%s", str);
	int len = std::strlen(str);

	pam_init();
	for(int i = 0; i != len; ++i)
		prev[i] = pam_extend(str, i);

	std::reverse(str, str + len);
	pam_init();
	for(int i = 0; i != len; ++i)
		next[len - i - 1] = pam_extend(str, i);

	int ans = 0;
	for(int i = 0; i + 1 < len; ++i)
		ans = std::max(ans, prev[i] + next[i + 1]);
	std::printf("%d", ans);
	return 0;
}
