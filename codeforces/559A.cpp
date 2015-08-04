/* Codeforces 559A. Gerald's Hexagon
 *   geometry, math */
#include <cstdio>
#include <algorithm>
#include <iostream>

int A[6];
int main()
{
	for(int i = 0; i != 6; ++i)
		std::cin >> A[i];
	long long ans = 0;
	int now = A[0];
	int a = 1, b = 5, inc = 1;
	while(a < b)
	{
		if(!A[a]) ++a, --inc;
		if(!A[b]) --b, --inc;
		if(a == b) break;
		int x = std::min(A[a], A[b]);
		A[a] -= x, A[b] -= x;
		ans += x * inc + x * (2 * now + (x - 1ll) * inc);
		now += x * inc;
	}

	std::cout << ans;
	return 0;
}
