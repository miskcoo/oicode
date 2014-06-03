/*
 * 437B - The Child and Set 
 */
#include <cstdio>
#include <algorithm>

int ans[100000];
char mark[100001];
int main()
{
	int count = 0;
	int sum, limit;
	std::scanf("%d %d", &sum, &limit);
	for(int i = 30; i >= 0; --i)
	{
		if((1 << i) > sum)
			continue;
		int j = std::min(limit >> i, sum >> i);
		for(int k = 1, c = 1; c <= j && (k << i) <= limit; ++k)
		{
			if(mark[k << i]) continue;
			ans[count++] = k << i;
			mark[k << i] = 1;
			sum -= 1 << i;
			++c;
		}
	}

	if(sum != 0)
	{
		std::printf("-1");
	} else {
		std::printf("%d\n", count);
		for(int i = 0; i != count; ++i)
			std::printf("%d ", ans[i]);
	}
	return 0;
}
