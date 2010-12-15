// topswap_perm1.cpp

// See http://azspcs.net/Contest/Cards
// Contest ends: Feb. 12 2011, 4pm

#include "std_lib_facilities_ubuntu_regvec.h"

ostream& operator<<(ostream& os, const vector<int>& v) {
  const int n = v.size();
  if (n == 0) return os;
  else if (n == 1) os << v[0];
  else {  // v has at least 2 elements
    os << v[0];
    for(int i = 1; i < n; ++i) os << ", " << v[i];
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

////////////////////////////////////////////////////////////

int n_vals[25] = { 2,  3,  5,  7, 11, 
                  13, 17, 19, 23, 29, 
                  31, 37, 41, 43, 47, 
                  53, 59, 61, 67, 71,
                  73, 79, 83, 89, 97};


void init_deck(vector<int>& deck) {
  srand(time(NULL));
  for(int i = 0; i < deck.size(); ++i) {
    deck[i] = i+1;
  }
}

int do_all_top_swaps(vector<int>& deck) {
  int count = 0;
  while (deck[0] != 1) {
    reverse(deck.begin(), deck.begin() + deck[0]);  // do 1 top-swap
    ++count;
  }
  return count;
}


int try_all_perm(int n) {
  cout << "Topswop Permuter, n = " << n << endl;
  vector<int> deck(n), start(n), best(n);
  init_deck(deck);
  int highest_count_so_far = -1;
  while (next_permutation(deck.begin(), deck.end())) {
    //cout << deck << endl;
    copy(deck.begin(), deck.end(), start.begin());
    int count = do_all_top_swaps(start);
    if (count > highest_count_so_far) {
      highest_count_so_far = count;
      copy(deck.begin(), deck.end(), best.begin());
    }
  }
  cout << best << " (n=" << n << ", optimal_count=" << highest_count_so_far 
       << ")" << endl;
  return highest_count_so_far;
}

int experiment_perm() {
  cout << "\n\nTopswop Permuter Experiment";
  try_all_perm(13);
//   for(int i = 0; i < 5; ++i) {
//     int n = n_vals[i];
//     try_all_perm(n);
//   }
 }

void test_perm() {
  vector<int> deck(4);
  init_deck(deck);
  while (next_permutation(deck.begin(), deck.end())) {
    cout << deck << endl;
  }
  cout << "done\n";
}

int main() {
  experiment_perm();
  //  test_perm();
}
