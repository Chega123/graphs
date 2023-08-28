#include <iostream>
#include <vector>
#include <list>
#include <stack>
#include <queue>
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
    void delete_node(Node* a) {
        int i = 0;
        while (!a->edges.empty()) {
            delete_edge(a->edges.front());;
        }
        while (Nodes[i] != a && i < Nodes.size()) {
            i++;
        }

        Nodes.erase(Nodes.begin() + i);
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
};
//en clases esta de default en private

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
    CNode<Cgraph<int, int>>* A = grafo.insertar_node(0);
    CNode<Cgraph<int, int>>* B = grafo.insertar_node(1);
    CNode<Cgraph<int, int>>* C = grafo.insertar_node(2);
    CNode<Cgraph<int, int>>* D = grafo.insertar_node(3);
    Cedge<Cgraph<int, int>>* A1 = grafo.insertar_edge(A, B, 1, 1);
    Cedge<Cgraph<int, int>>* A2 = grafo.insertar_edge(B, C, 2, 1);
    Cedge<Cgraph<int, int>>* A3 = grafo.insertar_edge(C, D, 3, 1);
    Cedge<Cgraph<int, int>>* A4 = grafo.insertar_edge(D, A, 4, 1);

    pair<vector<int>, vector<CNode<Cgraph<int, int>>*>> result = grafo.dijkstra(A);
    vector<int> dist = result.first;
    vector<CNode<Cgraph<int, int>>*> prev = result.second;
    for (int i = 0; i < dist.size(); i++) {
        cout << "Distance from node " << A->value << " to node " << i << ": " << dist[i] << endl;
        vector<CNode<Cgraph<int, int>>*> path = grafo.get_path(prev, grafo.Nodes[i]);
        cout << "Path: ";
        for (CNode<Cgraph<int, int>>* node : path) {
            cout << node->value << " ";
        }
        cout << endl;
    }

    return 0;
}
