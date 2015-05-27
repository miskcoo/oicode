/* BZOJ-3265: 志愿者招募加强版 
 *   线性规划，单纯形法 */
#include <cstdio>
  
const float eps = 1.0e-5;
const int MaxN = 10001, MaxM = 1001;
  
int n, m;
float ans;
float A[MaxN][MaxM], C[MaxN], F[MaxM];
  
bool cmp(float x)
{
    return x < eps && x > -eps;
}
  
void pivot(int l, int e)
{
    C[l] /= A[l][e];
    for(int i = 1; i <= m; ++i)
        if(i != e) A[l][i] /= A[l][e];
    A[l][e] = 1.0 / A[l][e];
  
    for(int i = 1; i <= n; ++i)
    {
        if(i != l && !cmp(A[i][e]))
        {
            C[i] -= C[l] * A[i][e];
            for(int j = 1; j <= m; ++j)
                if(j != e) A[i][j] -= A[l][j] * A[i][e];
            A[i][e] = -A[l][e] * A[i][e];
        }
    }
  
    ans += F[e] * C[l];
    for(int i = 1; i <= m; ++i)
        if(i != e) F[i] -= F[e] * A[l][i];
    F[e] = -F[e] * A[l][e];
}
  
void simplex()
{
    for(;;)
    {
        float lim, val = eps;
        int i, j, l, e;
        for(i = 1, e = 0; i <= m; ++i)
        {
            if(F[i] > val)
            {
                e = i;
                val = F[i];
            }
        }
  
        if(e == 0) return;
        for(lim = 1.0e100, i = 1; i <= n; ++i)
        {
            if(A[i][e] > 0 && lim > C[i] / A[i][e])
            {
                l = i;
                lim = C[i] / A[i][e];
            }
        }
  
        if(lim > 1.0e50) break;
        else pivot(l, e);
    }
}
  
int main()
{
    std::scanf("%d %d", &m, &n);
    for(int i = 1; i <= m; ++i)
        std::scanf("%f", F + i);
    for(int i = 1; i <= n; ++i){
        int k, x, y;
        std::scanf("%d", &k);
        while(k --> 0)
        {
            std::scanf("%d %d", &x, &y);
            for(int j = x; j <= y; ++j)
                A[i][j] = 1;
        }
  
        std::scanf("%f", C + i);
    }
  
    simplex();
    printf("%.0f\n", ans);
    return 0;
}
