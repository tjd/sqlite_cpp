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
#include <assert.h>

using namespace std;

class Topswop {
private:
  vector<int> p;
  int s; // score for p
  bool score_up_to_date;
  vector<int> cpy; // utility vector for score

  int calc_score() {
    assert(cpy.size() == p.size());
    for(int i = 0; i < p.size(); ++i) {
      cpy[i] = p[i];
    }
  //  cpy = p;
    s = 0;
    while (cpy[0] != 1) {
      reverse(cpy.begin(), cpy.begin() + cpy[0]);
      ++s;
    }
    score_up_to_date = true;
  }
  
public:

  // default constuctor (required by map)
 Topswop() 
   : p(0), cpy(0), s(-1), score_up_to_date(false) {}
  
 Topswop(int n) : p(n), cpy(n), s(0), score_up_to_date(true) {
    for(int i = 0; i < n; ++i) {
      p[i] = i + 1;
    }
  }
  
  // copy constructor
  Topswop(const Topswop& other) 
    : p(other.p), cpy(other.cpy), s(other.s), 
      score_up_to_date(other.score_up_to_date) 
  {}

  // assignment operator
  Topswop& operator=(const Topswop& other) {
    if (this != &other) {
      p = other.p;
      cpy = other.cpy; 
      s = other.s;
      score_up_to_date = score_up_to_date;
    }
    return *this;
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

ofstream log("best.log", ios::app);

///////////////////////////////////////////////////////////
//
// Store the best topswops found so far in a map.
//   e.g. best[34] is the best Topswop for size 34
//
///////////////////////////////////////////////////////////

// TODO: put all this in a class
//map<int, Topswop> best;

class Best {
private:
  vector<Topswop> lst;

public:
  Best() : lst(96) {}

  int size() const {return lst.size();}

  Topswop operator[](int n) const {return lst[n-2];}
  Topswop& operator[](int n) {return lst[n-2];}

  int total_score() {
    int result = 0;
    for(int i = 0; i < lst.size(); ++i) {
      result += lst[i].score();
    }
    return result;
  }
  
  void print() {
    for(int i = 0; i < lst.size(); ++i) {
      Topswop p = lst[i];
      cout << "n=" << (i+2) << ";" << p.score() << ": " << p << endl;
    }
    int ts = total_score();
    cout << "Total for score for best = " << ts << endl;
  }
  
  void save() {
    for(int i = 0; i < lst.size(); ++i) {
      Topswop p = lst[i];
      log << (i+2) << " " << p.score() << ": " << p << endl;
    }
  }

} best;

#endif
