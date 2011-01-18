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

bool less_than(const Topswop& a, const Topswop& b) {
  return a.score() < b.score();
}

bool greater_than(const Topswop& a, const Topswop& b) {
  return a.score() > b.score();
}


vector<Topswop>* select(vector<Topswop>* pop) {
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

void mutator(Topswop& t) {
  if (rand_double() <= MUTATION_RATE) t.rand_swap();    
}

void mutate(vector<Topswop>* pop) {
  assert(pop != 0);
  assert(pop->size() == POP_SIZE);
  for_each(pop->begin(), pop->end(), mutator);
}

int ga(int max_iter = 5) {
  //
  // initialization
  //
  noteln("initialization");
  vector<Topswop>* pop = new vector<Topswop>(POP_SIZE);
  for(vector<Topswop>::iterator p = pop->begin(); p != pop->end(); ++p) {
    *p = Topswop(N);
    p->randomize();
  }

//   for(int i = 0; i < pop->size(); ++i) {
//     (*pop)[i] = Topswop(N);
//     (*pop)[i].randomize();
//   }

  noteln("starting main iterations");
  for(int iter = 0; iter < max_iter; ++iter) {
//     if (iter % 1 == 0) {
//       cout << "iteration = " << iter << endl;
//       cout << *pop;
//     }
    vector<Topswop>* next_pop = select(pop);
    delete pop;
    pop = next_pop;
    mutate(pop);
  }
}

int main() {
  ga();
}
