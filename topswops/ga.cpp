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
const int N = 7;
const double MUTATION_RATE = 0.10;

inline bool less_than(const Topswop& a, const Topswop& b) {
  return a.score() < b.score();
}

inline bool greater_than(const Topswop& a, const Topswop& b) {
  return a.score() > b.score();
}


vector<Topswop>* select_roulette(vector<Topswop>* pop) {
  assert(pop != 0);
  assert(pop->size() == POP_SIZE);

  sort(pop->begin(), pop->end(), greater_than);

  vector<double> accum(pop->size());
  double total_fitness = 0;
  for(int i = 0; i < pop->size(); ++i) {
    int si = (*pop)[i].score();
    total_fitness += si;
  }

  for(int i = 0; i < pop->size(); ++i) {
    int si = (*pop)[i].score();
    accum[i] = si / total_fitness + ((i == 0) ? 0: accum[i-1]);
  }

  vector<Topswop>* next_pop = new vector<Topswop>(pop->size());

  // roulette-wheel selection for next population
  for(int n = 0; n < pop->size(); ++n) {
    double r = rand_double();
    for(int i = 0; i < pop->size(); ++i) {
      if (accum[i] >= r) {
        (*next_pop)[n] = (*pop)[i];
        break;
      }
    } // for
  } // for

  return next_pop;
}

inline void simple_mutate(Topswop& t) {
  for(int i = 0; i < 0.1 * N; ++i)
    t.rand_swap();    
}

// The top 75% (or so) make it to the next generation.
// The other 25% come from mutations of that top 75%.
vector<Topswop>* select_simple(vector<Topswop>* pop, double top_pct = 0.75) {
  assert(pop != 0);
  assert(pop->size() == POP_SIZE);
  assert(top_pct >= 0 && top_pct <= 1);

  // sort pop from best to worst
  sort(pop->begin(), pop->end(), greater_than);

  // copy the top_pct elements of pop into next_pop
  vector<Topswop>* next_pop = new vector<Topswop>(pop->size());
  const int end_top = top_pct * pop->size();
  copy(pop->begin(), pop->begin() + end_top, next_pop->begin());

  // randomly shuffle the top elements of pop
  random_shuffle(pop->begin(), pop->begin() + end_top);

  // mutate (1-top_pct) elements of pop and add them to end of
  // next_pop
  copy(pop->begin(), pop->begin() + (1 - top_pct) * pop->size(),
       next_pop->begin() + end_top);
  for_each(next_pop->begin() + end_top, next_pop->end(), simple_mutate);

  return next_pop;
}

void mutator(Topswop& t) {
  if (rand_double() <= MUTATION_RATE) t.rand_swap();    
}

void mutate(vector<Topswop>* pop) {
  assert(pop != 0);
  assert(pop->size() == POP_SIZE);
  for_each(pop->begin(), pop->end(), mutator);
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
  vector<Topswop>* pop = new vector<Topswop>(POP_SIZE);
  generate(pop->begin(), pop->end(), init_ga_topswop);

  noteln("starting main iterations");
  for(int iter = 0; iter < max_iter; ++iter) {
    vector<Topswop>* next_pop = select_simple(pop);
    delete pop;
    pop = next_pop;
  }
}

int main() {
  ga();
}
