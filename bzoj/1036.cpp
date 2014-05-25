/*
 * BZOJ-1036 树的统计
 *  树链剖分
 */
#include <cstdio>
#include <cstring>
#include <algorithm>

int N, M;
inline int max(int a, int b)
{
	return a > b ? a : b;
}

template<int Size>
class segment_tree
{
	int sum[Size * 4 + 4];
	int mvalue[Size * 4 + 4];
	int* data;
public:
	void dfs_init(int now, int head, int tail)
	{
		if(head == tail)
		{
			sum[now] = data[head];
			mvalue[now] = data[head];
			return;
		}

		int m = (head + tail) >> 1;
		int l = now << 1, r = l + 1;
		dfs_init(now << 1, head, m);
		dfs_init((now << 1) + 1, m + 1, tail);
		sum[now] = sum[l] + sum[r];
		mvalue[now] = max(mvalue[l], mvalue[r]);
	}

	void init(int* data)
	{
		this->data = data;
		std::memset(sum, 0, sizeof(sum));
		std::memset(mvalue, 0, sizeof(mvalue));
		dfs_init(1, 1, N);
	}

	void modify(int now, int head, int tail, int pos, int v)
	{
		if(head == tail)
		{
			sum[now] = v;
			mvalue[now] = v;
			return;
		}

		int m = (head + tail) >> 1;
		int l = now << 1, r = l + 1;
		if(pos <= m) modify(l, head, m, pos, v);
		else modify(r, m + 1, tail, pos, v);

		sum[now] = sum[l] + sum[r];
		mvalue[now] = max(mvalue[l], mvalue[r]);
	}

	int ask_max(int now, int head, int tail, int a, int b)
	{
		if(head == a && tail == b)
			return mvalue[now];

		int m = (head + tail) >> 1;
		int l = now << 1, r = l + 1;
		if(b <= m) return ask_max(l, head, m, a, b);
		if(a > m) return ask_max(r, m + 1, tail, a, b);
		return max(ask_max(l, head, m, a, m), 
			ask_max(r, m + 1, tail, m + 1, b));
	}

	int ask_sum(int now, int head, int tail, int a, int b)
	{
		if(head == a && tail == b)
			return sum[now];

		int m = (head + tail) >> 1;
		int l = now << 1, r = l + 1;
		if(b <= m) return ask_sum(l, head, m, a, b);
		if(a > m) return ask_sum(r, m + 1, tail, a, b);
		return ask_sum(l, head, m, a, m) 
			+ ask_sum(r, m + 1, tail, m + 1, b);
	}
};

template<int VSize, int ESize>
class graph
{
	int head[VSize + 1];
	int next[ESize + 1];
	int point[ESize + 1];
	int total;
public:
	void init()
	{
		reused = 0;
		total = 0;
		son[0] = 0;
		std::memset(head, 0, sizeof(head));
	}

	void init_segment(int* data)
	{
		for(int i = 1; i <= N; ++i)
		{
			int v;
			std::scanf("%d", &v);
			data[relab[i]] = v;
		}
		st.init(data);
	}

	void add_edge(int u, int v)
	{
		point[++total] = v;
		next[total] = head[u];
		head[u] = total;
	}
private:
	int fa[VSize + 1];
	int son[VSize + 1];
	int top[VSize + 1];
	int depth[VSize + 1];
	int relab[VSize + 1];
	int dleave[VSize + 1];
	int reused;

	int max_tsize[VSize + 1];
	int tsize[VSize + 1];

	segment_tree<VSize> st;
public:
	void dfs_tsize(int u, int dep, int f)
	{
		tsize[u] = 1;
		depth[u] = dep;
		max_tsize[u] = 0;
		for(int k = head[u]; k; k = next[k])
		{
			int v = point[k];
			if(v == f) continue;
			dfs_tsize(v, dep + 1, u);
			tsize[u] += tsize[v];
			if(max_tsize[u] < tsize[v])
			{
				son[u] = v;
				max_tsize[u] = tsize[v];
			}
		}
	}

	void dfs_link(int u, int f)
	{
		if(son[f] == u)
			top[u] = top[f];
		else top[u] = u;
		fa[u] = f;
		relab[u] = ++reused;
		if(son[u]) dfs_link(son[u], u);
		for(int k = head[u]; k; k = next[k])
		{
			int v = point[k];
			if(v == f || v == son[u]) 
				continue;
			dfs_link(v, u);
		}
		dleave[u] = reused;
	}

	void modify(int u, int value)
	{
		st.modify(1, 1, N, relab[u], value);
	}

	int ask_sum(int u, int v)
	{
		int result = 0;
		while(top[u] != top[v])
		{
			if(depth[top[u]] < depth[top[v]])
				std::swap(u, v);
			result += st.ask_sum(1, 1, N, relab[top[u]], relab[u]);
			u = fa[top[u]];
		}
		if(depth[u] > depth[v])
			std::swap(u, v);
		return result + st.ask_sum(1, 1, N, relab[u], relab[v]);
	}

	int ask_max(int u, int v)
	{
		int result = -0x7fffffff;
		while(top[u] != top[v])
		{
			if(depth[top[u]] < depth[top[v]])
				std::swap(u, v);
			result = max(result, st.ask_max(1, 1,
				N, relab[top[u]], relab[u]));
			u = fa[top[u]];
		}
		if(depth[u] > depth[v])
			std::swap(u, v);
		return max(result, st.ask_max(1, 1, N, relab[u], relab[v]));
	}
};

graph<30000, 60000> gp;
int data[30001];

int main()
{
	gp.init();
	std::scanf("%d", &N);
	for(int i = 1; i != N; ++i)
	{
		int u, v;
		std::scanf("%d %d", &u, &v);
		gp.add_edge(u, v);
		gp.add_edge(v, u);
	}
	gp.dfs_tsize(1, 1, 0);
	gp.dfs_link(1, 0);
	gp.init_segment(data);
	int M;
	std::scanf("%d", &M);
	for(int i = 0; i != M; ++i)
	{
		char c[10];
		int a, b;
		std::scanf("%s %d %d", c, &a, &b);
		switch(c[1])
		{
		case 'M':
			std::printf("%d\n", gp.ask_max(a, b));
			break;
		case 'S':
			std::printf("%d\n", gp.ask_sum(a, b));
			break;
		case 'H':
			gp.modify(a, b);
			break;
		}
	}
	return 0;
}
