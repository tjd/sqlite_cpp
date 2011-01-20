// topswop.h

// See http://azspcs.net/Contest/Cards
// Contest ends: Feb. 12 2011, 4pm

#ifndef TOP_SWOP_H
#define TOP_SWOP_H 201101L

//#define NDEBUG

#include <vector>
#include <iostream>
#include <algorithm>
#include <numeric>
#include <iostream>
#include <fstream>
#include <sstream>
#include <assert.h>
#include <stdlib.h>

using namespace std;

inline double rand_double() {return rand() / RAND_MAX;}

template<class T> string to_string(const T& t) {
  ostringstream os;
  os << t;
  return os.str();
}

/////////////////////////////////////////////////////////////////////////


inline void note(const string& s) {
  cout << '[' << s << ']' << flush;
}

inline void noteln(const string& s) {
  note(s);
  cout << endl;
}

template<class T>
inline void print_var(const string& name, const T& var) {
  cout << name << " = " << var << endl;
}

struct Trace {
  Trace(string fname) : fn(fname) {
    cout << "calling " << fn << "..." << endl;
  }

  ~Trace() {
    cout << "... " << fn << " done" << endl;
  }

private:
  string fn;
}; // Trace

/////////////////////////////////////////////////////////////////////////

class Topswop {
private:
  vector<int> p;
  mutable int s; // score for p
  mutable bool score_up_to_date;
  mutable vector<int> cpy; // utility vector for score

  int calc_score() const {
    assert(cpy.size() == p.size());
    cpy = p;
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

  int score() const {
    if (!score_up_to_date) calc_score();
    return s;
  }
  
  int size() const {return p.size();}
  int operator[](int i) const {
    assert(i >= 0 && i < size());
    return p[i];
  }
  
  bool local_improve_swap() {
    int best_i = -1, best_j = -1, best_score = score();
    for(int i = 0; i < size(); ++i) {
      for(int j = i + 1; j < size(); ++j) {
        swap(p[i], p[j]);
        int ss = calc_score();
        if (ss > best_score) {
          best_score = ss; 
          best_i = i;
          best_j = j;
        }
        swap(p[i], p[j]);
      }
    }
    score_up_to_date = true;
    if (best_i != -1) {
      swap(p[best_i], p[best_j]);
      s = best_score;
      return true;
    }
    return false;
  }

  bool local_improve_reverse() {
    int best_i = -1, best_j = -1, best_score = score();
    for(int i = 0; i < size(); ++i) {
      for(int j = i + 1; j < size(); ++j) {
        reverse(p.begin() + i, p.begin() + j);
        int ss = calc_score();
        if (ss > best_score) {
          best_score = ss; 
          best_i = i;
          best_j = j;
        }
        reverse(p.begin() + i, p.begin() + j);
      }
    }
    score_up_to_date = true;
    if (best_i != -1) {
      reverse(p.begin() + best_i, p.begin() + best_j);
      s = best_score;
      return true;
    }
    return false;
  }

  bool back_improve() {
    bool changed = false;
    for(int i = 1; i < size(); ++i) {
      if (p[i] == i + 1) {
        reverse(p.begin(), p.begin() + i + 1);
        score_up_to_date = false;
        changed = true;
      }
    }
    return changed;
  }

  void randomize() {
    random_shuffle(p.begin(), p.end());
    score_up_to_date = false;
  }
  
  void do_swap(int i, int j) {
    //    Trace t("swap");
    int temp = p[i];
    p[i] = p[j];
    p[j] = temp;
    score_up_to_date = false;
  }
  
  void rand_swap() {
    //    Trace t("rand_swap");
    int a = rand() % size();
    int b = rand() % size(); 
    do_swap(a, b);
  }

  void rev(int i, int j) {
    reverse(p.begin() + i, p.begin() + j);
    score_up_to_date = false;
  }
}; // class Topswop

bool operator<(const Topswop& a, const Topswop& b) {
  return a.score() < b.score();
}

bool operator>(const Topswop& a, const Topswop& b) {
  return a.score() > b.score();
}

int operator+(const Topswop& a, const Topswop& b) {
  return a.score() + b.score();
}


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

///////////////////////////////////////////////////////////

ofstream log("best.log", ios::app);

///////////////////////////////////////////////////////////
//
// Store the best topswops found so far in a map.
//   e.g. best[34] is the best Topswop for size 34
//
///////////////////////////////////////////////////////////

class Best {
private:
  vector<Topswop> lst;

public:
  Best() : lst(96) {}

  int size() const {return lst.size();}

  Topswop operator[](int n) const {
    assert(n >= 2 && n <= 97);
    return lst[n-2];
  }

  Topswop& operator[](int n) {
    assert(n >= 2 && n <= 97);
    return lst[n-2];
  }

  int total_score() const {
    int result = 0;
    for(int i = 0; i < lst.size(); ++i) {
      result += lst[i].score();
    }
    return result;
  }
  
  void print() const {
    for(int i = 0; i < lst.size(); ++i) {
      Topswop p = lst[i];
      cout << "n=" << (i+2) << ";" << p.score() << ": " << p << endl;
    }
    int ts = total_score();
    cout << "Total for score for best = " << ts << endl;
  }
  
  void save() const {
    for(int i = 0; i < lst.size(); ++i) {
      Topswop p = lst[i];
      log << (i+2) << " " << p.score() << ": " << p << endl;
    }
  }

} best;

#endif
