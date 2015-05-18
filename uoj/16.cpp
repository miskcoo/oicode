/* UOJ-#16. 【NOIP2014】联合权值 */
#include <cstdio>
#include <cstring>

const int mod_v = 10007;
const int MaxN = 200010, MaxE = 400010;
int total;
int head[MaxN], next[MaxE], point[MaxE];
int weight[MaxN];
int ans_sum = 0, ans_max = 0;

void add_edge(int u, int v)
{
	point[++total] = v;
	next[total] = head[u];
	head[u] = total;
}

void solve_vertex(int u)
{
	int t = 0, deg = 0;
	int max1 = 0, max2 = 0;
	for(int k = head[u]; k; k = next[k])
	{
		++deg;
		int w = weight[point[k]];
		t = (t + w) % mod_v;
		if(w >= max1)
		{
			max2 = max1;
			max1 = w;
		} else if(w >= max2) {
			max2 = w;
		}
	}
		
	int max_v = max1 * max2;
	if(max_v > ans_max)
		ans_max = max_v;
		
	for(int k = head[u]; k; k = next[k])
	{
		int w = weight[point[k]];
		ans_sum = (ans_sum + w * (t - w)) % mod_v;
	}
}

int main()
{
	int n;
	std::scanf("%d", &n);
	for(int i = 1; i != n; ++i)
	{
		int u, v;
		std::scanf("%d %d", &u, &v);
		add_edge(u, v);
		add_edge(v, u);
	}
	
	for(int i = 1; i <= n; ++i)
		std::scanf("%d", weight + i);
	
	for(int u = 1; u <= n; ++u)
		solve_vertex(u);
		
	ans_sum = ((ans_sum % mod_v) + mod_v) % mod_v;
	std::printf("%d %d", ans_max, ans_sum);
	return 0;
}
