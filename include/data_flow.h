#ifndef DATAFLOW_H
#define DATAFLOW_H

#include <queue>
#include <map>
#include <iostream>
#include <fstream>
#include <operator.h>

using namespace std;

template<class T>
class DataFlow {

private:
    int id;
    std::string name;
    std::map<int, Operator<T> *> op_array;
    std::map<int, std::vector<int>> graph;
    int num_op_in;
    int num_op_out;
    int num_op;
    int max_level;

    void addOperator(Operator<T> *op) {
        if(DataFlow<T>::op_array.find(op->getId()) == DataFlow<T>::op_array.end()) {
            if (op->getDataFlowId() == -1) {
                op->setDataFlowId(DataFlow<T>::id);
                DataFlow<T>::op_array[op->getId()] = op;
                DataFlow<T>::num_op++;
                if (op->getType() == OP_IN) {
                    DataFlow<T>::num_op_in++;
                }
                if (op->getType() == OP_OUT) {
                    DataFlow<T>::num_op_out++;
                }
            }
        }
    }
    /*
    void load(const vector<tuple<string, int, int, int>>& d) {
        for(auto c : d){
            string op = std::get<0>(c);
            int node_id = std::get<1>(c);
            int src_0_id = std::get<2>(c);
            int src_1_id = std::get<3>(c);
            T constant = std::get<3>(c);

            if(op == "add"){
                auto node = new Add<T>(node_id);
            }else if(op == "sub"){
                auto node = new Sub<T>(node_id);
            }else if(op == "mult"){
                auto node = new Mult<T>(node_id);
            }else if(op == "addi"){
                auto node = new Addi<T>(node_id,constant);
            }else if(op == "subi"){
                auto node = new Subi<T>(node_id,constant);
            }else if(op == "multi"){
                auto node = new Multi<T>(node_id,constant);
            }

            cout << std::get<0>(c) << " " << endl;
        }
    }
     */

public:

    DataFlow(int id, std::string name) : id(id), name(std::move(name)), num_op_in(0), num_op_out(0), num_op(0),
                                         max_level(0) {}

    ~DataFlow() {
        DataFlow<T>::op_array.clear();
        for (auto g:DataFlow<T>::graph) {
            g.second.clear();
        }
        DataFlow<T>::graph.clear();
    }

    Operator<T> *removeOperator(int op_id) {
        Operator<T> *r = DataFlow<T>::op_array[op_id];
        DataFlow<T>::op_array.erase(op_id);
        r->setDataFlowId(-1);
        return r;
    }

    void compute() {
        auto n = DataFlow<T>::getMaxLevel();
        unsigned long allIsEnd = 0;
        while (allIsEnd != DataFlow<T>::getNumOpIn()) {
            allIsEnd = 0;
            for (int i = 0; i <= n; ++i) {
                for (auto item:DataFlow<T>::getOpArray()) {
                    auto op = item.second;
                    if (op->getLevel() == i) {
                        op->compute();
                        if (op->getType() == OP_IN && op->isEnd()) {
                            allIsEnd++;
                        }
                    }
                }
                if (allIsEnd == DataFlow<T>::getNumOpIn()) {
                    break;
                }
            }
        }
    }

    const std::map<int, Operator<T> *> &getOpArray() const {
        return op_array;
    }

    Operator<T> *getOp(int op_id) {
        if (DataFlow<T>::op_array.find(op_id) != DataFlow<T>::op_array.end()) {
            return DataFlow<T>::op_array[op_id];
        }
        return nullptr;
    }

    void toDOT(const std::string& fileNamePath) {

        std::ofstream myfile;
        myfile.open(fileNamePath);
        myfile << "digraph " << DataFlow<T>::name << "{" << std::endl;
        for (auto op:DataFlow<T>::op_array) {
            if (op.second->getType() == OP_IN) {
                myfile << " " << op.first << " [ label = in" << op.second->getId() << " ]" << std::endl;
            } else if (op.second->getType() == OP_OUT) {
                myfile << " " << op.first << " [ label = out" << op.second->getId() << " ]" << std::endl;
            } else if (op.second->getType() == OP_IMMEDIATE) {
                myfile << " " << op.first;
                myfile << " [ label = " << op.second->getLabel();
                myfile << ", value = " << op.second->getConst();
                myfile << "]" << std::endl;
                myfile << " \"" << op.first << "." << op.second->getConst() << "\"[ label = "
                       << op.second->getConst()
                       << " ]" << std::endl;

            } else {
                myfile << " " << op.first << " [ label = " << op.second->getLabel() << "]" << std::endl;
            }

        }
        for (auto op:DataFlow<T>::op_array) {
            if (op.second->getType() == OP_IMMEDIATE) {
                myfile << " \"" << op.first << "." << op.second->getConst() << "\" -> " << op.first << std::endl;
            }
            for (auto op_dst:op.second->getDst()) {
                myfile << " " << op.first << " -> " << op_dst->getId() << std::endl;
            }
        }
        myfile << "}" << std::endl;
        myfile.close();
    }

    void toJSON(const std::string &fileNamePath) {
        std::ofstream myfile;
        myfile.open(fileNamePath);
        myfile << "[" << endl;
        
        char str_node_sub[] = R"({"data":{"id":"%d","op1":"%d","op2":"%d","type":"sub"},"group":"nodes"})";
        char str_node[] = R"({"data":{"id":"%d","type":"%s"},"group":"nodes"})";
        char str_edge[] = R"({"data":{"id":"%d","source":"%d","target":"%d"},"group":"edges"})";
        
        char buf[256];
        int numOp = DataFlow<T>::getNumOp();
        int numEdge = DataFlow<T>::getNumEdges();
        int cnt = 0;
        int max_id = 0;
        int id_edges = 0;
        for (auto item:DataFlow<T>::op_array) {
            cnt++;
            auto op = item.second;
            if(op->getLabel() == "sub"){
                 sprintf(buf, str_node_sub, op->getId(),op->getSrcA(),op->getSrcB(), op->getLabel().c_str());
            }else{
                sprintf(buf, str_node, op->getId(), op->getLabel().c_str()); 
            }
            if (op->getId() > max_id) {
                max_id = op->getId();
            }
            myfile << buf << "," << endl;
        }
        id_edges = max_id + 1;
        cnt = 0;
        for (auto item:DataFlow<T>::op_array) {
            auto op = item.second;
            for (auto neighbor:op->getDst()) {
                cnt++;
                sprintf(buf, str_edge, id_edges++, op->getId(), neighbor->getId());
                if (cnt < numEdge)
                    myfile << buf << "," << endl;
                else
                    myfile << buf << endl;
            }
        }
        myfile << "]";
        myfile.close();
    }

    void loadFromJSON(const std::string& fileNamePath) {
        //TODO: Not Implemented!
    }

    void connect(Operator<T> *src, Operator<T> *dst, PORT dstPort) {

        DataFlow<T>::addOperator(src);
        DataFlow<T>::addOperator(dst);
        DataFlow<T>::graph[src->getId()].push_back(dst->getId());

        src->getDst().push_back(dst);

        if (dstPort == PORT_A) {
            dst->setSrcA(src);
        } else if (dstPort == PORT_B) {
            dst->setSrcB(src);
        } else if (dstPort == PORT_BRANCH) {
            dst->setBranchIn(src);
        }
        DataFlow<T>::updateOpLevel();
    }

    void updateOpLevel() {
        std::queue<int> q;
        int parent;
        for (auto op:DataFlow<T>::op_array) {
            if (op.second->getType() == OP_IN) {
                q.push(op.first);
                while (!q.empty()) {
                    parent = q.front();
                    q.pop();
                    for (auto child:DataFlow<T>::graph[parent]) {
                        int lp = DataFlow<T>::op_array[parent]->getLevel();
                        int lc = DataFlow<T>::op_array[child]->getLevel();
                        if (lp >= lc) {
                            DataFlow<T>::op_array[child]->setLevel(lp + 1);
                        }
                        q.push(child);
                    }
                }
            }
        }
        for (auto op:DataFlow<T>::op_array) {
            if (op.second->getType() == OP_IN) {
                int level = 0;
                for (auto child:DataFlow<T>::graph[op.first]) {
                    if (DataFlow<T>::op_array[child]->getLevel() > level) {
                        level = DataFlow<T>::op_array[child]->getLevel();
                    }
                }
                if (level > 0)
                    level = level - 1;
                op.second->setLevel(level);
            }
        }
        for (auto op:DataFlow<T>::op_array) {
            if (op.second->getLevel() > DataFlow<T>::max_level) {
                DataFlow<T>::max_level = op.second->getLevel();
            }
        }
    }

    int getId() const {
        return id;
    }

    void setId(int df_id) {
        DataFlow<T>::id = df_id;
    }

    int getMaxLevel() const {
        return DataFlow<T>::max_level;
    }

    void setMaxLevel(int maxLevel) {
        DataFlow<T>::max_level = maxLevel;
    }

    const std::string &getName() const {
        return name;
    }

    const std::map<int, std::vector<int>> &getGraph() const {
        return graph;
    }

    int getNumOpIn() const {
        return num_op_in;
    }

    int getNumOpOut() const {
        return num_op_out;
    }

    int getNumOp() const {
        return num_op;
    }

    int getNumEdges() const {
        int num_edges = 0;
        for (const auto &v:DataFlow<T>::graph) {
            num_edges += v.second.size();
        }
        return num_edges;
    }
};


#endif //DATAFLOW_H
