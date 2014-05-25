/* 
 * BZOJ-1497 [NOI2006]最大获利
 *  贾教网络流
 */ 
#include <cstdio>
#include <algorithm>
#include <cstring>

template<int VSize, int ESize>
class netflow_dinic
{
	int total;
	int head[VSize + 1];
	int next[ESize + 1];
	int cap[ESize + 1];
	int point[ESize + 1];

	int cur[VSize + 1];
	int flag[VSize + 1];
	int que[VSize + 1];
	int S, T;

	int answer;
public:
	bool bfs()
	{
		std::memset(flag, 0, sizeof(flag));
		int qhead = 0, qtail = 0;
		que[qtail++] = T;
		flag[T] = 1;
		while(qhead != qtail)
		{
			int u = que[qhead++];
			for(int k = head[u]; k; k = next[k])
			{
				int v = point[k];
				if(cap[k ^ 1] && flag[v] == 0)
				{
					flag[v] = flag[u] + 1;
					que[qtail++] = v;
					if(v == S) return true;
				}
			}
		}

		return flag[S] != 0;
	}

	int dfs(int u, int min_cap)
	{
		if(T == u)
		{
			answer += min_cap;
			return min_cap;
		}

		int used = 0;
		for(int& k = cur[u]; k; k = next[k])
		{
			int v = point[k];
			if(cap[k] && flag[v] == flag[u] - 1)
			{
				int g = dfs(v, std::min(cap[k], min_cap - used));
				used += g;
				cap[k] -= g;
				cap[k ^ 1] += g;
				if(used == min_cap) 
					return used;
			}
		}

		return used;
	}
	
	int dinic(int s, int t)
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
		std::memset(head, 0, sizeof(head));
	}

	void add_edge(int u, int v, int cap, int capr = 0)
	{
		point[total] = v;
		this->cap[total] = cap;
		next[total] = head[u];
		head[u] = total++;

		point[total] = u;
		this->cap[total] = capr;
		next[total] = head[v];
		head[v] = total++;
	}
};

netflow_dinic<5010, 5010 * 4 + 50010 * 2> gp;
int edge_to[5001];

int main()
{
	int N, M;
	gp.init();
	std::scanf("%d %d", &N, &M);
	int s = 5008, t = 5009;
	for(int i = 1; i <= N; ++i)
	{
		int v;
		std::scanf("%d", &v);
		gp.add_edge(s, i, v << 1);
	}

	int sum = 0;
	for(int i = 1; i <= M; ++i)
	{
		int u, v, w;
		std::scanf("%d %d %d", &u, &v, &w);
		sum += w;
		gp.add_edge(u, v, w, w);
		edge_to[u] += w;
		edge_to[v] += w;
	}

	for(int i = 0; i <= N; ++i)
		gp.add_edge(i, t, edge_to[i]);

	std::printf("%d", sum - gp.dinic(s, t) / 2);
	return 0;
}
