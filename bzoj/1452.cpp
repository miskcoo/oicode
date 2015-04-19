/* BZOJ-1452: [JSOI2009]Count
 *  二维树状数组 */
#include <cstdio>

const int MaxC = 101, MaxN = 301;

int n, m, data[MaxN][MaxN];
class tree_array
{
	int ta[MaxN][MaxN];
public:
	void modify(int x, int y, int v)
	{
		for(; x <= n; x += x & -x)
			for(int z = y; z <= m; z += z & -z)
				ta[x][z] += v;
	}

	int ask(int x, int y)
	{
		int v = 0;
		for(; x; x -= x & -x)
			for(int z = y; z; z -= z & -z)
				v += ta[x][z];
		return v;
	}
} ta[MaxC];

int main()
{
	std::scanf("%d %d", &n, &m);
	for(int x = 1; x <= n; ++x)
	{
		for(int y = 1; y <= m; ++y)
		{
			int v;
			std::scanf("%d", &v);
			ta[v].modify(x, y, 1);
			data[x][y] = v;
		}
	}

	int q;
	std::scanf("%d", &q);
	for(int i = 0; i != q; ++i)
	{
		int op;
		std::scanf("%d", &op);
		if(op == 1)
		{
			int x, y, c;
			std::scanf("%d %d %d", &x, &y, &c);
			ta[data[x][y]].modify(x, y, -1);
			ta[data[x][y] = c].modify(x, y, 1);
		} else {
			int x1, y1, x2, y2, c;
			std::scanf("%d %d %d %d %d", &x1, &x2, &y1, &y2, &c);
			int ans = ta[c].ask(x2, y2) + ta[c].ask(x1 - 1, y1 - 1)
					- ta[c].ask(x1 - 1, y2) - ta[c].ask(x2, y1 - 1);
			std::printf("%d\n", ans);
		}
	}
	return 0;
}
