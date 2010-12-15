// topswop.h

// See http://azspcs.net/Contest/Cards
// Contest ends: Feb. 12 2011, 4pm


#ifndef TOP_SWOP_H
#define TOP_SWOP_H 201012L

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

///////////////////////////////////////////////////////////////////////

typedef vector<int> perm;

// file name for file storing info about n
string fname(int n) {
  return "best_so_far/topswop_num_" + int_to_str(n) + ".txt";
}

int get_current_score(int n) {
  const string name = fname(n);
  ifstream fin(name.c_str());
  if (!fin.good()) error("Unable to open " + name + " for reading");
  int score;
  fin >> score;
  return score;
}

perm* get_current_perm(int n) {
  const string name = fname(n);
  ifstream fin(name.c_str());
  if (!fin.good()) error("Unable to open " + name + " for reading");

  // get the score
  int score;
  fin >> score;

  //cout << "score = " << score << endl;

  // get the permutation: int csv list
  perm* p = new perm(n);
  string s;
  getline(fin, s);
  getline(fin, s);

  //cout << "s = \"" << s << "\"\n";

  // replace every ',' with a space
  for(int i = 0; i < s.size(); ++i) {
    if (s[i] == ',') s[i] = ' ';
  }

  istringstream iss(s);
  int num = -1;
  int i = 0;
  while (iss >> num) {
    (*p)[i] = num;
    ++i;
  }

  return p;
}

bool set_current_score(int n, int score, const vector<int>& v) {
  const int curr_score = get_current_score(n);
  if (score > curr_score) {
    const string name = fname(n);
    ofstream fout(name.c_str());
    if (!fout.good()) error("Unable to open " + name + " for writing");
    cout << "Setting " << name << ": " << score << "\n" << v;
    fout << score << '\n' << v;
    return true;
  }
  return false;
}


// check if this is better the currently saved solution for n
void check_score(int n, int new_score, const vector<int>& new_perm) {
  int curr = get_current_score(n);
  if (new_score > curr) {
    set_current_score(n, new_score, new_perm);
  }
}

void fix_shuffle(vector<int>& v) {
  if (v[0] == 1) {
    for(int i = 1; i < v.size(); ++i) {
      if (v[i] != i) {
        swap(v[0], v[i]);
        return;
      }
    }
  }
}

inline void shuffle(vector<int>& v) {
  random_shuffle(v.begin(), v.end());
  fix_shuffle(v);
}

inline void shuffle_no_fix(vector<int>& v) {
  random_shuffle(v.begin(), v.end());
}


// Note: Optimal values for 2, 3, 5, 7, 11, 13 have been found, so
// need to do any computation on those.
const int max_optimal_val = 13;
const int n_vals_size = 25;
const int n_vals[n_vals_size] = { 
  2,  3,  5,  7, 11, 
  13, 17, 19, 23, 29, 
  31, 37, 41, 43, 47, 
  53, 59, 61, 67, 71,
  73, 79, 83, 89, 97
};

// returns a pointer to a new vector<int> initialized to 1, 2, ..., n
vector<int>* range(int n) {
  vector<int>* v = new vector<int>(n);
  int i = 1;
  for(perm::iterator p = v->begin(); p < v->end(); ++p) {
    *p = i;
    ++i;
  }
  return v;
}

bool is_perm(vector<int> v) {
  sort(v.begin(), v.end());
  for(int i = 0; i < v.size(); ++i) {
    if (v[i] != i + 1)
      return false;
  }
  return true;
}

void init_deck(vector<int>& deck) {
  srand(time(NULL));
  for(int i = 0; i < deck.size(); ++i) {
    deck[i] = i+1;
  }
}

// Would simplify most calling code if deck were passed by value.
// The STL reverse is pretty good: a by-hand reverse function was
// slightly slower.
int do_all_top_swops(vector<int>& deck) {
  int count = 0;
  while (deck[0] != 1) {
    reverse(deck.begin(), deck.begin() + deck[0]);  // do 1 top-swop
    ++count;
  }
  return count;
}

int do_all_top_swops_copy(vector<int> deck) {
  int count = 0;
  while (deck[0] != 1) {
    reverse(deck.begin(), deck.begin() + deck[0]);  // do 1 top-swop
    ++count;
  }
  return count;
}

void perturb_2rot(int n) {
  // read in the score and vector for n
  const string name = fname(n);
  ifstream fin(name.c_str());
  int curr;
  fin >> curr;
  vector<int> v;
  string x;
  while (fin >> x) {
    if (x[x.size()-1] == ',') 
      x = x.substr(0, x.size()-1);
    v.push_back(str_to_int(x));
  }

  // swap all distinct pairs of v to see if a higher score results
  int best_so_far = curr;
  vector<int> best(v);
  for(int i = 0; i < n; ++i) {
    for(int j = i + 1; j < n; ++j) {
      swap(v[i], v[j]);
      vector<int> vc(v);
      int s = do_all_top_swops(vc);
      if (s > best_so_far) {
        best_so_far = s;
        copy(v.begin(), v.end(), best.begin());
      }
      swap(v[i], v[j]);
    }
  }

  // save score if it's an improvement
  if (best_so_far > curr) {
    int diff = best_so_far - curr;
    cout << "\nn = " << n << ", score = " << best_so_far
         << " (" << diff << " improvement)" << endl
         << best << endl;    
    fin.close();
    set_current_score(n, best_so_far, best);
  }
}


inline void rot3(int& a, int& b, int& c) {
  int temp = a;
  a = b;
  b = c;
  c = temp;
}

void perturb_3rot(int n) {
  // read in the score and vector for n
  const string name = fname(n);
  ifstream fin(name.c_str());
  int curr;
  fin >> curr;
  vector<int> v;
  string x;
  while (fin >> x) {
    if (x[x.size()-1] == ',') 
      x = x.substr(0, x.size()-1);
    v.push_back(str_to_int(x));
  }
  

  // rotate all distinct triples of v to see if a higher score results
  int best_so_far = curr;
  vector<int> best(v);
  for(int i = 0; i < n; ++i) {
    for(int j = i + 1; j < n; ++j) {
      for(int k = j + 1; k < n; ++k) {
        rot3(v[i], v[j], v[k]);
        vector<int> vc(v);
        int s = do_all_top_swops(vc);
        if (s > best_so_far) {
          best_so_far = s;
          copy(v.begin(), v.end(), best.begin());
        }
        
        rot3(v[i], v[j], v[k]);
        vc = v;
        s = do_all_top_swops(vc);
        if (s > best_so_far) {
          best_so_far = s;
          copy(v.begin(), v.end(), best.begin());
        }
        rot3(v[i], v[j], v[k]);
      }
    }
  }

  // save score if it's an improvement
  if (best_so_far > curr) {
    int diff = best_so_far - curr;
    cout << "\nn = " << n << ", score = " << best_so_far
         << " (" << diff << " improvement)" << endl
         << best << endl;    
    fin.close();
    set_current_score(n, best_so_far, best);
  }
}

#endif
