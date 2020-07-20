# 编译原理期末大作业实验报告

计科（大数据、人工智能） 17341155  王永康

[toc]

## 实验内容
对于两个正则表达式r和s，判断这两个正则表达式的关系。

正则表达式的关系有4种：

1) r和s等价，即r描述的语言和s描述的语言相等；

2) r描述的语言是s描述的语言的真子集；

3) s描述的语言是r描述的语言的真子集；

4) 非上述情况。

输入的正则表达式只包含小写字母'a'-'z', '|', '*', '?', '+', 'E', '(', ')'。其中，'a'-'z'是所描述语言字符集中的字符，'E'表示epsilon（空串），其它符号含义和教材相同。

## 实验步骤

流程图如下：@需要作图

```mermaid

```

### 正则表达式的预处理
对于输入的正则表达式首先进行预处理，给每个**自然连接**的元素之间加上 `.` 表示两个元素是连接的关系。比如输入 `a*b+cd|d`，预处理将输出 `a*.b+.c.d|d` 。这样做是为了得到符号与符号之间的运算关系，方便后续利用符号栈的方式将**正则表达式**转成 NFA 。

具体的添加 `.` 的规则为：对于一个输入的正则表达式 `s` (字符串)

- `s[i]` 和 `s[i+1]` 均为字符集中元素时，`s[i]` 后面添加一个 `.` 
- `s[i]`  为字符集元素时，`s[i+1]` 是 `(` 时，`s[i]` 后面添加一个 `.`
- `s[i]` 为 `)` ,` s[i+1]` 为 `(` 或字符时，`s[i]` 后面添加一个 `.`
- `s[i]` 为 `*`、`?`、`+` 时，`s[i+1]` 为 `(` 或字符时，`s[i]` 后面添加一个 `.`

### 正则表达式基本运算符转 NFA 的方法

正则表达式转成 NFA 有如下几条规则：对于正则表达式 r 和 正则表示 s, 四种运算对应的 NFA 的连接关系如下：@需要作图（用什么软件呢？）

- 对于基础步：
  
- 
  
- r.s：连接运算

  ```mermaid
  graph LR
  r --> s
  ```

  

- r|s：或运算

- r* : 克林闭包

- r+：

- r?：

相应的，可以设计如下图数据结构来表示一个 NFA 对应的图：注这里的 NFA 最多只有一个 accept 状态。

```C++
class Node{
public:
    ...
    int id; // 节点的 ID
    std::vector<std::pair<int,char>> edges; // 与该节点相连的边
};

class Graph{
public:
    ...
    int start; // 开始的节点对应的 index
    int end; // 接受状态节点对饮的 index
    std::vector<Node> graph;
};
```

### 符号栈将整个正则表达式转成 NFA

设计的数据结构如下：

```C++
class Nfa{
public:
    ...
    Graph graph;
};
```

下面采用**符号栈**方式将正则表达式一步一步转成 NFA。

建立两个栈，一个是符号栈(`. | * ? + `)，另外一个是 NFA 的对应的图数据结构栈(`Graph`)。

算法具体如下：

>初始化两个栈为空;
>依次读入预处理好的正则表达式字符串;
>当遇到字符时，对该字符构造一个 Graph 然后入 Graph 栈;
>当遇到运算符时，分以下情况处理：
>
>- 如果是 ( , 直接压入符号栈;
>
>- 如果当前符号栈为空，直接压入符号栈;
>
>-  如果是 ), 不断弹出符号栈的符号直到遇到 (, 其中弹出的过程中的符号按如下方式进行处理：
>
>  P: 
>
>  - 如果是 *, 弹出 Graph 栈的一个栈顶元素，然后按照 * 的规则生成新的 Graph 结点然后入 Graph 栈；
>  - 如果 |, 弹出 Graph 栈顶的两个元素，然后按照 | 的规则生成新的 Graph 结点然后入 Graph 栈；
>  - 如果遇到的是 .， 弹 Graph 栈顶的两个元素，然后按照 . 的规则生成新的 Graph 节点然后进入 Graph 栈；
>  - 如果遇到的是 ?，弹出 Graph 栈顶的一个元素，然后按照 ? 的规则生成新的 Graph 节点然后进入 Graph 栈；
>  - 如果遇到的 +，弹出 Graph 栈顶的一个元素，然后按照 + 的规则生成新的 Graph 节点然后进入 Graph 栈。
>
>- 如果遇到是符号，如果当前栈顶的优先级高于或当前符号优先级，那么弹出，处理方式与上面相同；
>
>- 最后，清空符号栈，处理方式与 P 相同。
>
>得到一个 NFA 的 Graph。

### NFA 转 DFA

设计 DFA 的数据结构如下:

```c++
class Dfa: public Nfa{
public:
    ...
    std::vector<int> startId; // 开始节点
    std::vector<int> acceptId; // 接受状态
    std::vector<Node> DfaGraph; // 生成的 DFA 对应的图
    ...
};
```

NFA 转  DFA 的算法如下：

![image-20200720101953570](../assets/image/image-20200720101953570.png)

其中计算 $\epsilon-closure(s)$ 的算法如下：

![image-20200720102818768](../assets/image/image-20200720102818768.png)

这里的实现还是有一定的复杂的，具体细节见源码。

### 比较两个 DFA 的关系

当把两个正则表达式生成了对应的 DFA 后，就可以来比较它们之间的关系。

#### DFS 遍历两个 DFA 从而确定包含关系

DFS 遍历两个 DFA 的包含关系的算法如下：

> **int contain(Dfa dfa1, Dfa dfa2):** 
>
> // return 0: dfa1 包含 dfa2；return -1: dfa1 不包含 dfa2。
>
> 同时从 dfa1 和 dfa2 的 s0(起始状态) 出发，做 DFS 遍历；
>
> 每次按照 a-z 的字典序转移到下一个状态 n1, n2; DFS 的标记状态为 pair(n1, n2)；
>
> 如果当前状态被标记，则不访问。
>
> 遍历过程中如果出现以下情况：
>
> - n2 为接受状态，而 n1 不是接受状态的时候，则返回 -1, 即 dfa1 不包含 dfa2；
> - 如果 n1 对于某一个字符没有下一状态，而 n2 有，则直接返回 -1；
> - 遍历完如果未出现上述两种情况，则返回 0，即 dfa1 包含 dfa2。

比较关系时，调用两次 contain：根据 flag1 = contain(dfa1, dfa2) 和 flag2 = contain(dfa2, dfa1) 来判断两个 DFA 之间的关系：

- flag1 == -1 && flag2 == -1，互不包含，无法比较；
- flag1 == 0 && flag2 == -1，dfa1 包含 dfa2，大于关系；
- flag1 == -1 && flag2 == 0, 小于关系；
- 否则，相等关系。

## 实验结果

测试用例：

```
9
(a|E)+* a+*
((a|E)b*)* (a|b)*
aa*bb*cc*dd* a*b*c*d*
a+b+c+d+ (a|b|c|d)*
a*(bb*c|E|bb*d)c? a*b+d?
avasfsfasfsasafvgb (a|b|c|d|e|f|g|h|i|j|k|l|m|n|o|p|q|r|s|t|u|v|w|x|y|z)*
(a*b*i*klklkladnn*?+ddd|z+?)* (a|b|c|d|e|f|g|h|i|j|k|l|m|n|o|p|q|r|s|t|u|v|w|x|y|z)*
(xr)* x*r*
(x|r)*xr(x|r)* (xr)+

```
预期结果
```
=
=
<
<
!
<
<
!
>
```

实验结果：

![image-20200720110327940](../assets/image/image-20200720110327940.png)

## 实验总结

这一次比较正则表达式的大作业确实具有一定的挑战性，不过在努力下还是完成了实验，主要有以下一些体会：

- 加深了自己对正则语言、NFA、DFA 三种之间的关系的理解，也更加深刻了解了它们之间的转换规则。
- 最初做该实验时，个人选择的是直接把正则表达式转成 DFA 来处理，不过其算法实现着实过于复杂，选择从 NFA 入手，这个确实减少了不少工作量；
- 个人觉得实验的最难的一个点是在实现正则表达式到 NFA 的转换这个过程，这里涉及了正则表达式的语法分析的过程，不过利用之前表达式求值的符号栈的思路解决了这个问题。

- 关于算法的效率问题，个人觉得算法主要效率取决于 NFA 转 DFA 那一步，特别是在判断 $\epsilon-closure(s)$ 得到的状态是否的新的 DFA 状态这里。个人设计了对一系列整数的哈希函数来提高效率。

总之，这一次实验非常有效把课堂上所学的理论和实践相结合了起来，实在是受益良多、获益匪浅。

## 附录

```c++
#include<iostream>
#include<map>
#include<string>
#include<vector>
#include<unordered_map>
#include<map>
#include<stack>
#include<algorithm>

using std::cout;
using std::endl;
const int NN = 26;
const char DOT = '.';
const char EPSILON = 'E';

bool isLetter(char c){
    return (c>='a' && c <= 'z') || c == EPSILON;
}

bool priority(char a, char b){
    if(a == b) return true;
    if(a == '(') return false;
    if(a == '*' || a == '?' || a == '+') return true;
    if(b == '|') return true;

    return false;
}

static constexpr long long _MOD_ = (1LL << 31) - 1;
static constexpr long long _BASE_ = 41;

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
        //Graph temp;
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

            //startNode.addEdgeTo(endNode,EPSILON);

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
        //return *this;
    }
    int start;
    int end;
    std::vector<Node> graph;

};

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

class Dfa: public Nfa{
public:
    Dfa(std::string& str):Nfa(str){
        for(int i=0;i<this->graph.graph.size();i++){
           idToIndex.emplace(this->graph.graph[i].id,i);  
        }


        currentID = 0;
        //cout<<"1222"<<endl;
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
                //cout<<"56*** "<<cur.size()<<" "<<trans.size()<<endl;
                if(trans.size()>0 ){
                    //cout<<"57*** "<<trans.size()<<endl;
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
            //cout<<"82*** id: "<<id<<endl;
            for(auto& edge: graph.graph[index].edges){
                //cout<<"83***"<< edge.second <<" "<<edge.first <<endl;
                if(edge.second == ch && mark.count(edge.first) == 0){
                    mark[edge.first] = true;
                    res.push_back(edge.first);
                    //cout<<"85****** "<<ch<<" "<<edge.first<<endl;
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
        //cout<<"100** "<<res.size()<<endl;
        for(auto& id:res){
            mark[id] = true;
            if(!flag && id == this->graph.graph[this->graph.end].id){
                flag = true;
            }
        }
        while(stk.size() != 0){
            int t = stk.back();
            stk.pop_back();
            
            //cout<<"113:** "<<t<<endl;
            int index = idToIndex[t];

            for(auto& edge: graph.graph[index].edges){
                //cout<<"117** "<<edge.second <<endl;
                if(edge.second == EPSILON && mark.count(edge.first) == 0){
                    mark[edge.first] = true;
                    res.push_back(edge.first);
                    //cout<<"120** "<<res.size()<<endl;
                    stk.push_back(edge.first);
                    if(!flag && edge.first == this->graph.graph[this->graph.end].id){
                        //cout<<"121****end: "<<this->graph.graph[this->graph.end].id<<endl;
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
            //cout<<"x"<<x<<endl;
            hashValue = ((hashValue*_BASE_)%_MOD_ + y%_BASE_)%_MOD_;
        }
        //cout<<"hashvalue: "<<hashValue<<endl;
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
//private:
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

class Regex{
public:
    friend int compare(Regex& reg1, Regex& reg2){
        Dfa dfa1(reg1.getString());
        Dfa dfa2(reg2.getString());

        //dfa1.display();
        //dfa1.displayDfa();
        
        //dfa2.display();
        //dfa2.displayDfa();

        int flag1 = dfa1.contain(dfa2);
        int flag2 = dfa2.contain(dfa1);

        if(flag1 == -1 && flag2 == -1) return -2; // uncomparable;
        else if(flag1 == -1 && flag2 == 0) return -1; // < 
        else if(flag1 == 0 && flag2 == 0) return 0; // ==
        else return 1; // >
        //if(flag1 == 0 && flag2 == -1) return 1;// >

    }
    Regex(std::string& str){
        str.push_back(EPSILON);
        for(int i=0;i<str.length()-1;++i){
            if(isLetter(str[i]) && isLetter(str[i+1])){
                regString.push_back(str[i]);
                regString.push_back('.');
            }else if(isLetter(str[i]) && str[i+1] == '('){
                regString.push_back(str[i]);
                regString.push_back('.');
            }else if(str[i] == ')' && str[i+1] == '('){
                regString.push_back(str[i]);
                regString.push_back('.');
            }else if(str[i] == ')' && isLetter(str[i+1])){
                regString.push_back(str[i]);
                regString.push_back('.');
            }else if((str[i] == '*'|| str[i] == '?' || str[i] == '+') && (isLetter(str[i+1]) || str[i+1] == '(')){
                regString.push_back(str[i]);
                regString.push_back('.');
            }else{
                regString.push_back(str[i]);
            }
        }
        regString = regString.substr(0,regString.length()-1);
        //cout<<regString<<endl;
    }
    std::string& getString(){
        return regString;
    }
private:
    std::string regString;

};

int main(){
    int T;
    std::cin>>T;
    while(T--){
        std::string s1,s2;
        std::cin>>s1>>s2;
        Regex reg1(s1);
        Regex reg2(s2);

        int flag = compare(reg1,reg2);

        if(flag == 0) cout<<"="<<endl;
        else if(flag == -1) cout<<"<"<<endl;
        else if(flag == 1) cout<<">"<<endl;
        else cout<<"!"<<endl;
    }
    return 0; 
    
}

```

[源码链接]()（注：github 上的代码与上面代码不一致，因为个人写的过程中是把整个程序拆分成许多 .h 和 .cpp 文件，这样使整个项目结构更加清晰）。

