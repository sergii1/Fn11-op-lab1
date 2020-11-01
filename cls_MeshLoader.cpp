#include "cls_MeshLoader.h"

std::vector<Node> cls_MeshLoader::getNodes() const {
    return m_Nodes;
}

std::vector<FiniteElement> cls_MeshLoader::getFiniteElements() const {
    return m_FEs;
}

std::vector<BoundaryFiniteElement> cls_MeshLoader::getSurfaceFiniteElements() const {
    return m_SFEs;
}

std::vector<FiniteElement> cls_MeshLoader::findFE_byNodes(unsigned p_ID1, unsigned p_ID2, unsigned p_ID3) const {
    std::list<FiniteElement> p_FEs;
    auto IT = m_FEs.begin();
    auto End = m_FEs.end();
    while (IT != End) {
        IT = std::find_if(IT, End, [&](const FiniteElement &FE) -> bool {
            bool flag1 = false;
            bool flag2 = false;
            bool flag3 = false;
            for (int i = 0; i < m_N_NodesinFE; ++i) {
                if (p_ID1 == FE.m_NodeIDs[i])
                    flag1 = true;
                if (p_ID2 == FE.m_NodeIDs[i])
                    flag2 = true;
                if (p_ID3 == FE.m_NodeIDs[i])
                    flag3 = true;
            }
            return flag1 && flag2 && flag3;
        });
        if (IT != End) {
            p_FEs.push_back(*IT);
            ++IT;
        }
    }
    return std::vector<FiniteElement>(p_FEs.begin(), p_FEs.end());
}

std::vector<FiniteElement> cls_MeshLoader::findFE_byEdge(unsigned p_ID1, unsigned p_ID2) const {
    std::list<FiniteElement> p_FEs;
    auto IT = m_FEs.begin();
    auto End = m_FEs.end();
    while (IT != End) {
        IT = std::find_if(IT, End, [&](const FiniteElement &FE) -> bool {
            bool flag1 = false;
            bool flag2 = false;
            for (int j = 0; j < m_N_NodesinFE; ++j) {
                if (p_ID1 == FE.m_NodeIDs[j])
                    flag1 = true;
                if (p_ID2 == FE.m_NodeIDs[j])
                    flag2 = true;
            }
            return (flag1 && flag2);
        });
        if (IT != End) {
            p_FEs.push_back(*IT);
            ++IT;
        }
    }
    return std::vector<FiniteElement>(p_FEs.begin(), p_FEs.end());
}

std::vector<Node> cls_MeshLoader::getSFNs_byIDSurface(int p_IDSurface) const {
    std::set<Node> p_Nodes;
    auto IT = m_SFEs.begin();
    auto End = m_SFEs.end();
    while (IT != End) {
        IT = std::find_if(IT, End, [&](const BoundaryFiniteElement &SFE) -> bool {
            return SFE.m_IDBoundary == p_IDSurface;
        });
        if (IT != End) {
            for (const auto &it : IT->m_NodeIDs)
                p_Nodes.insert(m_Nodes[it - 1]);
            ++IT;
        }
    }
    return std::vector<Node>(p_Nodes.begin(), p_Nodes.end());
}

std::vector<FiniteElement> cls_MeshLoader::getFEs_byTypeID(int p_IDType) const {
    std::list<FiniteElement> p_FEs;
    auto IT = m_FEs.begin();
    auto End = m_FEs.end();
    while (IT != End) {
        std::find_if(IT, End, [&](const FiniteElement &FE) { return (FE.m_IDType == p_IDType); });
        if (IT != End) {
            p_FEs.push_back(*IT);
            ++IT;
        }
    }
    return std::vector<FiniteElement>(p_FEs.begin(), p_FEs.end());
}

std::vector<BoundaryFiniteElement> cls_MeshLoader::getSFEs_byIDSurface(int p_IDSurface) const {
    std::list<BoundaryFiniteElement> p_SFEs;
    auto IT = m_SFEs.begin();
    auto End = m_SFEs.end();
    while (IT != End) {
        std::find_if(IT, End,
                     [&](const BoundaryFiniteElement &SFE) -> bool { return (SFE.m_IDBoundary == p_IDSurface); });
        if (IT != End) {
            p_SFEs.push_back(*IT);
            ++IT;
        }
    }
    return std::vector<BoundaryFiniteElement>(p_SFEs.begin(), p_SFEs.end());
}

std::vector<std::vector<Node>> cls_MeshLoader::Get_Nodes_Neighbors() const {
    std::vector<std::set<int>> result_int(m_Nodes.size() + 1);
    for (auto &it : m_FEs)
        for (auto node_it : it.m_NodeIDs) {
            for (auto node_it2 : it.m_NodeIDs)
                result_int[node_it].insert(node_it2);
            result_int[node_it].erase(node_it);
        }

    std::vector<std::vector<Node>> result(m_Nodes.size() + 1);
    for (int i = 1; i < m_Nodes.size() + 1; ++i) {
        std::vector<Node> temp_vec;
        temp_vec.clear();
        temp_vec.reserve(result_int[i].size());
        for (auto &it_set : result_int[i])
            temp_vec.push_back(m_Nodes[it_set - 1]);
        result[i] = std::move(temp_vec);
    }
    return result;
}

void cls_MeshLoader::Transform_to_Quadratic() {
    std::set<Edge> Edges;
    for (auto &IT : m_FEs) {
        //std::vector<unsigned int> CurrentNodes(IT.m_NodeIDs);
        int N = IT.m_NodeIDs.size();
        for (int i = 0; i < N; ++i)
            for (int j = i + 1; j < N; ++j) {
                Edge current_edge(i, j, 0);
                auto FoundedEdge = std::find(Edges.begin(), Edges.end(), current_edge);
                if (FoundedEdge == Edges.end()) {
                    //если ранее ребро не встречалось, сделать новый центральный узел для этого ребра...
                    Node LeftNode;
                    LeftNode = m_Nodes[i];
                    Node RightNode = m_Nodes[j];
                    unsigned Center_ID = m_Nodes.rbegin()->m_ID + 1;
                    Node CenterNode(Center_ID, (LeftNode.m_X1 + RightNode.m_X1) / 2,
                                    (LeftNode.m_X2 + RightNode.m_X2) / 2, (LeftNode.m_X3 + RightNode.m_X3) / 2, true);
                    current_edge.UpdateCenter(Center_ID);
                    m_Nodes.push_back(CenterNode);
                    Edges.insert(current_edge);
                    IT.m_NodeIDs.push_back(Center_ID);
                } else {
                    // если ранее ребро встречалось, получить идентификатор узла центра
                    IT.m_NodeIDs.push_back(FoundedEdge->Center_ID);
                }
            }
        //IT.m_NodeIDs.shrink_to_fit();
    }

    for (auto &IT : m_SFEs) {
        int N = IT.m_NodeIDs.size();
        for (int i = 0; i < N; ++i)
            for (int j = i + 1; j < N; ++j) {
                Edge current_edge(i, j, 0);
                auto founded_edge = std::find(Edges.begin(), Edges.end(), current_edge);
                if (founded_edge != Edges.end())
                    IT.m_NodeIDs.push_back(founded_edge->Center_ID);
            }
        IT.m_NodeIDs.shrink_to_fit();
    }
    //m_SFEs.shrink_to_fit();
    //m_Nodes.shrink_to_fit();
}
