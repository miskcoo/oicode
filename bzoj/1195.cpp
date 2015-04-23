/* BZOJ-1195: [HNOI2006]最短母串
 *   AC自动机+状压DP */
#include <cstdio>
#include <algorithm>

const int MaxN = 12, MaxL = 51, MaxAlpha = 26;
const int MaxS = MaxN * MaxL * (1 << MaxN);
struct node_t
{
	int accept;
	node_t *fail, *ch[MaxAlpha];
} node[MaxL * MaxN];
char str[MaxL * MaxN], ch[MaxS];
short f[MaxL * MaxN][1 << MaxN], set[MaxS];
int used, prev[MaxS];
node_t *root = node + used++, *que[MaxS];

void add_string(const char* str, int mask)
{
	node_t *n = root;
	for(int i = 0; str[i]; ++i)
	{
		int x = str[i] - 'A';
		if(!n->ch[x]) n->ch[x] = node + used++;
		n = n->ch[x];
	}

	n->accept |= mask;
}

void make_fail()
{
	int qhead = 0, qtail = 0;
	que[qtail++] = root;
	while(qhead != qtail)
	{
		node_t *n = que[qhead++];
		for(int i = 0; i != MaxAlpha; ++i)
		{
			node_t *z = n->ch[i];
			if(!z) 
			{
				n->ch[i] = n->fail ? n->fail->ch[i] : root;
			} else {
				node_t *q = n->fail;
				while(q && !q->ch[i]) q = q->fail;
				z->fail = q ? q->ch[i] : root;
				z->accept |= z->fail->accept;
				que[qtail++] = z;
			}
		}
	}
}

void solve(int l)
{
	const short inf = 30000;
	for(int i = 0; i != used; ++i)
		for(int s = 0; s != 1 << l; ++s)
			f[i][s] = inf;

	int qhead = 0, qtail = 0;
	que[qtail] = root;
	set[qtail++] = 0;
	f[root - node][0] = 0;
	bool ok = false;
	while(!ok)
	{
		node_t *n = que[qhead];
		int u = n - node, s = set[qhead];
		for(int x = 0; !ok && x != MaxAlpha; ++x)
		{
			int z = n->ch[x] - node;
			int mask = n->ch[x]->accept;
			if(f[z][s | mask] > f[u][s] + 1)
			{
				f[z][s | mask] = f[u][s] + 1;
				if((s | mask) == (1 << l) - 1)
					ok = true;
				prev[qtail] = qhead;
				ch[qtail] = x;
				set[qtail] = s | mask;
				que[qtail++] = n->ch[x];
			}
		}

		++qhead;
	}

	int now = qtail - 1, len = 0;
	while(now)
	{
		str[len++] = ch[now] + 'A';
		now = prev[now];
	}

	str[len] = 0;
	std::reverse(str, str + len);
	std::puts(str);
}

int main()
{
	int n;
	std::scanf("%d", &n);
	for(int i = 0; i != n; ++i)
	{
		std::scanf("%s", str);
		add_string(str, 1 << i);
	}

	make_fail();
	solve(n);
	return 0;
}
