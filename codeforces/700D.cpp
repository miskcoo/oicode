/* Codeforces 700D. Huffman Coding on Segment
 *   data structures, greedy */
#include <cstdio>
#include <cstring>
#include <vector>
#include <algorithm>
#include <cmath>
#include <queue>
#include <functional>

const int MaxN = 100010;
int block_size, block_size2, A[MaxN];
struct ques_t
{
	int id, l, r;
	bool operator < (const ques_t& z) const
	{
		int b1 = l / block_size, b2 = z.l / block_size;
		if(b1 == b2)
		{
			if(b1 & 1) return r > z.r;
			else return r < z.r;
		} else return b1 < b2;
	}
} Q[MaxN];

std::vector<int> large;
int tmp[MaxN];
int cnt[MaxN], num[MaxN], nonzero[MaxN];
long long ans[MaxN];

void modify(int x, int k)
{
	--num[cnt[x]];
	cnt[x] += k;
	++num[cnt[x]];
}

long long calc()
{
	typedef std::pair<long long, int> data_t;
	std::priority_queue<
		data_t, 
		std::vector<data_t>, 
		std::greater<data_t>
	> T;

	for(int i = 1; i <= block_size2 * 2; ++i)
		tmp[i] = num[i];

	for(int x : large)
	{
		if(cnt[x] > block_size2 * 2)
			T.push(data_t(cnt[x], 1));
	}

	long long ans = 0;

	int prev = 0;
	for(int i = 1; i <= block_size2; ++i)
	{
		if(tmp[i])
		{
			if(prev)
			{
				--tmp[i];
				++tmp[i + prev];
				ans += i + prev;
				prev = 0;
			}

			int t = tmp[i] >> 1;
			tmp[i << 1] += t;
			ans += t * i * 2;

			if(tmp[i] & 1)
				prev = i;

			tmp[i] = 0;
		}
	}

	if(prev) T.push(data_t(prev, 1));

	for(int i = block_size2 + 1; i <= block_size2 * 2; ++i)
		if(tmp[i]) 
		{
			T.push(data_t(i, tmp[i]));
			tmp[i] = 0;
		}

	if(T.empty()) return ans;
	while(T.size() != 1 || T.top().second != 1)
	{
		data_t it = T.top(); T.pop();
		if(it.second == 1)
		{
			data_t it2 = T.top(); T.pop();
			T.push(data_t(it.first + it2.first, 1));
			ans += it.first + it2.first;

			if(--it2.second != 0) 
				T.push(it2);
		} else {
			int tot = it.second >> 1;
			ans += tot * it.first * 2;
			T.push(data_t(it.first << 1, tot));
			it.second &= 1;
			if(it.second) T.push(it);
		}
	}

	return ans;
}

int main()
{
	int n, q, max_v = 0;
	std::scanf("%d", &n);
	block_size2 = 1000;
	block_size = 300;

	for(int i = 1; i <= n; ++i)
	{
		std::scanf("%d", A + i);
		++cnt[A[i]];
		if(A[i] >= max_v) 
			max_v = A[i];
	}

	for(int i = 1; i <= max_v; ++i)
	{
		if(cnt[i] > block_size2 * 2)
			large.push_back(i);
	}

	std::memset(cnt, 0, sizeof(cnt));

	std::scanf("%d", &q);
	for(int i = 0; i != q; ++i)
	{
		Q[i].id = i;
		std::scanf("%d %d", &Q[i].l, &Q[i].r);
	}

	std::sort(Q, Q + q);

	int prev_l = 1, prev_r = 0;
	for(int i = 0; i != q; ++i)
	{
		int l = Q[i].l, r = Q[i].r;

		if(prev_r < r)
		{
			for(int j = prev_r + 1; j <= r; ++j)
				modify(A[j], 1);
		} else {
			for(int j = prev_r; j > r; --j)
				modify(A[j], -1);
		}

		if(prev_l < l)
		{
			for(int j = prev_l; j < l; ++j)
				modify(A[j], -1);
		} else {
			for(int j = l; j < prev_l; ++j)
				modify(A[j], 1);
		}

		ans[Q[i].id] = calc();
		prev_l = l, prev_r = r;
	}

	for(int i = 0; i != q; ++i)
		std::printf("%I64d\n", ans[i]);
	return 0;
}
