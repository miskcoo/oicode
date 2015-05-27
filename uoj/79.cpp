/* UOJ-#79. 一般图最大匹配 
 *   带花树 */
#include <cstdio>
#include <cstring>
#include <algorithm>

const int MaxN = 510, MaxM = 250010;
int n, m, total, mark_v, qhead, qtail;
int head[MaxN], point[MaxM], next[MaxM], que[MaxN];
int fa[MaxN], state[MaxN], pre[MaxN], mate[MaxN], mark[MaxN];

void add_edge(int u, int v)
{
	point[++total] = v;
	next[total] = head[u];
	head[u] = total;
}

int find(int v)
{
	if(fa[v] == v) return v;
	return fa[v] = find(fa[v]);
}

int get_lca(int a, int b)
{
	++mark_v;
	a = find(a), b = find(b);
	for(;; std::swap(a, b))
	{
		if(a)
		{
			if(mark[a] == mark_v) return a;
			mark[a] = mark_v;
			a = mate[a] ? find(pre[mate[a]]) : 0;
		}
	}
}

void blossom(int a, int b, int r)
{
	while(find(a) != r)
	{
		pre[a] = b;
		if(state[mate[a]] == 1)
		{
			que[qtail++] = mate[a];
			state[mate[a]] = 0;
		}

		if(find(a) == a) fa[a] = r;
		if(find(mate[a]) == mate[a])
			fa[mate[a]] = r;

		b = mate[a]; a = pre[b];
	}
}

int match(int u)
{
	std::memset(state, -1, sizeof(state));
	std::memset(pre, 0, sizeof(pre));
	for(int i = 1; i <= n; ++i) fa[i] = i;
	qhead = qtail = 0;
	que[qtail++] = u; state[u] = 0;
	while(qhead != qtail)
	{
		int u = que[qhead++];
		for(int k = head[u]; k; k = next[k])
		{
			int v = point[k];
			if(state[v] == -1)
			{
				state[v] = 1;  // odd vertex
				pre[v] = u;
				if(!mate[v])
				{
					int tmp;
					while(u)
					{
						tmp = mate[u];
						mate[v] = u;
						mate[u] = v;
						v = tmp, u = pre[tmp];
					}

					return 1;
				} else {
					que[qtail++] = mate[v];
					state[mate[v]] = 0;   // even vertex
				}
			} else if(state[v] == 0 && find(u) != find(v)) {
				int lca = get_lca(u, v);
				blossom(u, v, lca);
				blossom(v, u, lca);
			}
		}
	}

	return 0;
}

int main()
{
	std::scanf("%d %d", &n, &m);
	int ans = 0;
	for(int i = 0; i != m; ++i)
	{
		int u, v;
		std::scanf("%d %d", &u, &v);
		add_edge(u, v);
		add_edge(v, u);
		if(!mate[u] && !mate[v])
		{
			++ans;
			mate[u] = v;
			mate[v] = u;
		}
	}

	for(int i = 1; i <= n; ++i)
		if(!mate[i]) ans += match(i);

	std::printf("%d\n", ans);
	for(int i = 1; i <= n; ++i)
		std::printf("%d ", mate[i]);
	return 0;
}

