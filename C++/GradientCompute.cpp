#include "GradientCompute.h"
#include <math.h>
#include "types.h"
#include <iostream>
using namespace std;

double GradientCompute::P_func(int w, int S){
  double result = 1;
  int w_act = activ_time[S][w];
  for(int tau =0; tau<delta; tau++){
    for(auto & v: inv_graph.adj_list[w]){
      if (matrix[w][v.end]!= -1 && activ_time[S][v.end] == (w_act-1-tau)){
        result = result * (1 - matrix[w][v.end]*exp(-tau/eta));
      }
    }
  }
  return 1-result;
}

double GradientCompute::Z_func(int S, int v, int w){
  double result = 0;
  int w_act = activ_time[S][w];
  int v_act = activ_time[S][v];
  double e = 0;
  for(int tau = 0; tau < w_act - v_act -1; tau++){
    e = exp(-tau/eta);
    result +=  e/(1 - matrix[w][v]*e);
  }
  return result;
}

double GradientCompute::Part_deriv_P(int S, int v, int w){
  int w_act = activ_time[S][w];
  double result = 1;
  for(int tau = 0; tau < delta; tau++){
    for(auto & v_prime: inv_graph.adj_list[w]){
      if(matrix[w][v_prime.end]!= -1 && activ_time[S][v_prime.end]==w_act-1-tau && v_prime.end!=v){
        result *= (1-matrix[w][v_prime.end]*exp(-tau/eta));
      }
    }
  }
  result *=result*exp(-(w_act - activ_time[S][v] -1)/eta);
  return result;
}

double GradientCompute::Gradient(int v, int w){
  double result = 0;
  for(int S=0; S<number_of_seedsets; S++){
    if(activ_time[S][w]!=-1){
      int w_act = activ_time[S][w];
      int v_act = activ_time[S][v];
      if(matrix[w][v]!=-1 && v_act <= (w_act -1) && v_act >= (w_act-delta-1)){
        result += result + 1/P_func(w,S)*Part_deriv_P(S,v,w) + Z_func(S,v,w);
      }
    }
  }
  return result;
}

double** GradientCompute::GradientDescent(int iter, double alpha){
  for(int i=0; i<iter; i++){
    for(int v= 0; v<nodes_num; v++){
      for(int w =0; w<graph.adj_list[i].size(); w++){
          matrix[w][v] -= alpha* Gradient(v,graph.adj_list[i][w].end);
      }
    }
  }
  return matrix;
}
