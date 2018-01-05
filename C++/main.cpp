#include "DataGeneration.h"
#include "GradientCompute.h"
#include <vector>


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
    int number_of_seedsets = 4;
    vector<Spread> spreads;
    for(int ttt = 0; ttt < number_of_seedsets; ttt++) {
        int seed_set_size =1;
        Set seed_set = data_gen.generateSeedSet(seed_set_size, graph.N);
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
    for(int i=0; i<graph.N; i++){
      for(int j=0; j<graph.N; j++){
        if(res_matrix[i][j]!=-1){
          cout<<i<<"  "<<j<<"  "<<res_matrix[i][j]<<endl;
        }
      }
    }
}
