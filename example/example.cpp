
#include <algorithm>
#include <iostream>
#include <string>
#include <type_traits>
#include <vector>


int x = 0, y = 1;

void WAR(int param) {
  x = y;
  y = param;
}