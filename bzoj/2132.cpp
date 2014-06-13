/*
 * BZOJ-2132: 圈地计划
 *  最小割
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
		answer = 0;
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

dinic<11010, 11010 * 5> cf;
int C[101][101];
int map[101][101];

int main()
{
	cf.init();
	int s = 11001, t = 11002;
	int N, M;
	std::scanf("%d %d", &N, &M);
	int po = 0;
	int ans = 0;
	for(int i = 0; i != N; ++i)
	{
		for(int j = 0; j != M; ++j)
		{
			int v;
			std::scanf("%d", &v);
			ans += v;
			map[i][j] = ++po;
			if((i ^ j) & 1)
				cf.add_edge(s, map[i][j], v);
			else cf.add_edge(map[i][j], t, v);
		}
	}
	
	for(int i = 0; i != N; ++i)
	{
		for(int j = 0; j != M; ++j)
		{
			int v;
			std::scanf("%d", &v);
			ans += v;
			if((i ^ j) & 1)
				cf.add_edge(map[i][j], t, v);
			else cf.add_edge(s, map[i][j], v);
		}
	}

	for(int i = 0; i != N; ++i)
	{
		for(int j = 0; j != M; ++j)
			std::scanf("%d", C[i] + j);
	}

	for(int i = 0; i != N; ++i)
	{
		const int dir[4][2] = {{0, 1}, {1, 0}, {-1, 0}, {0, -1}};
		for(int j = 0; j != M; ++j)
		{
			for(int d = 0; d != 4; ++d)
			{
				int nx = i + dir[d][0];
				int ny = j + dir[d][1];
				if(nx < 0 || ny < 0 || nx >= N || ny >= M)
					continue;
				ans += C[i][j];
				cf.add_edge(map[i][j], map[nx][ny], C[i][j] + C[nx][ny]);
			}
		}
	}

	std::printf("%d\n", ans - cf.solve(s, t));
	return 0;
}
