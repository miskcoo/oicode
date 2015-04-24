/* SPOJ-GSS1 Can you answer these queries I
 *   线段树维护最大连续子段 */
#include <cstdio>
#include <algorithm>

const int MaxN = 200010;

struct node_t
{
	int ans, sum, left, right;
} node[MaxN];

node_t merge(const node_t& a, const node_t& b)
{
	node_t n;
	n.sum = a.sum + b.sum;
	n.left = std::max(a.left, a.sum + b.left);
	n.right = std::max(a.right + b.sum, b.right);
	n.ans = std::max(a.right + b.left, std::max(a.ans, b.ans));
	return n;
}

node_t ask(int now, int head, int tail, int a, int b)
{
	if(head == a && tail == b) return node[now];
	int m = (head + tail) >> 1;
	if(b <= m) return ask(now << 1, head, m, a, b);
	if(m < a) return ask((now << 1) + 1, m + 1, tail, a, b);
	node_t y = ask(now << 1, head, m, a, m);
	node_t z = ask((now << 1) + 1, m + 1, tail, m + 1, b);
	return merge(y, z);
}

void build(int now, int head, int tail)
{
	if(head == tail)
	{
		int v;
		std::scanf("%d", &v);
		node[now].sum = node[now].ans = v;
		node[now].left = node[now].right = v;
		return;
	}

	int m = (head + tail) >> 1;
	build(now << 1, head, m);
	build((now << 1) + 1, m + 1, tail);
	node[now] = merge(node[now << 1], node[(now << 1) + 1]);
}

int main()
{
	int n;
	std::scanf("%d", &n);
	build(1, 1, n);
	int m;
	std::scanf("%d", &m);
	for(int i = 0; i != m; ++i)
	{
		int a, b;
		std::scanf("%d %d", &a, &b);
		node_t z = ask(1, 1, n, a, b);
		std::printf("%d\n", z.ans);
	}
	return 0;
}
