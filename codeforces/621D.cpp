/* Codeforces 621D. Rat Kwesh and Cheese
 *  brute force, math */
#include <iostream>
#include <cmath>

using namespace std;

const char* expr_str[] = 
	{ "x^y^z", "x^z^y", "(x^y)^z", "(x^z)^y",
	 "y^x^z", "y^z^x", "(y^x)^z", "(y^z)^x",
	 "z^x^y", "z^y^x", "(z^x)^y", "(z^y)^x" };

long double val[12], x, y, z;

int main()
{
	cin >> x >> y >> z;
	val[0] = pow(y, z) * log(x);
	val[1] = pow(z, y) * log(x);
	val[3] = val[2] = y * z * log(x);

	val[4] = pow(x, z) * log(y);
	val[5] = pow(z, x) * log(y);
	val[7] = val[6] = x * z * log(y);

	val[8] = pow(x, y) * log(z);
	val[9] = pow(y, x) * log(z);
	val[11] = val[10] = x * y * log(z);

	int ans_id = 0;
	for(int i = 1; i != 12; ++i)
		if(val[i] > val[ans_id] + 1.0e-9)
			ans_id = i;
	cout << expr_str[ans_id];
	return 0;
}
