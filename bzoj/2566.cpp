/*
 * BZOJ-2566 xmastree
 *  树分治+堆维护
 */
#include <cstdio>
#include <cstring>
#include <utility>
#include <set>

const int MaxN = 12001;
const int LogMaxN = 15;
typedef std::pair<int, int> color_dis;
std::multiset<color_dis> info[MaxN];
std::multiset<int> distance;
int color[MaxN];
int center[MaxN][LogMaxN];
int dist[MaxN][LogMaxN];

inline int find_min_dist(int root, int color)
{
	typedef std::multiset<color_dis>::iterator iter;
	iter beg = info[root].lower_bound(color_dis(color, 0));
	iter next = beg;
	if(beg == info[root].end() || beg->first != color)
		return -1;
	if(++next == info[root].end())
		return -1;
	if(beg->first != next->first)
		return -1;
	return beg->second + next->second;
}

inline void erase_color_dist(
	int root, int color, int dist)
{
	typedef std::multiset<color_dis>::iterator iter;
	iter it = info[root].lower_bound(color_dis(color, dist));
	info[root].erase(it);
}

inline void insert_color_dist(
	int root, int color, int dist)
{
	info[root].insert(color_dis(color, dist));
}

template<int VSize, int ESize>
class graph
{
	int head[VSize + 1];
	int next[ESize + 1];
	int point[ESize + 1];
	int length[ESize + 1];
	int total;
public:
	void init()
	{
		total = 0;
		std::memset(head, 0, sizeof(head));
		std::memset(mark, 0, sizeof(mark));
	}

	void add_edge(int u, int v, int w)
	{
		point[++total] = v;
		length[total] = w;
		next[total] = head[u];
		head[u] = total;
	}

private:
	int mark[VSize + 1];
	int sub_size[VSize + 1];
	int max_sub_size[VSize + 1];
	int root, root_max_sub;
private:
	void tree_dfs_size(int u, int fa)
	{
		max_sub_size[u] = 0;
		sub_size[u] = 1;
		for(int k = head[u]; k; k = next[k])
		{
			int v = point[k];
			if(mark[v] || fa == v)
				continue;
			tree_dfs_size(v, u);
			sub_size[u] += sub_size[v];
			if(max_sub_size[u] < sub_size[v])
				max_sub_size[u] = sub_size[v];
		}
	}

	void tree_dfs_center(int u, int fa, int r)
	{
		int t = sub_size[r] - sub_size[u];
		if(max_sub_size[u] < t)
			max_sub_size[u] = t;
		if(root_max_sub > max_sub_size[u])
		{
			root = u;
			root_max_sub = max_sub_size[u];
		}
		for(int k = head[u]; k; k = next[k])
		{
			int v = point[k];
			if(mark[v] || fa == v)
				continue;
			tree_dfs_center(v, u, r);
		}
	}

	void tree_calc(int u, int fa, int d, int level)
	{
		info[root].insert(color_dis(color[u], d));
		center[u][level] = root;
		dist[u][level] = d;
		for(int k = head[u]; k; k = next[k])
		{
			int v = point[k];
			if(mark[v] || v == fa)
				continue;
			tree_calc(v, u, d + length[k], level);
		}
	}
public:
	void tree_divide(int u, int level)
	{
		root_max_sub = 0x7fffffff;
		tree_dfs_size(u, 0);
		tree_dfs_center(u, 0, u);
		tree_calc(root, 0, 0, level);
		mark[root] = 1;
		
		for(int k = head[root]; k; k = next[k])
		{
			int v = point[k];
			if(mark[v]) continue;
			tree_divide(v, level + 1);
		}
	}
};

graph<MaxN, MaxN * 2> gp;

int main()
{
	int N;
	gp.init();
	std::scanf("%d", &N);
	for(int i = 1; i <= N; ++i)
		std::scanf("%d", color + i);
	for(int i = 1; i != N; ++i)
	{
		int u, v, w;
		std::scanf("%d %d %d", &u, &v, &w);
		gp.add_edge(u, v, w);
		gp.add_edge(v, u, w);
	}
	gp.tree_divide(1, 0);

	// 记录每一次分治同色点到重心的距离
	for(int i = 1; i <= N; ++i)
	{
		int pre_color = -1;
		typedef std::multiset<color_dis>::iterator iter;
		for(iter beg = info[i].begin(), 
			end = info[i].end(); beg != end; ++beg)
		{
			if(beg->first != pre_color)
			{
				pre_color = beg->first;
				iter next = beg;
				++next;
				if(next != end && next->first == beg->first)
				{
					distance.insert(next->second + beg->second);
					beg = next;
				}
			}
		}
	}

	std::printf("%d\n", distance.empty() ? -1 : *distance.begin());

	int M;
	std::scanf("%d", &M);
	for(int i = 0; i != M; ++i)
	{
		int node, c;
		std::scanf("%d %d", &node, &c);
		for(int j = 0; center[node][j]; ++j)
		{
			// 先把到当前节点重心的颜色的最短距离删掉
			// 为了防止改变颜色后影响到这个距离
			int v = find_min_dist(center[node][j], color[node]);
			if(v != -1) distance.erase(distance.find(v));
			erase_color_dist(center[node][j], color[node], dist[node][j]);

			// 重新计算最短距离
			v = find_min_dist(center[node][j], color[node]);
			if(v != -1) distance.insert(v);

			v = find_min_dist(center[node][j], c);
			if(v != -1) distance.erase(distance.find(v));
			insert_color_dist(center[node][j], c, dist[node][j]);

			v = find_min_dist(center[node][j], c);
			if(v != -1) distance.insert(v);
		}
		color[node] = c;
		std::printf("%d\n", distance.empty() ? -1 : *distance.begin());
	}
	return 0;
}
