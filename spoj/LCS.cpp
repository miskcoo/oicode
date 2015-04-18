/* SPOJ-1811 Longest Common Substring
 *  后缀自动机 */
#include <cstdio>
#include <cstring>

template<typename Type>
class memory_pool
{
	int used, remain;
	Type* data[1 << 16];
public:
	memory_pool() : used(0), remain(0) {}
	~memory_pool() 
	{
		for(int i = 0; i != used; ++i)
			delete[] data[i];
	}

	Type* get()
	{
		if(remain == 0)
		{
			data[used++] = new Type[1 << 16];
			remain = 1 << 16;
		}

		return data[used - 1] + (--remain);
	}
};

const int MaxAlpha = 26, MaxN = 250001;
struct node_t
{
	int right;
	node_t *fa, *son[MaxAlpha];

	node_t() : right(0), fa(0)
	{
		std::memset(son, 0, sizeof(son));
	}
};

memory_pool<node_t> mem;
node_t *sam_init = mem.get(), *sam_last = sam_init;

void sam_extend(int c)
{
	node_t *n = mem.get(), *p = sam_last;
	sam_last = n;
	n->right = p->right + 1;
	for(; p && !p->son[c]; p = p->fa)
		p->son[c] = n;
	if(!p) 
	{
		n->fa = sam_init;
	} else {
		if(p->right + 1 == p->son[c]->right)
		{
			n->fa = p->son[c];
		} else {
			node_t *q = p->son[c], *r = mem.get();
			*r = *q;
			r->right = p->right + 1;
			q->fa = n->fa = r;
			for(; p && p->son[c] == q; p = p->fa)
				p->son[c] = r;
		}
	}
}

char str[MaxN];

int main()
{
	std::scanf("%s", str);
	for(int i = 0; str[i]; ++i)
		sam_extend(str[i] - 'a');
	std::scanf("%s", str);
	int ans = 0, len = 0;
	node_t *now = sam_init;
	for(int i = 0; str[i]; ++i)
	{
		int x = str[i] - 'a';
		if(now->son[x])
		{
			++len;
			now = now->son[x];
		} else {
			for(; now && !now->son[x]; now = now->fa);
			if(!now) len = 0, now = sam_init;
			else len = now->right + 1, now = now->son[x];
		}

		if(ans < len) ans = len;
	}

	std::printf("%d", ans);
	return 0;
}
