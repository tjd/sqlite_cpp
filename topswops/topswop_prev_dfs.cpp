// topswop_prev_dfs.cpp

// deprecated

// See http://azspcs.net/Contest/Cards
// Contest ends: Feb. 12 2011, 4pm

#define NDEBUG
#include "topswop.h"

perm* pop(vector<perm*>& stack) {
  assert(!stack.empty());
  perm* temp = stack.back();
  stack.pop_back();
  return temp;
}

void random_start(perm* p) {
  if (!is_perm(*p)) error("*p must be a perm");
  if (p->front() != 1) error("perm must start with 1");
  random_shuffle(p->begin() + 1, p->end());
  while (!has_some_home_num_skip_first(p))
    random_shuffle(p->begin() + 1, p->end());
  if (p->front() != 1) error("perm must start with 1");
}


/*

// Doing this from scratch does not work very well at all.
void test6_do_not_run() {
  // start at 14 since we have optimal score for 13
  for(int i = 14; i <= 97; ++i) {
    const int n = i;
    cout << "---------------------------------------" << endl;
    cout << n << endl;
    cout << "---------------------------------------" << endl;
    perm* p_1(get_current_perm(n-1));
    //cout << "*p_1: " << *p_1 << endl;
    perm p(*p_1);
    //cout << "p: " << p << endl;
    p.push_back(n);
    //cout << n << " appended to p" << endl;
    search_back(p);
    //cout << "searched" << endl;
    delete p_1;
    //cout << "deleted" << endl;
  }
}
*/


void random_improve(int n, int tries) {
  for(int i = 0; i < tries; ++i) {
    perm_ptr p(range(n));
    shuffle(*p);
    do_all_top_swops(*p);
    search_back(*p);
    //delete p;
  }
}

void random_improve() {
  for(int i = 6; i < n_vals_size; ++i) {
    random_improve(n_vals[i], 10);
    cout << '.' << flush;
  }
}


void test4(int n) {
  int score = get_current_score(n);
  perm* p = get_current_perm(n);
  cout << "n = " << n << ", score = " << score << ": " << *p << endl;
  if (search_back(*p)) {
    set_current_perm(n, *p);
  }
}

void test4() {
  int score37 = get_current_score(37);
  perm* p37 = get_current_perm(37);
  cout << "n = 37, score = " << score37 << ": " << *p37 << endl;
  if (search_back(*p37)) {
    set_current_perm(37, *p37);
  }
}

// depth-first search with occasional stack shuffles
void test3() {
  const int n = 97;
  const int shuffle_every = 10000000;
  cout << "Topswop Reverse Search, n = " << n 
       << ", shuffle = " << shuffle_every << endl;

  vector<perm*> stack;

  // initialize first permutation
  perm* init = range(n);
  cout << "init = " << *init << endl;

  perm best(*init);
  int best_score = 0;
  int num_added = add_kids(init, stack);
  assert(stack.size() == n-1);

  int count = 0;
  while (!stack.empty()) {
    ++count;
    if (count >= shuffle_every) {
      count = 0;
      int r1 = randint(n);
      int r2 = randint(n);
      swap(stack[r1], stack[r2]);
      cout << '.' << flush;
    }
    perm* curr_perm(pop(stack));
    assert(is_perm(*curr_perm));
    int score = do_all_top_swops_copy(*curr_perm);
    if (score > best_score) {
      best = *curr_perm;
      best_score = score;
      cout << "n = " << n << endl;
      cout << "best score so far = " << best_score << endl;
      cout << "best so far = " << best << endl;
    }
    num_added = add_kids(curr_perm, stack);
    assert(is_perm(*curr_perm)); 
    delete curr_perm;
  }
  cout << endl;
  cout << "------------------------------\n";
  cout << "n = " << n << endl;
  cout << "best score = " << best_score << endl;
  cout << "best = " << best << endl;
}

//  depth-first search with random start and random restarts
void test2() {
  const int n = 97;
  const int restart = 100000000;
  cout << "Topswop Reverse Search with Random Restarts, n = " << n 
       << ", restart = " << restart << endl;

  vector<perm*> stack;

  // initialize first permutation
  perm* init = range(n);
  random_start(init);
  cout << "init = " << *init << endl;

  perm best(*init);
  int best_score = 0;
  int num_added = add_kids(init, stack);
  assert(stack.size() == n-1);

  int count = 0;
  while (true) {
    perm* curr_perm = pop(stack);

    //cout << "(stack popped)" << endl;
    assert(is_perm(*curr_perm));
    //cout << "*curr_perm = " << *curr_perm << endl;
    int score = do_all_top_swops_copy(*curr_perm);
    //cout << "(score calculated)" << endl;
    if (score > best_score) {
      best = *curr_perm;
      best_score = score;
      cout << "n = " << n << endl;
      cout << "best score so far = " << best_score << endl;
      cout << "best so far = " << best << endl;
    }
    num_added = add_kids(curr_perm, stack);
    assert(is_perm(*curr_perm)); 
    delete curr_perm;
    ++count;
    if (count >= restart || stack.empty()) {
      //cout << '.';
      for(int i = 0; i < stack.size(); ++i) {
        delete stack[i];
      }
      stack.clear();
      init = range(n);
      random_start(init);
      add_kids(init, stack);
      count = 0;
    }

  } // while

  cout << endl;
  cout << "------------------------------\n";
  cout << "n = " << n << endl;
  cout << "best score = " << best_score << endl;
  cout << "best = " << best << endl;
}

// vanilla depth-first search
void test1() {
  const int n = 97;
  cout << "Topswop Reverse Search, n = " << n << endl;

  vector<perm*> stack;

  // initialize first permutation
  perm* init = range(n);
  cout << "init = " << *init << endl;
//   add_kids(init, stack);
//   for(int i = 0; i < stack.size(); ++i) {
//     cout << *stack[i] << endl;
//   }
// 
  perm best(*init);
  int best_score = 0;
  int num_added = add_kids(init, stack);
  assert(stack.size() == n-1);
  //cout << num_added << " children added" << endl;

  while (!stack.empty()) {
    //print_stack(stack);
    perm* curr_perm = pop(stack);
    //cout << "(stack popped)" << endl;
    assert(is_perm(*curr_perm));
    //cout << "*curr_perm = " << *curr_perm << endl;
    int score = do_all_top_swops_copy(*curr_perm);
    //cout << "(score calculated)" << endl;
    if (score > best_score) {
      best = *curr_perm;
      best_score = score;
      cout << "n = " << n << endl;
      cout << "best score so far = " << best_score << endl;
      cout << "best so far = " << best << endl;
    }
    num_added = add_kids(curr_perm, stack);
    assert(is_perm(*curr_perm)); 
    //cout << contains(stack, curr_perm) << endl;
    //cout << "deleting: " << *curr_perm << endl;
    //assert(contains(stack, curr_perm) == -1);
    delete curr_perm;
    //cout << num_added << " children added" << endl;
  }
  cout << endl;
  cout << "------------------------------\n";
  cout << "n = " << n << endl;
  cout << "best score = " << best_score << endl;
  cout << "best = " << best << endl;
}



int main() {
  //  test4(73);
  //test6();
  improve_current_best_perms();
  //ensure_increasing_scores();
  //random_improve();
}
