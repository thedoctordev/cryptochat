#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

int euclides(int a, int b, int s, int t) {

    if (a == 0) {
        s = 0;
        t = 1;
        return b;
    }

    int s1, t1;
    int mdc = euclides(b % a, a, s1, t1);

    s = t1 - (b / a) * s1;
    t = s1;

    return mdc;
}

int main() {

    int a, b;
    printf("Digite dois numeros inteiros: \n");
    scanf("%d %d", &a, &b);

    int s, t;
    int mdc = euclides(a, b, s, t);

    printf("mdc(%d, %d) = %d\n", a, b, mdc);
    printf("Coeficientes s e t da combinacao linear: s = %d, t = %d\n", s, t);

    return 0;
}