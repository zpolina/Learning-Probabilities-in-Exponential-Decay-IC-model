#ifndef GRADIENTCOMPUTE_H
#define GRADIENTCOMPUTE_H

#include "types.h"
using namespace std;

class GradientCompute{
public:
  double** matrix;
  vector<Spread> spreads;
  vector<vector< int > > activ_time;
  int eta;
  int delta;
  int number_of_seedsets;
  int nodes_num;
  Graph graph;
  Graph inv_graph;
  GradientCompute(double** matrix, Graph graph, vector<Spread> &spreads,int eta, int delta, int number_of_seedsets, int nodes_num){
    this->matrix = matrix;
    this->graph = graph;
    this->spreads = spreads;
    this->eta = eta;
    this->delta = delta;
    this->number_of_seedsets = number_of_seedsets;
    this->nodes_num = nodes_num;
    vector<int> nodes_per_set(spreads.size(), 0);
    for(int i =0; i<spreads.size(); i++){
      for(int j =0; j<spreads[i].size(); j++){
        nodes_per_set[i] += spreads[i][j].size();
      }
    }
    activ_time.resize(number_of_seedsets);
    for(int i = 0; i < spreads.size(); i++){
      activ_time[i].resize(nodes_per_set[i], -1);
      for(int j = 0; j < spreads[i].size(); j++){
        for(auto k : spreads[i][j]) {
            activ_time[i][k] = j;
        }
      }
    }
    inv_graph.N = graph.N;
    inv_graph.adj_list = new AdjList[inv_graph.N];
    for(int i = 0; i < graph.N; i++){
      for(int j=0; j<graph.adj_list[i].size(); j++){
        inv_graph.adj_list[graph.adj_list[i][j].end].push_back(Edge(i));
      }
    }

  }
  double P_func(int w, int S);
  double Z_func(int S, int v, int w);
  double Part_deriv_P(int S, int v, int w);
  double Gradient(int v, int w);
  double** GradientDescent(int iter, double alpha);
  ~GradientCompute(){
   for(int i = 0; i < nodes_num; ++i) {
       delete[] matrix[i];
   }
   delete[] matrix;
  }
};


#endif
