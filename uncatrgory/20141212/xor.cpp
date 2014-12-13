#include <cstdio>

const int MaxN = 15000000;
const int MaxH = 24;
class functional_trie
{
	int total, num;

	struct node_t
	{
		int son[2];
		int size;
	} node[MaxN];
	int root[MaxN];
public:
	functional_trie() : total(1), num(1) {}

	int insert(int now, int x, int bit)
	{
		if(!bit)
		{
			node[total].size = node[now].size + 1;
			return total++;
		}

		int b = (x >> (bit - 1)) & 1;

		int id = total++;
		node[id] = node[now];
		node[id].son[b] = insert(node[now].son[b], x, bit - 1);
		node[id].size = node[node[id].son[0]].size 
			+ node[node[id].son[1]].size;
		return id;
	}

	void add_value(int x)
	{
		root[num] = insert(root[num - 1], x, MaxH);
		++num;
	}

	int find_value(int l, int r, int x)
	{
		int ans = 0;
		l = root[l - 1], r = root[r];
		for(int i = MaxH - 1; i >= 0; --i)
		{
			int b = ((x >> i) & 1) ^ 1;
			if(node[node[r].son[b]].size > node[node[l].son[b]].size)
			{
				ans = (ans << 1) | 1;
			} else {
				b ^= 1;
				ans <<= 1;
			}

			l = node[l].son[b];
			r = node[r].son[b];
		}

		return ans;
	}
} trie;

int main()
{
	std::freopen("xor.in", "r", stdin);
	std::freopen("xor.out", "w", stdout);
	int n, m, xor_sum = 0;
	std::scanf("%d %d", &n, &m);
	trie.add_value(0);
	for(int i = 0; i != n; ++i)
	{
		int x;
		std::scanf("%d", &x);
		trie.add_value(xor_sum ^= x);
	}

	for(int i = 0; i != m; ++i)
	{
		char op[2];
		std::scanf("%s", op);
		if(*op == 'A')
		{
			int x;
			std::scanf("%d", &x);
			trie.add_value(xor_sum ^= x);
		} else {
			int l, r, x;
			std::scanf("%d %d %d", &l, &r, &x);
			std::printf("%d\n", trie.find_value(l, r, x ^ xor_sum));
		}
	}
	return 0;
}
