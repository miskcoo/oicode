/* BZOJ-3451: Tyvj1953 Normal
 *   树分治+FFT  */
#include <cstdio>
#include <complex>
#include <cmath>
#include <algorithm>

const int MaxL = 16, MaxN = 1 << MaxL;
typedef std::complex<double> complex_t;
complex_t eps[MaxN], inv_eps[MaxN];

void init_eps(int n)
{
	double angle = 2.0 * acos(-1) / n;
	for(int i = 0; i != n; ++i)
	{
		eps[i] = complex_t(cos(angle * i), sin(angle * i));
		inv_eps[i] = conj(eps[i]);
	}
}

void transform(int p, int n, complex_t *x, complex_t *w)
{
	for(int i = 0, j = 0; i != n; ++i)
	{
		if(i > j) std::swap(x[i], x[j]);
		for(int l = n >> 1; (j ^= l) < l; l >>= 1);
	}

	for(int i = 2; i <= n; i <<= 1)
	{
		int m = i >> 1;
		for(int j = 0; j < n; j += i)
		{
			for(int k = 0; k != m; ++k)
			{
				complex_t z = x[j + m + k] * w[p / i * k];
				x[j + m + k] = x[j + k] - z;
				x[j + k] += z;
			}
		}
	}
}

int total, root, root_size, dist_num, fft_len;
int head[MaxN], point[MaxN << 1], next[MaxN << 1];
int size[MaxN], max_size[MaxN], mark[MaxN];
double ans;
complex_t f[MaxN], g[MaxN], t[MaxN];

void add_edge(int u, int v)
{
	point[++total] = v;
	next[total] = head[u];
	head[u] = total;
}

void dfs_size(int u, int fa)
{
	max_size[u] = size[u] = 1;
	for(int k = head[u]; k; k = next[k])
	{
		int v = point[k];
		if(!mark[v] && v != fa)
		{
			dfs_size(v, u);
			size[u] += size[v];
			if(max_size[u] < size[v])
				max_size[u] = size[v];
		}
	}
}

void dfs_root(int u, int fa, int r)
{
	if(max_size[u] < size[r] - size[u])
		max_size[u] = size[r] - size[u];
	if(root_size > max_size[u])
	{
		root = u;
		root_size = max_size[u];
	}
	for(int k = head[u]; k; k = next[k])
	{
		int v = point[k];
		if(!mark[v] && v != fa)
			dfs_root(v, u, r);
	}
}

void dfs_dist(int u, int fa, int d)
{
	f[d] += 1.0;
	for(int k = head[u]; k; k = next[k])
	{
		int v = point[k];
		if(!mark[v] && v != fa)
			dfs_dist(v, u, d + 1);
	}
}

void divide(int now)
{
	root_size = ~0u >> 1;
	dfs_size(now, 0);
	dfs_root(now, 0, now);
	mark[root] = 1;

	int p = 1;
	while(p < size[now]) p <<= 1;
	for(int i = 0; i != p; ++i) g[i] = 1.0;
	for(int k = head[root]; k; k = next[k])
	{
		int v = point[k];
		if(mark[v]) continue;
		dfs_dist(v, root, 1);
		transform(fft_len, p, f, eps);
		for(int i = 0; i != p; ++i)
		{
			t[i] = g[i] * f[i];
			g[i] += f[i];
		}
		transform(fft_len, p, t, inv_eps);
		for(int i = 0; i != p; ++i)
		{
			f[i] = 0;
			ans += int(t[i].real() / p + 0.5) / (i + 1.0);
		}
	}

	for(int k = head[root]; k; k = next[k])
	{
		int v = point[k];
		if(!mark[v]) divide(v);
	}
}

int main()
{
	int n;
	std::scanf("%d", &n);
	for(int i = 1; i != n; ++i)
	{
		int u, v;
		std::scanf("%d %d", &u, &v);
		add_edge(u + 1, v + 1);
		add_edge(v + 1, u + 1);
	}

	for(fft_len = 1; fft_len < n; fft_len <<= 1);
	init_eps(fft_len);

	divide(1);
	std::printf("%.4lf", ans * 2.0 + n);
	return 0;
}
