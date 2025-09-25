
int x = 0, y = 1;
int *p = &x;

// This is a problem because this code is non-idempotent.
// If this code runs twice, we might have an issue.
//
// For example if x = 0, y = 1 initially, then after the first execution
// x = 1, y = 3 then after another execution
// x = 3, y = 3
void WAR() {
  x = y;
  y = 3;
}

// In this case there's no problem, this code is repeatable.
void NoWAR() {
  y = 3;
  x = y;
}

void WAR2() {
  x = x + 1;
}

// deeply unsound.
void WARBroken() {
  y = x;
  *p = x;
}

void false_positive() {
  x = 10;
  y = x;
  x = 20;
}

int a, w, b, z;

int RIO();

void WARFromPaper() {
   if (a > 0) {
      x = w;
      w = 5;
      b = 2;
   } else {
      int i = RIO();
      if (i>1) {
        x = 6;
        y = 7;
        b = 9;
      } else {
        x = 7;
        z = 8;
        w = 5;
      }
  }
}