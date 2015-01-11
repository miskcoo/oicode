/* BZOJ-2038: [2009国家集训队]小Z的袜子(hose)
 *  分块-莫队算法 */
#include <cmath>
#include <cstdio>
#include <cstring>
#include <algorithm>

const int MaxN = 50001;
struct question_t
{
	int block;
	int id, l, r;
	long long ans;

	friend bool operator < (const question_t& a, const question_t& b)
	{
		if(a.block == b.block) 
			return a.r < b.r;
		return a.block < b.block;
	}
} ques[MaxN];
int color[MaxN], count[MaxN];
long long ans1[MaxN], ans2[MaxN];

long long gcd(long long a, long long b)
{
	return b ? gcd(b, a % b) : a;
}

int inflate(int x)
{
	return count[color[x]]++;
}

int deflate(int x)
{
	return --count[color[x]];
}

int main()
{
	int n, m;
	std::scanf("%d %d", &n, &m);
	for(int i = 1; i <= n; ++i)
		std::scanf("%d", color + i);

	int bs = (int)std::sqrt(double(n)) + 1;
	for(int i = 0; i != m; ++i)
	{
		int l, r;
		std::scanf("%d %d", &l, &r);
		ques[i].id = i;
		ques[i].block = l / bs;
		ques[i].l = l, ques[i].r = r;
	}

	std::sort(ques, ques + m);

	int l, r;
	long long ans = 0;
	for(int i = 0; i != m; ++i)
	{
		if(!i || ques[i].block != ques[i - 1].block)
		{
			l = ques[i].l, r = l - 1, ans = 0;
			std::memset(count, 0, sizeof(count));
		}

		while(r != ques[i].r)
			ans += inflate(++r);

		while(l < ques[i].l)
			ans -= deflate(l++);
		while(l > ques[i].l)
			ans += inflate(--l);

		ques[i].ans = ans;
	}

	for(int i = 0; i != m; ++i)
	{
		int id = ques[i].id;
		long long len = ques[i].r - ques[i].l;
		long long a = len * (len + 1) / 2;
		long long b = ques[i].ans;
		long long g = gcd(a, b);
		ans2[id] = a / g, ans1[id] = b / g;
		if(b == 0) ans2[id] = 1;
	}

	for(int i = 0; i != m; ++i)
		std::printf("%lld/%lld\n", ans1[i], ans2[i]);

	return 0;
}

