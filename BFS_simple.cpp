#include <iostream>
#include <queue>
#include <fstream>
#include <string>
using namespace std;

struct Edge
{
    Edge(int c) : neigh_id(c) { }
    int neigh_id;
    int label = 0;  //1 is tree-edge, 2 is cross-edge
};

struct Node
{
    string name;   //every node has a name like "a", "b", "c"... 
    int label = 0;   //0 is unvisited, then it will get a nonzero BFS-label 
    vector<Edge*> neighs;    //each vertex has a vector-array of neighbors
};

int n; // num_vertices
int cnt = 0;   //the counter to be used in BFS-based node ordering
Node* nodes;   //we use index 0, 1, 2 to track vertices, each one with name, label, and neighbors

void bfs(int i)   //algorithm is straight from Levitin's book (I added edge-labels)
{
    queue<int> q;
    cnt++;
    nodes[i].label = cnt;
    q.push(i);
    while (!q.empty())
    {
        int v = q.front();
        for (auto& e : nodes[v].neighs)
        {
            int w = e->neigh_id;
            if (nodes[w].label == 0)   //if neighbor is an unvisited vertex
            {
                cnt++;
                nodes[w].label = cnt;  //label it as visited (with the order)
                q.push(w);
                e->label = 1;          //label the edge as tree edge
            }
            else
                e->label = 2;
        }
        q.pop();
    }
}

void cleanupMemory()    //this part is not important for the purposes of BFS or the Algorithms course
{
    for (int v = 0; v < n; v++)
    {
        for (auto e_it = nodes[v].neighs.begin(); e_it < nodes[v].neighs.end(); ++e_it)
        {
            delete *e_it;   //remove the edge objects from the heap
            *e_it = nullptr;  //we don't want dangling pointers
        }
        nodes[v].neighs.clear();  //remove the null pointers from the vector
    }
    delete[] nodes;
}

int main()
{
    string s;
    int v, w;  //nodes v, w as in the book, edges will be from v to w

    string filename;
    if (0)
        filename = "karate.paj";    //Download from http://networkdata.ics.uci.edu/data/karate/karate.paj
    else
        filename = "levitin.paj";   //the example on the DFS-BFS slides

    ifstream infile(filename);
    infile >> s >> n;              //Read the number of nodes

    nodes = new Node[n];           //Create nodes for vertices

    for (int i = 0; i < n; i++)    //Read each node info line by line
    {
        infile >> v >> s;         //Node index and the name
        v--;                      //We want indices to start from 0 (not 1)
        nodes[v].name = s;
    }

    infile >> s;               //Skip the line in the file that contains string "*Edges"
    while (infile >> v >> w)   //Read node index pairs
    {
        v--; w--;              //We like to start indices from 0
        Edge* edge_v_w = new Edge(w);   //v to w
        Edge* edge_w_v = new Edge(v);   //w to v because this is an undirected graph
        nodes[v].neighs.push_back(edge_v_w);
        nodes[w].neighs.push_back(edge_w_v);
    };

    //Do "bfs" for each connected component
    for (int i=0; i<n; i++)      //this loop will run only one iteration for connected graphs
        if (nodes[i].label == 0) 
            bfs(i);

    for (int v = 0; v < n; v++)    //print the BFS ordering
        cout << nodes[v].name << ": " << nodes[v].label << endl;

    cleanupMemory();    //who cares about this part :)
}
