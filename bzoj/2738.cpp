/* BZOJ-2738: 矩阵乘法
 *   整体二分+二维树状数组 */
#include <cstdio>
#include <algorithm>

const int MaxN = 510, MaxQ = 60010;
struct tree_array
{
	int size;
	int ta[MaxN][MaxN];

	void modify(int x, int y, int w)
	{
		for(++x; x <= size; x += x & -x)
			for(int z = y + 1; z <= size; z += z & -z)
				ta[x][z] += w;
	}

	int ask(int x, int y)
	{
		int w = 0;
		for(; x; x -= x & -x)
			for(int z = y; z; z -= z & -z)
				w += ta[x][z];
		return w;
	}
} ta;

struct data_t
{
	int x, y, v;
	bool operator < (const data_t& r) const
	{
		return v < r.v;
	}
} data[MaxN * MaxN];

struct ques_t
{
	int x1, y1, x2, y2, k, id;
} ques[MaxQ];

int n, m;
int ans[MaxQ];
int map[MaxN * MaxN];

void divide(int l, int r, int a, int b, int va, int vb)
{
	if(a > b) return;
	if(l == r)
	{
		for(int i = a; i <= b; ++i)
			ans[ques[i].id] = map[l];
		return;
	}

	int m = (l + r) >> 1;
	for(int i = va; i <= vb; ++i)
		if(data[i].v <= m) ta.modify(data[i].x, data[i].y, 1);

	int pa = a, pb = b;
	while(pa <= pb)
	{
		int x1 = ques[pa].x1, x2 = ques[pa].x2;
		int y1 = ques[pa].y1, y2 = ques[pa].y2;
		int w = ta.ask(x2, y2) + ta.ask(x1 - 1, y1 - 1)
			- ta.ask(x2, y1 - 1) - ta.ask(x1 - 1, y2);
		if(w >= ques[pa].k)
		{
			++pa;
		} else {
			ques[pa].k -= w;
			std::swap(ques[pa], ques[pb--]);
		}
	}

	for(int i = va; i <= vb; ++i)
		if(data[i].v <= m) ta.modify(data[i].x, data[i].y, -1);

	data_t d; d.v = m;
	int vm = std::upper_bound(data + va, data + vb + 1, d) - data;

	divide(l, m, a, pb, va, vm - 1);
	divide(m + 1, r, pb + 1, b, vm, vb);
}

int main()
{
	int tot = 0;
	std::scanf("%d %d", &n, &m);
	for(int i = 0; i != n; ++i)
		for(int j = 0; j != n; ++j, ++tot)
		{
			data[tot].x = i;
			data[tot].y = j;
			std::scanf("%d", &data[tot].v);
		}

	std::sort(data, data + tot);
	int prev = -1, cnt = 0;
	for(int i = 0; i != tot; ++i)
	{
		if(data[i].v != prev)
		{
			prev = data[i].v;
			map[++cnt] = prev;
		}

		data[i].v = cnt;
	}

	for(int i = 0; i != m; ++i)
	{
		ques[i].id = i;
		std::scanf("%d %d %d %d %d", &ques[i].x1, &ques[i].y1, &ques[i].x2, &ques[i].y2, &ques[i].k);
	}

	ta.size = n;
	divide(1, cnt, 0, m - 1, 0, tot - 1);
	for(int i = 0; i != m; ++i)
		std::printf("%d\n", ans[i]);
	return 0;
}
