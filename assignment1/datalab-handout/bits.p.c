#line 1 "<command-line>"
#include "/usr/include/stdc-predef.h"
#line 14 "bits.c"
#include "btest.h"
#include "/usr/lib/gcc/x86_64-linux-gnu/5/include-fixed/limits.h"
#line 22
team_struct team=
{
#line 28
    "mira9150",

   "Michael Rahn",

   "mira9150",
#line 36
   "",

   ""};
#line 181
int bitAnd(int x, int y) {
  return ~( ~x | ~y);
}
#line 190
int tmax(void) {
  return ~(1 << 31);
}
#line 200
int evenBits(void) {
  return 85+(85<<8)+(85<<16)+(85<<24);
}
#line 211
int getByte(int x, int n) {
    return x>>(n<<3)&255;
}
#line 221
int isEqual(int x, int y) {
  return !(x^y);
}
#line 231
int negate(int x) {
  return ~x+1;
}
#line 242
int divpwr2(int x, int n) {
  return (x>>n)+((!!(((1<<n)+~0)&x))&(x>>31)&1);
}
#line 254
int addOK(int x, int y) {
  return !(!(((x>>31)&1)^((y>>31)&1))&(((( x+y)>>31)&1)^((x>>31)&1)));
}
#line 267
int multFiveEights(int x) {
  int a=(  x<<2)+x;
  return (a>>3) +(( !!(7&a))&(a>>31));
}
#line 278
int isLessOrEqual(int x, int y) {
  return !(x^y) |((  !((!(x>>31))&(y>>31)))&((  x+(~y)+1)>>31))   |((  x>>31) &(!(y>>31)));
}
#line 291
int bitMask(int highbit, int lowbit) {
  int i=  highbit+(~(lowbit+~0));
  return (~(i>>31))&((~(~0<<i<<1))<<lowbit);
}
#line 304
int isPower2(int x) {
  return !(( x&(x+~0)) | !(x<<1));
}
#line 315
int leastBitPos(int x) {
#line 321
  int mx=  x+~0;
  int fM=  x^mx;
  return fM & ~mx;
}
#line 332
int bitParity(int x) {
  int r=  255 ^ x ^( x>>8) ^( x>>16) ^( x>>24);
  int m=  3 &(  r ^( r>>2) ^( r>>4) ^( r>>6));
  return 1 &(  m^(m>>1));
}
#line 345
int logicalNeg(int x) {
  return (( ~x &( x+~0)) >>31) & 1;
}
