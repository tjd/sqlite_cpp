// util.h

#ifndef TOP_SWOP_UTIL_H
#define TOP_SWOP_UTIL_H 201012L

#include "std_lib_facilities_ubuntu_regvec.h"
#include <assert.h>
#include <memory>

string sep = ", ";

template<class T>
ostream& operator<<(ostream& os, const vector<T>& v) {
  const int n = v.size();
  if (n == 0) return os;
  else if (n == 1) os << v[0];
  else {  // v has at least 2 elements
    os << v[0];
    for(int i = 1; i < n; ++i) os << sep << v[i];
  }
  return os;
}

template<class T>
bool operator==(const vector<T>& a, const vector<T>& b) {
  if (a.size() != b.size()) return false;
  for(int i = 0; i < a.size(); ++i) {
    if (a[i] != b[i]) return false;
  }
  return true;
}
 
string int_to_str(int n) {
  if (n == 0) return "0";
  string s;
  while (n > 0) {
    int last_digit = n % 10;
    s += string(1, last_digit + '0');
    n = (n - last_digit) / 10;
  }
  reverse(s.begin(), s.end());
  return s;
}

int str_to_int(const string& s) {
  int pow = 1;
  int total = 0;
  for(int i = s.size()-1; i >= 0; --i) {
    char c = s[i];
    if (isdigit(c)) {
      int digit = c - '0';  // e.g. '7' - '0' == 7
      total += pow * digit;
      pow *= 10;
    }
  }
  return s[0] == '-' ? -total : total;
}

inline void rotate_left(int& a, int& b, int& c) {
  int aa = a, bb = b, cc = c;
  a = bb;
  b = cc;
  c = aa;
}

inline void ping(char c) {cout << c << flush;}

void underline(const string& s, char c = '=') {
  cout << s << '\n';
  cout << string(s.size(), c) << endl;
}

#endif
