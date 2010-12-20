// Node.h

// See http://azspcs.net/Contest/Cards
// Contest ends: Feb. 12 2011, 4pm

#ifndef TOP_SWOP_NODE_H
#define TOP_SWOP_NODE_H 201012L

#include "topswop.h"

struct Node {
public:
  int get_score() const {return score;};
  void recalculate_score() {
    score = do_all_top_swops_copy(p);
  }

private:
  perm p;
  int score;
};

#endif
