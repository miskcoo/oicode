/* BZOJ-2865: 字符串识别
 *   后缀数组+DP或堆 */
#include <queue>
#include <cstdio>
#include <cstring>
#include <algorithm>

const int MaxN = 500010;
char str[MaxN];
int satemp[4][MaxN], sa[MaxN], rank[MaxN], height[MaxN];

bool sa_cmp(int n, int *x, int a, int b, int j)
{
	return x[a] == x[b] && a + j < n && b + j < n && x[a + j] == x[b + j];
}

void suffix_array(int n, int m)
{
	int i, j, k, p;
	int *x = satemp[0], *y = satemp[1];
	int *ws = satemp[2], *wv = satemp[3];
	for(i = 0; i != m; ++i) ws[i] = 0;
	for(i = 0; i != n; ++i) ++ws[x[i] = str[i]];
	for(i = 1; i != m; ++i) ws[i] += ws[i - 1];
	for(i = n - 1; i >= 0; --i) sa[--ws[x[i]]] = i;
	for(j = 1, p = 0; p < n; m = p, j <<= 1)
	{
		for(p = 0, i = n - j; i != n; ++i) y[p++] = i;
		for(i = 0; i != n; ++i) if(sa[i] >= j) y[p++] = sa[i] - j;
		for(i = 0; i != n; ++i) wv[i] = x[y[i]];
		for(i = 0; i != m; ++i) ws[i] = 0;
		for(i = 0; i != n; ++i) ++ws[wv[i]];
		for(i = 1; i != m; ++i) ws[i] += ws[i - 1];
		for(i = n - 1; i >= 0; --i) sa[--ws[wv[i]]] = y[i];
		std::swap(x, y); x[sa[0]] = 0;
		for(i = p = 1; i != n; ++i)
			x[sa[i]] = sa_cmp(n, y, sa[i - 1], sa[i], j) ? p - 1 : p++;
	}

	for(i = 0; i != n; ++i) rank[sa[i]] = i;
	for(i = k = 0; i != n; height[rank[i++]] = k)
	{
		if(!rank[i]) k = 0;
		else for(k ? --k : 0, j = sa[rank[i] - 1]; str[i + k] == str[j + k]; ++k);
	}
}

struct data_t
{
	int r, v;
	bool operator < (const data_t& z) const
	{
		return v > z.v;
	}
};

std::priority_queue<data_t> que;

int main()
{
	std::scanf("%s", str);
	int len = std::strlen(str);
	suffix_array(len, 127);
	for(int i = 0; i != len; ++i)
	{
		data_t d;
		int z = rank[i];
		d.v = std::max(height[z], height[z + 1]) + 1;
		d.r = i + d.v - 1;

		if(d.r < len) que.push(d);

		int v = ~0u >> 1;
		while(!que.empty() && que.top().r < i) 
		{
			data_t g = que.top();
			v = std::min(v, g.v + i - g.r);
			que.pop();
		}

		if(v <= len)
		{
			data_t g;
			g.v = v, g.r = i;
			que.push(g);
		}

		std::printf("%d\n", que.top().v);
	}

	return 0;
}
