/*
 * BZOJ-1564 [NOI2009]二叉查找树
 *  区间 DP
 */
#include <cstdio>
#include <algorithm>

struct info
{
	int key;
	int data;
	int frequence;
} data[72];
long long f[72][72][72];
long long sum[72];

bool compare_data(const info& a, const info& b)
{
	return a.data < b.data;
}

bool compare_key(const info& a, const info& b)
{
	return a.key < b.key;
}

int main()
{
	int N, K;
	std::scanf("%d %d", &N, &K);
	for(int i = 1; i <= N; ++i)
		std::scanf("%d", &data[i].data);
	for(int i = 1; i <= N; ++i)
		std::scanf("%d", &data[i].key);
	for(int i = 1; i <= N; ++i)
		std::scanf("%d", &data[i].frequence);
	std::sort(data + 1, data + N + 1, compare_key);
	for(int i = 1; i <= N; ++i)
		data[i].key = i;
	std::sort(data + 1, data + N + 1, compare_data);
	sum[0] = 0;
	for(int i = 1; i <= N; ++i)
		sum[i] = sum[i - 1] + data[i].frequence;
	for(int i = 1; i <= N; ++i)
	{
		for(int w = 1; w <= N; ++w)
		{
			f[i][i][w] = data[i].frequence;
			if(data[i].key < w)
				f[i][i][w] += K;
		}
	}

	for(int len = 1; len <= N; ++len)
	{
		for(int l = 1; len + l - 1 <= N; ++l)
		{
			int r = len + l - 1;
			for(int w = 1; w <= N; ++w)
			{
				long long value = 0x7FFFFFFFFFFFFFFLL;
				for(int u = l; u <= r; ++u)
				{
					value = std::min(value,
						f[l][u - 1][w] + f[u + 1][r][w]
						+ sum[r] - sum[l - 1] + K);
					if(data[u].key >= w)
					{
						int kw = data[u].key;
						value = std::min(value, 
							f[l][u - 1][kw] + f[u + 1][r][kw]
							+ sum[r] - sum[l - 1]);
					}
				}
				f[l][r][w] = value;
			}
		}
	}
	std::printf("%lld", f[1][N][1]);
	return 0;
}
