/* BZOJ-1483: [HNOI2009]梦幻布丁
 *   链表+启发式合并  */
#include <cstdio>
#include <algorithm>

const int MaxN = 1000100;
int ans = 0;
int head[MaxN], next[MaxN], tail[MaxN];
int color[MaxN], size[MaxN], map[MaxN];

int main()
{
	int n, m;
	std::scanf("%d %d", &n, &m);
	for(int i = 1; i <= n; ++i)
	{
		std::scanf("%d", color + i);
		next[i] = head[color[i]];
		if(!head[color[i]]) tail[color[i]] = i;
		head[color[i]] = i;
		++size[color[i]];
		map[color[i]] = color[i];
		if(color[i] != color[i - 1]) ++ans;
	}

	for(int i = 0; i != m; ++i)
	{
		int op;
		std::scanf("%d", &op);
		if(op == 2) 
		{
			std::printf("%d\n", ans);
		} else {
			int x, y;
			std::scanf("%d %d", &x, &y);
			if(x == y) continue;
			if(size[map[x]] > size[map[y]])
				std::swap(map[x], map[y]);
			x = map[x], y = map[y];
			if(size[x] == 0) continue;
			size[y] += size[x]; size[x] = 0;

			for(int k = head[x]; k; k = next[k])
			{
				if(color[k - 1] == y) --ans;
				if(color[k + 1] == y) --ans;
			}

			for(int k = head[x]; k; k = next[k])
				color[k] = y;
			next[tail[x]] = head[y]; tail[x] = 0;
			head[y] = head[x]; head[x] = 0;
		}
	}
	return 0;
}

