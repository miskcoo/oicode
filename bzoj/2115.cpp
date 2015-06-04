/* BZOJ-2115: [Wc2011] Xor
 *   高斯消元 */
#include <cstdio>

const int MaxN = 50010, MaxM = 200010, MaxL = 62;
int total;
int head[MaxN], point[MaxM], next[MaxM], mark[MaxN];
long long w[MaxL], weight[MaxM], dist[MaxN];

void add_edge(int u, int v, long long w)
{
	point[++total] = v;
	weight[total] = w;
	next[total] = head[u];
	head[u] = total;
}

void add_value(long long v)
{
	for(int i = MaxL; i >= 0; --i)
	{
		if((v >> i) & 1) {
			if(w[i]) 
			{
				v ^= w[i];
			} else {
				w[i] = v;
				break;
			}
		}
	}
}

void dfs(int u)
{
	mark[u] = 1;
	for(int k = head[u]; k; k = next[k])
	{
		int v = point[k];
		if(mark[v])
		{
			add_value(dist[v] ^ dist[u] ^ weight[k]);
		} else {
			dist[v] = dist[u] ^ weight[k];
			dfs(v);
		}
	}
}

int main()
{
	int n, m;
	std::scanf("%d %d", &n, &m);
	for(int i = 0; i != m; ++i)
	{
		int u, v;
		long long w;
		std::scanf("%d %d %lld", &u, &v, &w);
		add_edge(u, v, w);
		add_edge(v, u, w);
	}

	dfs(1);
	long long ans = dist[n];
	for(int i = MaxL; i >= 0; --i)
	{
		if(w[i] && !((ans >> i) & 1))
			ans ^= w[i];
	}

	std::printf("%lld", ans);
	return 0;
}
