#include "DataGeneration.h"
#include "GradientCompute.h"
#include <vector>


using namespace std;
int main()
{
    string filename = "../graphs/facebook_graph.txt";
    Graph graph;
    DataGeneration data_gen;
    data_gen.readGraph(filename, graph);
    data_gen.setProbabilities(graph);
    int eta = 5, delta = 24;
    int number_of_seedsets = 2;
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
    }
    for (int i =0; i < graph.N; i++){
      for(int j = 0; j< graph.adj_list[i].size(); j++){
        matrix[graph.adj_list[i][j].end][i] = 0.5;///should be random initial values of probabilities
      }
    }

    GradientCompute gradient(matrix, graph, spreads, eta, delta, number_of_seedsets, graph.N);
    gradient.GradientDescent(5, 0.001);
}
