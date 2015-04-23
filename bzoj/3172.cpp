/* BZOJ-3172: [Tjoi2013]单词
 *   AC自动机   */
#include <cstdio>

const int MaxN = 1000010, MaxAlpha = 26;
struct node_t
{
	int w;
	node_t *fail, *ch[MaxAlpha];
} node[MaxN];
int used;
char str[MaxN];
node_t *root = node + used++, *que[MaxN], *pos[200];

node_t* add_string(const char* str)
{
	node_t *now = root;
	for(int i = 0; str[i]; ++i)
	{
		int x = str[i] - 'a';
		if(!now->ch[x])
			now->ch[x] = node + used++;
		now = now->ch[x];
		++now->w;
	}

	return now;
}

void make_fail()
{
	int qhead = 0, qtail = 0;
	que[qtail++] = root;
	while(qhead != qtail)
	{
		node_t *n = que[qhead++];
		for(int x = 0; x != MaxAlpha; ++x)
		{
			if(!n->ch[x]) continue;
			node_t *z = n->fail;
			while(z && !z->ch[x]) z = z->fail;
			n->ch[x]->fail = z ? z->ch[x] : root;
			que[qtail++] = n->ch[x];
		}
	}

	for(int i = qtail - 1; i; --i)
		que[i]->fail->w += que[i]->w;
}

int main()
{
	int n;
	std::scanf("%d", &n);
	for(int i = 0; i != n; ++i)
	{
		std::scanf("%s", str);
		pos[i] = add_string(str);
	}

	make_fail();
	for(int i = 0; i != n; ++i)
		std::printf("%d\n", pos[i]->w);
	return 0;
}
