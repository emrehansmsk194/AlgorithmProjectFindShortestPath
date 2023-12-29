#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <queue>
#include <string>
using namespace std;
class Graph {
private:
    int V; // number of Vertex
    int E; // number of Edges
    vector<string> labelsOfNodes,cityNames;
    vector<vector<int>> adj;  // adjacency list without weights
    vector<vector<int>>edgeWeights;
    vector<vector<int>>distanceMatrix;


public:
    Graph(int V=0): V(V), E(0), adj(V), edgeWeights(V) {}
    bool adjacent(const string& x, const string& y) {
        int xIndex = find_vertex_index(x);
        int yIndex = find_vertex_index(y);
        if (xIndex != -1 && yIndex != -1) {
            for (int adjVertex : adj[xIndex]) { 
                if (adjVertex == yIndex) {
                    return true;
                }
            }
        }
        return false;
    }
    vector<string>neighbors(const string& x) {
        vector<string>neighborNames;
        int xIndex = find_vertex_index(x);

        if (xIndex != -1) {
            for (int neighborIndex : adj[xIndex]) {
                neighborNames.push_back(labelsOfNodes[neighborIndex]);
            }
        }
        return neighborNames;
    }
    void add_Vertex(string& x) {
        labelsOfNodes.push_back(x);
        adj.push_back({});
        edgeWeights.push_back({});
        V++;
    }

    int find_vertex_index(const string& x) {
        for (int i = 0; i < labelsOfNodes.size(); i++) {
            if (labelsOfNodes[i] == x) {
                return i;
            }
        }
        return -1;

    }

    void add_edge(const string& x, const string& y) { // edges without weight
        int xIndex = find_vertex_index(x);
        int yIndex = find_vertex_index(y);
       
        if (xIndex != -1 && yIndex != -1) {
            int distance = distanceMatrix[xIndex][yIndex];
            adj[xIndex].push_back(yIndex);
            adj[yIndex].push_back(xIndex);
            edgeWeights[xIndex].push_back(distance);
            edgeWeights[yIndex].push_back(distance);
            E++;
        }



    }

   

 




    string get_vertex_value(const string& x) {
        int xIndex = find_vertex_index(x);
        if (xIndex != -1) {
            return labelsOfNodes[xIndex];
        }
        return "";
    }
    void set_vertex_value(const string& x, const string& v) {
        int xIndex = find_vertex_index(x);
        if (xIndex != -1) {
            labelsOfNodes[xIndex] = v;
        }
    }
   
    int numberOfVertices() {
        return V;
    }
    int numberOfEdges() {
        return E;
    }
    void PrintVertices() {
        for (int i = 0; i < labelsOfNodes.size(); i++) {
            cout << labelsOfNodes[i]<<"" << endl;
        }
    }
    void PrintEdges() const {
        for (int i = 0; i < adj.size(); ++i) {
           
           cout << "Vertex " << labelsOfNodes[i] << " has edges with: ";
            for (int j = 0; j < adj[i].size(); ++j) {
                cout << labelsOfNodes[adj[i][j]] << " ";
            }
           cout << endl;
        }
    }
    void PrintWeights() const {
        for (int i = 0; i < edgeWeights.size(); i++) {

            cout << "Vertex " << labelsOfNodes[i] << " has distance with: ";
            for (int j = 0; j < edgeWeights[i].size(); j++) {
                cout << edgeWeights[i][j] << " ";
            }
            cout << endl;
        }

    }

    void AddCitiesToGraph(const string& filename) {
        ifstream adjacent_cities(filename);
        string line;

        if (adjacent_cities.is_open()) {
            while (getline(adjacent_cities, line)) {
                istringstream iss(line);
                string city, adjacentCity;
                getline(iss, city, ','); // Virgüle kadar olan kısmı oku (ana şehir)
                add_Vertex(city);
                while (getline(iss, adjacentCity, ',')) {
                    //graph.add_Vertex(adjacentCity);
                    add_edge(city, adjacentCity);
                }
            }
            adjacent_cities.close();
        }
        else {
            cout << "File couldn't open!" << endl;
        }
    }
    void readCityDistances(const string& filename) {
        ifstream city_distances(filename);
        string line;
        if (city_distances.is_open()) {
            getline(city_distances, line); // İlk satırı oku
            istringstream iss(line);
            string cityName;
            getline(iss, cityName, ';'); // Plate Number
            getline(iss, cityName, ';'); // City Name
            while (getline(iss, cityName, ';')) {
                cityNames.push_back(cityName);
            }
            distanceMatrix.resize(cityNames.size(), vector<int>(cityNames.size(), 0));
            int row = 0;
            while (getline(city_distances, line)) {
                istringstream iss(line);
                string part;
                int col = 0;
                getline(iss, part, ';'); // Plaka numarasını atla
                getline(iss, part, ';'); // Şehir adını atla
                while (getline(iss, part, ';')) {
                    distanceMatrix[row][col] = stoi(part);
                    col++;
                }
                row++;
            }
        }
        city_distances.close();
    }

    void PrintSelectedCity(int cityIndex) { // menu function
        if (cityIndex != -1) {
            cout <<"You selected the "<<  labelsOfNodes[cityIndex]<<endl;
        }
    }
    void kClosestCities(int cityIndex, int k) { // menu function
        vector<int> sortingDistances;
            for (int j = 0; j < distanceMatrix[cityIndex].size(); j++) {
                sortingDistances.push_back(distanceMatrix[cityIndex][j]);
            }
        
        for (int i = 0; i < sortingDistances.size(); i++) { // bubble sort
            for (int j = 0; j < sortingDistances.size() - i - 1; j++) {
                if (sortingDistances[j] > sortingDistances[j + 1]) { // swap
                    int temp = sortingDistances[j];
                    sortingDistances[j] = sortingDistances[j + 1];
                    sortingDistances[j + 1] = temp;
                }
            }
        }
        for (int i = 0; i < k+1; i++) { // print k closest distance
            if (sortingDistances[i] != 0) {            
                cout << sortingDistances[i] << " km: ";
                for (int j = 0; j < distanceMatrix[cityIndex].size(); j++) {
                    if (sortingDistances[i] == distanceMatrix[cityIndex][j]) {                    
                        cout << labelsOfNodes[j] <<(i<sortingDistances.size() ?  "," : "\n");
                        
                    }
                }
              
            }
            cout << endl;
        }

      
    }
    void shortest_path(int src, int dest) { // Dijkstra's algorithm (menu function)
       
        const int MAX_DIST = INT_MAX; // infinite value
        vector<int>dist(V, MAX_DIST); // distances of each node
        vector<int>prev(V, -1);  // previous node
        priority_queue<pair<int, int>,vector <pair<int, int>>,greater<pair<int, int>>> pq; // The pair with the smallest distance is at the top of the priority queue
        pq.push({ 0,src });
        dist[src] = 0;

        while (!pq.empty()) {
            int u = pq.top().second;
            pq.pop();

            for (int i = 0; i < adj[u].size(); i++) {
                int v = adj[u][i];
                int weight = edgeWeights[u][i];

                if (dist[v] > dist[u] + weight) {
                    dist[v] = dist[u] + weight; // find minimum
                    prev[v] = u;
                    pq.push({ dist[v],v });
                }
            }
        }
        if (dist[dest] == MAX_DIST) {
            cout << "Path not found.\n";
        }
        else {
            vector<int>path;
            for (int at = dest; at != -1; at = prev[at]) {
                path.push_back(at);
            }
            reverse(path.begin(), path.end());
            cout << "Distance of the shortest path: " << dist[dest] << "\nPath:";
            for (int i = 0; i < path.size(); i++) {
                cout << labelsOfNodes[path[i]] << (i < path.size() - 1 ? "->" : "\n");
            }

        }


    }






};




int main()
{
    Graph graph;
    graph.readCityDistances("CityDistances.txt");
    graph.AddCitiesToGraph("adjacent cities.txt");
    //graph.PrintVertices();
    //graph.PrintEdges();
    string city, destinationCity;
    int k;
    while (true) { // menu
        cout << "Enter a city(press x to exit.):  ";
        cin >> city;
        if (city == "x" || destinationCity=="x")
            break;
        int city_index = graph.find_vertex_index(city);
        if (city_index == -1) {
            cout << "City not found. Please enter a valid city name." << endl;
            continue;
        }
        graph.PrintSelectedCity(city_index);
        cout << "Enter the value of k for find k closest cities: ";
        cin >> k;
        graph.kClosestCities(city_index, k);
        int dest_index = -1;
        while (dest_index == -1) {
            cout << "Please enter the destination city (press x to exit): ";
            cin >> destinationCity;
            if (destinationCity == "x")
                break;

            dest_index = graph.find_vertex_index(destinationCity);
            if (dest_index == -1) {
                cout << "City not found. Please enter a valid city name." << endl;
            }
        }

        if (destinationCity != "x") {
            graph.shortest_path(city_index, dest_index);
        }
    }



    return 0;
}

