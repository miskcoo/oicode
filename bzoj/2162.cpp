/*
 * BZOJ 2162: 男生女生
 *  网络流，DP
 */
#include <cstdio>
#include <algorithm>
#include <cstring>

template<int VSize, int CSize>
class graph
{
	static const int max_v = 0x7fffff;
	static const int mod_v = 19921228;
	int w[VSize + 1][VSize + 1];
	int f[VSize + 1][VSize + 1];
	int C[CSize + 1][CSize + 1];
	int mark[VSize + 1];
	int mark_v;
	int node;
	int vertex;
	int source, tink;
public:
	void init(int n)
	{
		node = n;
		vertex = node * 2 + 2;
		source = node * 2 + 1;
		tink = node * 2 + 2;
		mark_v = 1;
		std::memset(mark, 0, sizeof(mark));
	}

	void read_data()
	{
		int m;
		std::scanf("%d", &m);
		for(int i = 1; i <= node; ++i)
		{
			for(int j = 1; j <= node; ++j)
				w[i][j + node] = max_v;
		}

		for(int i = 0; i != m; ++i)
		{
			int u, v;
			std::scanf("%d %d", &u, &v);
			w[u][v + node] = 0;
		}

		for(int i = 1; i <= node; ++i)
		{
			w[source][i] = node + 1;
			w[i + node][tink] = node;
		}
	}

	int augment(int u, int rest)
	{
		if(u == tink) return rest;
		mark[u] = mark_v;
		int used = 0;
		for(int v = 1; v <= vertex; ++v)
		{
			if(!w[u][v] || mark[v] == mark_v)
				continue;
			int t = augment(v, std::min(w[u][v], rest - used));
			w[u][v] -= t;
			w[v][u] += t;
			used += t;
		}

		return used;
	}

	void init_combination(int n)
	{
		C[0][0] = 1;
		for(int i = 1; i <= n; ++i)
		{
			C[i][0] = C[i][i] = 1;
			for(int j = 1; j < i; ++j)
			{
				C[i][j] = C[i - 1][j - 1] + C[i - 1][j];
				if(C[i][j] >= mod_v) C[i][j] -= mod_v;
			}
		}
	}

	void dfs(int u)
	{
		mark[u] = mark_v;
		for(int v = 1; v <= vertex; ++v)
		{
			if(!w[u][v] || mark[v] == mark_v)
				continue;
			dfs(v);
		}
	}

	void solve(int k)
	{
		while(augment(source, max_v))
			++mark_v;

		dfs(source);
		int X = 0, Y = 0;
		for(int u = 1; u <= node; ++u)
		{
			X += (mark[u] == mark_v);
			Y += (mark[u + node] != mark_v);
		}

		if(X * Y < k) 
		{
			std::printf("%d %d\n0", X, Y);
			return;
		}

		long long pre = 0;
		long long ans = 0;
		init_combination(X * Y + 1);
		for(int i = 1; i <= X; ++i)
		{
			for(int j = 1; j <= Y; ++j)
			{
				pre = 1LL * C[X][i] * C[Y][j] % mod_v * C[i * j][k] % mod_v;
				if(((i + j) ^ (X + Y)) & 1)
					ans = (ans - pre) % mod_v;
				else ans = (ans + pre) % mod_v;
			}
		}

		std::printf("%d %d\n", X, Y);
		std::printf("%lld\n", ans < 0 ? ans + mod_v : ans);
	}
};

graph<150, 2800> gp;

int main()
{
	int n, k;
	std::scanf("%d %d", &n, &k);
	gp.init(n);
	gp.read_data();
	gp.solve(k);
	return 0;
}
