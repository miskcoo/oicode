/* BZOJ-1901: Zju2112 Dynamic Rankings
 *  树状数组套权值线段树 */
#include <cstdio>
#include <cstring>
#include <algorithm>

template<typename Type>
class memory_pool
{
	Type* data[1 << 16];
	int used, remain;
public:
	memory_pool() : used(0), remain(1 << 16) {}
	~memory_pool() 
	{
		for(int i = 0; i != used; ++i)
			delete[] data[i];
	}

	Type* fetch() 
	{
		if(remain != 1 << 16) return data[used - 1] + ++remain;
		data[used++] = new Type[1 << 16];
		return data[used - 1] + (remain = 0);
	}
};

struct node_t
{
	int w;
	node_t *l, *r;
};

const int MaxN = 20001;
memory_pool<node_t> mem;
int size, data[MaxN], map[MaxN], oper[MaxN][4];
node_t *left[MaxN], *right[MaxN], *root[MaxN];

node_t* update(node_t* now, int head, int tail, int pos, int val)
{
	node_t* n = mem.fetch();
	*n = *now;
	n->w += val;

	if(head == tail) return n;
	int m = (head + tail) >> 1;
	if(pos <= m) n->l = update(now->l, head, m, pos, val);
	else n->r = update(now->r, m + 1, tail, pos, val);
	return n;
}

void modify(int n, int x, int pos, int val)
{
	for(; x <= n; x += x & -x)
		root[x] = update(root[x], 1, size, pos, val);
}

int ask(int lc, int rc, int head, int tail, int k)
{
	if(head == tail) return head;

	int l = 0, r = 0;
	for(int i = 0; i != lc; ++i) l += left[i]->l->w;
	for(int i = 0; i != rc; ++i) r += right[i]->l->w;

	int m = (head + tail) >> 1;
	if(k <= r - l)
	{
		for(int i = 0; i != lc; ++i) left[i] = left[i]->l;
		for(int i = 0; i != rc; ++i) right[i] = right[i]->l;
		return ask(lc, rc, head, m, k);
	} else {
		for(int i = 0; i != lc; ++i) left[i] = left[i]->r;
		for(int i = 0; i != rc; ++i) right[i] = right[i]->r;
		return ask(lc, rc, m + 1, tail, k - (r - l));
	}
}

int get_ans(int l, int r, int k)
{
	int lc = 0, rc = 0;
	for(--l; l; l -= l & -l) left[lc++] = root[l];
	for(; r; r -= r & -r) right[rc++] = root[r];
	return data[ask(lc, rc, 1, size, k)];
}

int main()
{
	node_t* nil = mem.fetch();
	nil->l = nil->r = nil, nil->w = 0;
	
	int n, m, tot = 0;
	std::scanf("%d %d", &n, &m);
	for(int i = 1; i <= n; ++i)
		std::scanf("%d", data + i);

	tot = n;
	for(int i = 0; i != m; ++i)
	{
		char op[2];
		std::scanf("%s", op);
		if(op[0] == 'C')
		{
			oper[i][0] = 1;
			std::scanf("%d %d", oper[i] + 1, oper[i] + 2);
			data[++tot] = oper[i][2];
		} else {
			oper[i][0] = 0;
			std::scanf("%d %d %d", oper[i] + 1, oper[i] + 2, oper[i] + 3);
		}
	}

	std::memcpy(map, data, sizeof(data));
	std::sort(data + 1, data + tot + 1);
	size = std::unique(data + 1, data + tot + 1) - data - 1;
	for(int i = 1; i <= tot; ++i)
		map[i] = std::lower_bound(data + 1, data + size + 1, map[i]) - data;

	for(int i = 1; i <= n; ++i)
		root[i] = nil;
	for(int i = 1; i <= n; ++i)
		modify(n, i, map[i], 1);
	for(int i = 0; i != m; ++i)
	{
		if(oper[i][0])
		{
			int pos = std::lower_bound(data + 1, data + size + 1, oper[i][2]) - data;
			int p = oper[i][1];
			modify(n, p, map[p], -1);
			modify(n, p, map[p] = pos, 1);

		} else {
			std::printf("%d\n", get_ans(oper[i][1], oper[i][2], oper[i][3]));
		}
	}
	return 0;
}
