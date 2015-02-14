/* BZOJ-3674: 可持久化并查集加强版
 *  可持久化线段树 */
#include <cstdio>
#include <algorithm>

const int MaxN = 2000001, MaxM = 5000000;
int used, n, m, root[MaxN];
struct node_t
{
	int son[2], deep, val;
} node[MaxM];

int build_tree(int l, int r)
{
	int p = ++used;
	if(l == r)
	{
		node[p].val = l;
		return p;
	}

	int m = (l + r) >> 1;
	node[p].son[0] = build_tree(l, m);
	node[p].son[1] = build_tree(m + 1, r);
	return p;
}

int modify(int now, int l, int r, int pos, int v)
{
	int p = ++used;
	node[p] = node[now];
	if(l == r)
	{
		node[p].val = v;
		return p;
	}

	int m = (l + r) >> 1;
	if(pos <= m) node[p].son[0] = modify(node[now].son[0], l, m, pos, v);
	else node[p].son[1] = modify(node[now].son[1], m + 1, r, pos, v);
	return p;
}

int ask(int now, int l, int r, int pos)
{
	if(l == r) return now;
	int m = (l + r) >> 1;
	if(pos <= m) return ask(node[now].son[0], l, m, pos);
	else return ask(node[now].son[1], m + 1, r, pos);
}

void add(int now, int l, int r, int pos)
{
	if(l == r) 
	{
		++node[now].deep;
		return;
	}

	int m = (l + r) >> 1;
	if(pos <= m) add(node[now].son[0], l, m, pos);
	else add(node[now].son[1], m + 1, r, pos);
}

int find(int r, int v)
{
	int p = ask(r, 1, n, v);
	if(node[p].val == v) return p;
	return find(r, node[p].val);
}

int main()
{
	std::scanf("%d %d", &n, &m);
	root[0] = build_tree(1, n);
	int lastans = 0;
	for(int i = 1; i <= m; ++i)
	{
		int oper;
		std::scanf("%d", &oper);
		root[i] = root[i - 1];
		if(oper == 1)
		{
			int a, b;
			std::scanf("%d %d", &a, &b);
			int pa = find(root[i - 1], a ^ lastans);
			int pb = find(root[i - 1], b ^ lastans);
			if(node[pa].val == node[pb].val) continue;
			if(node[pa].deep > node[pb].deep) 
				std::swap(pa, pb);
			root[i] = modify(root[i - 1], 1, n, node[pa].val, node[pb].val);
			if(node[pa].deep == node[pb].deep)
				add(root[i], 1, n, node[pb].val);
		} else if(oper == 2) {
			int k;
			std::scanf("%d", &k);
			root[i] = root[k ^ lastans];
		} else {
			int a, b;
			std::scanf("%d %d", &a, &b);
			int pa = find(root[i], a ^ lastans);
			int pb = find(root[i], b ^ lastans);
			if(node[pa].val == node[pb].val) 
				lastans = 1;
			else lastans = 0;
			std::printf("%d\n", lastans);
		}
	}
	return 0;
}
