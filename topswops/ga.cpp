// ga.cpp

// See http://azspcs.net/Contest/Cards
// Contest ends: Feb. 12 2011, 4pm

/*

This is an attemp at implementing a genetic algorithm (GA) for
topswops. Or, at least, a GA-inspired algorithm as the basic GA
framework usually seems to needs lots of problem-specific tweaking to
make it work well (if at all).

Here is the basic GA algorithm::

  - **Initialization.** Initialize the population, i.e. a set of
    candidate elements. We assume the existence of a fitness function
    f that returns a numeric estimate of the quality of any element in
    the population. Essentially, the GA is optimizing f.

  - **Selection.** A new population is generated by choosing elements
    from the previous generation in a more or less random way that is
    biased by the fitness of the elements, i.e. more fit elements have
    a better chance of surviving into future generates, while less fit
    elements are soon deleted.

  - **Reproduction.** The elements selected in the previous set are
    now "bred", i.e. somehow combined to make a new population. The
    intuiton is that by combining two (or more) pretty good elements
    we may be able to create a new, even better, element.

    **Mutation** is an important part of some GAs, e.g. when combining
    elements small random mistakes may be introduced to increase
    diversity.

    Many, many reproduction operations have been proposed.

Selection and reproduction are repeated until some terminating
condition is met, e.g. a best-possible solution is found, a time-limit
is reached, etc.

 */

#include "topswop.h"

const int POP_SIZE = 100;
const int N = 10;
const double TOP_PCT = 0.75;  // between 0 and 1
const double MUTATE_PCT = 0.10;  // between 0 and 1

const int END_TOP = N * TOP_PCT;
const int MUTATE_NUM = N * MUTATE_PCT;

vector<Topswop> pop(POP_SIZE);

inline bool less_than(const Topswop& a, const Topswop& b) {
  return a.score() < b.score();
}

inline bool greater_than(const Topswop& a, const Topswop& b) {
  return a.score() > b.score();
}

inline void simple_mutate(Topswop& t) {
  Trace trace("simple_mutate");
  for(int i = 0; i < MUTATE_NUM; ++i) {
    note("i = " + to_string(i));
    //    cout << t << endl;
    t.rand_swap();    
  }
}

// The top 75% (or so) make it to the next generation.
// The other 25% come from mutations of that top 75%.
void select_simple() {
  Trace t("select_simple");

  assert(TOP_PCT >= 0 && TOP_PCT <= 1);

  // sort pop from best to worst
  noteln("sorting");
  sort(pop.begin(), pop.end(), greater_than);

  // randomly shuffle the top elements of pop
  noteln("shuffling");
  random_shuffle(pop.begin(), pop.begin() + END_TOP);

  // mutate (1-top_pct) elements of pop
  noteln("mutating");
  for_each(pop.begin() + END_TOP, pop.end(), simple_mutate);
}

// create a new Topswop of size N with its elements in random order
Topswop init_ga_topswop() {
  Topswop t(N);
  t.randomize();
  return t;
}

int ga(int max_iter = 5) {
  //
  // initialization
  //
  noteln("initialization");
  generate(pop.begin(), pop.end(), init_ga_topswop);

  noteln("starting main iterations");
  for(int iter = 0; iter < max_iter; ++iter) {
    note(to_string(iter));
    select_simple();
  }
}

int main() {
  cout << "Simple GA Test\n\n";
  print_var("POP_SIZE", POP_SIZE);
  print_var("N", N);
  print_var("TOP_PCT", TOP_PCT);
  print_var("MUTATE_PCT", MUTATE_PCT);
  print_var("END_TOP", END_TOP);
  print_var("MUTATE_NUM", MUTATE_NUM);
  cout << "\n\n";
  ga();
}
