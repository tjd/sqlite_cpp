// grasp.cpp

// topswop_perm.cpp

// See http://azspcs.net/Contest/Cards
// Contest ends: Feb. 12 2011, 4pm

#define NDEBUG
#include "topswop.h"

const bool show_n = false;

const string dbname = "best_so_far.db";


/*

Basic GRASP pseudocode for minimizing an objective function f:

   f_best = infinity
   repeat some number of times:
      x = GreedyRandomizedConstruction()
      x = LocalSearch(x)
      if f(x) < f_best then
         f_best = f_x
         x_best = x
      end
    end

GreedyRandomizedConstruction is not clearly explained in the
literature, but the gist seems to be that x is a *set* of basic
elements, and so x can be constructed by adding one basic element at a
time to it. At any time, only a certain list of candidate basic
elements can be added to x (e.g. some elements are not candidates
because they are already in x, or violate some sort of constraint on
the structure of x), and greedy function g is used to rank these
candidate elements. 

These candidate elements are stored on the restricted candidate list,
or RCL for short. Only the candidate elements that meet some threshold
quality are considered, and various ways of calculating this threshold
are available (e.g. a simple threshold would be to only consider the
10 best candidates).

What could be the basic elements for permutations? One idea is to use
a set of vectors with every entry set to 0 exact for one, which is a
value 1 to n (for an n-element permutation). For example, for
permutations of size n = 3, we would have these basic elements::

   100  010  001
   200  020  002
   300  030  003

Then a permutation is denote by a set of 3 of these basic elements,
with exactly one element chosen from each row and column.

I don't see what greedy function could be used to construct partial
solutions except for previous good solutions. And in that case the
elements are always added in the same order, i.e. first 1, then 2,
then 3, etc. Thus all that matters is the insertion *position* of the
newly added elements.

So given an already-constructed permutation of n-1 elements consisting
of 1, ..., n-1 in some order, we can construct a permutation of size n
by insertion n into one of the n different insertion positions of the
n-1 permutation (i.e. before any of the n-1 elements, or after the
last element). For each such insertion we calculate a greedy heuristic
score, and then choose an insertion at random from the insertions with
quality above some threshold.

 */

// Greedily generate a permutation 1 element bigger than pn1.
// TODO: make this more efficient!
perm greedy_next(const perm& pn1) {
  const int n = 1 + pn1.size();
  perm cp1(pn1);
  cp1.insert(cp1.begin(), n);
  int best_score = do_all_top_swops_copy(cp1);
  int i_best = 0;
  for(int i = 1; i < n; ++i) {
    perm cpy(pn1);
    cpy.insert(cpy.begin() + i, n);
    int score = do_all_top_swops_copy(cpy);
    if (score > best_score) {
      best_score = score;
      i_best = i;
    }
  }

  perm result(pn1);
  result.insert(result.begin() + i_best, n);
  return result;
}

// Construct a permutation of 1, ..., n in a greedy way from the best
// known permutation of 1, ..., n-1.
perm greedy_prev_construct(int n) {
  assert(n > 13);
  perm_ptr pn1(get_current_perm(n-1));
  return greedy_next(*pn1);
}


// Construct a permutation of 1, ..., n in a greedy way by greedily
// constructing smaller permutations starting at n = 13.  
perm greedy_full_construct(int n) {
  assert(n > 13);
  perm_ptr p13(get_current_perm(13));
  perm prev(*p13);
  perm curr(greedy_next(prev));
  for(int i = 15; i < n; ++i) {
    prev = curr;
    curr = greedy_next(prev);
  }
  return curr;
}

void test_greedy_prev_construct() {
  cout << "Running test_greedy_prev_construct() ..." << endl;
  for(int i = 14; i < 98; ++i) {
    const int n = i;
    cout << "(n=" << n << ")" << endl;
    perm p(greedy_prev_construct(n));
    set_current_perm(n, p);
  }
}

void test_greedy_full_construct() {
  cout << "Running test_greedy_full_construct() ..." << endl;
  for(int i = 14; i < 98; ++i) {
    const int n = i;
    cout << "(n=" << n << ")" << endl;
    perm p(greedy_full_construct(n));
    set_current_perm(n, p);
  }
}

int main() {
  underline("GRASP-Like Experimental Code");
  //  test_greedy_prev_construct();
  test_greedy_full_construct();
}
