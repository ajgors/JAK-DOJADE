#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <climits> 
#include <cstring>
#include "Queue.h"
#include "List.h"
#include "String.h"
#include "Vector.h"
#include "HashMap.h"
#include "MinHeap.h"
#include "UniquePointer.h"
#include "Point.h"
#include "Pair.h"

const int SHOW_ROUTE = 1;
const int bufferSize = 64;


struct hashInt {
    size_t operator()(int x) const{
        x = ((x >> 16) ^ x) * 0x45d9f3b;
        x = ((x >> 16) ^ x) * 0x45d9f3b;
        x = (x >> 16) ^ x;
        return x;
    }
};


struct Vertex {
    int city;
    int distance;
};


struct Graph {
	Vector<List<Vertex>> adjList;
	Hashmap<String, bool, hashString> visited;
	Hashmap<String, int, hashString> stringIndex;
	Hashmap<int, String, hashInt> indexString;
};


void addEdge(Graph& graph, int from, int to, int distance) {
    graph.adjList[from].pushBack({ to, distance });
}


struct Map {
    char** arr;
    size_t width, height;

    Map(size_t wdith, size_t height)
        : width(wdith), height(height)
    {
        arr = new char* [height];
        for (int i = 0; i < height; i++) {
            arr[i] = new char[width];
        }
    }

    ~Map() {
        for (int i = 0; i < height; i++) {
            delete[] arr[i];
        }
        delete[] arr;
    }
};


int findCityName(Graph& graph, Map& map, int x, int y) {
    char current = map.arr[y][x];
    String city(bufferSize);

    const int DIRECTIONS_SIZE = 8;
    Pair<int, int> directions[DIRECTIONS_SIZE] = { {0,-1}, {0,1}, {1,0}, {-1,0}, {-1,-1}, {1,-1}, {-1,1}, {1,1} };

    for (int i = 0; i < DIRECTIONS_SIZE; i++) {
        int newX = x + directions[i].first;
        int newY = y + directions[i].second;
        if (newX >= 0 && newX < map.width && newY >= 0 && newY < map.height) {
            current = map.arr[newY][newX];
            if (current != '.' && current != '#' && current != '*') {
                //Go to the start of the word
                while (current != '.' && current != '#' && current != '*') {
                    newX -= 1;
                    if (newX >= 0 && newX < map.width) {
                        current = map.arr[newY][newX];
                    }
                    else {
                        break;
                    }
                }
                newX += 1;
                current = map.arr[newY][newX];
                //Read word
                while (current != '.' && current != '#' && current != '*') {
                    city += current;
                    newX += 1;
                    if (newX >= 0 && newX < map.width) {
                        current = map.arr[newY][newX];
                    }
                    else {
                        break;
                    }
                }

                if (city != "") {
					bool& visited = graph.visited[city];
                    if (!visited) {
                        visited = true;
                        graph.adjList.pushBack(List<Vertex>());
						int index = graph.adjList.size() - 1;
					    graph.stringIndex[city] = index;
                        graph.indexString[index] = city;
                        return index;
                    }
                    else {
                        return graph.stringIndex[city];
                    }
                }
            }
        }
    }
    return -1;
}


class CurLocation {
public:
    int y;
    int x;
    int distance;

    CurLocation(int y, int x, int distance)
        : y(y), x(x), distance(distance)
    {
    }

    bool operator<(const CurLocation& other) const {
        return distance > other.distance;
    }
};


int findConnected(Hashmap<Point, bool, hashPoint>& visited, Graph& graph, Map& map, int x, int y) {
    int sourceCity = findCityName(graph, map, x, y);
    List<Point> found;
    
    Queue<CurLocation> queue;
    CurLocation source(y, x, 0);
    queue.enqueue(source);
    
    visited.put({ y,x }, true);
    map.arr[y][x] = '.';
    char current;

    const int DIRECTIONS_SIZE = 4;
    Pair<int, int> directions[DIRECTIONS_SIZE] = { {0,-1}, {0,1}, {-1,0}, {1,0} };

    while (!queue.empty()) {
        CurLocation p = queue.dequeue();

        if (map.arr[p.y][p.x] == '*') {
            visited.clear();
            
            addEdge(graph, sourceCity, findCityName(graph, map, p.x, p.y), p.distance);
            map.arr[p.y][p.x] = '.';
            found.pushBack({ p.y, p.x });
            continue;
        }

        for (int i = 0; i < DIRECTIONS_SIZE; i++) {
            Point tmp;
            tmp.x = p.x + directions[i].first;
            tmp.y = p.y + directions[i].second;
            if (visited[tmp] == false && tmp.y >= 0 && tmp.y < map.height && tmp.x >= 0 && tmp.x < map.width) {
                current = map.arr[tmp.y][tmp.x];
                if (current == '#' || current == '*') {
                    queue.enqueue(CurLocation(tmp.y, tmp.x, p.distance + 1));
                    visited.put(tmp, true);
                }
            }
        }
    }
    for (auto& point : found) {
        map.arr[point.y][point.x] = '*';
    }
    map.arr[y][x] = '*';
    visited.clear();

    return -1;
}


void addCities(Graph& graph, Map& map) {
    Hashmap<Point, bool, hashPoint> visited(100);
    for (int y = 0; y < map.height; y++) {
        for (int x = 0; x < map.width; x++) {
            if (map.arr[y][x] == '*') {
                findConnected(visited, graph, map, x, y);
            }
        }
    }
}


struct cityInfo {
    int distance = INT_MAX;
    int previous;
    bool visited = false;
};


void dijkstra(Graph& graph, int from, int to, int type) {

    if (from == to) {
        printf("0\n");
        return;
    }

    Vector<cityInfo> cities(graph.adjList.size());
    cities[from].distance = 0;

    MinHeap<Pair<int, int>> unvisited(8000);
    unvisited.push({ 0,from });

    while (!unvisited.empty()) {
        int vertex = unvisited.top().second;
        
        cityInfo& current = cities[vertex];
        if (current.visited) {
            continue;
        }

        current.visited = true;

        for (auto& adjacentVertex : graph.adjList[vertex]) {
            cityInfo& adjCity = cities[adjacentVertex.city];
            if (!adjCity.visited) {
                int newDistance = cities[vertex].distance + adjacentVertex.distance;
                if (newDistance < adjCity.distance) {
                    adjCity.distance = newDistance;
                    adjCity.previous = vertex;
                    unvisited.push({ newDistance, adjacentVertex.city });
                }
            }
        }
    }

    std::cout << cities[to].distance << " ";

    if (type == SHOW_ROUTE) {
        List<int> route;
        int vertex = cities[to].previous;

        while (vertex != from) {
            route.pushBack(vertex);
            vertex = cities[vertex].previous;
        }

        for (auto it = route.rbegin(); it != route.rend(); ++it) {
            std::cout << graph.indexString[*it].c_str() << " ";
        }
    }
    std::cout << "\n";
}


Map* createMap() {
    size_t width, height;
    std::cin >> width >> height;
    Map* map = new Map(width, height);
    return map;
}


void loadMap(Map& map) {
    for (int y = 0; y < map.height; y++) {
        for (int x = 0; x < map.width; x++) {
            char tmp = getchar();
            if (tmp == '\n') {
                x--;
                continue;
            };

            map.arr[y][x] = tmp;
        }
    }
}


void getInput(String& from, String& to, int& number) {
    char buff[bufferSize];
    if (fgets(buff, bufferSize, stdin) != nullptr) {
        // Find first space
        char* space1 = strchr(buff, ' ');
        if (space1) {
            *space1 = '\0';
            from = buff;
            ++space1;
        }

        // Find second space
        char* space2 = strchr(space1, ' ');
        if (space2) {
            *space2 = '\0';
            to = space1;
            ++space2;
            number = atoi(space2);
        }
    }
}


void addAirlines(Graph& graph, Map& map) {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int flights;
    std::cin >> flights;

    if (flights > 0) {
        getchar();
    }
    while (flights > 0) {
        String from, to;
        int distance = 0;
        getInput(from, to, distance);
        addEdge(graph, graph.stringIndex[from], graph.stringIndex[to], distance);
        --flights;
    }
}


void answerInput(Graph& graph) {
    size_t n;
    std::cin >> n;
    if (n > 0) {
        getchar();
    }

    while (n > 0) {
        String from, to;
        int type = 0;
        getInput(from, to, type);
        dijkstra(graph, graph.stringIndex[from], graph.stringIndex[to], type);
        n--;
    }
}

#include <chrono>
int main() {
    Graph graph;
    UniquePointer<Map> map(createMap());
    loadMap(*map);
	auto start = std::chrono::high_resolution_clock::now();
    addCities(graph, *map);
    addAirlines(graph, *map);
    answerInput(graph);
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> diff = end - start;
	std::cout << diff.count() << " s\n";
	return 0;
    
}