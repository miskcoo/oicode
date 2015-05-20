/* BZOJ-1209: [HNOI2004]最佳包裹
 *   三维凸包 */
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <algorithm>

const int MaxN = 110;
const double eps = 1.0e-9;
double cmp(double a, double b = 0) { return a - b < eps && a - b > -eps; }
double abs(double x) { return x < 0 ? -x : x; }
double sqr(double x) { return x * x; }
struct point_t
{
	double x, y, z;
	point_t(double x = 0, double y = 0, double z = 0) : x(x), y(y), z(z) {}
	point_t operator - (const point_t& r) const
	{
		return point_t(x - r.x, y - r.y, z - r.z);
	}

	point_t operator + (const point_t& r) const
	{
		return point_t(x + r.x, y + r.y, z + r.z);
	}

	double operator * (const point_t& r) const
	{
		return x * r.x + y * r.y + z * r.z;
	}

	double len2() const
	{
		return sqr(x) + sqr(y) + sqr(z);
	}

	double len() const
	{
		return sqrt(len2());
	}

	void noise()
	{
		x += (std::rand() % 1000 - 500) * 1.0e-12;
		y += (std::rand() % 1000 - 500) * 1.0e-12;
		z += (std::rand() % 1000 - 500) * 1.0e-12;
	}
} pt[MaxN];

point_t cross(const point_t& a, const point_t& b)
{
	return point_t(
		a.y * b.z - a.z * b.y, 
		b.x * a.z - b.z * a.x,
		a.x * b.y - a.y * b.x);
}

point_t cross(const point_t& a, const point_t& b, const point_t& c)
{
	return cross(b - a, c - a);
}

double mixed(const point_t& a, const point_t& b, const point_t& c)
{
	return cross(a, b) * c;
}

double mixed(const point_t& p, const point_t& a, const point_t& b, const point_t& c)
{
	return mixed(a - p, b - p, c - p);
}

double dist2(const point_t& a, const point_t& b)
{
	return (a - b).len2();
}

double dist(const point_t& a, const point_t& b)
{
	return (a - b).len();
}

struct face_t
{
	int ok;
	int a, b, c;
};

double area(const face_t& face)
{
	return 0.5 * cross(pt[face.a], pt[face.b], pt[face.c]).len();
}

double dir(const point_t& p, const face_t& face)
{
	return mixed(
		pt[face.b] - pt[face.a],
		pt[face.c] - pt[face.a],
		p - pt[face.a]);
}

int face_tot;
face_t face[MaxN * 10];
int near[MaxN][MaxN];

void update_face(int, int);

void inflate(int p, int a, int b)
{
	int f = near[a][b];
	if(face[f].ok)
	{
		if(dir(pt[p], face[f]) > eps)
		{
			update_face(p, f);
		} else {
			face_t fac;
			fac.ok = 1;
			fac.a = b, fac.b = a, fac.c = p;
			near[b][a] = near[a][p] = near[p][b] = face_tot;
			face[face_tot++] = fac;
		}
	}
}

void update_face(int p, int f)
{
	face[f].ok = 0;
	inflate(p, face[f].b, face[f].a);
	inflate(p, face[f].c, face[f].b);
	inflate(p, face[f].a, face[f].c);
}

void convex_hull(int n)
{
	if(n < 4) return;

	// find line
	bool found = false;
	for(int i = 1; i != n; ++i)
	{
		if(dist2(pt[0], pt[i]) > eps)
		{
			found = true;
			std::swap(pt[1], pt[i]);
			break;
		}
	}

	if(!found) return;

	// find face
	found = false;
	for(int i = 2; i != n; ++i)
	{
		if(cross(pt[0], pt[1], pt[i]).len2() > eps)
		{
			found = true;
			std::swap(pt[2], pt[i]);
			break;
		}
	}

	if(!found) return;

	// find simplex
	found = false;
	for(int i = 3; i != n; ++i)
	{
		if(!cmp(mixed(pt[0], pt[1], pt[2], pt[i])))
		{
			found = true;
			std::swap(pt[3], pt[i]);
			break;
		}
	}

	if(!found) 
	{
		// decayed
		pt[3].noise();
	}

	// construct initial simplex
	face_t fac;
	for(int i = 0; i != 4; ++i)
	{
		fac.a = (i + 1) & 3;
		fac.b = (i + 2) & 3;
		fac.c = (i + 3) & 3;
		if(dir(pt[i], fac) > eps)
			std::swap(fac.b, fac.c);
		fac.ok = 1;
		near[fac.a][fac.b] = face_tot;
		near[fac.b][fac.c] = face_tot;
		near[fac.c][fac.a] = face_tot;
		face[face_tot++] = fac;
	}

	// update convex hull
	for(int i = 4; i != n; ++i)
	{
		for(int j = 0; j != face_tot; ++j)
		{
			if(face[j].ok && dir(pt[i], face[j]) > eps)
			{
				update_face(i, j);
				break;
			}
		}
	}

	// remove deleted face
	int tot = face_tot;
	face_tot = 0;
	for(int j = 0; j != tot; ++j)
	{
		if(face[j].ok)
			face[face_tot++] = face[j];
	}
}

int main()
{
	int n;
	std::scanf("%d", &n);
	for(int i = 0; i != n; ++i)
		std::scanf("%lf %lf %lf", &pt[i].x, &pt[i].y, &pt[i].z);

	convex_hull(n);
	double ans = 0;
	for(int i = 0; i != face_tot; ++i)
		ans += area(face[i]);
	std::printf("%.6lf\n", ans);
	return 0;
}
