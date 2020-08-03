/*
 * The class Graph has a member "graph" which is used to store the randomly generated graph.
 * Here the vertex 0 is mapped to 1000.This value can be changed by changing the global variable offset.
 * After the Dijkstera algo is used to find the minimim distance between "start" and "stop" node/vertex in the graph the usefull data is stored in a vector ListOfVertices.
 * Each element of ListOfVertices contains the vertex number, the minimim distance of that vertex from start and the previous vertex number which had to traversed to reach the vertex.
 */
#include <iostream> 
#include <queue>
#include <limits>
#include <vector>

using namespace std; 
const int offset = 1000;
// Class Node to store vertices data. 
class Node 
{ 
    int vertex; 
    double distanceFromStart;
    int preVertex;
    bool visit = false;
    public: 
    Node (int _vertex = 0, double _distanceFromStart = numeric_limits<double>::max(), int _preVertex = numeric_limits<int>::max() )
    { 
        distanceFromStart = _distanceFromStart;
        vertex = _vertex;
        preVertex = _preVertex;
    }
    
    double get_dfs() { return distanceFromStart; } 
    void set_dfs(double distance) { distanceFromStart = distance; }
    int get_preVertex(){ return preVertex; }
    void set_preVertex(int n) { preVertex = n; }
    int get_vertex() { return vertex; }
    void set_vertex(int _vertex) { vertex = _vertex; }
    void set_visit()    { visit = true; }
    bool get_visit()     { return ( (visit == true) ? true : false );}
};
// Class Graph to store randomly generated graph.  
class Graph
{
public:
    int size;   //Declaring size/ no of vertices of the graph.
    double density,min,max; //Declaring the edge density of the graph.
    double** graph;
    Graph(int _size =0, double _density = 0.0, double _min =0.0, double _max = 0.0) //Constructor to construct any object of type Graph.
    {
        size = _size;
        density = _density;
        min = _min;
        max = _max;
    }
    void generate_graph(); //Method to generate a random graph
    void adjacency_list();  //Method to display a graph in adjacency_list format.
    void adjacency_matrix();    //Method to display a graph in adjacency_matrix format.
    int V() { return size; }    //Returns the no of vertices in graph.
    int E();    //Returns the number of edges of a given graph.
    bool adjacent(int x,int y)  //Checks  weather vertices x,y are connected.
    {  return (graph[x - offset][y - offset] > 0 ? true : false ); }

    void remove(int x,int y)
    {
        x -= offset;
        y -= offset;
        graph[x][y] = graph[y][x] = 0;
    }

    double get_edgeValue(int x, int y)
    {
        x -= offset;
        y -= offset;
        return graph[x][y];
    }
    
    void set_edgeValue(int x, int y,double value)
    {
        x -= offset;
        y -= offset;
        graph[x][y] = graph[y][x] = value;
    }
    
    void add(int x,int y)
    {
        x -= offset;
        y -= offset;
        srand(time(NULL));  //Seeding from system clock.
        graph[x][y] = graph[y][x] = (rand() / (RAND_MAX / (max - min))) + min;  //Assigns the weight to the edges in the given range.
    }
    
    void printNeighbours( int x )
    {
        cout <<x <<"->[ ";
        for(int j = 0, i = x - offset;j < size; ++j)
        {
            if(graph[i][j] > 0)
                cout << j+offset <<",";
        }
        cout <<"]" <<endl;
    }
};

int Graph:: E() //Returns the number of edges of a given graph.
{
        int edge_count=0;
        for(int i = 0;i < size; ++i)
            for(int j = 0;j < i; ++j)
                if (graph[i][j] > 0)
                    ++edge_count;
        return (edge_count);
}

void Graph :: adjacency_matrix()    //Method to print adjacency matrix of the graph.
{
    cout << "The adjacency matrix is as follows:\n";
    for(int i = 0; i<size; ++i) 
    {
        for(int j=0;j<size;++j)
            cout << graph[i][j] << " ";
        cout <<endl<<endl;
    }
}
    
void Graph :: adjacency_list()    //Method to print adjacency list.
{
    cout << "The adjacency list is as follows:\n";
    for(int i = 0; i<size; ++i) 
    {
        cout << i+offset << "--> [ ";
        for(int j=0;j<size;++j)
            if (graph[i][j] > 0)
                cout <<"[" <<j+offset <<"," <<graph[i][j] <<"] ";
        cout << "]" << endl<<endl;
    }
}

void Graph :: generate_graph() 
{
    srand(time(NULL));  //Seeding from system clock.
    graph = new double*[size];    //Generating 1 * size order array.
    for(int i = 0; i<size; ++i) 
        graph[i] = new double[size];  //Inserting rows.
    for(int i = 0; i<size; ++i) 
        for(int j=0;j<size;++j)
        {    
            if(i==j) 
                graph[i][j] = 0;    //Diagonal elements are made 0 to avoid looping.
            //Generating a random number between 0....1.00.
            //If random value is less than density stick edges between i and j non diagonal vertices.
            else if (((rand() % 100)/100.0) <= density)
                graph[i][j] = graph[j][i] = (rand() / (RAND_MAX / (max - min))) + min;  //Assigns the weight to the edges in the given range.
        }
}
// To compare two vertices 
class myComparator 
{ 
    public: 
    double operator() (Node& v1, Node& v2) { return v1.get_dfs() > v2.get_dfs(); } 
}; 

class PriorityQueue
{
public:
    priority_queue <Node, vector<Node>, myComparator > PQ;
    void minPriority()  { PQ.pop(); }   //Removes the minimum element.
    void insert(Node _element)  { PQ.push(_element); }  //Inserts a new member into queue.
    Node top() { return PQ.top(); }     //Get the top/min element of the queue.
    int size() { return PQ.size(); }    //Get the size of the queue.
    bool empty() { return ( ( PQ.empty() ) ? true : false );}   //Check weather queue is empty.
};

class ShortestPath
{
    Graph g;
    int start,stop;
    int size;
public:
    ShortestPath(const Graph& _graph)
    {
        g =_graph;
        size = g.V();
    }
    void path(int _start, int _stop, vector<Node> &ListOfVertices );    //Method to find the best minimum distance path between start and stop.
    double path_size(int start, int stop, vector<Node> &ListOfVertices ) { return ( ListOfVertices[ stop - offset ].get_dfs() ); }  //Gives the minimum path distance/weight.
};

void ShortestPath :: path( int _start, int _stop, vector<Node> &ListOfVertices )
{
    start = _start;
    stop = _stop;
    ListOfVertices[ start - offset ].set_dfs(0);   //Set start node distance from start to zero.
    ListOfVertices[ start - offset ].set_preVertex(start);   //Set start node pre node as itself.
    PriorityQueue unvisited;   
    for(int i =0; i < size; ++i)    //Initilize a priority_queue of unvisited nodes. 
        unvisited.insert(ListOfVertices[i]);
    int current_vertex;
    do 
    { 
        Node Current = unvisited.top();
        current_vertex = Current.get_vertex(); //Offset is reduecd by 1000.
        if ( ListOfVertices[ current_vertex - offset ].get_visit() )    //To screen out the obselete nodes in the queue unvisited after the node dfs is updated.
            continue;
        else
        {
            vector<int> neighbour;
            for(int j = offset;j < (offset + size); ++j)
                    if(g.adjacent(current_vertex,j))
                        neighbour.push_back(j);
                    
            for(int i = 0;i < neighbour.size(); ++i)
            {
                int adj_vertex = neighbour[i];
                double adj_dfs = ListOfVertices[adj_vertex - offset].get_dfs();
                if( ListOfVertices[ adj_vertex - offset ].get_visit() )//Screen out visited nodes.
                    continue;
                else
                {
                    double new_adj_dfs;
                    new_adj_dfs =  g.get_edgeValue(current_vertex, adj_vertex)+ Current.get_dfs();
                    if( new_adj_dfs < adj_dfs )
                    {
                        ListOfVertices[adj_vertex - offset].set_dfs(new_adj_dfs);
                        ListOfVertices[adj_vertex - offset].set_preVertex(current_vertex );
                        //Push the updated values of the neighbours.
                        unvisited.insert( ListOfVertices[adj_vertex - offset] );
                    }   
                }   
            }
        }
        unvisited.minPriority();
    }while ( ( current_vertex ) != stop );   //Go on untill the stop node is travesed.
    
    for(int i = ListOfVertices[stop - offset].get_vertex(); i != start; i = ListOfVertices[i - offset].get_preVertex() )
        cout << i <<"<--";
    cout << start <<endl;
}
//Driver code.
int main()
{
    int size, start, stop;
    double density, min, max;
    cout << "Enter the number of vertices: "; 
    cin >> size;
    cout << "Enter the edges density between 0.00....1.00 :"<<" ";
    cin >> density;
    cout << "Enter the minimim allowed positive distance value: "; 
    cin >> min;
    cout << "Enter the maximum allowed positive distance value: "; 
    cin >> max;
    
    Graph random_graph(size, density, min, max); //Object declaretion.
    random_graph.generate_graph(); //Generating a undiretced graph of order size * size.
    //    random_graph.adjacency_matrix();   //Display adjacency_matrix.
    cout << "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$" << endl;
    random_graph.adjacency_list();   //Display adjacency_list. 
    cout << "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$" << endl;
    ShortestPath pathOf_random_graph(random_graph);
    cout << "Enter the source/start vertex number from 1000 to " << (offset + size - 1) <<": ";
    cin >> start;
    cout << "Enter the destination/stop vertex number which is not equal to start vertex: ";
    cin >> stop;
    vector<Node>ListOfVertices(size,Node());
    for(int i =0; i < size; ++i)
        ListOfVertices[i].set_vertex(i + offset);   //Assigning vertex/ node names from 'offset' value.
    cout<< "The minimum distance path between "<< start << " and " <<stop <<" is:"<<endl;
    pathOf_random_graph.path(start,stop,ListOfVertices);
    cout<< "The weight associated with the minimum distance path is: ";
    cout << pathOf_random_graph.path_size(start, stop, ListOfVertices) << "Units."<< endl;
}
