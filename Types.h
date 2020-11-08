#include <vector>
#include <iostream>

struct Node {
    //id узла
    unsigned m_ID;
    double m_X1;
    double m_X2;
    double m_X3;
    //является ли вершиной или внутренним узлом
    bool is_Vertex;

    Node() = default;

    Node(int ID, double x, double y, double z, bool Apex) :
            m_ID(ID), m_X1(x), m_X2(y), m_X3(z), is_Vertex(Apex) {}

    bool operator<(const Node &p_node) const {
        return m_ID < p_node.m_ID;
    }

    friend std::ostream &operator<<(std::ostream &p_Out, const Node &p_Node) {
        p_Out << "ID узла - " << p_Node.m_ID << std::endl;
        p_Out << "X1 - " << p_Node.m_X1 << std::endl;
        p_Out << "X2 - " << p_Node.m_X2 << std::endl;
        p_Out << "X3 - " << p_Node.m_X3 << std::endl;
        //if (p_Node.is_Vertex) p_Out << "Является вершиной КЭ" << std::endl;
        //else p_Out << "Является внутренним узлом КЭ" << std::endl;
        return p_Out;
    }
};

struct FiniteElement {
    //id конечного элемента
    unsigned m_ID;
    //id типа материала
    unsigned m_IDType;
    //список id узлов конечного элемента
    std::vector<unsigned> m_NodeIDs;

    friend std::ostream &operator<<(std::ostream &p_Out, const FiniteElement &p_FE) {
        p_Out << "ID КЭ - " << p_FE.m_ID << std::endl;
        p_Out << "ID материала - " << p_FE.m_IDType << std::endl;
        p_Out << "ID узлов КЭ : ";
        std::copy(p_FE.m_NodeIDs.begin(), p_FE.m_NodeIDs.end(), std::ostream_iterator<int>(p_Out, " "));
        p_Out << std::endl;
        return p_Out;
    }
};

struct BoundaryFiniteElement {
    //id граничного конечного элемента
    unsigned m_ID;
    //id границы
    unsigned m_IDBoundary;
    //список id узлов граничного конечного элемента
    std::vector<unsigned> m_NodeIDs;

    friend std::ostream &operator<<(std::ostream &p_Out, const BoundaryFiniteElement &p_SFE) {
        p_Out << "ID поверхностного КЭ - " << p_SFE.m_ID << std::endl;
        p_Out << "ID поверхности - " << p_SFE.m_IDBoundary << std::endl;
        p_Out << "ID узлов поверхностного КЭ : ";
        std::copy(p_SFE.m_NodeIDs.begin(), p_SFE.m_NodeIDs.end(), std::ostream_iterator<int>(p_Out, " "));
        p_Out << std::endl;
        return p_Out;
    }
};

struct Edge {
    //id узла начала ребра
    unsigned FirstNode_ID;
    //id узла конца ребра
    unsigned LastNode_ID;
    //id узла центра ребра
    unsigned Center_ID;

    Edge(unsigned p_ID1, unsigned p_ID2, unsigned p_ID3) : FirstNode_ID(p_ID1), LastNode_ID(p_ID2), Center_ID(p_ID3) {}

    bool operator==(const Edge &right_edge) const {
        if (right_edge.FirstNode_ID == FirstNode_ID && right_edge.LastNode_ID == LastNode_ID)
            return true;
        else if (right_edge.FirstNode_ID == LastNode_ID && right_edge.LastNode_ID == FirstNode_ID)
            return true;
        else return false;
    }

    bool operator<(const Edge &Right_Edge) const {
        return Center_ID < Right_Edge.Center_ID;
    }

    void UpdateCenter(unsigned p_NodeID) {
        Center_ID = p_NodeID;
    }
};
