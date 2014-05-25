/*
 * BZOJ-3083 遥远的国度
 *  树链剖分+DFS序
 */
#include <cstdio>
#include <cstring>
#include <algorithm>

int N, M;
template<int Size>
class segment_tree
{
	int elem[Size * 4 + 4];
	int lazy[Size * 4 + 4];
	char has_lazy[Size * 4 + 4];
	int* data;
public:
	int dfs_init(int now, int head, int tail)
	{
		if(head == tail)
			return elem[now] = data[head];

		int m = (head + tail) >> 1;
		int a = dfs_init(now << 1, head, m);
		int b = dfs_init((now << 1) + 1, m + 1, tail);
		if(a < b) return elem[now] = a;
		return elem[now] = b;
	}

	void init(int* data)
	{
		this->data = data;
		std::memset(has_lazy, 0, sizeof(has_lazy));
		std::memset(elem, 0, sizeof(elem));
		dfs_init(1, 1, N);
	}

	void pushdown(int now)
	{
		if(!has_lazy[now])
			return;
		has_lazy[now] = 0;
		int l = now << 1;
		has_lazy[l] = 1;
		elem[l] = lazy[l] = lazy[now];
		int r = l + 1;
		has_lazy[r] = 1;
		elem[r] = lazy[r] = lazy[now];
	}

	void pushup(int now)
	{
		int l = now << 1;
		if(elem[l] < elem[l + 1])
			elem[now] = elem[l];
		else elem[now] = elem[l + 1];
	}

	void modify(int now, int head, int tail, int a, int b, int v)
	{
		pushdown(now);
		if(head == a && tail == b)
		{
			has_lazy[now] = 1;
			elem[now] = lazy[now] = v;
			return;
		}

		int m = (head + tail) >> 1;
		if(b <= m)
		{
			modify(now << 1, head, m, a, b, v);
			pushup(now);
			return;
		}
		if(a > m)
		{
			modify((now << 1) + 1, m + 1, tail, a, b, v);
			pushup(now);
			return;
		}

		modify(now << 1, head, m, a, m, v);
		modify((now << 1) + 1, m + 1, tail, m + 1, b, v);
		pushup(now);
	}

	int ask_min(int now, int head, int tail, int a, int b)
	{
		pushdown(now);
		if(head == a && tail == b)
			return elem[now];

		int m = (head + tail) >> 1;
		if(b <= m) return ask_min(now << 1, head, m, a, b);
		if(a > m) return ask_min((now << 1) + 1, m + 1, tail, a, b);
		
		int x = ask_min(now << 1, head, m, a, m);
		int y = ask_min((now << 1) + 1, m + 1, tail, m + 1, b);
		if(x < y) return x;
		return y;
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

	void modify(int u, int v, int value)
	{
		while(top[u] != top[v])
		{
			if(depth[top[u]] < depth[top[v]])
				std::swap(u, v);
			st.modify(1, 1, N, relab[top[u]], relab[u], value);
			u = fa[top[u]];
		}
		
		if(depth[u] > depth[v])
			std::swap(u, v);
		st.modify(1, 1, N, relab[u], relab[v], value);
	}

	bool is_child(int u, int fa)
	{
		return relab[fa] <= relab[u]
			&& dleave[fa] >= dleave[u];
	}

	int ask_min(int u, int root)
	{
		if(u == root)
			return st.ask_min(1, 1, N, 1, N);
		if(is_child(root, u))
		{
			int k = head[u];
			while(!is_child(root, point[k]))
				k = next[k];
			int child = point[k];
			int cut_begin = relab[child];
			int cut_end = dleave[child];
			int result = 0x7FFFFFFF;
			if(cut_begin != 1)
				result = st.ask_min(1, 1, N, 1, cut_begin - 1);
			if(cut_end != N)
			{
				int value = st.ask_min(1, 1, N, cut_end + 1, N);
				if(value < result) 
					result = value;
			}
			return result;
		} else {
			return st.ask_min(1, 1, N, relab[u], dleave[u]);
		}
	}
};

graph<200000, 200000> gp;
int data[100001];

int main()
{
	gp.init();
	std::scanf("%d %d", &N, &M);
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
	int root;
	std::scanf("%d", &root);
	for(int i = 0; i != M; ++i)
	{
		int opt;
		std::scanf("%d", &opt);
		int p1, p2, v;
		switch(opt)
		{
		case 1:
			std::scanf("%d", &root);
			break;
		case 2:
			std::scanf("%d %d %d", &p1, &p2, &v);
			gp.modify(p1, p2, v);
			break;
		case 3:
			std::scanf("%d", &v);
			std::printf("%d\n", gp.ask_min(v, root));
			break;
		}
	}
	return 0;
}
