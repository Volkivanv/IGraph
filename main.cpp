#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

int position(std::vector<int>vec,int n){
    if(vec.size() == 0) return 0;
    int a = 0;
    int b = vec.size()-1;
    int m;
    if (vec[a] >= n) return 0;
    if (vec[b] <= n) return vec.size();
    while(b - a > 1){
        m = (a+b)/2;

        if(vec[m] <= n){
            a = m;
        } else b = m;

    }
    return a+1;

}


class IGraph {
public:

public:

    virtual ~IGraph() {}

    IGraph() {};

    IGraph(int inPower) {};

    IGraph(const IGraph& other) {};

    virtual void AddEdge(int from, int to) = 0;
    // Метод принимает вершины начала и конца ребра и добавляет ребро

    virtual int verticesCount() const = 0; // Метод должен считать текущее количество вершин

    virtual std::vector<int> Vertices() const = 0; // Метод должен вернуть текущие список текущих вершин

    virtual void getNextVertices(int vertex, std::vector<int> &vertices) const = 0;
    // Для конкретной вершины метод выводит в вектор “вершины” все вершины, в которые можно дойти по ребру из данной

    virtual void getPrevVertices(int vertex, std::vector<int> &allVertices) const = 0;
    // Для конкретной вершины метод выводит в вектор “вершины” все вершины, из которых можно дойти по ребру в данную

    virtual void show() = 0;


};

class ListGraph: public IGraph
{

public:
    std::vector<int>allVertices;
    std::vector<std::vector<int>> adj;

     ListGraph(int inPower): IGraph(inPower)
     {
         adj.resize(inPower);
         allVertices.resize(inPower);
         for(int i = 0; i < allVertices.size(); i++){
             allVertices[i] = i;
         }
     };
     ListGraph(){};

   /*  ListGraph(const IGraph& other){

     }

     ListGraph& operator = ( IGraph* other){


     }

*/

     virtual ~ListGraph() final{};



     virtual void AddEdge(int from, int to) final {
         int pos;
        if(std::find(allVertices.begin(), allVertices.end(), from) == allVertices.end()){
            std::vector<int> vecEmpty;
            int _pos = position(allVertices,from);
            allVertices.insert(allVertices.begin()+_pos,from);
            adj.insert(adj.begin()+_pos,vecEmpty);

        };
        if(std::find(allVertices.begin(), allVertices.end(), to) == allVertices.end()){
            std::vector<int> vecEmpty;
            int _pos = position(allVertices,to);
            allVertices.insert(allVertices.begin()+_pos,to);
            adj.insert(adj.begin()+_pos,vecEmpty);

        }
            auto itFrom = std::find(allVertices.begin(), allVertices.end(), from);
            pos = itFrom - allVertices.begin();

            if(std::find(adj[pos].begin(), adj[pos].end(), to) == adj[pos].end()) {
                adj[pos].push_back(to);
            } else {
                std::cout<<"Graph contains this connection"<<std::endl;
            }


    };

     virtual int verticesCount() const final{
        return allVertices.size();
     }

     virtual std::vector<int> Vertices() const final{
         return allVertices;
     };

     virtual void getNextVertices(int vertex, std::vector<int> &vertices) const final{
         vertices.resize(0);
         auto itFind = std::find(allVertices.begin(), allVertices.end(), vertex);
         if(itFind!=allVertices.end())  {
            int pos = itFind - allVertices.begin();
            if(!adj[pos].empty()) {
                vertices = adj[vertex];
            } else{
                vertices.resize(0);
            }
         } else {
            std::cout<<"Incorrect vertex"<<std::endl;
         }
     }

     virtual void getPrevVertices(int vertex, std::vector<int> &vertices) const final{
         vertices.resize(0);
         if(std::find(allVertices.begin(), allVertices.end(), vertex)!=allVertices.end()){
             for(int i = 0; i < adj.size(); i++){
                 if(std::find(adj[i].begin(), adj[i].end(), vertex)!=adj[i].end()){
                     vertices.push_back(allVertices[i]);
                 }
             }
         } else {
             std::cout<<"Incorrect vertex"<<std::endl;
         }
     }

     virtual void show() final{
         for(int i = 0; i<adj.size();i++){
             std::cout << allVertices[i] << " | ";
             for(int j = 0; j < adj[i].size();j++){

                 std::cout<<adj[i][j]<<" ";
             }
             std::cout<<std::endl;
         }
     }

};
/*
class MatrixGraph:virtual public IGraph
{

public:
    MatrixGraph(int inPower):IGraph(inPower){
        adj.resize(inPower);
        for(int i = 0; i<inPower;i++){
            adj[i].resize(power);
        }
    }

    MatrixGraph(): MatrixGraph(5){};

    MatrixGraph(IGraph* other){
        power = other->power;
        if(other->getClass() == "MatrixGraph") {
            adj = other->adj;
        }else{
            adj.resize(power);
            for(int i = 0; i < power; i++){
                adj[i].resize(power);
                for(int j = 0; j < other->adj[i].size(); j++){
                    adj[i][other->adj[i][j]] = 1;
                }
            }
        }
    }

    MatrixGraph& operator = ( IGraph* other){
        if(this == other) return *this;
        power = other->power;
        if(other->getClass() == "MatrixGraph") {
            adj = other->adj;
        }else{
            adj.resize(power);
            for(int i = 0; i < power; i++){
                adj[i].resize(power);
                for(int j = 0; j < other->adj[i].size(); j++){
                    adj[i][other->adj[i][j]] = 1;
                }
            }
        }
    }

    virtual ~MatrixGraph() final{}

    virtual std::string getClass(){
        return "MatrixGraph";
    };

    virtual void AddEdge(int from, int to) final{
        if((from<power)&&(to<power)&&(to!=from)){
            if(adj[from][to] == 0) {
                adj[from][to] = 1;
            } else {
                std::cout<<"Graph contains this connection"<<std::endl;
            }
        } else {
            std::cout<<"You inputted incorrect edge"<<std::endl;
        }
    };
    // Метод принимает вершины начала и конца ребра и добавляет ребро

    virtual int verticesCount() const final{
        return power;
    }; // Метод должен считать текущее количество вершин

    virtual void getNextVertices(int vertex, std::vector<int> &allVertices) const final{
        allVertices.resize(0);
        if((vertex > 0)&&(vertex < power))  {
            for(int j = 0; j < adj[vertex].size(); j++){
                if(adj[vertex][j]!=0) {
                    allVertices.push_back(j);
                }
            }
        } else {
            std::cout<<"Incorrect vertex"<<std::endl;
        }
    }
    // Для конкретной вершины метод выводит в вектор “вершины” все вершины, в которые можно дойти по ребру из данной

    virtual void getPrevVertices(int vertex, std::vector<int> &allVertices) const final{
        allVertices.resize(0);
        if((vertex > 0)&&(vertex < power))  {
            for(int i = 0; i < adj.size(); i++){
                if(adj[i][vertex]!=0) {
                    allVertices.push_back(i);
                }
            }
        } else {
            std::cout<<"Incorrect vertex"<<std::endl;
        }
    };
    // Для конкретной вершины метод выводит в вектор “вершины” все вершины, из которых можно дойти по ребру в данную

    virtual void show() final{

        std::cout<<"\t";

        for(int j = 0; j < adj[1].size();j++) {std::cout<<j<<"\t";};
        std::cout<<std::endl;

        std::cout<<"\t";

        for(int j = 0; j < adj[1].size();j++) {std::cout<<"-"<<"\t";};
        std::cout<<std::endl;

        for(int i = 0; i < adj.size(); i++){
            std::cout<<i<<"|"<<"\t";
            for(int j = 0; j < adj[i].size();j++){
                std::cout<<adj[i][j]<<"\t";
            }
            std::cout<<std::endl;
        }
    }

};

*/

void show(std::vector<int>& vec){
    if(vec.size() == 0){
        std::cout<<"Vector has no elements"<<std::endl;
    } else {
        for(int i = 0; i < vec.size();i++){
            std::cout << vec[i] << " ";
        }
        std::cout << std::endl;
    }
}



int main() {
    int num;
    std::cout<<"Input power of Graphs"<<std::endl;
    std::cin>>num;
    auto gr = new ListGraph(num);


   // auto mgr = new MatrixGraph(num);
    std::cout<<"Empty ListGraph"<<std::endl;
    gr->show();
 //   std::cout<<"Empty MatrixGraph"<<std::endl;
  //  mgr->show();

    std::vector<int> nextVertices, prevVertices, mNextVertices, mPrevVertices;

    std::string request;
    std::cout << "input request 'new' for add edge or 'exit' for continue"<<std::endl;
    std::cin >> request;
    while (request!="exit"){
        if(request == "new"){
            std::cout << "Input new edge from to"<<std::endl;
            int from, to;
            std::cin >> from >> to;
            gr->AddEdge(from, to);
         //   mgr->AddEdge(from, to);
        }
        std::cout<<"input request new or exit"<<std::endl;
        std::cin>>request;
    }
    std::cout<<"ListGraph after edge adding"<<std::endl;
    gr->show();
  //  std::cout<<"MatrixGraph after edge adding"<<std::endl;
  //  mgr->show();

    std::cout<<"number of vertex for ListGraph"<<std::endl;
    std::cout << gr->verticesCount() << std::endl;

    std::cout<<"number of vertex for MatrixGraph"<<std::endl;
  //  std::cout << mgr->verticesCount() << std::endl;
    std::cout<<"Input vertex for showing vector next allVertices"<<std::endl;
    int vert;
    std::cin>> vert;
    std::cout<<"next Vertices for ListGraph"<<std::endl;
    gr->getNextVertices(vert,nextVertices);
    show(nextVertices);
  //  std::cout<<"next Vertices for MatrixGraph"<<std::endl;
  //  mgr->getNextVertices(vert, mNextVertices);
  //  show(mNextVertices);
    std::cout<<"Input vertex for showing vector previous allVertices"<<std::endl;
    std::cin>>vert;
    std::cout<<"previous Vertices for ListGraph"<<std::endl;
    gr->getPrevVertices(vert,prevVertices);
    show(prevVertices);

  //  std::cout<<"previous Vertices for MatrixGraph"<<std::endl;
  //  mgr->getPrevVertices(vert,mPrevVertices);
   // show(mPrevVertices);
    /*
    std::cout<<"Constructor from IGraph"<<std::endl;
    auto cgr = new ListGraph(mgr);
    cgr->show();

    std::cout<<"assignment operator"<<std::endl;
    auto igr = new ListGraph(5);

    std::cout<<"graph before"<<std::endl;
    igr->show();

    std::cout<<"after assignment"<<std::endl;
    *igr = mgr;

    igr->show();

    delete cgr;
    delete igr;
    delete gr;
  //  delete mgr;



*/
    return 0;
}
