#ifndef TYPES_H
#define TYPES_H

#include <vector>
#include <set>

class Edge {
public:
    int end;
    double prob;
    Edge(double e) {
        end = e;
        prob = 0;
    }
    bool operator < (const Edge& rhs) const
    {
        return (this->end < rhs.end);
    }
};

typedef std::vector< Edge > AdjList;
typedef std::set<int> Set;
typedef std::vector< Set > Spread;

class Graph {
public:
    AdjList* adj_list;
    int N;
};

#endif