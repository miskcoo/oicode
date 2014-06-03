/*
 * 427B - Prison Transfer 
 */
#include <cstdio>
#include <iostream>

int main()
{
	int N, T, C;
	std::scanf("%d %d %d", &N, &T, &C);
	int ans = 0;
	int len = 0;
	for(int i = 0; i != N; ++i)
	{
		int v;
		std::scanf("%d", &v);
		if(v <= T)
		{
			++len;
		} else {
			if(len >= C)
				ans += len - C + 1;
			len = 0;
		}
	}
	if(len >= C)
		ans += len - C + 1;
	std::printf("%d", ans);
	return 0;
}
