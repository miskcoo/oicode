/* BZOJ-2553: [BeiJing2011]禁忌
 *  AC自动机+矩阵快速幂DP  */
#include <cstdio>
#include <algorithm>
#include <cstring>

const int MaxN = 5, MaxT = 16, MaxS = MaxN * MaxT + 2, MaxAlpha = 26;
struct node_t
{
	int accept;
	int fail, ch[MaxAlpha];
} node[MaxS];

typedef long double value_t;
int n, len, alphabet;
char str[MaxN][MaxT];
int used = 1, que[MaxS];

void add_string(const char* str)
{
	int now = 1;
	for(int i = 0; str[i]; ++i)
	{
		int x = str[i] - 'a';
		if(!node[now].ch[x])
			node[now].ch[x] = ++used;
		now = node[now].ch[x];
	}

	node[now].accept = 1;
}

void make_fail()
{
	int qhead = 0, qtail = 0;
	que[qtail++] = 1;
	while(qhead != qtail)
	{
		int now = que[qhead++];
		for(int i = 0; i != alphabet; ++i)
		{
			if(node[now].ch[i]) 
			{
				int u = node[now].ch[i];
				int z = node[now].fail;
				for(; z && !node[z].ch[i]; z = node[z].fail);
				node[u].fail = z ? node[z].ch[i] : 1;
				node[u].accept |= node[node[u].fail].accept;
				que[qtail++] = u;
			} else {
				int f = node[now].fail;
				for(; f && !node[f].ch[i]; f = node[f].fail);
				node[now].ch[i] = f ? node[f].ch[i] : 1;
			}
		}
	}
}

struct matrix_t
{
	value_t m[MaxS][MaxS];
	matrix_t operator * (const matrix_t& r) const
	{
		matrix_t d;
		for(int i = 0; i <= used; ++i)
		{
			for(int j = 0; j <= used; ++j)
			{
				value_t v = 0.0;
				for(int k = 0; k <= used; ++k)
					v += m[i][k] * r.m[k][j];
				d.m[i][j] = v;
			}
		}

		return d;
	}
} m;

matrix_t matrix_pow(matrix_t x, int p)
{
	matrix_t v;
	for(int i = 0; i <= used; ++i)
		for(int j = 0; j <= used; ++j)
			v.m[i][j] = 0.0;
	for(int i = 0; i <= used; ++i)
		v.m[i][i] = 1.0;
	while(p)
	{
		if(p & 1) v = x * v;
		x = x * x;
		p >>= 1;
	}

	return v;
}

int main()
{
	std::scanf("%d %d %d", &n, &len, &alphabet);
	for(int i = 0; i != n; ++i)
		std::scanf("%s", str[i]);

	for(int i = 0; i != n; ++i)
	{
		bool cover = false;
		for(int j = 0; j != n; ++j)
		{
			if(j == i) continue;
			if(std::strstr(str[i], str[j]))
			{
				cover = true;
				break;
			}
		}

		if(!cover) add_string(str[i]);
	}

	make_fail();

	value_t invk = value_t(1.0) / alphabet;
//	g[1] = 1.0;

	m.m[0][0] = 1;
	for(int j = 1; j <= used; ++j)
	{
		for(int k = 0; k != alphabet; ++k)
		{
			int u = node[j].ch[k];
			if(node[u].accept) 
			{
			//	f[1] += invk * g[j];
				m.m[1][j] += invk;
			//	ans += invk * g[j];
				m.m[0][j] += invk;
			} else {
			//	f[u] += invk * g[j];
				m.m[u][j] += invk;
			}
		}
	}

	matrix_t ans = matrix_pow(m, len);
	std::printf("%.7lf\n", (double)ans.m[0][1]);
	return 0;
}

