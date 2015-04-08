/* BZOJ-3924: [Zjoi2015]幻想乡战略游戏
 *  动态树分治 */
#include <cstdio>
#include <algorithm>
#include <vector>

const int MaxN = 100010, MaxM = 200010;
int n, total;
int head[MaxN], next[MaxM], point[MaxM], weight[MaxM];
int size[MaxN], branch[MaxN], dist[MaxN];
int mark[MaxN], fa[MaxN], que[MaxN];

struct data_t
{
	int size;
	long long dist;

	data_t() : size(0), dist(0) {}
};

struct edge_t
{
	int v, w;

	edge_t(int v, int w) : v(v), w(w) {}
};

struct split_t
{
	std::vector<split_t*> branch;
	std::vector<data_t> branch_data;
	std::vector<edge_t> edge;
	data_t all;

	void update(int id, int army, int dist)
	{
		long long d = 1ll * army * dist;
		branch_data[id].size += army;
		branch_data[id].dist += d;
		all.size += army;
		all.dist += d;
	}
} split[MaxN];

struct split_ptr_t
{
	split_t* sp;
	int id, dist;

	void attach(int army) {
		sp->update(id, army, dist);
	}
};

std::vector<split_ptr_t> split_ptr[MaxN];

void add_edge(int u, int v, int w)
{
	point[++total] = v;
	weight[total] = w;
	next[total] = head[u];
	head[u] = total;
}

void add_split_branch(split_t* sp, int u, int id, int dist)
{
	split_ptr_t s;
	s.id = id;
	s.sp = sp;
	s.dist = dist;
	split_ptr[u].push_back(s);
}

int tree_order(int u)
{
	int qhead = 0, qtail = 0;
	que[qtail++] = u;
	fa[u] = 0;
	while(qhead != qtail)
	{
		int u = que[qhead++];
		for(int k = head[u]; k; k = next[k])
		{
			int v = point[k];
			if(mark[v] || fa[u] == v)
				continue;
			fa[v] = u;
			que[qtail++] = v;
		}
	}

	return qtail;
}

int tree_get_root(int tot, int r)
{
	for(int i = tot - 1; i >= 0; --i)
	{
		int u = que[i];
		size[u] = 1;
		for(int k = head[u]; k; k = next[k])
		{
			int v = point[k];
			if(!mark[v] && fa[u] != v)
				size[u] += size[v];
		}
	}

	int root, root_size = ~0u >> 1;
	for(int i = tot - 1; i >= 0; --i)
	{
		int u = que[i], sz = size[r] - size[u];
		for(int k = head[u]; k; k = next[k])
		{
			int v = point[k];
			if(!mark[v] && fa[u] != v && sz < size[v])
				sz = size[v];
		}

		if(sz < root_size)
		{
			root = u;
			root_size = sz;
		}
	}

	return root;
}

split_t* tree_divide(int u)
{
	int tot = tree_order(u);
	int root = tree_get_root(tot, u);
	tree_order(root);
	int branch_index = 0;
	for(int k = head[root]; k; k = next[k])
	{
		int v = point[k];
		if(!mark[v]) branch[v] = branch_index++;
	}

	branch[root] = branch_index++;

	split_t* sp = split + root;
	sp->branch_data.resize(branch_index);
	dist[root] = 0;
	for(int i = 0; i != tot; ++i)
	{
		int u = que[i];
		if(fa[u] != root && fa[u])
			branch[u] = branch[fa[u]];

		for(int k = head[u]; k; k = next[k])
		{
			int v = point[k];
			if(!mark[v] && fa[u] != v) 
				dist[v] = dist[u] + weight[k];
		}

		add_split_branch(sp, u, branch[u], dist[u]);
	}

	mark[root] = 1;

	for(int k = head[root]; k; k = next[k])
	{
		int v = point[k];
		if(!mark[v]) 
		{
			sp->branch.push_back( tree_divide(v) );
			sp->edge.push_back( edge_t(v, weight[k]) );
		}
	}

	return sp;
}

void attach_army(int u, int d)
{
	for(size_t i = 0; i != split_ptr[u].size(); ++i)
		split_ptr[u][i].attach(d);
}

long long get_value(split_t* sp)
{
	int tot_size = sp->all.size;

	for(size_t i = 0; i != sp->branch.size(); ++i)
	{
		int branch_size = sp->branch_data[i].size;
		if(branch_size * 2 > tot_size)
		{
			int other_size = tot_size - branch_size;
			edge_t edge = sp->edge[i];
			attach_army(edge.v, other_size);
			long long ans = get_value(sp->branch[i]);
			attach_army(edge.v, -other_size);
			return ans + 1ll * other_size * edge.w 
				+ sp->all.dist - sp->branch_data[i].dist;
		}
	}

	return sp->all.dist;
}

int main()
{
	int q;
	std::scanf("%d %d", &n, &q);
	for(int i = 1; i != n; ++i)
	{
		int u, v, w;
		std::scanf("%d %d %d", &u, &v, &w);
		add_edge(u, v, w);
		add_edge(v, u, w);
	}

	split_t* root = tree_divide(1);
	while(q --> 0)
	{
		int u, add;
		std::scanf("%d %d", &u, &add);
		attach_army(u, add);
		std::printf("%lld\n", get_value(root));
	}
	return 0;
}
