/* 
 * BZOJ-3333 派对计划
 *  树状数组+逆序对+线段树
 */
#include <cstdio>
#include <algorithm>

struct info
{
	int index;
	int value;

	bool operator < (const info& v) const
	{
		return value < v.value;
	}
} data[500001];
int rdata[500002];
int count[500001];
int relabel[500001];

template<typename Type, int Size>
class tree_array
{
	Type elem[Size + 1];
	int N;
public:
	void init(int sz)
	{
		N = sz;
	}

	void modify(int v)
	{
		for(; v <= N; v += (v & -v))
			++elem[v];
	}

	int ask(int v)
	{
		int result = 0;
		for(; v; v -= (v & -v))
			result += elem[v];
		return result;
	}
};

tree_array<int, 500000> ta;

inline int int_min(int a, int b)
{
	return rdata[a] < rdata[b] ? a : b;
}

template<typename Type, int Size>
class segment_tree
{
	Type elem[Size * 4 + 4];
public:
	void modify(int now, int pos, int head, int tail)
	{
		if(head == tail)
		{
			elem[now] = 500001;
			return;
		}

		int m = (head + tail) >> 1;
		if(pos <= m) modify(now << 1, pos, head, m);
		else modify((now << 1) + 1, pos, m + 1, tail);
		elem[now] = int_min(elem[now << 1], elem[(now << 1) + 1]);
	}

	int ask(int now, int a, int b, int head, int tail)
	{
		if(a == head && b == tail)
			return elem[now];

		int m = (head + tail) >> 1;
		if(b <= m) return ask(now << 1, a, b, head, m);
		if(a > m) return ask((now << 1) + 1, a, b, m + 1, tail);

		return int_min(ask(now << 1, a, m, head, m), 
			ask((now << 1) + 1, m + 1, b, m + 1, tail));
	}

	void build_tree(int now, int head, int tail)
	{
		if(head == tail)
		{
			elem[now] = head;
		} else {
			int m = (head + tail) >> 1;
			build_tree(now << 1, head, m);
			build_tree((now << 1) + 1, m + 1, tail);
			elem[now] = int_min(elem[now << 1], elem[(now << 1) + 1]);
		}
	}
};

segment_tree<int, 500000> st;

int main()
{
	rdata[500001] = 0x7fffffff;
	int N, M;
	std::scanf("%d %d", &N, &M);
	ta.init(N);
	for(int i = 1; i <= N; ++i)
	{
		std::scanf("%d", rdata + i);
		data[i].index = i;
		data[i].value = rdata[i];
	}
	data[0].value = data[1].value;
	std::sort(data + 1, data + N + 1);
	int total = 1;
	for(int i = 1; i <= N; ++i)
	{
		if(data[i].value != data[i - 1].value)
			++total;
		relabel[data[i].index] = total;
	}

	long long ans = 0;
	for(int i = N; i; --i)
	{
		count[i] = ta.ask(relabel[i] - 1);
		ans += count[i];
		ta.modify(relabel[i]);
	}

	st.build_tree(1, 1, N);
	std::printf("%lld\n", ans);
	for(int i = 0; i != M; ++i)
	{
		int pos, pp;
		std::scanf("%d", &pos);
		while(rdata[pp = st.ask(1, pos, N, 1, N)] <= rdata[pos])
		{
			ans -= count[pp];
			st.modify(1, pp, 1, N);
		}
		std::printf("%lld\n", ans);
	}

	return 0;
}
