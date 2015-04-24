/* BZOJ-3052: [wc2013]糖果公园
 *   带修改树上莫队 */
#include <cmath>
#include <cstdio>
#include <cstring>
#include <algorithm>

const int MaxN = 100010, MaxM = 200010, MaxL = 20;
int n, m, q, block_size, block_num, stop, total;
int head[MaxN], next[MaxM], point[MaxM], depth[MaxN];
int dist[MaxL][MaxN], block[MaxN], stack[MaxN];
int vis[MaxN], count[MaxN], C[MaxN];
long long V[MaxN], W[MaxN];
long long now_ans, ans[MaxN];

struct ques_t
{
	int x, y, t, id;
	bool operator < (const ques_t& b) const
	{
		if(block[x] == block[b.x])
		{
			if(block[y] == block[b.y])
				return t < b.t;
			return block[y] < block[b.y];
		}

		return block[x] < block[b.x];
	}
} ques[MaxN];

struct change_t
{
	int x, y, prev;
} change[MaxN];

void add_edge(int u, int v)
{
	point[++total] = v;
	next[total] = head[u];
	head[u] = total;
}

int dfs(int u)
{
	for(int i = 1; i != MaxL; ++i)
		dist[i][u] = dist[i - 1][dist[i - 1][u]];

	int size = 1;
	stack[stop++] = u;
	for(int k = head[u]; k; k = next[k])
	{
		if(point[k] != dist[0][u])
		{
			dist[0][point[k]] = u;
			depth[point[k]] = depth[u] + 1;
			size += dfs(point[k]);
			if(size >= block_size)
			{
				for(++block_num; stack[stop - 1] != u; --size)
					block[stack[--stop]] = block_num;
			}
		}
	}

	return size;
}

int get_lca(int u, int v)
{
	if(depth[u] < depth[v]) 
		std::swap(u, v);
	int diff = depth[u] - depth[v];
	for(int i = 0; diff; ++i, diff >>= 1)
		if(diff & 1) u = dist[i][u];
	for(int p = MaxL - 1; u != v; p ? --p : 0)
	{
		if(!p || dist[p][u] != dist[p][v])
		{
			u = dist[p][u];
			v = dist[p][v];
		}
	}

	return u;
}

void remove_color(int c) { now_ans -= V[c] * W[count[c]--]; }
void insert_color(int c) { now_ans += V[c] * W[++count[c]]; }
void toggle_color(int x)
{
	if(vis[x]) remove_color(C[x]);
	else insert_color(C[x]);
	vis[x] ^= 1;
}

void move_path(int u, int v)
{
	while(u != v)
	{
		if(depth[u] > depth[v])
		{
			toggle_color(u);
			u = dist[0][u];
		} else {
			toggle_color(v);
			v = dist[0][v];
		}
	}
}

void move_time(int s, int t)
{
	if(s == t) return;
	if(s < t)
	{
		while(s != t)
		{
			int x = change[++s].x;
			if(vis[x])
			{
				remove_color(C[x]);
				C[x] = change[s].y;
				insert_color(C[x]);
			} else C[x] = change[s].y;
		}
	} else {
		while(s != t)
		{
			int x = change[s].x;
			if(vis[x])
			{
				remove_color(C[x]);
				C[x] = change[s--].prev;
				insert_color(C[x]);
			} else C[x] = change[s--].prev;
		}
	}
}

int main()
{
	std::scanf("%d %d %d", &n, &m, &q);
	for(int i = 1; i <= m; ++i)
		std::scanf("%lld", V + i);
	for(int i = 1; i <= n; ++i)
		std::scanf("%lld", W + i);
	for(int i = 1; i != n; ++i)
	{
		int u, v;
		std::scanf("%d %d", &u, &v);
		add_edge(u, v);
		add_edge(v, u);
	}

	for(int i = 1; i <= n; ++i)
		std::scanf("%d", C + i);

	int *color = new int[n + 1];
	std::memcpy(color, C, sizeof(int) * (n + 1));
	int qtot = 0, ctot = 0;
	for(int i = 0; i != q; ++i)
	{
		int type, x, y;
		std::scanf("%d %d %d", &type, &x, &y);
		if(type == 0)
		{
			change[ctot].x = x;
			change[ctot].y = y;
			change[ctot++].prev = color[x];
			color[x] = y;
		} else {
			ques[qtot].id = qtot;
			ques[qtot].x = x;
			ques[qtot].y = y;
			ques[qtot++].t = ctot - 1;
		}
	}

	delete[] color;

	block_size = (int)std::pow(n, 0.66667) + 1;
	dfs(1);
	for(++block_num; stop; block[stack[--stop]] = block_num);
	std::sort(ques, ques + qtot);

	int now = -1, pu = 1, pv = 1;
	for(int i = 0; i != qtot; ++i)
	{
		int x = ques[i].x, y = ques[i].y, t = ques[i].t;
		move_time(now, t);
		move_path(pu, x);
		move_path(pv, y);
		int lca = get_lca(x, y);
		toggle_color(lca);
		ans[ques[i].id] = now_ans;
		toggle_color(lca);
		now = t, pu = x, pv = y;
	}

	for(int i = 0; i != qtot; ++i)
		std::printf("%lld\n", ans[i]);
	return 0;
}
