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
    int number_of_seedsets = 20;
    vector<Spread> spreads;
    for(int ttt = 0; ttt < number_of_seedsets; ttt++) {
        int seed_set_size =10;
        Set seed_set = data_gen.generateSeedSet(seed_set_size, graph.N);
        Spread spread;
        data_gen.generateSpread(graph, seed_set, spread, eta, delta);
        spreads.push_back(spread);
    }
    GradientCompute gradient(graph, spreads);


}
