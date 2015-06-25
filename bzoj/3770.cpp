/* BZOJ-3770: 疯狂的限制 */
#include <cstdio>
#include <vector>
#include <cstring>

const int MaxK = 501, MaxS = 100001;

int limit[MaxK][3], tot[MaxS];
char str[MaxS];
std::vector<int> pos[26];

int main()
{
	std::scanf("%s", str);
	int K, L, R, n = std::strlen(str);
	std::scanf("%d %d %d", &K, &L, &R);
	for(int i = 0; i != K; ++i)
	{
		char ch[2];
		int l, r;
		std::scanf("%s %d %d", ch, &l, &r);
		limit[i][0] = *ch - 'a';
		limit[i][1] = l;
		limit[i][2] = r;
	}

	for(int i = 0; i != 26; ++i)
		pos[i].push_back(-1);

	int now = 0;
	long long ans = 0;
	for(int i = 0; i != n; ++i)
	{
		int x = str[i] - 'a';
		pos[x].push_back(i);
		for(int j = 0; j != K; ++j)
			if(limit[j][1] == 0) ++tot[i];
		if(L <= tot[i] && tot[i] <= R) ++now;

		for(int j = 0; j != K; ++j)
		{
			if(limit[j][0] != x) continue;
			int l = limit[j][1], r = limit[j][2] + 1;
			int size = pos[x].size() - 1;
			if(size >= l && l)
			{
				int p = size - l;
				for(int k = pos[x][p] + 1; k <= pos[x][p + 1]; ++k)
				{
					if(++tot[k] == L) ++now;
					else if(tot[k] == R + 1) --now;
				}
			}

			if(size >= r)
			{
				int p = size - r;
				for(int k = pos[x][p] + 1; k <= pos[x][p + 1]; ++k)
				{
					if(--tot[k] == R) ++now;
					else if(tot[k] == L - 1) --now;
				}
			}
		}

		ans += now;
	}

	std::printf("%lld\n", ans);
	return 0;
}
