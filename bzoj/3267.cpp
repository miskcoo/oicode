/* BZOJ-3267: KC采花
 *  K 段最大子序列和
 *  线段树维护，费用流 */
#include <cstdio>
#include <algorithm>

const int MaxN = 100010;
struct range_t
{
	int l, r, v;
	range_t& operator = (int rhs)
	{
		v = rhs;
		return *this;
	}
};

struct node_t
{
	range_t left, right;
	range_t ans;
	int sum;

	void reverse() {
		sum = -sum;
		ans.v = -ans.v;
		left.v = -left.v;
		right.v = -right.v;
	}
};

int modify_pos, modify_val;
int lazy[MaxN * 4];
node_t node[MaxN * 4], node_m[MaxN * 4];

bool max_cmp(int a, int b) { return a > b; }
bool min_cmp(int a, int b) { return a < b; }

template<typename Comparer>
node_t& merge(node_t& t, const node_t& a, const node_t& b, Comparer cmp)
{
	// merge left
	t.left = a.left;
	if(cmp(a.sum + b.left.v, t.left.v))
	{
		t.left = a.sum + b.left.v;
		t.left.r = b.left.r;
	}

	// merge right
	t.right = b.right;
	if(cmp(a.right.v + b.sum, t.right.v))
	{
		t.right = a.right.v + b.sum;
		t.right.l = a.right.l;
	}

	// merge ans
	t.ans = cmp(a.ans.v, b.ans.v) ? a.ans : b.ans;
	if(cmp(a.right.v + b.left.v, t.ans.v))
	{
		t.ans.v = a.right.v + b.left.v;
		t.ans.l = a.right.l;
		t.ans.r = b.left.r;
	}

	// merge sum
	t.sum = a.sum + b.sum;
	return t;
}

void pushdown(int now, int head, int tail)
{
	if(!lazy[now]) return;
	int l = now << 1, r = l + 1;
	if(head != tail)
		lazy[l] ^= 1, lazy[r] ^= 1;
	lazy[now] = 0;

	node_t& max = node[now], &min = node_m[now];
	std::swap(max, min);
	max.reverse();
	min.reverse();
}

void modify(int now, int head, int tail)
{
	pushdown(now, head, tail);
	if(head == tail)
	{
		node[now].left = node[now].right = modify_val;
		node[now].ans = node[now].sum = modify_val;
		node_m[now] = node[now];
		return;
	}

	int m = (head + tail) >> 1;
	int l = now << 1, r = l + 1;
	if(modify_pos <= m) modify(l, head, m);
	else modify(r, m + 1, tail);

	merge(node[now], node[l], node[r], max_cmp);
	merge(node_m[now], node_m[l], node_m[r], min_cmp);
}

node_t ask(int now, int head, int tail, int l, int r)
{
	pushdown(now, head, tail);
	if(head == l && tail == r)
		return node[now];

	int m = (head + tail) >> 1;
	if(r <= m) return ask(now << 1, head, m, l, r);
	if(l > m) return ask((now << 1) + 1, m + 1, tail, l, r);

	node_t t1 = ask(now << 1, head, m, l, m);
	node_t t2 = ask((now << 1) + 1, m + 1, tail, m + 1, r);
	node_t ans = node[now];
	return merge(ans, t1, t2, max_cmp);
}

void reverse(int now, int head, int tail, int a, int b)
{
	if(head == a && tail == b)
	{
		lazy[now] ^= 1;
		pushdown(now, head, tail);
		return;
	}

	pushdown(now, head, tail);
	int m = (head + tail) >> 1;
	int l = now << 1, r = l + 1;
	bool flag = true;
	if(b <= m) 
	{
		flag = false;
		reverse(l, head, m, a, b);
		pushdown(r, m + 1, tail);
	}
	
	if(a > m) 
	{
		flag = false;
		reverse(r, m + 1, tail, a, b);
		pushdown(l, head, m);
	}

	if(flag)
	{
		reverse(l, head, m, a, m);
		reverse(r, m + 1, tail, m + 1, b);
	}

	merge(node[now], node[l], node[r], max_cmp);
	merge(node_m[now], node_m[l], node_m[r], min_cmp);
}

void build_tree(int now, int l, int r)
{
	if(l == r)
	{
		int v;
		std::scanf("%d", &v);
		node[now].ans = node[now].sum = v;
		node[now].ans.l = node[now].ans.r = l;
		node[now].left = node[now].right = node[now].ans;
		node_m[now] = node[now];
		return;
	}

	int m = (l + r) >> 1;
	int a = now << 1, b = a + 1;
	build_tree(a, l, m);
	build_tree(b, m + 1, r);
	merge(node[now], node[a], node[b], max_cmp);
	merge(node_m[now], node_m[a], node_m[b], min_cmp);
}

int stack[20][2];
int main()
{
	int n, m;
	std::scanf("%d", &n);
	build_tree(1, 1, n);
	std::scanf("%d", &m);
	while(m --> 0)
	{
		int oper;
		std::scanf("%d", &oper);
		if(oper == 0)
		{
			std::scanf("%d %d", &modify_pos, &modify_val);
			modify(1, 1, n);
		} else {
			int l, r, k;
			std::scanf("%d %d %d", &l, &r, &k);

			int ans = 0, stop = 0;
			for(int i = 0; i != k; ++i)
			{
				node_t ret = ask(1, 1, n, l, r);
				if(ret.ans.v <= 0) break;

				ans += ret.ans.v;
				if(i + 1 != k)
				{
					reverse(1, 1, n, ret.ans.l, ret.ans.r);
					stack[stop][0] = ret.ans.l;
					stack[stop++][1] = ret.ans.r;
				}
			}

			std::printf("%d\n", ans);

			for(int i = 0; i != stop; ++i)
				reverse(1, 1, n, stack[i][0], stack[i][1]);
		}
	}
	return 0;
}


