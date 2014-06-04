/*
 * Codeforces 439D. Devu and his Brother
 */
#include <vector>
#include <algorithm>
#include <iostream>
#include <iterator>

std::vector<int> A, B;
std::vector<long long> SA, SB;

long long solve(int v)
{
	long long ha = std::lower_bound(A.begin(), A.end(), v) - A.begin();
	long long lb = std::upper_bound(B.begin(), B.end(), v) - B.begin();
	lb = B.size() - lb;
	return ha * v - SA[ha] - lb * v + SB[lb];
}

int main()
{
	int a, b;
	std::cin >> a >> b;
	std::copy_n(std::istream_iterator<int>(std::cin),
		a, std::inserter(A, A.begin()));
	std::copy_n(std::istream_iterator<int>(std::cin),
		b, std::inserter(B, B.begin()));
	std::sort(A.begin(), A.end());
	std::sort(B.begin(), B.end());
	SA.push_back(0);
	for(int i = 1; i <= a; ++i)
		SA.push_back(SA[i - 1] + A[i - 1]);
	SB.push_back(0);
	for(int i = 1; i <= b; ++i)
		SB.push_back(SB[i - 1] + B[b - i]);
	long long ans = ~0LLU >> 1;
	for(int v : A) ans = std::min(ans, solve(v));
	for(int v : B) ans = std::min(ans, solve(v));
	std::cout << ans << std::endl;
	return 0;
}
