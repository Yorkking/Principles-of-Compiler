#ifndef GRAPH_H
#define GRAPH_h 1

#include<string>
#include<vector>
#include<unordered_map>
#include<map>
#include<stack>

#include"util.h"

class Node{
public:
    Node(int id = 0):id(id){
        edges.clear();
    }
    ~Node(){

    }
    void setID(int id){
        this->id = id;
    }
    Node& operator=(const Node& rhs){
        this->id = rhs.id;
        this->edges.clear();

        for(auto x:rhs.edges){
            this->edges.push_back(x);
        }
        return *this;
    }
    void addEdgeTo(const Node& node, char ch){
        edges.push_back(std::make_pair(node.id,ch));
    }
    int id;
    std::vector<std::pair<int,char>> edges;
};

class Graph{
public:
    Graph(){
        graph.clear();
    }
    Graph(char ch,int& maxID){
        init(ch,maxID);
    }
    void init(char ch,int& maxID){
        graph.clear();
        maxID++;
        Node startNode(maxID);
    
        maxID++;
        Node endNode(maxID);

        startNode.addEdgeTo(endNode,ch);

        start = graph.size();
        graph.push_back(startNode);
        end = graph.size();
        graph.push_back(endNode);
    }
    ~Graph(){
        
    }
    Graph& operator=(const Graph& rhs){
        this->start = rhs.start;
        this->end = rhs.end;
        this->graph.clear();
        for(auto x:rhs.graph){
            this->graph.push_back(x);
        }
        return *this;
    }
    void copy(int& maxID, Graph& rhs){

    }
    void display()const{
        cout<<"startID: "<<graph[start].id<<" endID: "<<graph[end].id<<endl;
        for(auto x:graph){
            cout<<"id: "<<x.id<<endl;
            for(auto edge: x.edges){
                cout<<"edge: "<<edge.first<<" "<<edge.second<<endl;
            }
            cout<<endl;
        }
    }

    void merge(char op, Graph& g, int& maxID){
        Node startNode;
        Node endNode;
        switch (op)
        {
        case '*': case '.' : case '|':
            basisMerge(op,g,maxID);
            break;
        case '?':
            graph[start].addEdgeTo(graph[end],EPSILON);
            break;
        case '+':
            
            maxID++;
            startNode.setID(maxID);

            maxID++;
            endNode.setID(maxID);

            startNode.addEdgeTo(graph[start],EPSILON);
            graph[end].addEdgeTo(graph[start],EPSILON);

            graph[end].addEdgeTo(endNode,EPSILON);
            start = graph.size();
            graph.push_back(startNode);

            end = graph.size();
            graph.push_back(endNode);
            
            break;
        default:
            break;
        }
    }
    void basisMerge(char op, Graph& g, int& maxID){
        Node startNode;
        Node endNode;
        switch (op)
        {
        case '.':
            graph[end].addEdgeTo(g.graph[g.start],EPSILON);
            end = graph.size() + g.end;
            for(auto x:g.graph){
                graph.push_back(x);
            }

            
            break;
        case '|':

            maxID++;
            startNode.setID(maxID);
            maxID++;
            endNode.setID(maxID);

            startNode.addEdgeTo(graph[start].id,EPSILON);
            startNode.addEdgeTo(g.graph[g.start].id,EPSILON);
            
            graph[end].addEdgeTo(endNode.id,EPSILON);
            g.graph[g.end].addEdgeTo(endNode.id,EPSILON);
            for(auto x:g.graph){
                graph.push_back(x);
            }
            start = graph.size();
            graph.push_back(startNode);
            end = graph.size();
            graph.push_back(endNode);
            break;
        case '*':
            maxID++;
            startNode.setID(maxID);

            maxID++;
            endNode.setID(maxID);

            startNode.addEdgeTo(graph[start],EPSILON);
            graph[end].addEdgeTo(graph[start],EPSILON);

            graph[end].addEdgeTo(endNode,EPSILON);

            startNode.addEdgeTo(endNode,EPSILON);

            start = graph.size();
            graph.push_back(startNode);

            end = graph.size();
            graph.push_back(endNode);
            break;
        default:
            break;
        }
    }
    int start;
    int end;
    std::vector<Node> graph;
};

#endif
