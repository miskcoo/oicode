/* BZOJ-2595: [Wc2008]游览计划
 *   最小斯坦纳树 */
#include <cstdio>
#include <cstring>

const int inf = 0x22222222;
const int MaxN = 10, MaxV = MaxN * MaxN * 2;
int n, m, k, qhead, qtail, mark_v;
int mark[MaxN][MaxN], ans[MaxN][MaxN];
int A[MaxN][MaxN], f[MaxN][MaxN][1 << MaxN];
struct state_t
{
	int x, y, s;
	state_t(int x = 0, int y = 0, int s = 0)
		: x(x), y(y), s(s) {}
} pre[MaxN][MaxN][1 << MaxN], que[MaxV];

const int dx[] = {0, 0, 1, -1};
const int dy[] = {1, -1, 0, 0};

void spfa(int s)
{
	while(qhead != qtail)
	{
		state_t u = que[qhead++];
		if(qhead == MaxV) qhead = 0;
		for(int i = 0; i != 4; ++i)
		{
			int nx = u.x + dx[i];
			int ny = u.y + dy[i];
			if(nx < 0 || ny < 0 || nx >= n || ny >= m)
				continue;
			int t = f[u.x][u.y][s] + A[nx][ny];
			if(t < f[nx][ny][s])
			{
				f[nx][ny][s] = t;
				pre[nx][ny][s] = state_t(u.x, u.y, s);
				if(mark[nx][ny] != mark_v)
				{
					mark[nx][ny] = mark_v;
					que[qtail++] = state_t(nx, ny, s);
					if(qtail == MaxV) qtail = 0;
				}
			}
		}

		mark[u.x][u.y] = 0;
	}
}

void dfs(int x, int y, int s)
{
	state_t prev = pre[x][y][s];
	if(!prev.s || f[x][y][s] == inf) return;
	ans[x][y] = 1;
	dfs(prev.x, prev.y, prev.s);
	if(prev.x == x && prev.y == y)
		dfs(prev.x, prev.y, s ^ prev.s);
}

int main()
{
	std::scanf("%d %d", &n, &m);
	std::memset(f, 0x22, sizeof(f));
	for(int i = 0; i != n; ++i)
	{
		for(int j = 0; j != m; ++j)
		{
			std::scanf("%d", A[i] + j);
			if(!A[i][j]) f[i][j][1 << k++] = 0;
		}
	}

	for(int s = 1; s != 1 << k; ++s)
	{
		++mark_v;
		qhead = qtail = 0;
		for(int i = 0; i != n; ++i)
		{
			for(int j = 0; j != m; ++j)
			{
				for(int s0 = s & (s - 1); s0; s0 = (s0 - 1) & s)
				{
					int t = f[i][j][s0] + f[i][j][s ^ s0] - A[i][j];
					if(t < f[i][j][s])
					{
						f[i][j][s] = t;
						pre[i][j][s] = state_t(i, j, s0);
					}
				}

				if(f[i][j][s] != inf) 
				{
					mark[i][j] = mark_v;
					que[qtail++] = state_t(i, j, s);
					if(qtail == MaxV) qtail = 0;
				}
			}
		}

		spfa(s);
	}

	int x, y;
	for(int i = 0; i != n; ++i)
		for(int j = 0; j != m; ++j)
			if(!A[i][j]) x = i, y = j;
	dfs(x, y, (1 << k) - 1);

	std::printf("%d\n", f[x][y][(1 << k) - 1]);
	for(int i = 0; i != n; ++i)
	{
		for(int j = 0; j != m; ++j)
		{
			if(!A[i][j]) std::putchar('x');
			else if(ans[i][j]) std::putchar('o');
			else std::putchar('_');
		}

		std::putchar('\n');
	}
	return 0;
}
