/*
 * POJ-1741 Tree
 *  树分治
 */
#include <cstdio>
#include <cstring>
#include <algorithm>

const int ESize = 200002;
const int VSize = 100002;

int weight[VSize];
int head[VSize];
char mark[VSize];
int point[ESize];
int next[ESize];
int tsize[VSize];
int max_tsize[VSize];
int total;
int cfg_root;
int tempans;
int answer;
int limit;

void init()
{
	std::memset(mark, 0, sizeof(mark));
	std::memset(head, 0, sizeof(head));
	total = 0;
	answer = 0;
}

void add_edge(int u, int v, int w)
{
	point[++total] = v;
	weight[total] = w;
	next[total] = head[u];
	head[u] = total;
}

void dfs_size(int u, int fa)
{
	tsize[u] = 1;
	max_tsize[u] = 0;
	for(int k = head[u]; k; k = next[k])
	{
		int v = point[k];
		if(v == fa || mark[v]) continue;
		dfs_size(v, u);
		tsize[u] += tsize[v];
		if(max_tsize[u] < tsize[v])
			max_tsize[u] = tsize[v];
	}
}

void dfs_cfg(int u, int fa, int r)
{
	if(tsize[r] - tsize[u] > max_tsize[u])
		max_tsize[u] = tsize[r] - tsize[u];
	if(max_tsize[u] < tempans)
	{
		tempans = max_tsize[u];
		cfg_root = u;
	}
	for(int k = head[u]; k; k = next[k])
	{
		int v = point[k];
		if(v == fa || mark[v]) continue;
		dfs_cfg(v, u, r);
	}
}

void dfs_dist(int u, int fa, int d)
{
	tsize[total++] = d;
	for(int k = head[u]; k; k = next[k])
	{
		int v = point[k];
		if(v == fa || mark[v]) continue;
		dfs_dist(v, u, d + weight[k]);
	}
}

int calc(int u, int dw)
{
	int count = 0;
	total = 0;
	dfs_dist(u, 0, dw);

	std::sort(tsize, tsize + total);
	int l = 0, r = total - 1;
	while(l < r)
	{
		while(tsize[l] + tsize[r] > limit && l < r) --r;
		count += r - l;
		++l;
	}
	return count;
}

void dfs(int u)
{
	tempans = 0x7fffffff;
	dfs_size(u, 0);
	dfs_cfg(u, 0, u);

	answer += calc(cfg_root, 0);
	mark[cfg_root] = 1;
	for(int k = head[cfg_root]; k; k = next[k])
	{
		int v = point[k];
		if(mark[v]) continue;
		answer -= calc(v, weight[k]);
		dfs(v);
	}
}

int main()
{
	for(;;)
	{
		int N;
		std::scanf("%d %d", &N, &limit);
		if(limit == 0 && N == 0) break;
		init();
		for(int i = 1; i != N; ++i)
		{
			int u, v, w;
			std::scanf("%d %d %d", &u, &v, &w);
			add_edge(u, v, w);
			add_edge(v, u, w);
		}
		dfs(1);
		std::printf("%d\n", answer);
	}
	return 0;
}
