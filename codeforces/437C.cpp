/*
 * 437C - The Child and Toy 
 */
#include <cstdio>
#include <iostream>
#include <cstring>
#include <algorithm>

template<int VSize, int ESize>
class graph
{
	int total;
	int head[VSize + 1];
	int next[ESize + 1];
	int point[ESize + 1];
	int weight[VSize + 1];
	int w2[VSize + 1];
	int removed[VSize + 1];
public:
	void init()
	{
		total = 0;
		std::memset(removed, 0, sizeof(removed));
		std::memset(head, 0, sizeof(head));
	}

	void add_edge(int u, int v)
	{
		point[++total] = v;
		next[total] = head[u];
		head[u] = total;
	}

	long long remove(int u)
	{
		long long count = 0;
		for(int k = head[u]; k; k = next[k])
		{
			int v = point[k];
			if(removed[v])
				continue;
			count += weight[v];
		}

		removed[u] = 1;
		return count;
	}

	void read_weight(int n)
	{
		for(int i = 1; i <= n; ++i)
			std::scanf("%d", weight + i);
	}

	long long solve(int n)
	{
		for(int i = 1; i <= n; ++i)
			w2[i] = i;
		std::sort(w2 + 1, w2 + 1 + n, [&](int a, int b) 
			{ return weight[a] > weight[b]; });
		long long count = 0;
		for(int i = 1; i <= n; ++i)
			count += remove(w2[i]);
		return count;
	}
};

graph<1001, 4001> gp;

int main()
{
	int n, m;
	std::scanf("%d %d", &n, &m);
	gp.init();
	gp.read_weight(n);
	for(int i = 0; i != m; ++i)
	{
		int u, v;
		std::scanf("%d %d", &u, &v);
		gp.add_edge(u, v);
		gp.add_edge(v, u);
	}
	std::cout << gp.solve(n) << std::endl;
	return 0;
}
