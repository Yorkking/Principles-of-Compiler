#ifndef DFA_H
#define DFA_H 1

#include<algorithm>

#include "NFA.h"

class Dfa: public Nfa{
public:
    Dfa(std::string& str):Nfa(str){
        for(int i=0;i<this->graph.graph.size();i++){
           idToIndex.emplace(this->graph.graph[i].id,i);  
        }
        currentID = 0;
        std::vector<int> s0;
        std::vector<int> start0(1,this->graph.graph[graph.start].id);
        bool hasAcc = getClosure(s0,start0);
        std::vector<std::vector<int>> Dstates;
        Dstates.push_back(s0);
        if(hasAcc){
            acceptId.push_back(0);
        }
        
        Node node(currentID);
        currentID++;
        startId.push_back(node.id);
        DfaGraph.push_back(node);

        std::map<long long,int> mark_id;
        mark_id[getHash(s0)] = 0;

        std::map<long long,bool> mark;
        while(1){
            bool flag = false;
            std::vector<int> cur;
            int NodeIndex = 0;
            for(auto& states: Dstates){
                long long hashV = getHash(states);
                if(mark.count(hashV) == 0){
                    flag = true;
                    mark[hashV] = true;
                    cur = states;
                    break;
                }
                NodeIndex++;
            }
            if(!flag) break;
            for(char ch='a'; ch <= 'z'; ch++){
                std::vector<int> trans;
                moveTo(cur,ch,trans);
                if(trans.size()>0 ){
                    std::vector<int> U;
                    bool isEnd = getClosure(U,trans);
                    long long v = getHash(U);
                    if(mark_id.count(v) == 0){
                        mark_id[v] = DfaGraph.size();
                        DfaGraph.push_back(Node(DfaGraph.size()));
                        if(isEnd){
                            acceptId.push_back(mark_id[v]);
                        }
                        Dstates.push_back(U);
                    }
                    int cur_v = getHash(cur);
                    int idx = mark_id[cur_v];
                    DfaGraph[idx].addEdgeTo(Node(mark_id[v]),ch);
                } 
            }
        }
    }
    void moveTo(std::vector<int>& cur, char ch, std::vector<int>& res){
        std::map<int,bool> mark;
        for(auto& id: cur){
            int index = idToIndex[id];
            for(auto& edge: graph.graph[index].edges){
                if(edge.second == ch && mark.count(edge.first) == 0){
                    mark[edge.first] = true;
                    res.push_back(edge.first);
                }
            }
        }
    }
    bool getClosure(std::vector<int>& res, std::vector<int>& T){
        bool flag = false;
        std::vector<int> stk = T; //里面存的是状态，即ID
        std::map<int,bool> mark;

        res.clear();
        res = T;
        for(auto& id:res){
            mark[id] = true;
            if(!flag && id == this->graph.graph[this->graph.end].id){
                flag = true;
            }
        }
        while(stk.size() != 0){
            int t = stk.back();
            stk.pop_back();
            int index = idToIndex[t];
            for(auto& edge: graph.graph[index].edges){
                if(edge.second == EPSILON && mark.count(edge.first) == 0){
                    mark[edge.first] = true;
                    res.push_back(edge.first);
                    stk.push_back(edge.first);
                    if(!flag && edge.first == this->graph.graph[this->graph.end].id){
                        flag = true;
                    }
                }
            }
        }
        return flag;
    }
    long long getHash(std::vector<int>& data){
        std::sort(data.begin(), data.end());
        long long int hashValue = 0;
        for(auto& x: data){
            long long int y = (long long) x;
            hashValue = ((hashValue*_BASE_)%_MOD_ + y%_BASE_)%_MOD_;
        }
        return hashValue;
    }
    void displayDfa(){
        cout<<"start:";
        for(auto& x:startId){
            cout<<" "<<x;
        }
        cout<<endl;
        cout<<"accept:";
        for(auto& x:acceptId){
            cout<<" "<<x;
        }
        cout<<endl;
        for(auto& x: DfaGraph){
            cout<<"id: "<<x.id<<endl;
            for(auto edge: x.edges){
                cout<<"edge: "<<edge.first<<" "<<edge.second<<endl;
            }
            cout<<endl;
        }
    }
    int contain(Dfa& rhs){
        std::map<std::pair<int,int>,int> mark;
        int s1 = startId[0];
        int s2 = rhs.startId[0];
        return dfs(s1,s2,mark,rhs);
    }
    int getNextState(int id, char ch){
        int index = id;
        int nextState = -1;
        for(auto& edge: DfaGraph[index].edges){
            if(edge.second == ch){
                nextState = edge.first;
                break;
            }
        }
        return nextState;
    }
    bool isAccept(int id){ 
        for(auto& id0 : acceptId){
            if(id0 == id) return true;
        }
        return false;
    }
    std::vector<int> startId;
    std::vector<int> acceptId;
    std::vector<Node> DfaGraph;
    std::unordered_map<int,int> idToIndex;
    int currentID;

    // -1 : this 不包含 rhs; 0: this 包含 rhs
    int dfs(int s1, int s2, std::map<std::pair<int,int>,int>& mark, Dfa& rhs){
        mark.emplace(std::make_pair(s1,s2),1);
        int index1 = s1;
        int index2 = s2;

        if(!this->isAccept(this->DfaGraph[index1].id) && rhs.isAccept(rhs.DfaGraph[index2].id)){
            return -1; // A 不包含 B
        }
        
        // 按照 s1 的顺序进行 dfs 遍历
        for(char ch = 'a'; ch <= 'z'; ch++){
            int nextState1 = this->getNextState(s1, ch);
            int nextState2 = rhs.getNextState(s2, ch);
            
            if(nextState1 == -1 && nextState2 != -1) return -1;
            if(nextState2 == -1) continue;
            if(mark.count(std::make_pair(nextState1, nextState2)) == 0){
                if(dfs(nextState1, nextState2, mark, rhs) == -1) return -1;
            }
        }
        return 0;
    }
};
#endif