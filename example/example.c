
int x = 0, y = 1;

void WAR(int param) {
  x = y;
  y = param;
}

void NoWAR(int param) {
  y = param;
  x = y;
}

int a, x, w, b, z;

int IN();

void WARFromPaper() {
   if (a > 0) {
      x = w;
      w = 5;
      b = 2;
   } else {
      int i = IN();
      if (i>1) {
        x = 6;
        y = 7;
        b = 9;
      } else {
        x = 7;
        z = 8;
        //w = 5;
      }
  }
}