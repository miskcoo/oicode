/* 
 * 437D - The Child and Zoo 
 *  并查集，贪心
 */
#include <cstdio>
#include <iostream>
#include <cstring>
#include <algorithm>

template<int Size>
class mset
{
public:
	int fa[Size + 1];
	int min_v[Size + 1];
	int size[Size + 1];
	void init(int n)
	{
		for(int i = 1; i <= n; ++i)
		{
			fa[i] = i;
			size[i] = 1;
			std::scanf("%d", min_v + i);
		}
	}

	int find(int u)
	{
		if(fa[u] == u)
			return u;
		return fa[u] = find(fa[u]);
	}

	int get_min(int u)
	{
		return min_v[find(u)];
	}

	int get_size(int u)
	{
		return size[find(u)];
	}

	bool merged(int u, int v)
	{
		return find(u) == find(v);
	}

	bool merge(int u, int v)
	{
		if((u = find(u)) != (v = find(v)))
		{
			fa[u] = v;
			min_v[v] = std::min(min_v[v], min_v[u]);
			size[v] += size[u];
			return true;
		}
		return false;
	}
};

template<int VSize, int ESize>
class graph
{
	int total;
	int head[VSize + 1];
	int next[ESize + 1];
	int point[ESize + 1];
	int w2[VSize + 1];
	mset<VSize> ms;
public:
	void init()
	{
		total = 0;
		std::memset(head, 0, sizeof(head));
	}

	void add_edge(int u, int v)
	{
		point[++total] = v;
		next[total] = head[u];
		head[u] = total;
	}
	
	void init_ms(int n)
	{
		ms.init(n);
	}

	double solve(int n, int m)
	{
		long long count = 0;
		for(int i = 1; i <= n; ++i)
			w2[i] = i;
		std::sort(w2 + 1, w2 + n + 1, [this](int a, int b)
			{ return ms.min_v[a] > ms.min_v[b]; } );
		for(int i = 1; i <= n; ++i)
		{
			int u = w2[i];
			for(int k = head[u]; k; k = next[k])
			{
				int v = point[k];
				if(ms.merged(u, v)) continue;
				if(ms.get_min(v) >= ms.get_min(u))
				{
					count += (long long)ms.get_min(u) 
						* ms.get_size(v) * ms.get_size(u);
					ms.merge(u, v);
				}
			}
		}

		return count / double(n) / (n - 1) * 2;
	}
};

graph<100000, 200000> gp;

int main()
{
	int n, m;
	std::scanf("%d %d", &n, &m);
	gp.init();
	gp.init_ms(n);
	for(int i = 0; i != m; ++i)
	{
		int u, v;
		std::scanf("%d %d", &u, &v);
		gp.add_edge(u, v);
		gp.add_edge(v, u);
	}
	std::printf("%.6lf", gp.solve(n, m));
	return 0;
}
