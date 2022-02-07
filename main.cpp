#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>

class IGraph {
public:

public:

    virtual ~IGraph() = default;

    IGraph() = default;;

    explicit IGraph(int inPower) {};

    IGraph(const IGraph& other) = default;

  //  IGraph& operator = (const IGraph& other) = default;

    virtual void AddEdge(int from, int to) = 0;
    // Метод принимает вершины начала и конца ребра и добавляет ребро

    [[nodiscard]] virtual int verticesCount() const = 0; // Метод должен считать текущее количество вершин

    [[nodiscard]] virtual std::vector<int> Vertices() const = 0; // Метод должен вернуть текущие список текущих вершин

    virtual void getNextVertices(int vertex, std::vector<int> &vertices) const = 0;
    // Для конкретной вершины метод выводит в вектор “вершины” все вершины, в которые можно дойти по ребру из данной

    virtual void getPrevVertices(int vertex, std::vector<int> &vertices) const = 0;
    // Для конкретной вершины метод выводит в вектор “вершины” все вершины, из которых можно дойти по ребру в данную

    virtual void show() = 0;


};

class ListGraph: public IGraph
{

public:
    std::map<int,std::vector<int>> nextVert;
    std::map<int,std::vector<int>> prevVert;

     explicit ListGraph(int inPower): IGraph(inPower)
     {
         if(inPower<0) inPower = 0;
         for(int i = 0; i < inPower; i++){
             std::vector<int> emptyVec;
             nextVert.insert(std::make_pair(i,emptyVec));
         }
     };
     ListGraph(): ListGraph(0){};

         ListGraph(const IGraph& other){
         std::vector<int> vec = other.Vertices();
         for(int & i : vec){
             std::vector<int>next,prev;
             other.getNextVertices(i,next);
             other.getPrevVertices(i,prev);
             nextVert.insert(std::make_pair(i,next));
             prevVert.insert(std::make_pair(i,prev));
         }
     }

     ListGraph& operator = (IGraph* other){
         if(this == other) return *this;

         std::vector<int> vec = other->Vertices();
         for(int & i : vec){
             std::vector<int>next,prev;
             other->getNextVertices(i,next);
             other->getPrevVertices(i,prev);
             nextVert.insert(std::make_pair(i,next));
             prevVert.insert(std::make_pair(i,prev));
         }
     }

    ListGraph& operator = (const IGraph& other){
        if(this == &other) return *this;

        std::vector<int> vec = other.Vertices();
        for(int & i : vec){
            std::vector<int>next,prev;
            other.getNextVertices(i,next);
            other.getPrevVertices(i,prev);
            nextVert.insert(std::make_pair(i,next));
            prevVert.insert(std::make_pair(i,prev));
        }
    }




    ~ListGraph() final{
         std::cout<<"Deleted"<<std::endl;
     };



     virtual void AddEdge(int from, int to) final {
         auto itNext = nextVert.find(from);
         auto itPrev = prevVert.find(to);
         std::vector<int> nv(1, to);
         std::vector<int> pv(1, from);
         std::vector<int> emptyVec;
         if(itNext==nextVert.end()){
             nextVert.insert(std::make_pair(from, nv));
             if(itPrev==prevVert.end()) {
                 nextVert.insert(std::make_pair(to, emptyVec));
                 prevVert.insert(std::make_pair(to, pv));
             } else {
                 if(std::find(itPrev->second.begin(), itPrev->second.end(), from) == itPrev->second.end()){
                     itPrev->second.push_back(from);
                 }
             }
             prevVert.insert(std::make_pair(from, emptyVec));
         } else{
             if(std::find(itNext->second.begin(), itNext->second.end(), to) == itNext->second.end()){
                 itNext->second.push_back(to);
             }
             if(itPrev==prevVert.end()){
                 nextVert.insert(std::make_pair(to, emptyVec));
                 prevVert.insert(std::make_pair(to, pv));
             } else {
                 if(std::find(itPrev->second.begin(), itPrev->second.end(), from)== itPrev->second.end()){
                     itPrev->second.push_back(from);
                 }
             }
         }

    };

     virtual int verticesCount() const final{
        return nextVert.size();
     }

     virtual std::vector<int> Vertices() const final{
         std::vector<int> vec;

         for(auto it = nextVert.begin();it!=nextVert.end();++it){
             vec.push_back(it->first);
         }
         return vec;
     };

     virtual void getNextVertices(int vertex, std::vector<int> &vertices) const final{
         vertices.resize(0);
         auto itFind = nextVert.find(vertex);
         if(itFind!=nextVert.end()){
             vertices = itFind->second;
         } else {
            std::cout<<"Incorrect vertex"<<std::endl;
         }
     }

     virtual void getPrevVertices(int vertex, std::vector<int> &vertices) const final{
         vertices.resize(0);
         auto itFind = prevVert.find(vertex);
         if(itFind!=prevVert.end()){
             vertices = itFind->second;
         } else {
             std::cout<<"Incorrect vertex"<<std::endl;
         }
     }

     virtual void show() final{

        std::cout<<"Next"<<std::endl;
         for(auto it = nextVert.begin(); it!=nextVert.end();++it){
             std::cout << it->first << " | ";
             for(int j = 0; j < it->second.size();j++){

                 std::cout<<it->second[j]<<" ";
             }
             std::cout<<std::endl;
         }

         std::cout<<"Previous"<<std::endl;

        for(auto it = prevVert.begin(); it!=prevVert.end();++it){
            std::cout << it->first << " | ";
            for(int j = 0; j < it->second.size();j++){

                std::cout<<it->second[j]<<" ";
            }
            std::cout<<std::endl;
        }
     }

};


class MatrixGraph:virtual public IGraph
{
    std::map<int,int>vertToIndex;
    std::vector<int>indexToVert;
    std::vector<std::vector<int>> matrix;


public:
    MatrixGraph(int inPower):IGraph(inPower){
        if(inPower<0) inPower = 0;
        for(int i = 0; i < inPower; i++){
            vertToIndex.insert(std::make_pair(i,i));
            indexToVert.push_back(i);
        }
        matrix.resize(inPower);
        for(int i = 0; i < inPower; i++){
            for(int j = 0; j < inPower; j++) {
                matrix[i].push_back(0);
            }
        }
    }

    MatrixGraph():MatrixGraph(0){};

    MatrixGraph(const IGraph& other){
        vertToIndex.clear();
        indexToVert = other.Vertices();
        matrix.resize(indexToVert.size());

        for(int i = 0; i< indexToVert.size(); i++){

            vertToIndex.insert(std::make_pair(indexToVert[i], i));
            matrix[i].resize(indexToVert.size());
        }

        for(int i = 0; i< indexToVert.size(); i++){
            std::vector<int>next;
            other.getNextVertices(indexToVert[i],next);
            for(int j = 0; j < next.size(); i++) {
                matrix[i][vertToIndex[next[j]]] = 1;
            }
        }
    }

    MatrixGraph& operator = (IGraph* other){
        if(this == other) return *this;
        vertToIndex.clear();
        matrix.resize(0);
        indexToVert = other->Vertices();
        matrix.resize(indexToVert.size());

        for(int i = 0; i< indexToVert.size(); i++){

            vertToIndex.insert(std::make_pair(indexToVert[i], i));
            matrix[i].resize(indexToVert.size());
        }

        for(int i = 0; i< indexToVert.size(); i++){
            std::vector<int>next;
            other->getNextVertices(indexToVert[i],next);
            for(int j = 0; j < next.size(); i++) {
                matrix[i][vertToIndex[next[j]]] = 1;
            }
        }

    }

    MatrixGraph& operator = (const IGraph& other){
        if(this == &other) return *this;
        vertToIndex.clear();
        matrix.resize(0);
        indexToVert = other.Vertices();
        matrix.resize(indexToVert.size());

        for(int i = 0; i< indexToVert.size(); i++){

            vertToIndex.insert(std::make_pair(indexToVert[i], i));
            matrix[i].resize(indexToVert.size());
        }

        for(int i = 0; i< indexToVert.size(); i++){
            std::vector<int>next;
            other.getNextVertices(indexToVert[i],next);
            for(int j = 0; j < next.size(); i++) {
                matrix[i][vertToIndex[next[j]]] = 1;
            }
        }

    }

    ~MatrixGraph() final{
        std::cout<<"Deleted"<<std::endl;
    }


    virtual void AddEdge(int from, int to) final{
        auto itFrom = vertToIndex.find(from);
        if(itFrom == vertToIndex.end()){
            indexToVert.push_back(from);
            vertToIndex.insert(std::make_pair(from,indexToVert.size()-1));
            std::vector<int> vec(indexToVert.size()-1,0);
            matrix.push_back(vec);
            for(auto & i : matrix){
                i.push_back(0);
            }
        }
        auto itTo = vertToIndex.find(to);
        if(itTo== vertToIndex.end()){
            indexToVert.push_back(to);
            vertToIndex.insert(std::make_pair(to,indexToVert.size()-1));
            std::vector<int> vec(indexToVert.size()-1,0);
            matrix.push_back(vec);
            for(auto & i : matrix){
                i.push_back(0);
            }
        }
        itFrom = vertToIndex.find(from);
        itTo = vertToIndex.find(to);
        matrix[itFrom->second][itTo->second] = 1;

    };
    // Метод принимает вершины начала и конца ребра и добавляет ребро

    virtual int verticesCount() const final{
        return vertToIndex.size();
    }; // Метод должен считать текущее количество вершин

    virtual std::vector<int> Vertices() const final{
        return indexToVert;
    };

    virtual void getNextVertices(int vertex, std::vector<int> &vertices) const final{
        vertices.resize(0);
        auto it = vertToIndex.find(vertex);
        if(it!=vertToIndex.end()){
            for(int i = 0; i < matrix.size();i++){
                if(matrix[it->second][i] == 1) vertices.push_back(indexToVert[i]);
            }
        } else {
            std::cout<<"Incorrect vertex"<<std::endl;
        }
    }
    // Для конкретной вершины метод выводит в вектор “вершины” все вершины, в которые можно дойти по ребру из данной

    virtual void getPrevVertices(int vertex, std::vector<int> &vertices) const final{
        vertices.resize(0);
        auto it = vertToIndex.find(vertex);
        if(it!=vertToIndex.end()){
            for(int i = 0; i < matrix.size();i++){
                if(matrix[i][it->second] == 1) vertices.push_back(indexToVert[i]);
            }
        } else {
            std::cout<<"Incorrect vertex"<<std::endl;
        }
    };
    // Для конкретной вершины метод выводит в вектор “вершины” все вершины, из которых можно дойти по ребру в данную

    virtual void show() final{


        std::cout<<"\t";

        for(int j = 0; j < matrix.size();j++) {std::cout<<indexToVert[j]<<"\t";};
        std::cout<<std::endl;

        std::cout<<"\t";

        for(int j = 0; j < matrix.size();j++) {std::cout<<"-"<<"\t";};
        std::cout<<std::endl;

        for(int i = 0; i < matrix.size(); i++){
            std::cout<<indexToVert[i]<<"|"<<"\t";
            for(int j = 0; j < matrix[i].size();j++){
                std::cout<<matrix[i][j]<<"\t";
            }
            std::cout<<std::endl;
        }
    }

};



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
    MatrixGraph g1;



    g1.AddEdge(1, 2);

    g1.AddEdge(1, 4);

    g1.AddEdge(1, 2);

    g1.AddEdge(100, 100);

    g1.AddEdge(100, 100);

    g1.AddEdge(-1, 1);
    g1.show();





    ListGraph g2 = g1;


    g2.show();












   /* int num;
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
    std::cout << mgr->verticesCount() << std::endl;
    std::cout<<"Input vertex for showing vector next allVertices"<<std::endl;
    int vert;
    std::cin>> vert;
    std::cout<<"next Vertices for ListGraph"<<std::endl;
    gr->getNextVertices(vert,nextVertices);
    show(nextVertices);
    std::cout<<"next Vertices for MatrixGraph"<<std::endl;
    mgr->getNextVertices(vert, mNextVertices);
    show(mNextVertices);
    std::cout<<"Input vertex for showing vector previous allVertices"<<std::endl;
    std::cin>>vert;
    std::cout<<"previous Vertices for ListGraph"<<std::endl;
    gr->getPrevVertices(vert,prevVertices);
    show(prevVertices);

    std::cout<<"previous Vertices for MatrixGraph"<<std::endl;
    mgr->getPrevVertices(vert,mPrevVertices);
    show(mPrevVertices);

    std::cout<<"Constructor from IGraph"<<std::endl;
    auto cgr = new ListGraph(*mgr);
    cgr->show();

    std::cout<<"assignment operator"<<std::endl;
    auto igr = new ListGraph(5);

    std::cout<<"graph before"<<std::endl;
    igr->show();

    std::cout<<"after assignment"<<std::endl;
    *igr = *mgr;

    igr->show();

    delete cgr;
    delete igr;
    delete gr;
  //  delete mgr;


*/

    return 0;
}
