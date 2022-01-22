#include <bitset>
#include <stdio.h>

#include "ziggurat.h"
#include "zig_random.h"

#define sections 10
typedef std::bitset<64> sign_bit;

int main(int argc, char *argv[]) {
  Xoshiro512StarStarRandom rng = Xoshiro512StarStarRandom();
  double count[sections];
  double generated;
  int samples = 8000000;
  printf("\n\n\n\n\n");
  printf("          Entering Ziggurat distribution generator\n");
  for (int itr = 0; itr < sections; itr++) {
    count[itr] = 0.0;
  }
  Ziggurat zig = Ziggurat();
  for (int itr = 0; itr < samples; itr++) {
    generated = zig.gen_instance(0.0, 1.0);
    if (generated < -4.0) {
      count[0] += 1.;
    } else if (generated < -3.0) {
      count[1] += 1.;
    } else if (generated < -2.0) {
      count[2] += 1.;
    } else if (generated < -1.0) {
      count[3] += 1.;
    } else if (generated < 0.0) {
      count[4] += 1.;
    } else if (generated < 1.0) {
      count[5] += 1.;
    } else if (generated < 2.0) {
      count[6] += 1.;
    } else if (generated < 3.0) {
      count[7] += 1.;
    } else if (generated < 4.0) {
      count[8] += 1.;
    } else {
      count[9] += 1.;
    }
    
  }

  for (int itr = 0; itr < sections; itr++) {
    count[itr] /= samples;
  }
  printf("          Sampled %d points\n", samples);
  for (int itr = 0; itr < sections; itr++) {
    if (itr == 0) {
      printf("           <-4          %F\n", count[itr]);
    } else if (itr == 1) { 
      printf("            -4          %F\n", count[itr]);
    } else if (itr == 2) { 
      printf("            -3          %F\n", count[itr]);
    } else if (itr == 3) { 
      printf("            -2          %F\n", count[itr]);
    } else if (itr == 4) { 
      printf("            -1          %F\n", count[itr]);
    } else if (itr == 5) { 
      printf("            +1          %F\n", count[itr]);
    } else if (itr == 6) { 
      printf("            +2          %F\n", count[itr]);
    } else if (itr == 7) { 
      printf("            +3          %F\n", count[itr]);
    } else if (itr == 8) { 
      printf("            +4          %F\n", count[itr]);
    } else if (itr == 9) { 
      printf("           >+4          %F\n", count[itr]);
    }
  }
  printf("\n\n\n\n\n");
  return 0;
}
