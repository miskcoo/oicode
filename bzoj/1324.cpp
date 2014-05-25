/* 
 * BZOJ-1324 Exca王者之剑
 *  网络流-最大点独立集
 */
#include <cstdio>
#include <cstring>

template<int MaxVertex, int MaxEdge>
class dinic
{
	int cur[MaxVertex + 1];
	int head[MaxVertex + 1];
	int next[(MaxEdge + 3) * 2];
	int point[(MaxEdge + 3) * 2];
	int cap[(MaxEdge + 3) * 2];
	int que[(MaxEdge + 3) * 2];
	int flag[MaxVertex + 1];
	int S, T;
	
	int total;
	int answer;
public:
	bool bfs()
	{
		std::memset(flag, 0, sizeof(flag));
		int qh = 0, qt = 1;
		que[0] = T;
		flag[T] = 1;
		while(qh != qt)
		{
			int u = que[qh++];
			for(int k = head[u]; k; k = next[k])
			{
				int v = point[k];
				if(cap[k ^ 1] && flag[v] == 0)
				{
					flag[v] = flag[u] + 1;
					que[qt++] = v;
					if(v == S) return true;
				}
			}
		}
		return flag[S] != 0;
	}

	int min(int a, int b)
	{
		return a < b ? a : b;
	}

	int dfs(int now, int min_cap)
	{
		if(now == T)
		{
			answer += min_cap;
			return min_cap;
		}

		int used = 0;
		for(int& k = cur[now]; k; k = next[k])
		{
			int v = point[k];
			if(cap[k] && flag[now] - 1 == flag[v])
			{
				int g = dfs(v, min(cap[k], min_cap - used));
				used += g;
				cap[k] -= g;
				cap[k ^ 1] += g;
				if(min_cap == used)
					return used;
			}
		}
		return used;
	}

	int solve(int s, int t)
	{
		S = s, T = t;
		while(bfs())
		{
			std::memcpy(cur, head, sizeof(head));
			dfs(S, 0x7fffffff);
		}

		return answer;
	}

	void init()
	{
		total = 2;
		answer = 0;
		std::memset(head, 0, sizeof(head));
		std::memset(cap, 0, sizeof(cap));
	}

	void add_edge(int f, int t, int c)
	{
		cap[total] += c;
		point[total] = t;
		next[total] = head[f];
		head[f] = total++;

		point[total] = f;
		next[total] = head[t];
		head[t] = total++;
	}
};

dinic<12000, 50500> mf;

int map_point(int x, int y)
{
	return x * 101 + y + 1;
}

int main()
{
	std::freopen("grid.in", "r", stdin);
	std::freopen("grid.out", "w", stdout);
	int N, M;
	std::scanf("%d %d", &N, &M);
	int sum = 0;
	int s = 11998, t = 11999;
	mf.init();
	for(int i = 0; i != N; ++i)
	{
		for(int j = 0; j != M; ++j)
		{
			int v;
			std::scanf("%d", &v);
			sum += v;
			int u = map_point(i, j);
			if((i ^ j) & 1)
			{
				const int max_v = 0x7fffffff;
				mf.add_edge(s, u, v);
				if(i != 0) mf.add_edge(u, map_point(i - 1, j), max_v);
				if(j != 0) mf.add_edge(u, map_point(i, j - 1), max_v);
				if(i != N) mf.add_edge(u, map_point(i + 1, j), max_v);
				if(j != M) mf.add_edge(u, map_point(i, j + 1), max_v);
			} else {
				mf.add_edge(u, t, v);
			}
		}
	}

	int ans = mf.solve(s, t);
	std::printf("%d", sum - ans);
	return 0;
}
