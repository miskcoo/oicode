/* BZOJ-4016: [FJOI2014]最短路径树问题
 *   树分治 */
#include <cstdio>
#include <cstring>
#include <algorithm>

const int MaxN = 30010, MaxM = 120010;

struct graph_t
{
	int total;
	int head[MaxN], point[MaxM], next[MaxM], weight[MaxM];

	void add_edge(int u, int v, int w)
	{
		point[++total] = v;
		weight[total] = w;
		next[total] = head[u];
		head[u] = total;
	}
} gp, gt;

struct tmp_t
{
	int u, w;
	bool operator < (const tmp_t& v) const
	{
		return u < v.u;
	}
} tmp[MaxN];

int mark_v = 2, mark2_v = 3;
int que[MaxN], mark[MaxN], dist[MaxN], mark2[MaxM];
void spfa()
{
	std::memset(dist, 77, sizeof(dist));
	int qhead = 0, qtail = 0;
	dist[1] = 0, mark[1] = 1;
	que[qtail++] = 1;
	while(qhead != qtail)
	{
		int u = que[qhead++];
		if(qhead == MaxN) qhead = 0;
		for(int k = gp.head[u]; k; k = gp.next[k])
		{
			int v = gp.point[k];
			int d = dist[u] + gp.weight[k];
			if(d < dist[v]) 
			{
				dist[v] = d;
				if(!mark[v])
				{
					mark[v] = 1;
					que[qtail++] = v;
					if(qtail == MaxN) qtail = 0;
				}
			}
		} 

		mark[u] = 0;
	}
}

void make_tree(int n)
{
	for(int u = 1; u <= n; ++u)
	{
		for(int k = gp.head[u]; k; k = gp.next[k])
		{
			int v = gp.point[k];
			if(dist[u] + gp.weight[k] == dist[v] && u != v)
				mark2[k] = 1;
		}
	}

	int qhead = 0, qtail = 0;
	mark[1] = 1;
	que[qtail++] = 1;
	while(qhead != qtail)
	{
		int u = que[qhead++], tot = 0;
		for(int k = gp.head[u]; k; k = gp.next[k])
		{
			if(mark2[k] && !mark[gp.point[k]])
			{
				tmp[tot].u = gp.point[k];
				tmp[tot++].w = gp.weight[k];
			}
		}

		std::sort(tmp, tmp + tot);
		for(int i = 0; i != tot; ++i)
		{
			int v = tmp[i].u, w = tmp[i].w;
			mark[v] = 1;
			que[qtail++] = v;
			gt.add_edge(u, v, w);
			gt.add_edge(v, u, w);
		}
	}
}

int now_ans = 0, now_num = 0;
int size[MaxN], max_size[MaxN], lazy_v, length;
int data_num, dists[MaxN], counts[MaxN], lazy[MaxN];
struct data_t
{
	int depth, dist;
} data[MaxN];

void calc(int u, int fa, int depth, int dist)
{
	if(depth > length) return;
	data[data_num].depth = depth;
	data[data_num++].dist = dist;
	if(lazy[length - depth] == lazy_v)
	{
		int d = dists[length - depth] + dist;
		if(d > now_ans)
		{
			now_ans = d;
			now_num = counts[length - depth];
		} else if(d == now_ans) now_num += counts[length - depth];
	}

	for(int k = gt.head[u]; k; k = gt.next[k])
	{
		int v = gt.point[k];
		if(v != fa && mark[v] != mark_v)
			calc(v, u, depth + 1, dist + gt.weight[k]);
	}
}

void tree_divide(int u)
{
	int qhead = 0, qtail = 0;
	mark2[u] = ++mark2_v;
	que[qtail++] = u;
	while(qhead != qtail)
	{
		int u = que[qhead++];
		for(int k = gt.head[u]; k; k = gt.next[k])
		{
			int v = gt.point[k];
			if(mark2[v] != mark2_v && mark[v] != mark_v)
			{
				que[qtail++] = v;
				mark2[v] = mark2_v;
			}
		}
	}

	for(int i = qtail - 1; i >= 0; --i)
	{
		int u = que[i];
		size[u] = 1;
		for(int k = gt.head[u]; k; k = gt.next[k])
		{
			int v = gt.point[k];
			size[u] += size[v];
			if(max_size[u] < size[v])
				max_size[u] = size[v];
		}
	}

	int root = 0, rsize = ~0u >> 1;
	for(int i = 0; i != qtail; ++i)
	{
		int z = que[i];
		if(size[u] - size[z] > max_size[z])
			max_size[z] = size[u] - size[z];
		if(max_size[z] < rsize)
		{
			root = z;
			rsize = max_size[z];
		}
	}

	mark[root] = mark_v;
	dists[0] = 0;
	counts[0] = 1;
	lazy[0] = ++lazy_v;
	for(int k = gt.head[root]; k; k = gt.next[k])
	{
		if(mark[gt.point[k]] == mark_v) continue;
		data_num = 0;
		calc(gt.point[k], root, 1, gt.weight[k]);
		for(int i = 0; i != data_num; ++i)
		{
			int d = data[i].depth, di = data[i].dist;
			if(lazy[d] != lazy_v) 
			{
				lazy[d] = lazy_v;
				counts[d] = 0;
				dists[d] = 0;
			}

			if(dists[d] < di)
			{
				dists[d] = di;
				counts[d] = 1;
			} else if(dists[d] == di) ++counts[d];
		}
	}

	for(int k = gt.head[root]; k; k = gt.next[k])
	{
		if(mark[gt.point[k]] != mark_v)
			tree_divide(gt.point[k]);
	}
}

int main()
{
	int n, m;
	std::scanf("%d %d %d", &n, &m, &length);
	for(int i = 0; i != m; ++i)
	{
		int u, v, w;
		std::scanf("%d %d %d", &u, &v, &w);
		gp.add_edge(u, v, w);
		gp.add_edge(v, u, w);
	}

	--length;
	spfa();
	make_tree(n);
	tree_divide(1);
	std::printf("%d %d\n", now_ans, now_num);
	return 0;
}
