/* BZOJ-1067: [SCOI2007]降雨量
 *  线段树 */
#include <cstdio>
#include <cstring>
#include <algorithm>

const int MaxN = 50010;
int n, m;
int node[MaxN << 2], year[MaxN], value[MaxN];

void modify(int now, int head, int tail, int pos, int v)
{
	if(head == tail)
	{
		node[now] = v;
		return;
	}

	int m = (head + tail) >> 1;
	if(pos <= m) modify(now << 1, head, m, pos, v);
	if(m < pos) modify((now << 1) + 1, m + 1, tail, pos, v);

	node[now] = std::max(node[now << 1], node[(now << 1) + 1]);
}

int ask(int now, int head, int tail, int a, int b)
{
	if(head == a && tail == b)
		return node[now];

	int m = (head + tail) >> 1;
	if(b <= m) return ask(now << 1, head, m, a, b);
	if(m < a) return ask((now << 1) + 1, m + 1, tail, a, b);

	return std::max(ask(now << 1, head, m, a, m), 
			ask((now << 1) + 1, m + 1, tail, m + 1, b));
}

int main()
{
	std::scanf("%d", &n);
	for(int i = 1; i <= n; ++i)
	{
		int y, r;
		std::scanf("%d %d", &y, &r);
		year[i] = y, value[i] = r;
		modify(1, 1, n, i, r);
	}

	std::scanf("%d", &m);
	for(int i = 0; i != m; ++i)
	{
		int y, x;
		std::scanf("%d %d", &y, &x);
		int r = std::lower_bound(year + 1, year + n + 1, x) - year;
		int l = std::lower_bound(year + 1, year + n + 1, y) - year;
		if(l == n + 1) l = n;
		if(r == n + 1) r = n;

		bool is_tight = true;
		if(r - l != x - y || year[l] != y || year[r] != x)
			is_tight = false;

		if(is_tight)
		{
			int max_v = 0;
			if(l + 1 <= r - 1)
				max_v = ask(1, 1, n, l + 1, r - 1);
			if(value[l] >= value[r] && value[r] > max_v)
				std::puts("true");
			else std::puts("false");
		} else {
			bool is_true = true;
			if(year[l] == y && year[r] == x && value[l] < value[r])
				is_true = false;
			int pl = year[l] == y ? l + 1 : l;
			int pr = r - 1;
			int max_v = 0;
			if(pl <= pr) max_v = ask(1, 1, n, pl, pr);
			if(year[r] == x && value[r] <= max_v) 
				is_true = false;
			if(year[l] == y && value[l] <= max_v)
				is_true = false;

			if(is_true) std::puts("maybe");
			else std::puts("false");
		}
	}
	return 0;
}
