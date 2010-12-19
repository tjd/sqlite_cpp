// topswop_prev_dfs.cpp

// See http://azspcs.net/Contest/Cards
// Contest ends: Feb. 12 2011, 4pm

#define NDEBUG
#include "topswop.h"

// print the current stack contents; used for debugging
void print_stack(const vector<perm*>& stack) {
  if (stack.empty()) cout << "   <stack empty>" << endl;
  else if (stack.size() == 1) cout << "   0: " << *stack[0] << endl;
  else {
    cout << "   0: " << *stack[0];
    for(int i = 1; i < stack.size(); ++i) 
      cout << "\n   " << i << ": " << *stack[i];
    cout << endl;
  }
}


/*
int add_kids(perm* v, vector<perm*>& stack) {
  //cout << "add_kids(" << *v << ", ";
  //print_stack(stack);
  //cout << ") called ..." << endl;
  assert(is_perm(*v));
#ifndef NDEBUG
  perm vc(*v);
#endif
  int count = 0;
  int i = 1;
  for(perm::iterator vp = v->begin(); vp < v->end(); ++vp) {
    //cout << "i = " << i << endl;
    if (*vp == i && i != 1) {
      perm* next_perm = new perm(*v);
      assert(is_perm(*next_perm));
      reverse(next_perm->begin(), next_perm->begin() + *vp);
      assert(is_perm(*next_perm));
      stack.push_back(next_perm);
      ++count;
    }
    ++i;
  }
  //cout << "out of for-loop" << endl;
  //cout << "vc = " << vc << endl;
  assert(count == count_home_skip_first(&vc));
  //cout << "... add_kids finished" << endl;
  return count;
}
*/

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
  while (!has_home_num(p))
    random_shuffle(p->begin() + 1, p->end());
  if (p->front() != 1) error("perm must start with 1");
}

/*
// generate a new permutation by searching backwards from p 

// TODO: probably has memory leaks; indeed failed with a std:bad_alloc
// on one run
// TODO: add a cut-off to abort the search after certain number of tries
// TODO: decrease file I/O by passing in best n-val so far
bool search_back(perm p) {
  //cout << "search_back(" << p << ") called ..." << endl;
  assert(is_perm(p));
  const int n = p.size();
  //cout << "  n = " << n << endl;
  const int p_score = do_all_top_swops_copy(p);
  //cout << "  p_score = " << p_score << endl;
  perm best(p);
  int best_score = p_score;
  vector<perm*> stack;
  stack.push_back(&p);
  while (!stack.empty()) {
    //cout << '(' << stack.size() << ')' << flush;
    //perm* curr_perm = pop(stack);
    // pop the stack
    perm* curr_perm = stack.back();
    stack.pop_back();

    //cout << "curr_perm = " << *curr_perm << endl;
    assert(is_perm(*curr_perm));
    int score = do_all_top_swops_copy(*curr_perm);
    //cout << "  score = " << score << endl;
    if (score > best_score) {
      int diff = score - best_score;
      best = *curr_perm;
      best_score = score;
      if (set_current_perm(n, best)) {
        cout << "\nn = " << n << " improvement of " << diff << endl;
        cout << "improved score = " << best_score << endl;
        cout << " improved perm = " << best << endl;
      }
    }
    add_kids(curr_perm, stack);
    //cout << "calling delete ..." << endl;
    assert(is_perm(*curr_perm));
    //cout << "curr_perm = " << *curr_perm << endl;
    if (curr_perm != &p)
      delete curr_perm;
    //cout << "... delete finished" << endl;
  } // while

  return false;
}

*/

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
    perm* p = range(n);
    shuffle(*p);
    do_all_top_swops(*p);
    search_back(*p);
    delete p;
  }
}

void random_improve() {
  for(int i = 6; i < n_vals_size; ++i) {
    random_improve(n_vals[i], 10);
    cout << '.' << flush;
  }
}

/*
void improve_current_best_perms() {
  for(int i = 2; i < 98; ++i) {
    const int n = i; 
    cout << "---------------------------------------" << endl;
    cout << n << endl;
    cout << "---------------------------------------" << endl;
    perm* p = get_current_perm(n);
    search_back(*p);
    delete p;
  }
}
*/

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
    perm* curr_perm = pop(stack);
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
