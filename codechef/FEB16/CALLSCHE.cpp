#include <cstdio>
#include <algorithm>
#include <cstring>

const int MaxN = 70, MaxV = 20000, MaxE = 2000000;
int total, S, T;
int head[MaxV], next[MaxE], point[MaxE];
int cur[MaxV], cap[MaxE], flag[MaxV], que[MaxV];
int L[MaxN], R[MaxN][MaxN], F[MaxN][MaxN][MaxN];

void add_edge0(int u, int v, int c)
{
	point[++total] = v;
	next[total] = head[u];
	cap[total] = c;
	head[u] = total;
}

void add_edge(int u, int v, int c)
{
	if(c <= 0) return;
	//printf("%d %d %d\n", u, v, c);
	add_edge0(u, v, c);
	add_edge0(v, u, 0);
}

bool bfs()
{
	std::memset(flag, 0, sizeof(flag));
	int qh = 0, qt = 0;
	que[qt++] = T, flag[T] = 1;
	while(qh != qt)
	{
		int u = que[qh++];
		for(int k = head[u]; k; k = next[k])
		{
			int v = point[k];
			if(!flag[v] && cap[k ^ 1])
			{
				flag[v] = flag[u] + 1;
				que[qt++] = v;
			}
		}
	}

	return flag[S];
}

int dfs(int u, int rest)
{
	if(u == T) return rest;
	int used = 0;
	for(int &k = cur[u]; k; k = next[k])
	{
		int g = std::min(rest - used, cap[k]);
		if(g && flag[point[k]] + 1 == flag[u])
		{
			g = dfs(point[k], g);
			cap[k] -= g;
			cap[k ^ 1] += g;
			used += g;
			if(used == rest) break;
		}
	}

	return used;
}

int flow()
{
	int ans = 0;
	while(bfs())
	{
		std::memcpy(cur, head, sizeof(cur));
		ans += dfs(S, ~0u >> 1);
	}

	return ans;
}

bool solve()
{
	total = 1;
	std::memset(head, 0, sizeof(head));

	int P, D, H, N, LTbegin, LTend, sum = 0;

	// input
	std::scanf("%d %d %d %d", &P, &D, &H, &N);

	for(int i = 0; i != P; ++i)
		std::scanf("%d", L + i);

	std::scanf("%d %d", &LTbegin, &LTend);

	for(int i = 0; i != D; ++i)
		for(int j = 0; j != H; ++j)
		{
			std::scanf("%d", R[i] + j);
			sum += R[i][j];
		}

	for(int k = 0; k != P; ++k)
		for(int i = 0; i != D; ++i)
			for(int j = 0; j != H; ++j)
				std::scanf("%d", F[k][i] + j);

	// build
	auto people = [=](int p, int d, int t) {
		// people, day, type (lunch, other)
		return P + D * P + 2 * D * p + 2 * d + t + 3;
	};

	auto day = [=](int d, int h) {
		// day, hour
		return (3 * D + 1) * P + d * H + h + 3;
	};

	S = 1, T = 2;
	for(int i = 0; i != D; ++i)
		for(int j = 0; j != H; ++j)
			add_edge(day(i, j), T, R[i][j]);

	for(int k = 0; k != P; ++k)
	{
		int pid = k + 3;
		add_edge(S, pid, L[k]);
		for(int i = 0; i != D; ++i)
		{
			int m[2] = {0, 0};
			for(int j = 0; j != H; ++j)
			{
				int lunch = (LTbegin <= j + 1 && j + 1 <= LTend) ? 0 : 1;
				m[lunch] += 1 - F[k][i][j];
				if(F[k][i][j])
					add_edge(people(k, i, lunch), day(i, j), 1);
			}

			int did = P + D * k + i + 3, LT = LTend - LTbegin + 1;
			if(N < m[0] + m[1] || LT == m[0]) return false;
			add_edge(pid, did, N - m[0] - m[1]);
			add_edge(did, people(k, i, 0), LT - m[0] - 1);
			add_edge(did, people(k, i, 1), H - LT - m[1]);
		}
	}

	return flow() == sum;
}

int main()
{
	int t;
	std::scanf("%d", &t);
	while(t --> 0)
		std::puts(solve() ? "Yes" : "No");
	return 0;
}
