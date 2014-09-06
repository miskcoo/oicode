/* POJ-2084 Game of Connections
 *  DP, Catalan 数 */
#include <cstdio>
#include <algorithm>

struct large_t
{
	int len;
	long long v[8];
};

const long long limit = 100000000ll;

large_t dp[2];
large_t* catalan(int N)
{
	large_t* f = dp, *g = dp + 1;
	g->len = 1, g->v[0] = 1;
	for(int i = 2; i <= N; ++i)
	{
		f->len = g->len;
		long long carry = 0;
		for(int j = 0; j != g->len; ++j)
		{
			f->v[j] = g->v[j] * (4 * i - 2) + carry;
			if(f->v[j] >= limit)
			{
				carry = f->v[j] / limit;
				f->v[j] %= limit;
			} else carry = 0;
		}

		while(carry)
		{
			f->v[f->len++] = carry % limit;
			carry /= limit;
		}

		for(int j = f->len - 1; j >= 0; --j)
		{
			long long t = carry * limit + f->v[j];
			f->v[j] = t / (i + 1);
			carry = t % (i + 1);
		}

		while(!f->v[f->len - 1])
			--f->len;
		std::swap(f, g);
	}

	return g;
}

int main()
{
	int N;
	while(std::scanf("%d", &N), N != -1)
	{
		if(N <= 1)
		{
			std::printf("1");
		} else {
			large_t* ans = catalan(N);
			std::printf("%lld", ans->v[ans->len - 1]);
			for(int i = ans->len - 2; i >= 0; --i)
				std::printf("%08lld", ans->v[i]);
		}
		std::puts("");
	}
	return 0;
}

