/* Codeforces 396E. On Iteration of One Well-Known Function
 *   数论 */
#include <cstdio>
#include <algorithm>
#include <vector>

const int MaxP = 1000001, MaxN = 78498;
char not_prime[MaxP];
int tot, prime[MaxP], pid[MaxP], fac[MaxP];

typedef std::pair<int, long long> range_t;
typedef std::vector<range_t> vec_t;
vec_t vec[MaxN];
long long k, ans[MaxN];

void linear_sieve(int n)
{
	for(int i = 2; i <= n; ++i)
	{
		if(!not_prime[i])
		{
			pid[i] = tot;
			prime[tot++] = i;
			fac[i] = i;
		}

		for(int j = 0; j != tot; ++j)
		{
			long long t = 1ll * i * prime[j];
			if(t > n) break;
			not_prime[t] = 1;
			fac[t] = prime[j];
			if(i % prime[j] == 0)
				break;
		}
	}
}

void insert_range(int p, range_t r)
{
	if(r.first >= k) 
	{
		ans[p] += r.second - r.first;
		return;
	}
	
	for(auto it = vec[p].begin(); it != vec[p].end(); )
	{
		if(it->second >= r.first && r.first >= it->first)
		{
			r.second = it->second + r.second - r.first;
			r.first = it->first;
			it = vec[p].erase(it);
		} else if(r.second >= it->first && it->first >= r.first) {
			r.second += it->second - it->first;
			it = vec[p].erase(it);
		} else ++it;
	}

	vec[p].push_back(r);
}

int main()
{
	int n;
	std::scanf("%d", &n);
	linear_sieve(1000000);

	for(int i = 0; i != n; ++i)
	{
		int p;
		long long a;
		std::scanf("%d %I64d", &p, &a);
		vec[pid[p]].push_back(range_t(0, a));
	}

	std::scanf("%I64d", &k);

	for(int i = tot - 1; i >= 0; --i)
	{
		if(vec[i].empty()) continue;
		sort(vec[i].begin(), vec[i].end());
//		range_t prev(-1, -1);
		for(range_t r : vec[i])
		{
//			if(prev.second >= r.first)
//				throw "";
//			prev = r;
			if(r.second > k)
			{
				ans[i] += r.second - k;
				r.second = k;
			}

			for(int p = prime[i] - 1; p != 1; p /= fac[p])
				insert_range(pid[fac[p]], range_t(r.first + 1, r.second + 1));
		}
	}

	int cnt = 0;
	for(int i = 0; i != tot; ++i)
		if(ans[i]) ++cnt;
	std::printf("%d\n", cnt);
	for(int i = 0; i != tot; ++i)
		if(ans[i]) std::printf("%d %I64d\n", prime[i], ans[i]);

	return 0;
}
