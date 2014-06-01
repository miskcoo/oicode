/*
 * 427C - Checkposts 
 *  tarjan 找强连通分量
 */
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <stack>
#include <iostream>

template<int VSize, int ESize>
class graph
{
	int total;
	int head[VSize + 1];
	int next[ESize + 1];
	int point[ESize + 1];
	int weight[VSize + 1];
public:
	void add_edge(int u, int v)
	{
		point[++total] = v;
		next[total] = head[u];
		head[u] = total;
	}

	void read_weight(int N)
	{
		for(int i = 1; i <= N; ++i)
			std::scanf("%d", weight + i);
	}
private:
	char in_stack[VSize + 1];
	int dfn[VSize + 1];
	int low[VSize + 1];
	int index;
	std::stack<int> st;

	long long ans_count;
	long long ans_min;
public:
	void init()
	{
		std::memset(dfn, 0, sizeof(dfn));
		std::memset(in_stack, 0, sizeof(in_stack));
		index = 0;
		total = 0;
		std::memset(head, 0, sizeof(head));
		ans_min = 0;
		ans_count = 1;
	}

	void tarjan(int u)
	{
		dfn[u] = low[u] = ++index;
		st.push(u);
		in_stack[u] = 1;
		for(int k = head[u]; k; k = next[k])
		{
			int v = point[k];
			if(dfn[v] == 0)
			{
				tarjan(v);
				low[u] = std::min(low[u], low[v]);
			} else if(in_stack[v]) {
				low[u] = std::min(low[u], dfn[v]);
			}
		}

		long long min_v = 0x7fffffffffffLL;
		long long count = 0;
		if(dfn[u] == low[u])
		{
			for(;;)
			{
				int v = st.top();
				in_stack[v] = 0;
				if(weight[v] < min_v)
				{
					min_v = weight[v];
					count = 1;
				} else if(min_v == weight[v]) {
					++count;
				}
				st.pop();
				if(v == u) break;
			}
			ans_count = ans_count * count % 1000000007;
			ans_min = ans_min + min_v;
		}
	}

	void solve(int N)
	{
		for(int i = 1; i <= N; ++i)
		{
			if(dfn[i] == 0)
				tarjan(i);
		}
		
		std::cout << ans_min << " " << ans_count;
	}
};

graph<120000, 320000> gp;

int main()
{
	int N;
	std::scanf("%d", &N);
	gp.init();
	gp.read_weight(N);
	int M;
	std::scanf("%d", &M);
	for(int i = 0; i != M; ++i)
	{
		int u, v;
		std::scanf("%d %d", &u, &v);
		gp.add_edge(u, v);
	}

	gp.solve(N);
	return 0;
}
