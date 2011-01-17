// topswop.h

// See http://azspcs.net/Contest/Cards
// Contest ends: Feb. 12 2011, 4pm

#ifndef TOP_SWOP_H
#define TOP_SWOP_H 201101L

#include <vector>
#include <iostream>
#include <algorithm>
#include <map>
#include <iostream>
#include <fstream>

using namespace std;

class Topswop {
 private:
  vector<int> p;
  int s; // score for p
  bool score_up_to_date;

  int calc_score() {
    vector<int> cpy(p);
    s = 0;
    while (cpy[0] != 1) {
      reverse(cpy.begin(), cpy.begin() + cpy[0]);
      ++s;
    }
    score_up_to_date = true;
  }
  
 public:

  //default constuctor required by map
 Topswop() 
   : p(0), s(-1), score_up_to_date(false) {}

  Topswop(int n) : p(n), s(0), score_up_to_date(true) {
    for(int i = 0; i < n; ++i) {
      p[i] = i + 1;
    }
  }
  
  // copy constructor
  Topswop(const Topswop& other) 
    : p(other.p), s(other.s), score_up_to_date(other.score_up_to_date) 
  {}

  // assignment operator
  Topswop& operator=(const Topswop& other) {
    if (this != &other) {
      p = other.p; 
      s = other.s;
      score_up_to_date = score_up_to_date;
    }
  }

  //destructor
  ~Topswop() {}

  int score() {
    if (!score_up_to_date) calc_score();
    return s;
  }
  
  int size() const {return p.size();}
  int operator[](int i) const {return p[i];}
  
  void randomize() {
    random_shuffle(p.begin(), p.end());
    score_up_to_date = false;
  }
  
  void swap(int i, int j) {
    int temp = p[i];
    p[i] = p[j];
    p[j] = temp;
    score_up_to_date = false;
  }
  
  void rev(int i, int j) {
    reverse(p.begin() + i, p.begin() + j);
    score_up_to_date = false;
  }
}; // class Topswop


const string sep = " ";
ostream& operator<<(ostream& os, const Topswop& p) {
  const int n = p.size();
  if (n == 0) return os;
  else if (n == 1) os << p[0];
  else {  // p has at least 2 elements
    os << p[0];
    for(int i = 1; i < n; ++i) os << sep << p[i];
  }
  return os;
}

///////////////////////////////////////////////////////////
//
// Store the best topswops found so far in a map.
//   e.g. best[34] is the best Topswop for size 34
//
///////////////////////////////////////////////////////////
map<int, Topswop> best;

void print_best() {
  for(int n = 2; n < 98; ++n) {
    Topswop p = best[n];
    cout << n << ";" << p.score() << ": " << p << endl;
  }
}

ofstream log("best.log", ios::app);

void save_best() {
  for(int n = 2; n < 98; ++n) {
    Topswop p = best[n];
    log << n << " " << p.score() << ": " << p << endl;
  }
}

#endif
