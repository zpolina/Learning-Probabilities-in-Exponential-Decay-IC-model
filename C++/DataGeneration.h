#ifndef DATAGENERATION_H
#define DATAGENERATION_H

#include <iostream>
#include <fstream>
#include <map>
#include <set>
#include <string>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <cmath>

#include "types.h"
using namespace std;

class DataGeneration{
public:
  DataGeneration(){

  };
  ~DataGeneration(){};
  void readGraph(string filename, Graph &graph);
  void writeSpreadToFile(string filename, Spread spread);
  void setProbabilities(Graph graph, int prob_min=0, int prob_max=1);
  Set generateSeedSet(int size, int num_nodes);
  void generateSpread(Graph graph, Set seed_set, Spread &D, int eta = 4, int delta=24);
};


#endif
