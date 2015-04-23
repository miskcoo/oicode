/* BZOJ-2938: [Poi2000]病毒
 *   AC自动机  */
#include <cstdio>

const int MaxN = 30010, MaxAlpha = 2;
struct node_t
{
	int accept, mark, vis;
	node_t *fail, *ch[MaxAlpha];
} node[MaxN];
int used;
char str[MaxN];
node_t *root = node + used++, *que[MaxN];

void add_string(const char* str)
{
	node_t *n = root;
	for(int i = 0; str[i]; ++i)
	{
		int x = str[i] - '0';
		if(!n->ch[x]) n->ch[x] = node + used++;
		n = n->ch[x];
	}

	n->accept = 1;
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
			node_t *z = n->fail, *r = n->ch[i];
			if(!r) 
			{
				n->ch[i] = n->fail->ch[i];
			} else {
				while(z && !z->ch[i]) z = z->fail;
				r->fail = z ? z->ch[i] : root;
				r->accept |= r->fail->accept;
				que[qtail++] = r;
			}
		}
	}
}

bool dfs(node_t *z)
{
	z->mark = 1;
	for(int i = 0; i != MaxAlpha; ++i)
	{
		node_t *r = z->ch[i];
		if(r->mark) return true;
		if(r->vis || r->accept) continue;
		r->vis = 1;
		if(dfs(r)) return true;
	}
	z->mark = 0;
	return false;
}

int main()
{
	int n;
	std::scanf("%d", &n);
	for(int i = 0; i != n; ++i)
	{
		std::scanf("%s", str);
		add_string(str);
	}

	make_fail();
	if(dfs(root)) std::puts("TAK");
	else std::puts("NIE");
	return 0;
}
