#include "minimum_circle_coverage.h"

#include<iostream>
#include<cstdio>
#include<cstring>
#include<algorithm>
#include<cmath>
using namespace std;

#define MAXIUM_POINT 10000

struct node { double x, y; }b[MAXIUM_POINT];
struct cycle{
    node centre;
    double radius;
};
node O;
double R;
double sqr(double x) { return x * x; }
double dis(node x, node y)
{
    return sqrt(sqr(x.x - y.x) + sqr(x.y - y.y));
}
bool incircle(node x)
{
    if (dis(O, x) <= R) return true;
    return false;
}
node solve(double a, double b, double c, double d, double e, double f)
{
    node res;
    double x,y;
    y = (f * a - c * d) / (b * d - e * a);
    x = (f * b - c * e) / (a * e - b * d);
    res.x = x;
    res.y = y;
    return res;
}
cycle find_minial_cycle(node* node, int node_number)
{
    int i, j, k;
    struct  cycle c1;
    R = 0;
    for (i = 1; i <= node_number; i++)
        if (!incircle(b[i]))
        {
            O.x = b[i].x; O.y = b[i].y; R = 0;
            for (j = 1; j < i; j++)
                if (!incircle(b[j]))
                {
                    O.x = (b[i].x + b[j].x) / 2;
                    O.y = (b[i].y + b[j].y) / 2;
                    R = dis(O, b[i]);
                    for (k = 1; k < j; k++)
                        if (!incircle(b[k]))
                        {
                            O = solve(
                                b[i].x - b[j].x, b[i].y - b[j].y, (sqr(b[j].x) + sqr(b[j].y) - sqr(b[i].x) - sqr(b[i].y)) / 2,
                                b[i].x - b[k].x, b[i].y - b[k].y, (sqr(b[k].x) + sqr(b[k].y) - sqr(b[i].x) - sqr(b[i].y)) / 2
                            );
                            R = dis(b[i], O);
                        }
                }
        }
    printf("%.10lf\n%.10lf %.10lf", R, O.x, O.y);
    c1.centre.x = O.x;
    c1.centre.y = O.y;
    c1.radius = R;
    return c1;
}

int main()
{
    struct cycle c1;
    int n;
    scanf_s("%d", &n);
    int i, j, k;
    for (i = 1; i <= n; i++)
        scanf_s("%lf%lf", &b[i].x, &b[i].y);
    random_shuffle(b + 1, b + n + 1);
    c1 = find_minial_cycle(b, n);
    printf("C1 x %f,y %f,r %f", c1.centre.x, c1.centre.x, c1.radius);

}