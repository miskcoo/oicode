/* UOJ-#17. 【NOIP2014】飞扬的小鸟 */ 
#include <cstdio>
#include <cstring>
#include <algorithm>

const int MaxN = 10010;
const int MaxM = 1010;

int dp[2][MaxM];
int up[MaxN], down[MaxN];
struct pipe_t
{
	int l, h;
	bool has_pipe;
} pipe[MaxN];

int game_win = 0;

void check(int& v, int t)
{
	if(t < v) v = t;
}

const int max_int = 20000000;
int solve(int n, int m, int k)
{
	int* f = dp[0], *g = dp[1];
	int pipe_num = 0;
	
	for(int i = 0; i != n; ++i)
	{
		for(int j = 0; j <= m; ++j)
			f[j] = max_int;
		int count = 0;
		for(int h = pipe[i].l + 1; h < pipe[i].h; ++h)
		{
			if(g[h] == max_int) continue;
			
			int top = h + up[i];
			if(top > m) top = m;
			check(f[top], g[h] + 1);
			++count;
		}
		
		for(int h = 1; h <= m; ++h)
		{
			int from = h - up[i];
			if(from <= 0) continue;
			check(f[h], f[from] + 1);
		}

		for(int h = m - up[i]; h <= m; ++h)
			check(f[m], f[h] + 1);
		
		for(int h = pipe[i].l + 1; h < pipe[i].h; ++h)
		{
			if(g[h] == max_int) continue;
			
			if(h - down[i] > 0)
				check(f[h - down[i]], g[h]);
			++count;
		}
		
		if(count == 0)
			return pipe_num;
			
		if(pipe[i].has_pipe) 
			++pipe_num;
			
		std::swap(f, g);
	}
	
	int ans = ~0u >> 1;
	for(int i = 1; i <= m; ++i)
	{
		if(g[i] == max_int) continue;
		if(g[i] < ans) ans = g[i];
	}
	
	game_win = 1;
	return ans;
}

int main()
{
	int n, m, k;
	std::scanf("%d %d %d", &n, &m, &k);
	for(int i = 0; i != n; ++i)
	{
		std::scanf("%d %d", up + i, down + i);
		pipe[i].l = 0;
		pipe[i].h = m + 1;
		pipe[i].has_pipe = false;
	}
	
	for(int i = 0; i != k; ++i)
	{
		int p, l, h;
		std::scanf("%d %d %d", &p, &l, &h);
		pipe[p].l = l;
		pipe[p].h = h;
		pipe[p].has_pipe = true;
	}
	
	int ans = solve(n, m, k);
	if(game_win)
	{
		std::printf("1\n%d", ans);
	} else {
		std::printf("0\n%d", ans);
	}
	return 0;
}

