/* BZOJ-3286: Fibonacci矩阵
 *   十进制矩阵快速幂 */
#include <cstdio>
#include <cstring>
 
typedef unsigned value_t;
typedef unsigned long long calc_t;
const value_t mod_v = 2012182013;
const int MaxN = 1000010;
 
struct matrix_t
{
    value_t m[3][3];
    matrix_t operator * (const matrix_t& r) const
    {
        matrix_t z;
/*      for(int i = 0; i != 2; ++i)
        {
            for(int j = 0; j != 3; ++j)
            {
                calc_t v = 0;
                for(int k = 0; k != 3; ++k)
                    v += (calc_t)m[i][k] * r.m[k][j];
                z.m[i][j] = v % mod_v;
            }
        } */
 
        z.m[0][0] = ((calc_t)m[0][0] * r.m[0][0] + (calc_t)m[0][1] * r.m[1][0]) % mod_v;
        z.m[0][1] = ((calc_t)m[0][0] * r.m[0][1] + (calc_t)m[0][1] * r.m[1][1]) % mod_v;
        z.m[0][2] = ((calc_t)m[0][0] * r.m[0][2] + (calc_t)m[0][1] * r.m[1][2] + m[0][2]) % mod_v;
 
        z.m[1][0] = ((calc_t)m[1][0] * r.m[0][0] + (calc_t)m[1][1] * r.m[1][0]) % mod_v;
        z.m[1][1] = ((calc_t)m[1][0] * r.m[0][1] + (calc_t)m[1][1] * r.m[1][1]) % mod_v;
        z.m[1][2] = ((calc_t)m[1][0] * r.m[0][2] + (calc_t)m[1][1] * r.m[1][2] + m[1][2]) % mod_v;
 
        for(int i = 0; i != 3; ++i)
            z.m[2][i] = m[2][i];
        return z;
    }
} one;
 
matrix_t matrix_pow(matrix_t x, int p)
{
    matrix_t v = one;
    while(p)
    {
        if(p & 1) v = x * v;
        if(p >> 1) x = x * x;
        p >>= 1;
    }
 
    return v;
}
 
matrix_t matrix_pow(matrix_t x, const char* p)
{
    int len = std::strlen(p);
    matrix_t v = one;
    for(int i = len - 1; i >= 0; --i)
    {
        int pw = p[i] - '0';
        if(pw) v = matrix_pow(x, pw) * v;
        x = matrix_pow(x, 10);
    }
 
    return v;
}
 
value_t get_value()
{
    static char x[MaxN];
    std::scanf("%s", x);
    value_t v = 0;
    for(int i = 0; x[i]; ++i)
        v = ((calc_t)v * 10 + (x[i] - '0')) % mod_v;
    return v;
}
 
void dec(char* x, int v)
{
    int len = std::strlen(x);
    int carry = v;
    for(int i = len - 1; i >= 0 && carry; --i)
    {
        x[i] -= carry;
        if(x[i] < '0') 
        {
            x[i] += 10;
            carry = 1;
        } else carry = 0;
    }
}
 
matrix_t A, B;
char n[MaxN], m[MaxN];
 
int main()
{
    std::scanf("%s %s", n, m);
 
    std::memset(one.m, 0, sizeof(one.m));
    for(int i = 0; i != 3; ++i)
        one.m[i][i] = 1;
 
    std::memset(A.m, 0, sizeof(A.m));
    A.m[1][0] = A.m[2][2] = 1;
    B = A;
 
    A.m[0][1] = get_value();
    A.m[0][0] = get_value();
    A.m[0][2] = get_value();
 
    B.m[0][1] = get_value();
    B.m[0][0] = get_value();
    B.m[0][2] = get_value();
 
    dec(m, 2); dec(n, 1);
    A = matrix_pow(A, m);
    B = A * matrix_pow(B * (B * A), n);
    calc_t ans = (calc_t)B.m[0][0] + B.m[0][1] + B.m[0][2];
    std::printf("%llu\n", ans % mod_v);
    return 0;
}
