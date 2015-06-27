/* Codeforces 555B. Case of Fugitive
 *   Sortings */
#include <cstdio>
#include <iostream>
#include <set>
#include <algorithm>

const int MaxN = 200010;
int n, m;
long long L[MaxN], R[MaxN];

struct range_t
{
	int id;
	long long l, r;
	bool operator < (const range_t& z) const
	{
		if(l <= z.l && z.r <= r)
			return false;
		else if(z.l <= l && r <= z.r)
			return true;
		return l < z.l;
	}
} range[MaxN];

struct bridge_t
{
	int id;
	long long len;
	bool operator < (const bridge_t& r) const
	{
		return len < r.len;
	}
};

std::multiset<bridge_t> s;

int ans[MaxN];

int main()
{
	std::ios::sync_with_stdio(false);
	std::cin >> n >> m;
	if(n - 1 > m) 
	{
		std::cout << "No\n";
		return 0;
	}

	for(int i = 0; i != n; ++i)
		std::cin >> L[i] >> R[i];
	for(int i = 0; i + 1 < n; ++i)
	{
		range[i].id = i;
		range[i].l = L[i + 1] - R[i];
		range[i].r = R[i + 1] - L[i];
	}

	std::sort(range, range + n - 1);

	for(int i = 0; i != m; ++i)
	{
		bridge_t A;
		A.id = i + 1;
		std::cin >> A.len;
		s.insert(A);
	}

	for(int i = 0; i + 1 < n; ++i)
	{
		bridge_t r;
		r.len = range[i].l;
		auto pos = s.lower_bound(r);
		if(pos == s.end() || range[i].r < pos->len)
		{
			std::cout << "No\n";
			return 0;
		}

		ans[range[i].id] = pos->id;
		s.erase(pos);
	}

	std::cout << "Yes\n";
	for(int i = 0; i + 1 < n; ++i)
		std::cout << ans[i] << " ";
	std::cout << std::endl;
	return 0;
}
