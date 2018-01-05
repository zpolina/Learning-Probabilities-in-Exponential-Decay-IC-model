#include "DataGeneration.h"
#include "GradientCompute.h"
#include <vector>
#include <map>
#include <fstream>


using namespace std;
int main()
{
    srand(0);
    string filename = "../graphs/facebook_graph.txt";
    Graph graph;
    DataGeneration data_gen;
    data_gen.readGraph(filename, graph);
    data_gen.setProbabilities(graph);
    int eta = 5, delta = 10;
    int number_of_seedsets = 20;
    vector<Spread> spreads;
    vector<Set> seed_set_for_test;
    for(int ttt = 0; ttt < number_of_seedsets; ttt++) {
        int seed_set_size = 20;
        Set seed_set = data_gen.generateSeedSet(seed_set_size, graph.N);
        seed_set_for_test.push_back(seed_set);
        Spread spread;
        data_gen.generateSpread(graph, seed_set, spread, eta, delta);
        spreads.push_back(spread);
    }
    double** matrix = new double*[graph.N];
    for (int i =0; i<graph.N; i++){
      matrix[i] = new double[graph.N];
      for(int j=0; j<graph.N; j++){
        matrix[i][j]= -1;
      }
    }
    for (int i =0; i < graph.N; i++){
      for(auto& w: graph.adj_list[i]){
        matrix[i][w.end] =  (double)rand() / (double)RAND_MAX ;;///should be random initial values of probabilities
      }
    }

    GradientCompute gradient;
    gradient.ValueAssigning(matrix, graph, spreads, eta, delta, number_of_seedsets, graph.N);
    double** res_matrix = gradient.GradientDescent(100, 0.001);
    for(int i=0; i < graph.N; i++){
       for(int j=0; j < graph.adj_list[i].size(); j++){
        if(res_matrix[i][graph.adj_list[i][j].end] != -1){
          //cout<<i<<"  "<<j<<"  "<<res_matrix[i][j]<<endl;
          graph.adj_list[i][j] = res_matrix[i][graph.adj_list[i][j].end];
        }
      }
    }
    vector<Spread> learned_spreads;
    for(auto & set: seed_set_for_test) {
        Spread spread;
        data_gen.generateSpread(graph, set, spread, eta, delta);
        learned_spreads.push_back(spread);
    }

    int* activ_time = new int[graph.N];
    int* learned_activ_time = new int[graph.N];
    fill(learned_activ_time, learned_activ_time+graph.N, -1);
    fill(activ_time, activ_time+graph.N, -1);
    map<int,int>  for_plot;
    for(int i = 0; i < number_of_seedsets; i++){
      for(int j = 0; j < spreads[i].size(); j++){
        for(auto k : spreads[i][j]) {
            activ_time[k] = j;
        }
      }
      for(int j = 0; j < learned_spreads[i].size(); j++){
        for(auto k : learned_spreads[i][j]){
            learned_activ_time[k] = j;
        }
      }
      for(int i =0; i<graph.N; i++){
          if(for_plot.find(learned_activ_time[i] - activ_time[i])==for_plot.end()){
            for_plot[learned_activ_time[i]-activ_time[i]] = 1;
          }else{
            for_plot[learned_activ_time[i] - activ_time[i]]++;
          }
      }
    }
    ofstream err_file("time_error.log");

    for(map<int,int>::iterator it = for_plot.begin(); it != for_plot.end(); it++) {
      err_file << it->first << ' ' << (double)it->second / number_of_seedsets/graph.N << endl;
    }
    err_file.close();
}
