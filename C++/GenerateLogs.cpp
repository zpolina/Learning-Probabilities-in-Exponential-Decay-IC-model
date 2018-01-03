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

void readGraph(string filename, Graph &graph)
{
    string line;
    ifstream file(filename);
    getline(file, line);

    //reading the first line (number of nodes)
    stringstream stream(line);
    stream >> graph.N;
    graph.adj_list = new AdjList[graph.N];

    while(getline(file, line)) {
        stringstream stream(line);
        int starting_node, ending_node;
        stream >> starting_node;
        Set adj_set;
        while(stream >> ending_node)
            adj_set.insert(ending_node);
        for(Set::iterator it = adj_set.begin(); it != adj_set.end(); it++) {
            graph.adj_list[starting_node].push_back(Edge(*it));
        }
    }
}

void writeSpreadToFile(string filename, Spread spread) {
    /*ofstream file(filename);
    file << spread.size() << endl;
    int a;
    while(cin >> a) {
        cout << spread[a].size() << endl;
    }
    file.close()*/
}

void setProbabilities(Graph graph, int prob_min=0, int prob_max=1)
{
    srand(time(NULL));

    for(int i = 0; i < graph.N; i++)
        for(int j = 0; j < graph.adj_list[i].size(); j++)
            graph.adj_list[i][j].prob = (double) rand() / (RAND_MAX);
}

Set generateSeedSet(int size, int num_nodes)
{
    Set res;

    srand(time(NULL));
    while(res.size() < size){
        int r;
        do{
            r = rand() % num_nodes;
        } while(res.find(r) != res.end());
        res.insert(r);
    }

    return res;
}

void generateSpread(Graph graph, Set seed_set, Spread &D, int eta = 4, int delta=24)
{
    bool* is_active = new bool[graph.N];
    memset(is_active, 0, graph.N * sizeof(bool));
    bool stable = false;
    int cur_time = 0;

    D.push_back(Set());
    for(Set::iterator it = seed_set.begin(); it != seed_set.end(); it++)
        D[0].insert(*it);

    srand(time(NULL));
    while(stable == false) {
        D.push_back(Set());
        stable = true;
        for(int tau = 0; tau < delta; tau++) {
            if(cur_time - tau < 0)
                break;
            for(Set::iterator it = D[cur_time-tau].begin();
                    it != D[cur_time-tau].end(); it++) {
                int node = (*it);
                for(int j = 0; j < graph.adj_list[node].size(); j++) {
                    int adj = graph.adj_list[node][j].end;
                    if(is_active[adj] == false){
                        double coin = (double) rand() / (RAND_MAX);
                        stable = false;
                        if(coin < graph.adj_list[node][j].prob * exp(-eta*tau)){
                            is_active[adj] = true;
                            D[cur_time+1].insert(adj);
                        }
                    }
                }
            }
        }
        cur_time = cur_time+1;
    }

    while(D.back().size() == 0)
        D.pop_back();
}

int main()
{
    string filename = "../graphs/facebook_graph.txt";
    Graph graph;
    readGraph(filename, graph);
    setProbabilities(graph);
    int eta, delta;
    cout << "eta: ";
    cin >> eta;
    cout << "Delta: ";
    cin >> delta;
    int number_of_seedsets;
    cout << "number of seed sets: ";
    cin >> number_of_seedsets;
    for(int ttt = 0; ttt < number_of_seedsets; ttt++) {
        int seed_set_size;
        cout << "\t--------------------------seed set #" << ttt << "--------------------------" << endl;
        cout << "seed set size: ";
        cin >> seed_set_size;
        Set seed_set = generateSeedSet(seed_set_size, graph.N);
        Spread spread;
        generateSpread(graph, seed_set, spread, eta, delta);

        cout << spread.size() << endl;
        int a;
        while(cin >> a) {
            cout << spread[a].size() << endl;
        }
    }
    cout << graph.N << endl;
    int a, b;
    while(cin >> a>>b)
        cout << graph.adj_list[a][b].prob << endl;
}