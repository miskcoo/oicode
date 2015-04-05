/* BZOJ-3594: [Scoi2014]方伯伯的玉米田 */
#include <cstdio>
#include <cstring>
#include <algorithm>

const int MaxN = 10010, MaxK = 510, MaxV = 5010;
const short inf = 6000;
int data[MaxN];
short ta1[MaxK][MaxV], ta2[MaxK + MaxV][MaxV];

void modify(short* ta, int n, int x, short v)
{
	for(++x; x <= n; x += x & -x)
		ta[x] = std::max(ta[x], v);
}

short ask(short* ta, int x)
{
	short ans = 0;
	for(++x; x; x -= x & -x)
		ans = std::max(ans, ta[x]);
	return ans;
}

int main()
{
	int n, k;
	std::scanf("%d %d", &n, &k);
	for(int i = 0; i != n; ++i)
		std::scanf("%d", data + i);

	int ans = 0;
	modify(ta1[0], MaxV, 0, inf);
	modify(ta2[0], MaxV, MaxV - 2, inf);
	for(int i = 0; i != n; ++i)
	{
		for(int j = 0; j <= k; ++j)
		{
			short v = std::max(ask(ta1[j], data[i]),
					ask(ta2[j + data[i]], MaxV - data[i] - 2)) + 1;
			modify(ta1[j], MaxV, data[i], v);
			modify(ta2[j + data[i]], MaxV, MaxV - data[i] - 2, v);
			if(ans < v) ans = v;
		}
	}

	std::printf("%d", int(ans - inf));
	return 0;
}
