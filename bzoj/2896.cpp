/* 
 * BZOJ-2896 桥
 *  树链剖分（长）或并查集（短）
 */
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <set>
#include <utility>

int N, M;
template<int Size>
class segment_tree
{
	int elem[Size * 4 + 4];
	int lazy[Size * 4 + 4];
public:
	void init()
	{
		std::memset(lazy, 0, sizeof(lazy));
		std::memset(elem, 0, sizeof(elem));
	}

	void modify(int now, int head, int tail, int a, int b)
	{
		if(lazy[now]) return;
		if(head == a && tail == b)
		{
			lazy[now] = 1;
			elem[now] = b - a + 1;
			return;
		}

		int m = (head + tail) >> 1;
		int l = now << 1, r = l + 1;
		if(b <= m)
		{
			modify(l, head, m, a, b);
            elem[now] = elem[l] + elem[r];
			return;
		}

		if(a > m) 
		{
			modify(r, m + 1, tail, a, b);
			elem[now] = elem[l] + elem[r];
			return;
		}

		modify(l, head, m, a, m);
		modify(r, m + 1, tail, m + 1, b);
		elem[now] = elem[l] + elem[r];
	}

	int ask_sum(int now, int head, int tail, int a, int b)
	{
		if(lazy[now]) return b - a + 1;
		if(head == a && b == tail)
			return elem[now];

        int total = -1;
		int m = (head + tail) >> 1;
		if(b <= m) total = ask_sum(now << 1, head, m, a, b);
		if(a > m) total = ask_sum((now << 1) + 1, m + 1, tail, a, b);

		if(total == -1) total = ask_sum(now << 1, head, m, a, m)
			+ ask_sum((now << 1) + 1, m + 1, tail, m + 1, b);
			
		if(lazy[now]) total = b - a + 1;
		return total;
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
		st.init();
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
	}

	void modify(int u, int v)
	{
		while(top[u] != top[v])
		{
			if(depth[top[u]] < depth[top[v]])
				std::swap(u, v);
			st.modify(1, 1, N, relab[top[u]], relab[u]);
			u = fa[top[u]];
		}
		
		if(depth[u] > depth[v])
			std::swap(u, v);
		if(depth[u] != depth[v])
			st.modify(1, 1, N, relab[son[u]], relab[v]);
	}

	int ask_sum(int u, int v)
	{
		int total = 0;
		while(top[u] != top[v])
		{
			if(depth[top[u]] < depth[top[v]])
				std::swap(u, v);
			total += depth[u] - depth[top[u]] + 1 - 
				st.ask_sum(1, 1, N, relab[top[u]], relab[u]);
			u = fa[top[u]];
		}
		
		if(depth[u] > depth[v])
			std::swap(u, v);
		if(depth[v] != depth[u])
			total += depth[v] - depth[u]
				- st.ask_sum(1, 1, N, relab[son[u]], relab[v]);
		return total;
	}
};

graph<30000, 60000> gp;
std::set<std::pair<int, int> > se;

template<int VSize, int ESize>
class graph2
{
	int loop_total;
	int loop_record[ESize + 1][2];

	int head[VSize + 1];
	int next[ESize + 1];
	int point[ESize + 1];
	char mark[VSize + 1];
	int total;
public:
	void init()
	{
		total = 0;
		loop_total = 0;
		std::memset(mark, 0, sizeof(mark));
		std::memset(head, 0, sizeof(head));
	}

	void add_edge(int u, int v)
	{
		point[++total] = v;
		next[total] = head[u];
		head[u] = total;
	}

	void dfs(int u, int f)
	{
		mark[u] = 1;
		for(int k = head[u]; k; k = next[k])
		{
			int v = point[k];
			if(v == f) continue;
			std::pair<int, int> pr(u, v);
			if(pr.first > pr.second)
				std::swap(pr.first, pr.second);
			if(se.count(pr)) continue;
			if(mark[v])
			{
				loop_record[loop_total][0] = u;
				loop_record[loop_total][1] = v;
				++loop_total;
				continue;
			}
			gp.add_edge(u, v);
			dfs(v, u);
		}
	}

	void init_loop()
	{
		for(int i = 0; i != loop_total; ++i)
		{
			int u = loop_record[i][0];
			int v = loop_record[i][1];
			gp.modify(u, v);
		}
	}
};

graph2<30000, 200000> gp2;
struct oper_info
{
	int type;
	int u, v;
} oper[40000];
int ans[40000];

int main()
{
	gp.init();
	gp2.init();
	std::scanf("%d %d", &N, &M);
	for(int i = 0; i != M; ++i)
	{
		int u, v;
		std::scanf("%d %d", &u, &v);
		gp2.add_edge(u, v);
		gp2.add_edge(v, u);
	}

	int oper_total = 0;
	int type;
	while(std::scanf("%d", &type), type != -1)
	{
		int u, v;
		std::scanf("%d %d", &u, &v);
		oper[oper_total].type = type;
		oper[oper_total].u = u;
		oper[oper_total].v = v;
		if(type == 0)
		{
			if(u > v) std::swap(u, v);
			se.insert(std::make_pair<int, int>(u, v));
		}
		++oper_total;
	}

	gp2.dfs(1, 0);
	gp.dfs_tsize(1, 0, 0);
	gp.dfs_link(1, 0);
	gp2.init_loop();

	int ans_total = 0;
	for(int i = oper_total - 1; i >= 0; --i)
	{
		oper_info& op = oper[i];
		if(op.type == 1)
		{
			ans[ans_total++] = gp.ask_sum(op.u, op.v);
		} else {
			gp.modify(op.u, op.v);
		}
	}

	for(int i = ans_total - 1; i >= 0; --i)
		std::printf("%d\n", ans[i]);

	return 0;
}
