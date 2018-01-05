#include "GradientCompute.h"
#include <math.h>
#include "types.h"
#include <iostream>
using namespace std;

double GradientCompute::P_func(int w, int S){
  double result = 1;
  int w_act = activ_time[S][w];
    for(auto & v: inv_graph.adj_list[w]){
      int v_act = activ_time[S][v.end];
      int tau = w_act - v_act - 1;
      if (matrix[v.end][w]!= -1 && tau < delta && tau>=0){
        result = result * (1 - matrix[v.end][w]*exp(-tau/eta));
      }
    }
  return 1-result;
}

double GradientCompute::Z_func(int S, int v, int w, bool flag){
  double result = 0;
  int w_act = activ_time[S][w];
  int v_act = activ_time[S][v];
  double e = 0;
  int upper_bound = 0;
  if(flag){
    upper_bound = w_act - v_act -2;
  }else{
    upper_bound = delta;
  }
  for(int tau = 0; tau < upper_bound; tau++){
    e = exp(tau/eta);
    result +=  1/(e - matrix[v][w]);
  }
  return -result;
}

double GradientCompute::Part_deriv_P(int S, int v, int w){
  int w_act = activ_time[S][w];
  double result = 1;
    for(auto & v_prime: inv_graph.adj_list[w]){
      int v_act = activ_time[S][v_prime.end];
      int tau = w_act - v_act - 1;
      if(matrix[v_prime.end][w]!= -1 && v_prime.end!=v && tau >= 0 && tau < delta){
        result *= (1-matrix[v_prime.end][w]*exp(-tau/eta));
      }
    }
  result *= exp(-(w_act - activ_time[S][v] -1)/eta);
  return result;
}

double GradientCompute::Gradient(int v, int w){
  double result = 0;
  for(int S=0; S<number_of_seedsets; S++){
    if(activ_time[S][w]!=-1){
      int w_act = activ_time[S][w];
      int v_act = activ_time[S][v];
      if(matrix[v][w]!=-1){
        if(v_act <= (w_act -1) && v_act >= (w_act-delta-1)){
          result +=  1/P_func(w,S)*Part_deriv_P(S,v,w) + Z_func(S,v,w,true);
        }
        else if(v_act<w_act - delta -1){
          result += Z_func(S,v,w, false);
        }
      }
    }
  }
  return result;
}

double** GradientCompute::GradientDescent(int iter, double alpha0){
  for(int i=0; i<iter; i++){
    double alpha = alpha0;///(1+0.3*i);
    double sum = 0;
    for(int v= 0; v<nodes_num; v++){
      for(auto& w: graph.adj_list[v]){
          double grad = Gradient(v,w.end);
          matrix[v][w.end] += alpha*grad;
          if (matrix[v][w.end] < 0)
            matrix[v][w.end] = 0;
          if (matrix[v][w.end] > 1)
              matrix[v][w.end] = 1;
          sum += abs(w.prob - matrix[v][w.end]);
      }
    }
    cout<<sum<<endl;
  }
  return matrix;
}
