#ifndef GRADIENTCOMOUTE_H
#define GRADIENTCOMOUTE_H

#include "types.h"
using namespace std;

class GradientCompute{
public:
  Graph graph;
  vector<Spread> spreads;
  GradientCompute(Graph &graph, vector<Spread> &spreads){
    this->graph = graph;
    this->spreads = spreads;
  }
  ~GradientCompute(){}
};


#endif
