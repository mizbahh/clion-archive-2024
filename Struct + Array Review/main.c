#include "main.h"

#include <stdio.h>

typedef struct point {
    int x;
    int y;
} point;

void changeme(point p) {
    // struct not pointer so use . not ->
    p.x=10;
    p.y=20;
}

int main() {
    point p1;
    p1.x=100;
    p1.y=100;

    changeme(p1);

    printf("%d %d" , p1.x, p1.y);
}
