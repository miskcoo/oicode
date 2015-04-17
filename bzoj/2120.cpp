/* BZOJ-2120: 数颜色
 *  线段树套线段树 */
#include <cstdio>

struct node_t
{
	int w;
	node_t *l, *r;
};

node_t* nil;

void destroy(node_t* now)
{
	if(now == nil) return;
	destroy(now->l);
	destroy(now->r);
	delete now;
}

node_t* modify(node_t* now, int head, int tail, int pos, int v)
{
	if(now->w + v == 0)
	{
		destroy(now);
		return nil;
	}

	if(now == nil)
	{
		now = new node_t;
		now->w = 0;
		now->l = now->r = nil;
	}

	now->w += v;
	if(head == tail) return now;

	int m = (head + tail) >> 1;
	if(pos <= m) now->l = modify(now->l, head, m, pos, v);
	else now->r = modify(now->r, m + 1, tail, pos, v);
	return now;
}

int ask(node_t* now, int head, int tail, int pos)
{
	if(now == nil) return 0;
	if(head == tail) return now->w;
	int m = (head + tail) >> 1;
	if(pos <= m) return ask(now->l, head, m, pos);
	return ask(now->r, m + 1, tail, pos) + now->l->w;
}

const int MaxN = 10010;
int n, m, data[MaxN], col[1000001];
node_t* node[MaxN << 2];

void modify_node(int now, int head, int tail, int pos, int k, int v)
{
	node[now] = modify(node[now], 0, n, k, v);
	if(head == tail) return;
	int m = (head + tail) >> 1;
	if(pos <= m) modify_node(now << 1, head, m, pos, k, v);
	else modify_node((now << 1) + 1, m + 1, tail, pos, k, v);
}

int get_ans(int now, int head, int tail, int a, int b, int left)
{
	if(head == a && tail == b)
		return ask(node[now], 0, n, left);

	int m = (head + tail) >> 1;
	if(b <= m) return get_ans(now << 1, head, m, a, b, left);
	if(m < a) return get_ans((now << 1) + 1, m + 1, tail, a, b, left);
	return get_ans(now << 1, head, m, a, m, left) + 
		get_ans((now << 1) + 1, m + 1, tail, m + 1, b, left);
}

void init(int n)
{
	static node_t nbase;
	nil = &nbase;
	nil->w = 0;
	nil->l = nil->r = nil;

	for(int i = 0; i <= n << 2; ++i)
		node[i] = nil;
}

void clear(int n)
{
	for(int i = 0; i <= n << 2; ++i)
		destroy(node[i]);
}

int main()
{
	std::scanf("%d %d", &n, &m);
	for(int i = 1; i <= n; ++i)
		std::scanf("%d", data + i);
	init(n);
	for(int i = 1; i <= n; ++i)
	{
		int prev = col[data[i]];
		col[data[i]] = i;
		modify_node(1, 1, n, i, prev, 1);
	}

	while(m --> 0)
	{
		char op[2];
		int a, b;
		std::scanf("%s %d %d", op, &a, &b);
		if(*op == 'R')
		{
			int next_now = 0, next_b = 0;
			int prev_now = 0, prev_b = 0;
			for(int i = a + 1; i <= n && !next_now; ++i)
			{
				if(data[i] == data[a])
					next_now = i;
			}

			for(int i = a + 1; i <= n && !next_b; ++i)
			{
				if(data[i] == b)
					next_b = i;
			}

			for(int i = a - 1; i && !prev_now; --i)
			{
				if(data[i] == data[a])
					prev_now = i;
			}

			for(int i = a - 1; i && !prev_b; --i)
			{
				if(data[i] == b)
					prev_b = i;
			}

			if(next_b) 
			{
				modify_node(1, 1, n, next_b, prev_b, -1);
				modify_node(1, 1, n, next_b, a, 1);
			}

			if(next_now) 
			{
				modify_node(1, 1, n, next_now, a, -1);
				modify_node(1, 1, n, next_now, prev_now, 1);
			}

			modify_node(1, 1, n, a, prev_now, -1);
			modify_node(1, 1, n, a, prev_b, 1);
			data[a] = b;
		} else {
			std::printf("%d\n", get_ans(1, 1, n, a, b, a - 1));
		}
	}

	clear(n);
	return 0;
}
