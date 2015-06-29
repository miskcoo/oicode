/* SPOJ-ABCDEF ABCDEF
 *   meet in the middle */
#include <cstdio>
#include <unordered_map>

const int MaxN = 100;
int n, s[MaxN];
std::unordered_map<int, int> count;

int main()
{
	std::scanf("%d", &n);
	for(int i = 0; i != n; ++i)
		std::scanf("%d", s + i);

	for(int a = 0; a != n; ++a)
		for(int b = 0; b != n; ++b)
			for(int c = 0; c != n; ++c)
				++count[s[a] * s[b] + s[c]];

	long long ans = 0;
	for(int d = 0; d != n; ++d)
	{
		if(s[d] == 0) continue;
		for(int e = 0; e != n; ++e)
			for(int f = 0; f != n; ++f)
				ans += count[(s[e] + s[f]) * s[d]];
	}

	std::printf("%lld", ans);
	return 0;
}

