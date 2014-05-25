/* 
 * BZOJ-3295 [Cqoi2011]动态逆序对
 *  CDQ 分治
 */
#include <cstdio>
#include <cstring>
#include <algorithm>

template<int Size>
class tree_array
{
	int size;
	long long elem[Size + 1];
public:
	void init(int sz)
	{
		size = sz;
	}

	void modify_add(int x, long long v)
	{
		for(; x <= size; x += x & -x)
			elem[x] += v;
	}

	long long ask_sum(int x)
	{
		long long result = 0;
		for(; x; x -= x & -x)
			result += elem[x];
		return result;
	}
};

struct query_struct
{
	int value, pos;
	int index;
	bool oper, skip;
} temp[100010], data[100010];
long long answer[100010];
long long ansr[100010];
char deleted[100010];
int raw_data[100010];
int position[100010];
tree_array<100010> ta;

bool compare_value(const query_struct& a, const query_struct& b)
{
	return a.value > b.value;
}

bool compare_pos(const query_struct& a, const query_struct& b)
{
	return a.pos > b.pos;
}

void divide(int l, int r)
{
	if(r - l <= 1) return;
	int m = (l + r) >> 1;

	for(int i = l; i != m; ++i)
	{
		temp[i] = data[i];
		temp[i].oper = true;
	}

	for(int i = m; i != r; ++i)
	{
		temp[i] = data[i];
		temp[i].oper = false;
	}

	std::sort(temp + l, temp + r, compare_value);
	for(int i = l; i != r; ++i)
	{
		if(temp[i].skip) continue;
		if(temp[i].oper)
			ta.modify_add(temp[i].pos, 1);
		else answer[temp[i].index] += ta.ask_sum(temp[i].pos);
	}

	for(int i = l; i != r; ++i)
	{
		if(temp[i].skip) continue;
		if(temp[i].oper)
			ta.modify_add(temp[i].pos, -1);
	}
	
	std::sort(temp + l, temp + r, compare_pos);
	for(int i = l; i != r; ++i)
	{
		if(temp[i].skip) continue;
		if(temp[i].oper)
			ta.modify_add(temp[i].value, 1);
		else answer[temp[i].index] += ta.ask_sum(temp[i].value);
	}

	for(int i = l; i != r; ++i)
	{
		if(temp[i].skip) continue;
		if(temp[i].oper)
			ta.modify_add(temp[i].value, -1);
	}
	
	divide(l, m);
	divide(m, r);
}

int main()
{
	int N, M;
	std::scanf("%d %d", &N, &M);
	ta.init(N);
	for(int i = 0; i != N; ++i)
	{
		std::scanf("%d", raw_data + i);
		position[raw_data[i]] = i + 1;
	}

	for(int i = 1; i <= M; ++i)
	{
		int v;
		std::scanf("%d", &v);
		temp[M - i].index = M - i;
		temp[M - i].value = v;
		temp[M - i].pos = position[v];
		if(deleted[v])
			temp[M - i].skip = true;
		else temp[M - i].skip = false;
		deleted[v] = 1;
	}

	int total = 0;
	for(int i = 0; i != N; ++i)
	{
		if(deleted[raw_data[i]])
			continue;
		data[total].index = total;
		data[total].value = raw_data[i];
		data[total].pos = i + 1;
		++total;
	}

	for(int i = 0; i != M; ++i)
	{
		data[total + i] = temp[i];
		data[total + i].index += total;
	}

	divide(0, N);

	long long ans = 0;
	for(int i = 0; i != N; ++i)
		ansr[N - i - 1] = (ans += answer[i]);
	for(int i = 0; i != M; ++i)
		std::printf("%lld\n", ansr[i]);

	return 0;
}
