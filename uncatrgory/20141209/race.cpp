#include <cstdio>
#include <algorithm>

const int MaxN = 100010;
int n, m;
int val[MaxN];
int left[MaxN], right[MaxN];
int node[MaxN * 4], lazy[MaxN * 4];

struct list_t
{
	int total;
	int head[MaxN], next[MaxN], point[MaxN];

	void add_edge(int u, int v)
	{
		point[++total] = v;
		next[total] = head[u];
		head[u] = total;
	}
} list_a, list_b;

void reverse(int now, int head, int tail, int a, int b)
{
	int l = now << 1, r = l + 1;
	if(head == a && tail == b)
	{
		lazy[now] ^= 1;
		if(a == b)
		{
			node[now] = lazy[now];
		} else {
			node[now] = node[l] + node[r];
			if(lazy[now]) node[now] = b - a + 1 - node[now];
		}

		return;
	}

	int m = (head + tail) >> 1;
	int flag = true;

	if(b <= m) 
	{
		reverse(l, head, m, a, b);
		flag = false;
	}

	if(m < a)
	{
		reverse(r, m + 1, tail, a, b);
		flag = false;
	}

	if(flag)
	{
		reverse(l, head, m, a, m);
		reverse(r, m + 1, tail, m + 1, b);
	}

	node[now] = node[l] + node[r];
	if(lazy[now]) node[now] = tail - head + 1 - node[now];
}

int ask(int now, int head, int tail, int a, int b, int lz)
{
	if(head == a && tail == b)
	{
		if(lz) return b - a + 1 - node[now];
		return node[now];
	}

	int m = (head + tail) >> 1;
	int l = now << 1, r = l + 1;

	lz ^= lazy[now];
	if(b <= m) return ask(l, head, m, a, b, lz);
	if(m < a) return ask(r, m + 1, tail, a, b, lz);

	return ask(l, head, m, a, m, lz) + ask(r, m + 1, tail, m + 1, b, lz);
}

int main()
{
	std::freopen("race.in", "r", stdin);
	std::freopen("race.out", "w", stdout);
	std::scanf("%d %d", &n, &m);
	for(int i = 0; i != n; ++i)
		std::scanf("%d", val + i);
	std::sort(val, val + n);
	val[n] = ~0u >> 1;

	int count = 0;
	for(int i = 0; i != m; ++i)
	{
		int a, b;
		std::scanf("%d %d", &a, &b);
		if(b <= val[0] || a >= val[n - 1] || a == b)
			continue;

		int l = 0, r = n;
		while(l < r)
		{
			int m = (l + r) >> 1;
			if(val[m] >= a) r = m;
			else l = m + 1;
		}

		left[count] = l + 1;

		l = 0, r = n;
		while(l < r)
		{
			int m = (l + r) >> 1;
			if(val[m] > b) r = m;
			else l = m + 1;
		}

		right[count] = l;
		if(left[count] >= right[count])
			continue;

		list_a.add_edge(left[count], right[count]);
		list_b.add_edge(right[count], left[count]);
		++count;
	}

	long long ans = (long long)n * (n - 1) * (n - 2) / 6;
	for(int i = 1; i <= n; ++i)
	{
		for(int k = list_a.head[i]; k; k = list_a.next[k])
			reverse(1, 1, n, i, list_a.point[k]);
		for(int k = list_b.head[i - 1]; k; k = list_b.next[k])
			reverse(1, 1, n, list_b.point[k], i - 1);

		long long tmp = 0;
		if(i != 1) tmp += ask(1, 1, n, 1, i - 1, 0);
		if(i != n) tmp += n - i - ask(1, 1, n, i + 1, n, 0);
		ans -= tmp * (tmp - 1) / 2;
	}

	std::printf("%lld", ans);
	return 0;
}
