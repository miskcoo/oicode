/*
 * BZOJ-3236 作业
 *  莫队算法+主席树
 */
#include <cmath>
#include <cstdio>
#include <cstring>
#include <algorithm>

template<typename Type, int Size>
class tree_array
{
	int size;
	Type elem[Size + 1];
public:
	void init(int N)
	{
		size = N;
		std::memset(elem, 0, 
			sizeof(Type) * (N + 1));
	}

	void modify(int v, Type x)
	{
		for(; v <= size; v += v & -v)
			elem[v] += x;
	}

	Type ask_sum(int v)
	{
		Type result = 0;
		for(; v; v -= v & -v)
			result += elem[v];
		return result;
	}

	Type ask_range(int a, int b)
	{
		return ask_sum(b) - ask_sum(a - 1);
	}
};

template<typename Type, int Size, int Node>
class segment_tree
{
	Type elem[Node + 1];
	int root[Size + 1];
	int ch[Node + 1][2];
	int used;
	int total;
public:
	void init(int N, int* seq)
	{
		total = N;
		root[0] = 0;
		ch[0][0] = ch[0][1] = 0;
		elem[0] = 0;
		used = 1;

		for(int i = 1; i <= N; ++i)
			root[i] = insert(root[i - 1], 1, N, seq[i]);
	}

	int insert(int now, int head, int tail, int pos)
	{
		int n = used++;
		elem[n] = elem[now] + 1;
		ch[n][0] = ch[now][0];
		ch[n][1] = ch[now][1];
		if(head == tail)
			return n;
		int m = (head + tail) >> 1;
		if(pos <= m) ch[n][0] = insert(ch[n][0], head, m, pos);
		else ch[n][1] = insert(ch[n][1], m + 1, tail, pos);
		return n;
	}

	int ask_sum(int now, int head, int tail, int a, int b)
	{
		if(!now) return 0;
		if(head == a && tail == b)
			return elem[now];
		
		int m = (head + tail) >> 1;
		if(b <= m) return ask_sum(ch[now][0], head, m, a, b);
		if(a > m) return ask_sum(ch[now][1], m + 1, tail, a, b);
		return ask_sum(ch[now][0], head, m, a, m)
			+ ask_sum(ch[now][1], m + 1, tail, m + 1, b);
	}

	int ask_range(int l, int r, int a, int b)
	{
		return ask_sum(root[r], 1, total, a, b)
			- ask_sum(root[l - 1], 1, total, a, b);
	}
};

struct oper
{
	int index;
	int l, r, a, b;
	bool operator < (const oper& v) const
	{
		return l < v.l;
	}
} op[1000000];
int ans[1000000][2];

bool sort_cmpr(const oper& a, const oper& b)
{
	return a.r < b.r;
}

int block_size;
tree_array<int, 100000> ta;
segment_tree<int, 100000, 200000 * 17> st;
int prev[100001], next[100001];
int seq[100001];

int main()
{
	int N, M;
	std::scanf("%d %d", &N, &M);
	block_size = (int)std::sqrt(double(N));
	int* temp = next;	// 暂用临时空间
	for(int i = 1; i <= N; ++i)
	{
		int v;
		std::scanf("%d", &v);
		seq[i] = v;
		prev[i] = temp[v];
		temp[v] = i;
	}
	temp = 0;			// 使用完毕
	for(int i = 1; i <= N; ++i)
	{
		next[i] = 0x7fffffff;
		next[prev[i]] = i;
	}
	next[0] = 0;
	for(int i = 0; i != M; ++i)
	{
		op[i].index = i;
		std::scanf("%d %d %d %d", 
			&op[i].l, &op[i].r,
			&op[i].a, &op[i].b);
	}
	ta.init(N);
	st.init(N, seq);
	std::sort(op, op + M);
	for(int r = 0; r != M; )
	{
		// 寻找一块内的询问
		int l = r;
		int rlimit = op[l].l + block_size;
		while(r < M && rlimit >= op[r].l)
			++r;
		// 右端点排序
		std::sort(op + l, op + r, sort_cmpr);
		ta.init(N);
		for(int i = op[l].l; i <= op[l].r; ++i)
		{
			if(prev[i] < op[l].l)
				ta.modify(seq[i], 1);
		}
		ans[op[l].index][0] = st.ask_range(
			op[l].l, op[l].r, op[l].a, op[l].b); 
		ans[op[l].index][1] = ta.ask_range(op[l].a, op[l].b);
		// 处理询问
		for(int i = l + 1; i < r; ++i)
		{
			int p = i - 1;
			// 右端点向右扩展
			for(int j = op[p].r + 1; j <= op[i].r; ++j)
			{
				if(prev[j] < op[p].l)
					ta.modify(seq[j], 1);
			}
			if(op[p].l < op[i].l)
			{
				// 左端点向右收缩
				for(int j = op[p].l; j != op[i].l; ++j)
				{
					if(next[j] > op[i].r)
						ta.modify(seq[j], -1);
				}
			} else {
				// 左端点向左扩展
				for(int j = op[i].l; j != op[p].l; ++j)
				{
					if(next[j] > op[i].r)
						ta.modify(seq[j], 1);
				}
			}
			ans[op[i].index][0] = st.ask_range(
				op[i].l, op[i].r, op[i].a, op[i].b); 
			ans[op[i].index][1] = ta.ask_range(op[i].a, op[i].b);
		}
	}
	for(int i = 0; i != M; ++i)
	{
		std::printf("%d %d\n", 
			ans[i][0], ans[i][1]);
	}
	return 0;
}
