/*
 * BZOJ-3261 最大异或和
 *  可持久化 Trie 树
 *
 * NOTE：2 1
 *       3 0
 *       Q 1 2 0
 */
#include <cstdio>

template<int RootMax, int DepthMax>
class functional_trie
{
	struct {
		int son[2];
		int size;
	} node[RootMax * DepthMax + RootMax];
	int root[RootMax];
	int node_used;
public:
	functional_trie()
	{
		node_used = 0;
		root[0] = 0;
		node[0].size = 0;
		node[0].son[0] = 0;
		node[0].son[1] = 0;
	}

	void add_elem(int now, int x)
	{
		for(int i = DepthMax; i >= 0; --i)
		{
			node[++node_used] = node[now];
			++node[node_used].size;
			int bit = (x >> i) & 1;
			node[node_used].son[bit] = node_used + 1;
			now = node[now].son[bit];
		}
		node[++node_used] = node[now];
		++node[node_used].size;
	}

	int ask(int l, int r, int x)
	{
		int result = 0;
		l = root[l], r = root[r];
		for(int i = DepthMax; i >= 0; --i)
		{
			int bit = ((x >> i) & 1) ^ 1;
			if(node[node[r].son[bit]].size == node[node[l].son[bit]].size)
				bit ^= 1;
			l = node[l].son[bit];
			r = node[r].son[bit];
			result |= bit << i;
		}
		return result;
	}

	void append(int r, int x)
	{
		root[r] = node_used + 1;
		add_elem(root[r - 1], x);
	}
};

functional_trie<600001, 23> trie;

int main()
{
	int N, M;
	int xor_sum = 0;
	std::scanf("%d %d", &N, &M);
	trie.append(1, 0);
	for(int i = 1; i <= N; ++i)
	{
		int v;
		std::scanf("%d", &v);
		trie.append(i + 1, xor_sum ^= v);
	}
	for(int i = 0; i != M; ++i)
	{
		char oper[2];
		std::scanf("%s", oper);
		if(*oper == 'Q')
		{
			int l, r, x;
			std::scanf("%d %d %d", &l, &r, &x);
			std::printf("%d\n", x ^ trie.ask(l - 1, r, x ^= xor_sum));
		} else {
			int x;
			std::scanf("%d", &x);
			trie.append(++N + 1, xor_sum ^= x);
		}
	}
	return 0;
}
