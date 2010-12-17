// topswop_rev.cpp

// See http://azspcs.net/Contest/Cards
// Contest ends: Feb. 12 2011, 4pm

#define NDEBUG
#include "topswop.h"

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

bool contains(vector<perm*>& stack, perm* p)  {
  for(int i = 0; i < stack.size(); ++i) {
    if (stack[i] == p)
      return i;
  }
  return -1;
}

int count_home_skip_first(perm* p) {
  int count = 0;
  int i = 2;
  for(perm::iterator vp = p->begin()+1; vp < p->end(); ++vp) {
    if (*vp == i) ++count;
    ++i;
  }
  return count;
}

int has_home_num(perm* p) {
  int i = 2;
  for(perm::iterator vp = p->begin()+1; vp < p->end(); ++vp) {
    if (*vp == i) return true;
    ++i;
  }
  return false;
}

int add_kids(perm* v, vector<perm*>& stack) {
  assert(is_perm(*v));
  int count = 0;
  int i = 1;
  for(perm::iterator vp = v->begin(); vp < v->end(); ++vp) {
    if (*vp == i && i != 1) {
      perm* next_perm = new perm(*v);
      reverse(next_perm->begin(), next_perm->begin() + *vp);
      assert(is_perm(*next_perm));
      stack.push_back(next_perm);
      ++count;
    }
    ++i;
  }
  return count;
}

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
      //random_shuffle(stack.begin(), stack.end());
      cout << '.' << flush;
    }
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
  test2();
}
