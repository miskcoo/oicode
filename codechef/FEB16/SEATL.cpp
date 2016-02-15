#include <cstdio>
#include <ctime>
#include <algorithm>
#include <unordered_map>
#include <vector>
using namespace std;

const int MaxN = 1000000;
typedef pair<int, int> data_t;
typedef vector<data_t> vec_t;
vec_t vecs[MaxN];
int t, ts, n, m, matrix[MaxN], order[MaxN];

int A(int x, int y) { return matrix[x * m + y]; }

void solve()
{
	ts = clock();
	scanf("%d %d", &n, &m);
	for(int i = 0, size = n * m; i != size; ++i)
		scanf("%d", matrix + i);
	unordered_map<int, int> s;
	for(int i = 0; i != n; ++i)
	{
		s.clear();
		for(int j = 0; j != m; ++j)
			++s[A(i, j)];

		vecs[i].clear();
		for(auto x : s) 
			vecs[i].push_back(make_pair(x.second, x.first));
		sort(vecs[i].begin(), vecs[i].end());
	}

	int ans = 0;
	for(int i = 0; i != m; ++i)
		order[i] = i;
	random_shuffle(order, order + m);

	for(int rid = 0; rid != m; ++rid)
	{
		int i = order[rid];
		s.clear();
		for(int j = 0; j != n; ++j)
			++s[A(j, i)];

		int row_max = 0;
		for(auto x : s) row_max = max(x.second, row_max);
		ans = max(ans, row_max);

		for(int j = 0; j != n; ++j)
		{
			const vec_t& vec = vecs[j];
			if(row_max + vec[vec.size() - 1].first <= ans)
				continue;
			int col_min = ans - row_max + 1;
			auto beg = lower_bound(vec.begin(), vec.end(), make_pair(col_min, 0));
			for(auto end = vec.end(); beg != end; ++beg)
			{
				int key = beg->second;
				int cnt = s[key] + beg->first;
				if(key == A(j, i)) --cnt;
				ans = max(ans, cnt);
			}
		}

		if((clock() - ts) / double(CLOCKS_PER_SEC) > 0.9 / t) break;
	}

	printf("%d\n", ans);
}

int main()
{
	ts = clock();
	scanf("%d", &t);
	for(int i = 0; i != t; ++i) 
		solve();
	return 0;
}
