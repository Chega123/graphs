#include <iostream>
#include <vector>
#include <list>
#include <stack>
#include <queue>
#include <random>
#include <algorithm>
#include <ctime>
#include <cmath>
using namespace std;

template<class G>
class CNode;

template <class G>
class Cedge;

//contexto: 

template <class NN, class EE>
class Cgraph
{
public:
    typedef Cgraph<NN, EE> G;
    typedef CNode<G> Node;
    typedef Cedge<G> Edge;
    typedef NN N;
    typedef EE E;
    vector<Node*> Nodes;


    Node* insertar_node(E value) {
        Node* temp = new Node(value);
        Nodes.push_back(temp);
        return temp;
    }

    Edge* insertar_edge(Node* a, Node* b, E e, bool d) {
        Edge* edge = new Edge(a, b, e, d);
        a->edges.push_back(edge);
        if (d == 1) {
            b->edges.push_back(edge);
        }
        return edge;
    }

    void delete_edge(Edge* aris) {
        aris->nodes[0]->edges.remove(aris);
        aris->nodes[1]->edges.remove(aris);
        delete aris;
    }
    void delete_node(vector<vector<Node*>>& matrix,Node* a) {
        int i = 0;
        while (!a->edges.empty()) {
            delete_edge(a->edges.front());;
        }
        while (Nodes[i] != a && i < Nodes.size()) {
            i++;
        }

        //recorre la matriz para buscar los eliminados y poner nullptr en esos espacios
        Nodes.erase(Nodes.begin() + i);
        for (int row = 0; row < matrix.size(); ++row) {
            for (int col = 0; col < matrix[row].size(); ++col) {
                if (matrix[row][col] == a) {
                    matrix[row][col] = nullptr;
                    break;
                }
            }
        }
        delete a;
    };
    void DFS(Node* node, vector<bool>& visited) {
        visited[node->value] = true;
        cout << node->value << " ";
        for (Edge* edge : node->edges) {
            if (edge->dir == 0 && edge->nodes[0] != node) {
                continue;
            }

            Node* next;
            if (edge->nodes[0] == node) {
                next = edge->nodes[1];
            }
            else {
                next = edge->nodes[0];
            }
            if (!visited[next->value]) {

                DFS(next, visited);
            }
        }
    }

    pair<vector<E>, vector<Node*>> dijkstra(Node* source) {
        vector<E> dist(Nodes.size(), numeric_limits<E>::max());
        vector<Node*> prev(Nodes.size(), nullptr);
        dist[source->value] = 0;

        priority_queue<pair<E, Node*>, vector<pair<E, Node*>>, greater<pair<E, Node*>>> pq;
        pq.push(make_pair(0, source));

        while (!pq.empty()) {
            Node* u = pq.top().second;
            pq.pop();

            for (Edge* edge : u->edges) {
                Node* v = edge->nodes[0] == u ? edge->nodes[1] : edge->nodes[0];
                E weight = edge->value;
                if (dist[v->value] > dist[u->value] + weight) {
                    dist[v->value] = dist[u->value] + weight;
                    prev[v->value] = u;
                    pq.push(make_pair(dist[v->value], v));
                }
            }
        }

        return make_pair(dist, prev);
    }
    vector<Node*> get_path(vector<Node*>& prev, Node* target) {
        vector<Node*> path;
        Node* u = target;
        while (u != nullptr) {
            path.push_back(u);
            u = prev[u->value];
        }
        reverse(path.begin(), path.end());
        return path;
    }
//crea la matriz de nodos
    void matrix_nodes(vector<vector<Node*>>& matrix) {
        for (auto& row : matrix) {
            for (auto& column : row) {
                column = insertar_node(1);
            }
        }
    };
//imprime nodos 
    void print_nodes(vector<vector<Node*>>& matrix) {
        for (auto& row : matrix) {
            for (auto& column : row) {
                if (column == nullptr) cout << "0 ";
                else  cout <<column->value << " ";
            }
            cout << endl;
        }
    };
// los conecta con aristas
    void connect_nodes(vector<vector<Node*>>& matrix) {
        for (int i = 0; i < matrix.size(); ++i) {
            for (int j = 0; j < matrix[i].size(); ++j) {
                Node* actual_node = matrix[i][j];

                if (i + 1 < matrix.size()) {
                    Node* node_down = matrix[i + 1][j];
                    insertar_edge(actual_node, node_down, 1, 1);
                }
                if (j + 1 < matrix[i].size()) {
                    Node* node_right = matrix[i][j + 1];
                    insertar_edge(actual_node, node_right, 1, 1);
                }
                if (i + 1 < matrix.size() && j + 1 < matrix[i].size()) {
                    Node* right_diagonal_node = matrix[i + 1][j + 1];
                    insertar_edge(actual_node, right_diagonal_node, 1, 1);
                }
                if (i + 1 < matrix.size() && j - 1 < matrix[i].size()) {
                    Node* left_diagonal_node = matrix[i + 1][j - 1];
                    insertar_edge(actual_node, left_diagonal_node, 1, 1);
                }
            }
        }
    }
/Snap
    void ThanosSnap(vector<vector<Node*>>& matrix, float percentage) {
        // Calculate number of victims
        int victims = floor(matrix.size() * matrix[0].size() * percentage / 100);

        // Collect all node indices
        std::vector<std::pair<int, int>> indices;
        for (int i = 0; i < matrix.size(); ++i) {
            for (int j = 0; j < matrix[i].size(); ++j) {
                indices.emplace_back(i, j);
            }
        }

        // Shuffle the indices randomly
        std::shuffle(indices.begin(), indices.end(), std::default_random_engine(std::time(nullptr)));

        // Eliminate victims
        for (int i = 0; i < victims; ++i) {
            int row = indices[i].first;
            int col = indices[i].second;
            delete_node(matrix,matrix[row][col]);
        }
        if (percentage == 50) { cout << "Thanos lo logro" << endl << endl; }
        else if (percentage > 50) { cout << "Thanos se paso con el chasquido" << endl << endl; }
        else { cout << "thanos chasqueo maso :/" << endl << endl; }

    }
};

    template<class G>
    class CNode {

    public:
        typedef typename G::N N;
        typedef typename G::Edge Edge;
        N value;
        list<Edge*> edges;

        CNode(N valor) {
            value = valor;
        }
    };

    template <class G>
    class Cedge {

    public:
        typedef typename G::E E;
        typedef typename G::Node Node;
        E value;
        Node* nodes[2];
        bool dir;

        Cedge(Node* a, Node* b, E e, bool d) {
            nodes[0] = a;
            nodes[1] = b;
            value = e;
            dir = d;
        }
    };


    

    int main()
    {
        Cgraph<int, int> grafo;
        cout << "inserta filas y luego columnas " << endl;
        int rows, columns;
        cin >> rows >> columns;
        vector<vector<CNode<Cgraph<int, int>>*>> matrix(rows, vector<CNode<Cgraph<int, int>>*>(columns));
        grafo.matrix_nodes(matrix);
        grafo.connect_nodes(matrix);
        grafo.print_nodes(matrix);
        cout << "porcentaje a eliminar";
        int percentage;
        cin >> percentage;
        grafo.ThanosSnap(matrix,percentage);
        grafo.print_nodes(matrix);

        return 0;
    }





    //pair<vector<int>, vector<CNode<Cgraph<int, int>>*>> result = grafo.dijkstra(A);
    //vector<int> dist = result.first;
    //vector<CNode<Cgraph<int, int>>*> prev = result.second;
    //for (int i = 0; i < dist.size(); i++) {
    //    cout << "Distance from node " << A->value << " to node " << i << ": " << dist[i] << endl;
    //    vector<CNode<Cgraph<int, int>>*> path = grafo.get_path(prev, grafo.Nodes[i]);
    //    cout << "Path: ";
    //    for (CNode<Cgraph<int, int>>* node : path) {
    //        cout << node->value << " ";
    //    }
    //    cout << endl;
    //
