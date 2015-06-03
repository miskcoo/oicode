/* BZOJ-4103: [Thu Summer Camp 2015]异或运算
 *   可持久化Trie+二分 */
#include <cstdio>

template<typename Type>
class memory_pool
{
	Type *data[1 << 16];
	int used, remain;
public:
	memory_pool() : used(0), remain(0) {}
	~memory_pool() 
	{
		for(int i = 0; i != used; ++i)
			delete[] data[i];
	}

	Type* get() 
	{
		if(!remain) data[used++] = new Type[remain = 1 << 16];
		return data[used - 1] + --remain;
	}
};

const int MaxN = 1010, MaxM = 300010, MaxL = 30;
struct node_t
{
	int size;
	node_t *ch[2];
} *nil, *root[MaxM], *tmp[MaxN][2];

int X[MaxN], Y[MaxM];
memory_pool<node_t> mem;

node_t* trie_insert(node_t *now, int val, int depth = MaxL)
{
	node_t *n = mem.get();
	*n = *now;
	++n->size;
	if(depth < 0) return n;
	int x = (val >> depth) & 1;
	n->ch[x] = trie_insert(n->ch[x], val, depth - 1);
	return n;
}

int trie_ask()
{
	int u, d, l, r, k;
	std::scanf("%d %d %d %d %d", &u, &d, &l, &r, &k);
	for(int i = u; i <= d; ++i)
	{
		tmp[i][0] = root[l - 1];
		tmp[i][1] = root[r];
	}

	int ans = 0;
	for(int t = MaxL; t >= 0; --t)
	{
		int count = 0;
		for(int i = u; i <= d; ++i)
		{
			int x = (X[i] >> t) & 1;
			count += tmp[i][1]->ch[!x]->size - tmp[i][0]->ch[!x]->size;
		}

		if(count < k)
		{
			k -= count;
		} else {
			ans |= 1 << t;
		}

		for(int i = u; i <= d; ++i)
		{
			int x = (X[i] >> t) & 1;
			int y = (ans >> t) & 1;
			tmp[i][0] = tmp[i][0]->ch[x ^ y];
			tmp[i][1] = tmp[i][1]->ch[x ^ y];
		}
	}

	return ans;
}

int main()
{
	static node_t nil_base;
	nil = &nil_base;
	nil->ch[0] = nil->ch[1] = nil;
	nil->size = 0;

	int n, m;
	std::scanf("%d %d", &n, &m);
	for(int i = 1; i <= n; ++i)
		std::scanf("%d", X + i);
	for(int i = 1; i <= m; ++i)
		std::scanf("%d", Y + i);
	root[0] = nil;
	for(int i = 1; i <= m; ++i)
		root[i] = trie_insert(root[i - 1], Y[i]);
	int p;
	std::scanf("%d", &p);
	for(int i = 0; i != p; ++i)
		std::printf("%d\n", trie_ask());
	return 0;
}
