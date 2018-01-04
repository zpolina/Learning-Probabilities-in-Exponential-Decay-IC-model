#include <iostream>
#include <fstream>
#include <map>
#include <set>
#include <string>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <cmath>

#include "DataGeneration.h"
#include "types.h"

using namespace std;

int main()
{
    string filename = "../graphs/facebook_graph.txt";
    Graph graph;
    DataGeneration dg;
    dg.readGraph(filename, graph);
    dg.setProbabilities(graph);
    int eta, delta;
    cout << "eta: ";
    cin >> eta;
    cout << "Delta: ";
    cin >> delta;
    int number_of_seedsets;
    cout << "number of seed sets: ";
    cin >> number_of_seedsets;
    for(int ttt = 0; ttt < number_of_seedsets; ttt++) {
        string filename_base = "LogFiles/facebook_log_";
        filename = filename_base + to_string(ttt) + ".txt";
        int seed_set_size;
        cout << "\t--------------------------seed set #" << ttt << "--------------------------" << endl;
        cout << "seed set size: ";
        cin >> seed_set_size;
        Set seed_set = dg.generateSeedSet(seed_set_size, graph.N);
        Spread spread;
        dg.generateSpread(graph, seed_set, spread, eta, delta);

        dg.writeSpreadToFile(filename, spread);
    }
}