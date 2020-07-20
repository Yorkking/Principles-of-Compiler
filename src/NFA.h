#ifndef NFA_H
#define NFA_H 1

#include<string>
#include<vector>
#include<unordered_map>
#include<map>
#include<stack>
#include "Graph.h"
#include "util.h"

class Nfa{
public:
    Nfa(std::string& regStr){
        maxID = 0;
        construct(regStr);
    }
    Nfa& operator=(const Nfa& rhs){
        this->maxID = rhs.maxID;
        this->graph = rhs.graph;
        return *this;
    }
    void construct(std::string& regStr){
        std::stack<char> ops;
        std::stack<Graph> res;

        for(int i=0;i<regStr.length();){
            
            //cout<<regStr[i]<<endl;

            if(isLetter(regStr[i])){
                res.push(Graph(regStr[i],maxID));
                i++;
                continue;
            }else if(ops.empty()){
                ops.push(regStr[i]);
            
            }else if(regStr[i] == '('){
                ops.push(regStr[i]);
            }else if(regStr[i] == ')'){

                while(ops.top() != '('){
                    Graph g1 = res.top();
                    if(ops.top() == '*' || ops.top() == '+' || ops.top() == '?'){
                        res.pop();
                        Graph g2;
                        g1.merge(ops.top(),g2,maxID);
                        res.push(g1);
                        ops.pop();

                    }else{
                        res.pop();
                        Graph g2 = res.top();
                        res.pop();
                        g2.merge(ops.top(),g1,maxID);
                        res.push(g2);

                        ops.pop();

                    }
                }
                ops.pop();

            }else{
                while(!ops.empty() && priority(ops.top(), regStr[i])){
                    Graph g1 = res.top();
                    //g1.display();
                    if(ops.top() == '*' || ops.top() == '+' || ops.top() == '?'){
                        res.pop();
                        Graph g2;
                        g1.merge(ops.top(),g2,maxID);
                        res.push(g1);
                        ops.pop();

                    }else{
                        res.pop();
                        Graph g2 = res.top();
                        //g2.display();
                        res.pop();
                        g2.merge(ops.top(),g1,maxID);
                        res.push(g2);

                        ops.pop();

                    }
                }
                ops.push(regStr[i]);
            }
            i++;
        }
        while(!ops.empty()){
            Graph g1 = res.top();
            //g1.display();
            if(ops.top() == '*' || ops.top() == '+' || ops.top() == '?'){
                res.pop();
                Graph g2;
                g1.merge(ops.top(),g2,maxID);
                res.push(g1);
                ops.pop();

            }else{
                res.pop();
                Graph g2 = res.top();
                //g2.display();
                res.pop();
                g2.merge(ops.top(),g1,maxID);
                res.push(g2);

                ops.pop();

            }
        }
        graph = res.top();
    }
    void display(){
        graph.display();
    }
    int maxID;
    Graph graph;
};
#endif