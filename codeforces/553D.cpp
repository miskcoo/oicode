/* Codeforces 553D. Nudist Beach
 *   graphs, greedy, meet-in-the-middle */
#include <cstdio>
#include <set>

const int MaxN = 100001, MaxM = 200001;
int total;
int head[MaxN], point[MaxM], next[MaxM];
int outside[MaxN], deg[MaxN], seq[MaxN];

typedef std::pair<double, int> vertex_t;
std::set<vertex_t> s0;
std::set<vertex_t>::iterator it[MaxN];

void add_edge(int u, int v)
{
	++deg[u];
	point[++total] = v;
	next[total] = head[u];
	head[u] = total;
}

int main()
{
	int n, m, k;
	std::scanf("%d %d %d", &n, &m, &k);
	for(int i = 0; i != k; ++i)
	{
		int v;
		std::scanf("%d", &v);
		outside[v] = 1;
	}

	for(int i = 0; i != m; ++i)
	{
		int u, v;
		std::scanf("%d %d", &u, &v);
		add_edge(u, v);
		add_edge(v, u);
	}

	for(int u = 1; u <= n; ++u)
	{
		if(outside[u]) continue;
		double count = deg[u];
		for(int k = head[u]; k; k = next[k])
			count -= outside[point[k]];
		it[u] = s0.insert(vertex_t(count / deg[u], u)).first;
	}

	int pos = 0;
	vertex_t now;
	for(int i = 0; i != n - k; ++i)
	{
		auto vert = s0.begin();
		if(*vert > now) now = *vert, pos = i;
		int u = vert->second;
		for(int k = head[u]; k; k = next[k])
		{
			int v = point[k];
			if(!outside[v])
			{
				vertex_t new_vert(it[v]->first - 1.0 / deg[v], v);
				s0.erase(it[v]);
				it[v] = s0.insert(new_vert).first;
			}
		}

		s0.erase(vert);
		outside[u] = 1;
		seq[i] = u;
	}

	std::printf("%d\n", n - k - pos);
	for(int i = pos; i != n - k; ++i)
		std::printf("%d ", seq[i]);

//	std::printf("rate: %.4lf\n", now.first);
	return 0;
}
