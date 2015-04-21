/* SPOJ-SUBST1 New Distinct Substrings
 *   后缀数组 */
#include <cstdio>
#include <cstring>
#include <algorithm>

const int MaxN = 50010;
char str[MaxN];
int sa[MaxN], rank[MaxN], height[MaxN], satemp[4][MaxN];

bool cmp(int n, int *x, int a, int b, int j)
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
	for(p = 0, j = 1; p < n; m = p, j <<= 1)
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
			x[sa[i]] = cmp(n, y, sa[i - 1], sa[i], j) ? p - 1 : p++;
	}

	for(i = 0; i != n; ++i) rank[sa[i]] = i;
	for(i = k = 0; i != n; height[rank[i++]] = k)
	{
		if(!rank[i]) k = 0;
		else for(k ? --k : 0, j = sa[rank[i] - 1]; str[i + k] == str[j + k]; ++k);
	}
}

int main()
{
	int t;
	std::scanf("%d", &t);
	while(t --> 0)
	{
		std::scanf("%s", str);
		int len = std::strlen(str);
		suffix_array(len, 127);
		long long ans = len * (len + 1ll) / 2;
		for(int i = 1; i != len; ++i)
			ans -= height[i];
		std::printf("%d\n", ans);
	}
	return 0;
}
