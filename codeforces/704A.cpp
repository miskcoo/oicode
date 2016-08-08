/* Codeforces 704A. Thor
 *   brute force, data structures, implementation */
#include <cstdio>
#include <cstring>
#include <queue>

const int MaxN = 300010;
char read[MaxN];
std::deque<int> event[MaxN], total;

int main()
{
	int n, q, msg = 0, ans = 0;
	std::scanf("%d %d", &n, &q);
	while(q --> 0)
	{
		int type;
		std::scanf("%d", &type);
		if(type <= 2)
		{
			int x;
			std::scanf("%d", &x);
			if(type == 1)
			{
				event[x].push_back(msg);
				total.push_back(msg);
				++msg, ++ans;
			} else {
				for(int e : event[x])
				{
					if(!read[e])
					{
						--ans;
						read[e] = 1;
					}
				}

				event[x].clear();
			}
		} else {
			int t;
			std::scanf("%d", &t);
			auto beg = total.begin();
			for(; beg != total.end(); ++beg)
			{
				if(*beg >= t) break;
				if(!read[*beg])
				{
					--ans;
					read[*beg] = 1;
				}
			}

			total.erase(total.begin(), beg);
		}

		std::printf("%d\n", ans);
	}
	return 0;
}
