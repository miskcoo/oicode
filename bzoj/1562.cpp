/* BZOJ-1562: [NOI2009]变换序列
 *  二分图匹配-匈牙利算法 */
#include <cstdio>
#include <algorithm>

const int MaxN = 10010;
int point[MaxN][2];
int match[MaxN], mark[MaxN];
int mapper[MaxN];
int mark_v;

int augment(int u)
{
	for(int k = 0; k != 2; ++k)
	{
		int v = point[u][k];
		if(mark[v] == mark_v)
			continue;
		mark[v] = mark_v;
		if(match[v] == 0 || augment(match[v]))
		{
			match[v] = u;
			return 1;
		}
	}

	return 0;
}

int main()
{
	int n;
	std::scanf("%d", &n);
	for(int i = 1; i <= n; ++i)
	{
		int d;
		std::scanf("%d", &d);
		int d1 = i + d + n;
		int d2 = i - d + n;
		while(d1 > n) d1 -= n;
		while(d2 > n) d2 -= n;
		if(d2 < d1) std::swap(d1, d2);
		point[i][0] = d1;
		point[i][1] = d2;
	}

	int count = 0;
	for(int i = n; i >= 1; --i)
	{
		++mark_v;
		count += augment(i);
	}

	if(count != n)
	{
		std::printf("No Answer");
	} else {
		for(int i = 1; i <= n; ++i)
			mapper[match[i]] = i;
		for(int i = 1; i != n; ++i)
			std::printf("%d ", mapper[i] - 1);
		std::printf("%d", mapper[n] - 1);
	}
	return 0;
}
