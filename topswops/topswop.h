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

class Topswop;
const string sep = " ";
ostream& operator<<(ostream& os, const Topswop& p);

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
  
  void set_score(int sc) {
    s = sc;
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

  bool local_improve_swap_home() {
    int best_a = -1, best_b = -1, best_score = score();
    for(int i = 0; i < size(); ++i) {
      int a = i, b = p[i] - 1;
      swap(a, b);
      int ss = calc_score();
      if (ss > best_score) {
        best_score = ss; 
        best_a = a;         
        best_b = b;
      }
      swap(a, b);
    }
    score_up_to_date = true;
    if (best_a != -1) {
      swap(best_a, best_b);
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
    int temp = p[i];
    p[i] = p[j];
    p[j] = temp;
    score_up_to_date = false;
  }
  
  void rand_swap() {
    int a = rand() % size();
    int b = rand() % size(); 
    do_swap(a, b);
  }

  void do_swap_home(int i) {
    do_swap(i, p[i] - 1);
  }

  void rand_swap_home() {
    int a = rand() % size();
    do_swap_home(a);
  }

  void do_rev(int i, int j) {
    reverse(p.begin() + i, p.begin() + j);
    score_up_to_date = false;
  }

  void do_rev_home(int i) {
    do_rev(i, p[i] - 1);
  }

  void rand_rev_home() {
    int a = rand() % size();
    do_rev_home(a);
  }

  void rand_rev() {
    int a = rand() % size();
    int b = rand() % size(); 
    do_rev(a, b);
  }

  friend Topswop dfs(const Topswop& root, int cutoff_score);

}; // class Topswop

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

bool operator<(const Topswop& a, const Topswop& b) {
  return a.score() < b.score();
}

bool operator>(const Topswop& a, const Topswop& b) {
  return a.score() > b.score();
}

int operator+(const Topswop& a, const Topswop& b) {
  return a.score() + b.score();
}

///////////////////////////////////////////////////////////

// no depth limit if cutoff_score == -1
Topswop dfs(const Topswop& root, int cutoff_score = -1) {
  const int n = root.size();
  vector<Topswop> stack;
  Topswop best(root);
  stack.push_back(root);
  int count = 0;
  while (!stack.empty()) {
    ++count;
    Topswop ts = stack.back();  // pop the top of
    stack.pop_back();           // the stack
    const int ts_score = ts.score();
    if (ts_score > best.score()) { // save ts if it's the best so far
      best = ts;
    } // if 
    
    // add ts's children to the stack
    if (ts_score < cutoff_score || cutoff_score == -1) {
      for(int i = 1; i < n; ++i) {
        if (ts[i] == i + 1) { // does ts[i] holds its home value?
          Topswop cpy(ts);
          cpy.do_rev(0, cpy[i]);
          cpy.set_score(ts_score + 1);
          stack.push_back(cpy);
        } // if
      } // for
    } // if
  } // while
  return best;
}

Topswop back_dfs(Topswop ts, int stop_after = -1) {
  const int n = ts.size();
  Topswop best(ts);
  int count = 0;
  while (ts[0] != 1) {
    ++count;
    if (stop_after != -1 && count >= stop_after) return best;
    ts.do_rev(0, ts[0]);
//     cout << "(n=" << n << ";" << count << "; stop_after=" 
//          << stop_after << ")" << flush;
    Topswop result = dfs(ts, -1);
    if (result.score() > best.score()) {
      best = result;
      //      cout << "\n! n = " << n << ", score = " << best.score() << endl
      //           << "  " << best
      //           << endl;
    }
  }
  return best;
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
