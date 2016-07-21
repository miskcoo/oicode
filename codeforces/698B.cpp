/* Codeforces 698B. Fix a Tree
 *  constructive algorithms, dfs and similar, dsu, trees */
#include <cstdio>

const int MaxN = 200010;
int n, fa[MaxN], deg[MaxN], que[MaxN], mark[MaxN];

void mark_circle(int u)
{
	int now = fa[u];
	while(now != u)
	{
		mark[now] = 1;
		now = fa[now];
	}

	mark[u] = 1;
}

int main()
{
	std::scanf("%d", &n);
	int loop = 0;
	for(int i = 1; i <= n; ++i)
	{
		std::scanf("%d", fa + i);
		++deg[fa[i]];
		if(fa[i] == i) loop = i;
	}

	int qh = 0, qt = 0;
	for(int i = 1; i <= n; ++i)
		if(deg[i] == 0) que[qt++] = i;

	while(qh != qt)
	{
		int u = que[qh++];
		if(--deg[fa[u]] == 0)
			que[qt++] = fa[u];
	}

	qt = qh = 0;
	int ans = 0;
	for(int i = 1; i <= n; ++i)
	{
		if(deg[i] && !mark[i])
		{
			que[qt++] = i;
			mark_circle(i);
			++ans;
		}
	}

	if(!loop)
	{
		for(int i = 0; i != qt; ++i)
			fa[que[i]] = que[0];
	} else {
		for(int i = 0; i != qt; ++i)
			fa[que[i]] = loop;
	}

	if(loop)
		std::printf("%d\n", ans - 1);
	else std::printf("%d\n", ans);
	for(int i = 1; i <= n; ++i)
		std::printf("%d ", fa[i]);
	return 0;
}
