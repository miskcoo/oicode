/*
 * 427A - Police Recruits 
 */
#include <cstdio>
#include <iostream>

int main()
{
	int N;
	std::scanf("%d", &N);
	long long tot = 0;
	long long ans = 0;
	for(int i = 0; i != N; ++i)
	{
		int v;
		std::scanf("%d", &v);
		if(v == -1)
		{
			if(tot) --tot;
			else ++ans;
		} else tot += v;
	}

	std::cout << ans;
	return 0;
}
