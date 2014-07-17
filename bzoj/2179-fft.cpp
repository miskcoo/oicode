/*
 * BZOJ-2179 FFT快速傅立叶
 *  FFT
 */
#include <complex>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <algorithm>

const int MaxN = 1 << 17;
std::complex<double> epsilon[MaxN];
std::complex<double> arti_epsilon[MaxN];
std::complex<double> temp[MaxN];

void init_epsilon(int n)
{
	const double pi = 3.14159265358979323846;
	for(int i = 0; i != n; ++i)
	{
		epsilon[i] = std::complex<double>(
			std::cos(2.0 * pi * i / n), 
			std::sin(2.0 * pi * i / n)); 
		arti_epsilon[i] = std::complex<double>(
			epsilon[i].real(), 
			-epsilon[i].imag());
	}
}

void dft(int n, std::complex<double>* buffer,
	int offset, int step, std::complex<double>* epsilon)
{
	if(n == 1) return;
	int m = n >> 1;
	dft(m, buffer, offset, step << 1, epsilon);
	dft(m, buffer, offset + step, step << 1, epsilon);
	for(int k = 0; k != m; ++k)
	{
		int pos = 2 * step * k;
		temp[k] = buffer[pos + offset] 
			+ epsilon[k * step] 
			* buffer[pos + offset + step];

		temp[k + m] = buffer[pos + offset] 
			- epsilon[k * step] 
			* buffer[pos + offset + step];
	}

	for(int i = 0; i != n; ++i)
		buffer[i * step + offset] = temp[i];
}

void read_number(int N, std::complex<double>* buffer)
{
	for(int i = N - 1; i >= 0; --i)
	{
		char ch;
		while((ch = std::getchar()) && (ch < '0' || ch > '9'));
		buffer[i] = ch - '0';
	}
}

std::complex<double> num_a[MaxN], num_b[MaxN];
int ans[MaxN + 2];
int main()
{
	int N;
	std::scanf("%d", &N);
	read_number(N, num_a);
	read_number(N, num_b);

	int p = 1;
	while(p < N) p <<= 1;
	N = p << 1;
	init_epsilon(N);
	dft(N, num_a, 0, 1, epsilon);
	dft(N, num_b, 0, 1, epsilon);
	for(int i = 0; i != N; ++i)
		num_a[i] *= num_b[i];

	dft(N, num_a, 0, 1, arti_epsilon);
	for(int i = 0; i != N; ++i)
		ans[i] = int(num_a[i].real() / N + 1.0e-9);
	for(int i = 0; i <= N; ++i)
	{
		ans[i + 1] += ans[i] / 10;
		ans[i] %= 10;
	}

	int len = N + 1;
	while(!ans[len] && len) --len;
	for(int i = len; i >= 0; --i)
		std::printf("%c", char(ans[i] + '0'));

	return 0;
}
