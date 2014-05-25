/*
 * BZOJ-1013 [JSOI2008]球形空间产生器sphere
 *  高斯消元
 */
#include <cstdio>
#include <algorithm>

double matrix[11][12];
double input[11][12];
double ans[11];
int index[11];

inline double sqr(double x)
{
	return x * x;
}

void gauss(int N)
{
	for(int i = 0; i != N; ++i)
		index[i] = i;
	for(int i = 0; i != N; ++i)
	{
		int pos = i;
		for(; pos != N; ++pos)
		{
			if(matrix[index[pos]][i] != 0.0)
				break;
		}

		if(pos == N) continue;
		std::swap(index[pos], index[i]);

		for(int j = i + 1; j < N; ++j)
		{
			int ii = index[i], ij = index[j];
			double d = matrix[ij][i] / matrix[ii][i];
			for(int k = N; k >= i; --k)
			{
				matrix[ij][k] = matrix[ii][k] * d 
					- matrix[ij][k];
			}
		}
	}

	for(int i = N - 1; i >= 0; --i)
	{
		double count = matrix[index[i]][N];
		for(int j = N - 1; j > i; --j)
			count -= matrix[index[i]][j] * ans[j];
		ans[i] = count / matrix[index[i]][i];
	}
}

int main()
{
	int N;
	std::scanf("%d", &N);
	for(int i = 0; i <= N; ++i)
	{
		for(int j = 0; j != N; ++j)
			std::scanf("%lf", input[i] + j);
	}

	for(int i = 0; i != N; ++i)
	{
		matrix[i][N] = 0.0;
		for(int j = 0; j != N; ++j)
		{
			matrix[i][j] = 2.0 * (input[i][j] - input[i + 1][j]);
			matrix[i][N] += sqr(input[i][j]) - sqr(input[i + 1][j]);
		}
	}

	gauss(N);
	for(int i = 0; i != N - 1; ++i)
		std::printf("%.3lf ", ans[i]);
	std::printf("%.3lf", ans[N - 1]);
	return 0;
}
