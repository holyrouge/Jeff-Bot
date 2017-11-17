#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

//given 2 RGB arrays, see if they're close enough to match
bool compareColors(int* c1, int* c2) {
    int color_threshold = 25;
    return sqrt( pow(c1[0] - c2[0], 2) + pow(c1[1] - c2[1], 2) + pow(c1[2] - c2[2], 2) ) < color_threshold;
}

void setup() {
    //testing
    int color1[3] = {255, 0, 0};
    int color2[3] = {231, 0, 0};
    printf("%d\n", compareColors(color1, color2));
}

int main() {
  setup();
  return 0;
}
