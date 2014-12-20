/* BZOJ-3781: 小B的询问
 *  分块-莫队算法 */
#include <cmath>
#include <cstdio>
#include <cstring>
#include <algorithm>

const int MaxN = 50001;
struct question_t
{
	int id, l, r;
	int block;
	unsigned ans;
} ask[MaxN];
int seq[MaxN];
unsigned occer[MaxN];
unsigned record[MaxN];

struct sort_functor
{
	int bs;
	sort_functor(int b) : bs(b) {}
	bool operator() (const question_t& a, const question_t& b)
	{
		if(a.block == b.block)
			return a.r < b.r;
		return a.l < b.l;
	}
};

unsigned deflate(int v)
{
	return 1u - (occer[seq[v]]-- << 1u);
}

unsigned inflate(int v)
{
	return 1u + (occer[seq[v]]++ << 1u);
}

int main()
{
	int n, m, k;
	std::scanf("%d %d %d", &n, &m, &k);
	int bs = (int)std::sqrt(double(n)) + 1;
	for(int i = 0; i != n; ++i)
		std::scanf("%d", seq + i);
	for(int i = 0; i != m; ++i)
	{
		ask[i].id = i;
		ask[i].ans = 0;
		std::scanf("%d %d", &ask[i].l, &ask[i].r);
		--ask[i].l, --ask[i].r;
		ask[i].block = ask[i].l / bs;
	}

	std::sort(ask, ask + m, sort_functor(bs));
	for(int i = 0; i != m; )
	{
		int bid = ask[i].block;
		std::memset(occer, 0, sizeof(occer));

		unsigned ans = 0;
		int l = ask[i].l, r = l - 1;
		while(ask[i].block == bid && i != m)
		{
			while(r != ask[i].r)
				ans += inflate(++r);

			while(l < ask[i].l)
				ans += deflate(l++);
			while(l > ask[i].l)
				ans += inflate(--l);

			ask[i].ans = ans;
			++i;
		}
	}

	for(int i = 0; i != m; ++i)
		record[ask[i].id] = ask[i].ans;
	for(int i = 0; i != m; ++i)
		std::printf("%u\n", record[i]);
	return 0;
}
