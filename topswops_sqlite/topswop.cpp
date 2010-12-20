// topswop.cpp

// deprecated

// See http://azspcs.net/Contest/Cards
// Contest ends: Feb. 12 2011, 4pm

#define NDEBUG
#include "topswop.h"

////////////////////////////////////////////////////////////

const bool quiet = true;

int try_all_random(int n, int tries) {
  if (!quiet) cout << "Topswop Random Guesser, n = " << n 
                   << ", tries = " << tries << endl;
  vector<int> deck(n), start(n), best(n);
  init_deck(deck);
  int highest_count_so_far = -1;
  for(int i = 0; i < tries; ++i) {
    random_shuffle(deck.begin(), deck.end());
    fix_shuffle(deck);
    copy(deck.begin(), deck.end(), start.begin());
    int count = do_all_top_swops(deck);
    if (count > highest_count_so_far) {
      highest_count_so_far = count;
      copy(start.begin(), start.end(), best.begin());
    }
  } // for
  if (!quiet) cout << best << " (n=" << n << ", count=" 
                   << highest_count_so_far 
                   << ")" << endl;

  check_score(n, highest_count_so_far, best);
  return highest_count_so_far;
}

void experiment_random(int tries) {
  if (!quiet) cout << "\n\nTopswop random guesser" << endl
                   << "Doing " << tries 
                   << " guesses for each value of n ... " << endl;
  int score = 0;

  // i starts at 5 because optimal solutions for lower values have
  // been found
  for(int i = 6; i < 25; ++i) {
    score += try_all_random(n_vals[i], tries);
  }
  if (!quiet) cout << "(score = " << score << " for tries = " << tries 
                   << ")" << endl;
}

int main() {
  const int tries = 100000;
  
  cout << "Topswop random guesser is running ..." << endl;
  int count = 1;
  while (count < tries) {
    cout << count << ' ' << flush;
    if (count % 25 == 0) cout << endl;
    ++count;
    experiment_random(tries);
  }
}
