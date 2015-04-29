/* BZOJ-4012: [HNOI2015]开店
 *   动态树分治 */
#include <cstdio>
#include <algorithm>
#include <vector>

const int MaxL = 18, MaxN = 150010, MaxM = MaxN * 2;
int total;
int head[MaxN], point[MaxM], next[MaxM], weight[MaxM];
int age[MaxN], fa[MaxN], que[MaxN], dist[MaxN];
int mark[MaxN], size[MaxN], max_size[MaxN];

struct data_t
{
	int age;
	long long dist;
	data_t(int a, int b) : age(a), dist(b) {}
	bool operator < (const data_t& v) const
	{
		return age < v.age;
	}
};

struct branch_t;
struct branch_data_t
{
	branch_t* child;
	std::vector<data_t> ages;
};

struct branch_t
{
	int root;
	std::vector<int> edges;
	std::vector<branch_data_t> branches;
} splits[MaxN];

struct belong_t
{
	int dist;
	branch_t *sp;
};
std::vector<belong_t> belong[MaxN];
typedef std::vector<belong_t>::iterator branch_iter;

void add_edge(int u, int v, int w)
{
	point[++total] = v;
	weight[total] = w;
	next[total] = head[u];
	head[u] = total;
}

int bfs_sort(int u)
{
	int qhead = 0, qtail = 0;
	que[qtail++] = u; fa[u] = 0;
	while(qhead != qtail)
	{
		int u = que[qhead++];
		size[u] = max_size[u] = 0;
		for(int k = head[u]; k; k = next[k])
		{
			int v = point[k];
			if(!mark[v] && v != fa[u])
			{
				fa[v] = u;
				que[qtail++] = v;
			}
		}
	}

	return qtail;
}

branch_t* divide(int now)
{
	int tot = bfs_sort(now);
	for(int i = tot - 1; i >= 0; --i)
	{
		int u = que[i];
		size[u] += 1;
		size[fa[u]] += size[u];
		if(max_size[fa[u]] < size[u])
			max_size[fa[u]] = size[u];
	}

	int root, root_child = ~0u >> 1;
	for(int i = 0; i != tot; ++i)
	{
		int u = que[i];
		if(max_size[u] < size[now] - size[u])
			max_size[u] = size[now] - size[u];
		if(max_size[u] < root_child)
		{
			root = u;
			root_child = max_size[u];
		}
	}

	bfs_sort(root);
	dist[root] = 0;
	for(int i = 0; i != tot; ++i)
	{
		int u = que[i];
		for(int k = head[u]; k; k = next[k])
		{
			int v = point[k];
			if(!mark[v] && v != fa[u])
				dist[v] = dist[u] + weight[k];
		}
	}

	mark[root] = 1;
	branch_t *sp = splits + root;
	sp->root = root;
	for(int i = 0; i != tot; ++i)
	{
		belong_t b;
		b.sp = sp;
		b.dist = dist[que[i]];
		belong[que[i]].push_back(b);
	}

	for(int k = head[root]; k; k = next[k])
	{
		int v = point[k];
		if(mark[v]) continue;
		branch_data_t data;
		int tot = bfs_sort(v);
		for(int i = 0; i != tot; ++i)
			data.ages.push_back(data_t(age[que[i]], dist[que[i]]));

		int inf = ~0u >> 1;
		data.ages.push_back(data_t(inf, 0));
		data.ages.push_back(data_t(-inf, 0));
		std::sort(data.ages.begin(), data.ages.end());
		for(int i = 1; i != data.ages.size(); ++i)
			data.ages[i].dist += data.ages[i - 1].dist;
		data.child = divide(v);
		sp->edges.push_back(weight[k]);
		sp->branches.push_back(data);
	}

	return sp;
}

struct dist_t
{
	long long dist, size;
	dist_t(long long a, long long b) : dist(a), size(b) {}
};

dist_t calc(branch_data_t &data, int l, int r)
{
	typedef std::vector<data_t>::iterator iter_t;
	iter_t beg = data.ages.begin(), end = data.ages.end();
	iter_t a = std::lower_bound(beg, end, data_t(l, 0)) - 1;
	iter_t b = std::upper_bound(beg, end, data_t(r, 0)) - 1;
	if(a == b) return dist_t(0, 0);
	return dist_t(b->dist - a->dist, b - a);
}

long long solve(branch_t *now, branch_iter it, int u, int l, int r)
{
	long long ans = 0;
	if(age[now->root] >= l && age[now->root] <= r)
		ans += it->dist;
	if(now->root == u)
	{
		for(int i = 0; i != now->branches.size(); ++i)
			ans += calc(now->branches[i], l, r).dist;
		return ans;
	}

	for(int i = 0; i != now->branches.size(); ++i)
	{
		branch_t *sp = now->branches[i].child;
		if(sp == (it + 1)->sp)
		{
			ans += solve(sp, it + 1, u, l, r);
		} else {
			dist_t z = calc(now->branches[i], l, r);
			ans += z.dist + z.size * it->dist;
		}
	}

	return ans;
}

int main()
{
	int n, Q, A;
	std::scanf("%d %d %d", &n, &Q, &A);
	for(int i = 1; i <= n; ++i)
		std::scanf("%d", age + i);
	for(int i = 1; i != n; ++i)
	{
		int u, v, w;
		std::scanf("%d %d %d", &u, &v, &w);
		add_edge(u, v, w);
		add_edge(v, u, w);
	}

	branch_t *root = divide(1);
	long long lastans = 0;
	for(int i = 0; i != Q; ++i)
	{
		int u, a, b;
		std::scanf("%d %d %d", &u, &a, &b);
		int l = (a + lastans) % A;
		int r = (b + lastans) % A;
		if(l > r) std::swap(l, r);
		lastans = solve(root, belong[u].begin(), u, l, r);
		std::printf("%lld\n", lastans);
	}
	return 0;
}
