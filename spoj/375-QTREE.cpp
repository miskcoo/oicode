/*
 * SPOJ-375 Query on a tree
 *  树链剖分
 */
#include <cstdio>
#include <cstring>
#include <algorithm>

int int_max(int a, int b)
{
	return a < b ? b : a;
}

template<int Size>
class seg_tree
{
	int elem[(1 + Size) << 2];
public:
	int ask_max(int now, int a, int b, int head, int tail)
	{
		if(head == a && tail == b)
			return elem[now];
		int m = (head + tail) >> 1;
		if(m >= b) return ask_max(now << 1, a, b, head, m);
		if(m < a) return ask_max(1 + (now << 1), a, b, m + 1, tail);
		return int_max(ask_max(now << 1, a, m, head, m), 
				ask_max((now << 1) + 1, m + 1, b, m + 1, tail));
	}

	void modify(int now, int k, int head, int tail, int v)
	{
		if(head == tail)
		{
			elem[now] = v;
		} else {
			int m = (head + tail) >> 1;
			if(m >= k) modify(now << 1, k, head, m, v);
			else modify(1 + (now << 1), k, m + 1, tail, v);
			elem[now] = int_max(elem[now << 1], elem[1 + (now << 1)]);
		}
	}
};

template<int VSize, int ESize>
class graph
{
	int N;

	int nweight[ESize + 1];
	int weight[VSize + 1];
	int head[VSize + 1];
	int point[ESize + 1];
	int next[ESize + 1];
	int total;

	int edge_relabel[ESize + 1];
	int father[VSize + 1];
	int csize[VSize + 1];
	int son[VSize + 1];
	int top[VSize + 1];
	int depth[VSize + 1];
	int relabel[VSize + 1];
	int rtotal;

	seg_tree<ESize> st;
public:
	void init(int vertex_size)
	{
		N = vertex_size;
		depth[1] = 1;
		father[1] = 0;
		total = 0;
		rtotal = 0;
		std::memset(head, 0, sizeof(head));
	}

	void add_edge(int u, int v, int w)
	{
		next[++total] = head[u];
		point[total] = v;
		nweight[total] = w;
		head[u] = total;
	}
public:
	void dfs(int u)
	{
		csize[u] = 1;
		son[u] = 0;
		int max_size = 0;
		for(int k = head[u]; k; k = next[k])
		{
			int v = point[k];
			if(v == father[u])
				continue;
			depth[v] = depth[u] + 1;
			father[v] = u;
			weight[v] = nweight[k];
			edge_relabel[(k + 1) >> 1] = v;
			dfs(v);
			if(csize[v] > max_size)
			{
				son[u] = v;
				max_size = csize[v];
			}
			csize[u] += csize[v];
		}
	}

	void make_heavy_link(int u, int t)
	{
		relabel[u] = ++rtotal;
		top[u] = t;
		st.modify(1, rtotal, 1, N, weight[u]);
		if(son[u]) make_heavy_link(son[u], t);
		for(int k = head[u]; k; k = next[k])
		{
			int v = point[k];
			if(v == father[u] || v == son[u]) 
				continue;
			make_heavy_link(v, v);
		}
	}

	void make_tree(int N)
	{
		st.make_tree(1, 1, N);
	}

	int ask_max(int u, int v)
	{
		int result = -0x7fffffff;
		while(top[u] != top[v])
		{
			if(depth[top[u]] < depth[top[v]])
				std::swap(u, v);
			result = int_max(result, st.ask_max(
				1, relabel[top[u]], relabel[u], 1, N));
			u = father[top[u]];
		}
		if(u != v)
		{
			if(depth[u] > depth[v]) std::swap(u, v);
			result = int_max(result, st.ask_max(
				1, relabel[son[u]], relabel[v], 1, N));
		}
		return result;
	}

	void modify(int u, int w)
	{
		st.modify(1, relabel[edge_relabel[u]], 1, N, w);
	}
};

graph<10000, 20000> gp;

int main()
{
	int T;
	std::scanf("%d", &T);
	while(T--)
	{
		int N;
		std::scanf("%d", &N);
		gp.init(N);
		for(int i = 1; i != N; ++i)
		{
			int u, v, w;
			std::scanf("%d %d %d", &u, &v, &w);
			gp.add_edge(u, v, w);
			gp.add_edge(v, u, w);
		}
		gp.dfs(1);
		gp.make_heavy_link(1, 1);

		for(;;)
		{
			char str[10];
			std::scanf("%s", str);
			if(str[0] == 'D') break;
			int u, v;
			std::scanf("%d %d", &u, &v);
			if(str[0] == 'C') gp.modify(u, v);
			else std::printf("%d\n", gp.ask_max(u, v));
		}
	}
	return 0;
}
