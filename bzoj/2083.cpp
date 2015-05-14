/* BZOJ-2083: [Poi2010]Intelligence test
 *   二分+贪心 */
#include <algorithm>
#include <cstdio>

const int MaxN = 1000010;
struct data_t
{
	int v, pos;
	bool operator < (const data_t& r) const
	{
		if(v == r.v) return pos < r.pos;
		return v < r.v;
	}
} data[MaxN];
int start[MaxN], end[MaxN];

int main()
{
	int n, m;
	std::scanf("%d", &n);
	for(int i = 1; i <= n; ++i)
	{
		data[i].pos = i;
		std::scanf("%d", &data[i].v);
	}

	std::sort(data + 1, data + n + 1);
	for(int i = 1; i <= n; ++i)
	{
		if(data[i].v != data[i - 1].v)
			start[data[i].v] = i;
		end[data[i].v] = i + 1;
	}

	std::scanf("%d", &m);
	while(m --> 0)
	{
		int len, flag = 1;
		std::scanf("%d", &len);
		data_t d;
		d.pos = 1;
		for(int i = 0; i != len; ++i)
		{
			std::scanf("%d", &d.v);
			if(flag)
			{
				int p = std::lower_bound(data + start[d.v], data + end[d.v], d) - data;
				if(data[p].v != d.v) flag = 0;
				else d.pos = data[p].pos + 1;
			}
		}

		std::puts(flag ? "TAK" : "NIE");
	}
	return 0;
}
