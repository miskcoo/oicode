#include <cstdio>
#include <cstdlib>

const int MaxN = 55, MaxE = 5000, MaxK = 55;
const int mod_v = 10007;
int f1[MaxN][MaxN][MaxK];
int f2[MaxN][MaxN][MaxK];
int g[MaxN][MaxN][MaxK];

struct graph_t
{
	int total;
	int head[MaxN];
	int next[MaxE], type[MaxE];
	int point[MaxE], from[MaxE];

	void add_edge(int u, int v, int t)
	{
		point[++total] = v;
		from[total] = u;
		type[total] = t;
		next[total] = head[u];
		head[u] = total;
	}
} g1, g2, g3;

void update(int& v, int a)
{
	v += a;
	if(v >= mod_v) 
		v -= mod_v;
}

void update2(int& v, int a, int b)
{
	update(v, a * b % mod_v);
}

void solve(int n, int m, int k)
{
	for(int u = 1; u <= n; ++u)
		f1[u][u][0] = g[u][u][0] = 1;

	for(int i = 1; i <= g1.total; ++i)
	{
		for(int j = 1; j <= g2.total; ++j)
		{
			if(g1.point[i] == g2.from[j] && g1.type[i] == g2.type[j])
				g[g1.from[i]][g2.point[j]][2] += 1;
		}
	}

	for(int pk = 1; pk <= k; ++pk)
	{
		for(int u = 1; u <= n; ++u)
		{
			for(int v = 1; v <= n; ++v)
			{
				for(int t = g3.head[u]; t; t = g3.next[t])
				{
					update(f1[u][v][pk], f1[g3.point[t]][v][pk - 1]);
					update(f2[u][v][pk], f2[g3.point[t]][v][pk - 1]);
				}

				for(int t = g1.head[u]; t; t = g1.next[t])
				{
					update(f2[u][v][pk], f1[g1.point[t]][v][pk - 1]);
					update(f2[u][v][pk], f2[g1.point[t]][v][pk - 1]);
				}

				for(int p = 1; p <= n; ++p)
				{
					for(int q = 2; q <= pk; ++q)
					{
						update2(f1[u][v][pk], g[u][p][q], f1[p][v][pk - q]);
						update2(f2[u][v][pk], g[u][p][q], f2[p][v][pk - q]);
					}
				}
			}
		}

		for(int p = 1; p <= g1.total; ++p)
		{
			for(int q = 1; q <= g2.total; ++q)
			{
				if(g1.type[p] != g2.type[q])
					continue;
				update(g[g1.from[p]][g2.point[q]][pk + 2], 
					f1[g1.point[p]][g2.from[q]][pk]);
			}
		}
	}
}

int main()
{
	std::freopen("kamion.in", "r", stdin);
	std::freopen("kamion.out", "w", stdout);
	int n, m, k;
	std::scanf("%d %d %d", &n, &m, &k);

	int pre_status = 0;
	for(int i = 0; i != m; ++i)
	{
		int u, v, t;
		if(pre_status)
		{
			u = pre_status;
			std::scanf("%d", &v);
		} else {
			std::scanf("%d %d", &u, &v);
		}

		char type[3];
		type[0] = 0;
		std::scanf("%s", type);
		if(!type[0] || type[0] >= '0' && type[0] <= '9')
		{
			t = 0;
			pre_status = std::atoi(type);
		} else {
			if(type[0] >= 'a' && type[0] <= 'z')
				t = 'a' - type[0] - 1;
			else t = type[0] - 'A' + 1;
			pre_status = 0;
		}

		if(t == 0) g3.add_edge(u, v, t);
		else if(t > 0) g1.add_edge(u, v, t);
		else g2.add_edge(u, v, -t);
	}

	solve(n, m, k);
	int ans = 0;
	for(int pk = 0; pk <= k; ++pk)
	{
		update(ans, f1[1][n][pk]);
		update(ans, f2[1][n][pk]);
	}

	std::printf("%d", ans);
	return 0;
}
