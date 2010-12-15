// topswap2.cpp

// See http://azspcs.net/Contest/Cards
// Contest ends: Feb. 12 2011, 4pm

#include "std_lib_facilities_ubuntu_regvec.h"

int n_vals[25] = { 2,  3,  5,  7, 11, 
                  13, 17, 19, 23, 29, 
                  31, 37, 41, 43, 47, 
                  53, 59, 61, 67, 71,
                  73, 79, 83, 89, 97};

const string outfile_name("topswop_output.txt");
ofstream fout(outfile_name.c_str());

ostream& operator<<(ostream& os, const vector<int>& v) {
  const int n = v.size();
  if (n == 0) return os;
  else if (n == 1) os << v[0];
  else {  // v has at least 2 elements
    os << v[0];
    for(int i = 1; i < n; ++i) {
      os << ", " << v[i];
    }
  }
  return os;
}

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

int try_all_random(int n, int tries) {
  cout << "Topswap Random Guesser, n = " << n << ", tries = " << tries << endl;
  vector<int> deck(n), start(n), best(n);
  init_deck(deck);
  int highest_count_so_far = -1;
  for(int i = 0; i < tries; ++i) {
    random_shuffle(deck.begin(), deck.end());
    copy(deck.begin(), deck.end(), start.begin());
    int count = do_all_top_swaps(deck);
    if (count > highest_count_so_far) {
      highest_count_so_far = count;
      copy(start.begin(), start.end(), best.begin());
    }
  } // for
  cout << best << " (n=" << n << ", count = " << highest_count_so_far 
       << ")" << endl;
  fout << best << endl;
  return highest_count_so_far;
}

int main() {
  const int tries = 10000000;
  cout << "Topswap random guesser" << endl
       << "Doing " << tries << " guesses for each value of n" << endl;
  cout << "Output written to file " << outfile_name;
  int score = 0;
  for(int i = 0; i < 25; ++i) {
    score += try_all_random(n_vals[i], tries);
  }
  cout << "done!\n";
  cout << "overall score = " << score << endl;
}


