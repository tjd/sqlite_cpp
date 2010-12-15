// topswap3.cpp

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

int try_all_random(int n, int tries, ofstream& fout) {
  cout << "Topswop Random Guesser, n = " << n << ", tries = " << tries << endl;
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
//      cout << '\n' << best << "-->(n=" << n << ", count=" 
//           << highest_count_so_far
//           << ")" << endl;
    }
  } // for
  cout << best << " (n=" << n << ", count=" << highest_count_so_far 
       << ")" << endl;
  fout << best << ( n < n_vals[24] ? ";" : "") << endl;
  return highest_count_so_far;
}

int experiment_random(int tries) {
  // open the output file
  const string outfile_name = "topswop_output.txt";
  ofstream fout(outfile_name.c_str());

  cout << "\n\nTopswop random guesser" << endl
       << "Doing " << tries << " guesses for each value of n ... " << endl;
  int score = 0;
  for(int i = 0; i < 25; ++i) {
    score += try_all_random(n_vals[i], tries, fout);
  }
  cout << "(score = " << score << " for tries = " << tries << ")" << endl;

  // rename the output file
  fout.close();
  string name_with_score = "topswop_output_" + int_to_str(score) + ".txt";
  rename(outfile_name.c_str(), name_with_score.c_str());
  cout << "Output written to file " << name_with_score << endl;
}

int main() {
  const int tries = 10000000;
  experiment_random(tries);
  //experiment_random(tries);
}
