// topswap_test1.cpp

// See http://azspcs.net/Contest/Cards
// Contest ends: Feb. 12 2011, 4pm

#include "std_lib_facilities_ubuntu.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

ostream& operator<<(ostream& os, const vector<int>& v) {
  const int n = v.size();
  if (n == 0) os << "{}";
  else if (n == 1) os << '{' << v[0] << '}';
  else {  // v has at least 2 elements
    os << '{' << v[0];
    for(int i = 1; i < v.size(); ++i) {
      os << ", " << v[i];
    }
    os << '}';
  }
  return os;
}

const int n = 97;
const int tries = 10000000;
vector<int> deck(n);

void init_deck() {
  // initialize random number generator with a random seed based on the
  // time
  srand(time(NULL));
  for(int i = 0; i < deck.size(); ++i) {
    deck[i] = i+1;
  }
}

void init_deck_sample() {
  deck[0]=3;
  deck[1]=6;
  deck[2]=5;
  deck[3]=1;
  deck[4]=4;
  deck[5]=2;
}

inline void swap(int& a, int& b) {
  int temp = a;
  a = b;
  b = temp;
}

// void shuffle() {
//   for(int i = 0; i < deck.size() - 1; ++i) {
//     int r = randint(i, deck.size());
//     swap(deck[i], deck[r]);
//   }
// }

int do_all_top_swaps() {
  int count = 0;
  while (deck[0] != 1) {
    reverse(deck.begin(), deck.begin() + deck[0]);  // do 1 top-swap
    ++count;
  }
  return count;
}


int main() {
  cout << "Topswap Random, n = " << n << ", tries = " << tries << endl;
  init_deck();
  vector<int> start(deck.size());
  int highest_count_so_far = -1;
  for(int i = 0; i < tries; ++i) {
    random_shuffle(deck.begin(), deck.end());
    copy(deck.begin(), deck.end(), start.begin());
    int count = do_all_top_swaps();
    if (count > highest_count_so_far) {
      highest_count_so_far = count;
      cout << start << "-->" << deck << " (" << count 
           << ", n = " << n << ")" << endl;
    }
    if (i % 1000000 == 0) cout << "(tries = " << i << ")" << endl;
  }  
  cout << "done!\n";
}


/*

n = 11
{7, 10, 6, 11, 9, 4, 8, 2, 1, 3, 5}-->{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11} (50, n = 11)
{2, 3, 8, 10, 6, 11, 9, 7, 1, 4, 5}-->{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11} (50, n = 11)
{4, 9, 11, 6, 10, 7, 8, 2, 1, 3, 5}-->{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11} (51, n = 11)

n = 13
{6, 2, 3, 9, 11, 5, 10, 13, 7, 12, 1, 4, 8}-->{1, 6, 5, 2, 3, 4, 7, 8, 9, 10, 11, 12, 13} (71, n = 13)
{3, 2, 4, 10, 1, 8, 12, 13, 9, 5, 11, 6, 7}-->{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13} (73, n = 13)
{5, 11, 13, 3, 7, 9, 2, 12, 8, 1, 4, 10, 6}-->{1, 6, 5, 4, 3, 2, 7, 8, 9, 10, 11, 12, 13} (73, n = 13)
{7, 5, 2, 13, 9, 1, 8, 11, 12, 10, 3, 4, 6}-->{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13} (73, n = 13)

 */
