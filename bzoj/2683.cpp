/* 
 * BZOJ-2683 简单题（Mokia）
 *  内存无限二维线段树
 *  内存有限 CDQ 分治
 */ 
#include <cstdio>
#include <cstring>
#include <algorithm>

struct info
{
	int order;
	int oper;
	int x, y, w;
	info(){}
	info(int a, int b, int c, int d, int e)
		: order(a), oper(b), x(c), y(d), w(e) {}
	bool operator < (const info& v) const
	{
		//注意啊注意QAQ 
		if(x == v.x)
			return order < v.order;
		return x < v.x; 
	}
} data[800000], temp[800000];
int weight[800000];

template<int Size>
class tree_array
{
	int max_size;
	int elem[Size + 1];
public:
	void init(int size)
	{
		max_size = size;
	}	
	
	void modify_add(int x, int v)
	{
		for(; x <= max_size; x += x & -x)
			elem[x] += v;
	}
	
	int ask_sum(int x)
	{
		int result = 0;
		for(; x; x -= x & -x)
			result += elem[x];
		return result;
	}
};

tree_array<500000> ta;

void divide(int l, int r)
{
	if(l == r) 
		return;
	int m = (l + r) >> 1;
	divide(l, m);
	
	int total = 0;
	for(int i = l; i <= m; ++i)
	{
		if(data[i].oper == 1)
			temp[total++] = data[i];
	}
	for(int i = m + 1; i <= r; ++i)
	{
		if(data[i].oper == 2)
			temp[total++] = data[i];
	}
	std::sort(temp, temp + total);
	
	for(int i = 0; i != total; ++i)
	{
		if(temp[i].oper == 1)
		{
			ta.modify_add(temp[i].y, temp[i].w);
		} else {
			int v = ta.ask_sum(temp[i].y);
			weight[temp[i].order] += v;
		}
	}
	
	for(int i = 0; i != total; ++i)
	{
		if(temp[i].oper == 1)
		{
			ta.modify_add(temp[i].y, -temp[i].w);
		}
	}
	
	divide(m + 1, r);
}

int main()
{
	int N;
	std::scanf("%d", &N);
	ta.init(N);
	int total = 0;
	int question = 0;
	for(; ;)
	{
		int type;
		std::scanf("%d", &type);
		if(type == 1)
		{
			int x, y, A;
			std::scanf("%d %d %d", &x, &y, &A);
			data[total++] = info(0, 1, x, y, A);
		} else if(type == 2) {
			int x1, y1, x2, y2;
			std::scanf("%d %d %d %d", &x1, &y1, &x2, &y2);
			data[total++] = info(question++, 2, x1 - 1, y1 - 1, 0);
			data[total++] = info(question++, 2, x2, y2, 0);
			data[total++] = info(question++, 2, x1 - 1, y2, 0);
			data[total++] = info(question++, 2, x2, y1 - 1, 0);
		} else break;
	}
	
	divide(0, total);
	for(int i = 0; i != question; i += 4)
	{
		std::printf("%d\n", weight[i] + weight[i + 1]
					- weight[i + 2] - weight[i + 3]);
	}
	return 0;
}
