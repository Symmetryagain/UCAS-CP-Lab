
#include <stdbool.h>
#include <stdio.h>

#ifndef SCOPE
#define SCOPE static
#endif

SCOPE void print_int(int d) { printf("%d\n", d); }
SCOPE void print_double(double x) { printf("%g\n", x); }
SCOPE void print_float(float x) { print_double(x); }
SCOPE void print_bool(bool b) {
  if (b)
    puts("true");
  else
    puts("false");
}
SCOPE int get_int(void) {
  int d = 0;
  scanf("%d", &d);
  return d;
}
SCOPE float get_float(void) {
  float x = 0.0f;
  scanf("%f", &x);
  return x;
}
SCOPE double get_double(void) {
  double x = 0.0;
  scanf("%lf", &x);
  return x;
}

const double shihudang = 64.0, songjiang = 63.0;
bool pantograph = true;

int K1806(int command_code, double from, double to) {
  double hukun;

  hukun = shihudang; // km
  if (command_code == 75038) {
    while (hukun > songjiang) {
      hukun = hukun - 0.01;
      if (hukun < from)
        break;
    }
    // lower the pandograph
    pantograph = false;
    while (hukun > songjiang) {
      hukun = hukun - 0.01;
      if (hukun >= to)
        continue;
      else
        break;
    }
    pantograph = true;
    return 0;
  } else
    return command_code;
}

int main() {
  // test whether zero will be filled
  const int Command_code[6] = {7, 5, 0, 3, 8};

  int i = 0, cmd = 0;
  while (i > -1) // true
  {              // test `while` in `while`
    while (i < 8) {
      cmd = cmd * 10 + Command_code[i];
      i = i + 1;
      if (cmd >= 10000)
        break;
    }
    if (cmd >= 10000)
      break;
  }
  if (K1806(cmd, 64.520, 64.120) == 0)
    print_int(0);
  else
    print_int(1);
  return 0;
}
