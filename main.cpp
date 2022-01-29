#include <iostream>
#include <vector>
#include <string>

bool found(std::vector<int> vec, int num);

class IGraph {
public:
    int power;
    std::vector<std::vector<int>> adj;
public:

    virtual ~IGraph() {}

    IGraph() {};

    IGraph(int inPower):power(inPower) {};

    IGraph(IGraph* other) {};

    virtual void AddEdge(int from, int to) = 0;
    // Метод принимает вершины начала и конца ребра и добавляет ребро

    virtual int verticesCount() const = 0; // Метод должен считать текущее количество вершин

    virtual void getNextVertices(int vertex, std::vector<int> &vertices) const = 0;
    // Для конкретной вершины метод выводит в вектор “вершины” все вершины, в которые можно дойти по ребру из данной

    virtual void getPrevVertices(int vertex, std::vector<int> &vertices) const = 0;
    // Для конкретной вершины метод выводит в вектор “вершины” все вершины, из которых можно дойти по ребру в данную

    virtual void show() = 0;

    virtual std::string getClass() = 0;

};

class ListGraph: virtual public IGraph
{

public:
     ListGraph(int inPower): IGraph(inPower)
     {
         adj.resize(power);
     };
     ListGraph(): ListGraph(5){};

     ListGraph( IGraph* other){
         this->power = other->power;
         if(other->getClass() == "ListGraph") {
             this->adj = other->adj;
         } else{
             adj.resize(other->power);
             for(int i = 0; i < adj.size(); i++){
                 for(int j = 0; j < other->adj[i].size(); j++){
                     if(other->adj[i][j]>0) {
                         adj[i].push_back(j);
                     }
                 }
             }
         }
     }

     ListGraph& operator = ( IGraph* other){
         if(this == other) return *this;
         power = other->power;
         if(other->getClass() == "ListGraph") {
             this->adj = other->adj;
         } else{
             adj.resize(other->power);
             for(int i = 0; i < adj.size(); i++){
                 for(int j = 0; j < other->adj[i].size(); j++){
                     if(other->adj[i][j]>0) {
                         adj[i].push_back(j);
                     }
                 }
             }
         }

     }



     virtual ~ListGraph() final{};

     virtual std::string getClass() final{
         return "ListGraph";
     };


     virtual void AddEdge(int from, int to) final {
        if((from<power)&&(to<power)&&(to!=from)){
            //auto found{std::find(adj[from].begin(), adj[from].end(), to )};
            if(!found(adj[from], to)) {
                adj[from].push_back(to);
            } else {
                std::cout<<"Graph contains this connection"<<std::endl;
            }
        } else {
            std::cout<<"You inputted incorrect edge"<<std::endl;
        }
    };

     virtual int verticesCount() const final{
        return power;
     }

     virtual void getNextVertices(int vertex, std::vector<int> &vertices) const final{
         vertices.resize(0);
         if((vertex > 0)&&(vertex < power))  {
            if(adj[vertex].size() > 0) {
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
         if((vertex > 0)&&(vertex < power))  {
             for(int i = 0; i < adj.size(); i++){
                 if(found(adj[i], vertex)){
                     vertices.push_back(i);
                 }
             }
         } else {
             std::cout<<"Incorrect vertex"<<std::endl;
         }
     }

     virtual void show() final{
         for(int i = 0; i<adj.size();i++){
             std::cout<<i<<" | ";
             for(int j = 0; j < adj[i].size();j++){

                 std::cout<<adj[i][j]<<" ";
             }
             std::cout<<std::endl;
         }
     }

};

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

    virtual void getNextVertices(int vertex, std::vector<int> &vertices) const final{
        vertices.resize(0);
        if((vertex > 0)&&(vertex < power))  {
            for(int j = 0; j < adj[vertex].size(); j++){
                if(adj[vertex][j]!=0) {
                    vertices.push_back(j);
                }
            }
        } else {
            std::cout<<"Incorrect vertex"<<std::endl;
        }
    }
    // Для конкретной вершины метод выводит в вектор “вершины” все вершины, в которые можно дойти по ребру из данной

    virtual void getPrevVertices(int vertex, std::vector<int> &vertices) const final{
        vertices.resize(0);
        if((vertex > 0)&&(vertex < power))  {
            for(int i = 0; i < adj.size(); i++){
                if(adj[i][vertex]!=0) {
                    vertices.push_back(i);
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

bool found(std::vector<int> vec, int num){
    if(vec.size() == 0)return false;
    for(int i = 0; i < vec.size(); i++){
        if(vec[i] == num) return true;
    }
    return false;
}

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
    auto mgr = new MatrixGraph(num);
    std::cout<<"Empty ListGraph"<<std::endl;
    gr->show();
    std::cout<<"Empty MatrixGraph"<<std::endl;
    mgr->show();

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
            mgr->AddEdge(from, to);
        }
        std::cout<<"input request new or exit"<<std::endl;
        std::cin>>request;
    }
    std::cout<<"ListGraph after edge adding"<<std::endl;
    gr->show();
    std::cout<<"MatrixGraph after edge adding"<<std::endl;
    mgr->show();

    std::cout<<"number of vertex for ListGraph"<<std::endl;
    std::cout << gr->verticesCount() << std::endl;

    std::cout<<"number of vertex for MatrixGraph"<<std::endl;
    std::cout << gr->verticesCount() << std::endl;
    std::cout<<"Input vertex for showing vector next vertices"<<std::endl;
    int vert;
    std::cin>> vert;
    std::cout<<"next Vertices for ListGraph"<<std::endl;
    gr->getNextVertices(vert,nextVertices);
    show(nextVertices);
    std::cout<<"next Vertices for MatrixGraph"<<std::endl;
    mgr->getNextVertices(vert, mNextVertices);
    show(mNextVertices);
    std::cout<<"Input vertex for showing vector previous vertices"<<std::endl;
    std::cin>>vert;
    std::cout<<"previous Vertices for ListGraph"<<std::endl;
    gr->getPrevVertices(vert,prevVertices);
    show(prevVertices);

    std::cout<<"previous Vertices for MatrixGraph"<<std::endl;
    mgr->getPrevVertices(vert,mPrevVertices);
    show(mPrevVertices);

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
    delete mgr;




    return 0;
}
