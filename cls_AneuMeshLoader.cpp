#include <c++/4.8.3/fstream>
#include "cls_AneuMeshLoader.h"

void cls_AneuMeshLoader::LoadMesh(const std::string &fileName) {
    std::ifstream file;
    file.open(fileName);
    if (!file) throw NoFileFoundException(fileName);
    unsigned p_N_Nodes;
    file >> p_N_Nodes;
    file >> m_Dim;
    m_Nodes.resize(p_N_Nodes);
    for (int i = 0; i < p_N_Nodes; ++i) {
        file >> m_Nodes[i].m_X1 >> m_Nodes[i].m_X2 >> m_Nodes[i].m_X3;
        m_Nodes[i].m_ID = i + 1;
    }
    unsigned p_N_FE;
    file >> p_N_FE;
    file >> m_N_NodesinFE;
    m_FEs.resize(p_N_FE);
    for (int i = 0; i < p_N_FE; ++i) {
        m_FEs[i].m_NodeIDs.resize(m_N_NodesinFE);
        file >> m_FEs[i].m_IDType;
        for (int j = 0; j < m_N_NodesinFE; ++j)
            file >> m_FEs[i].m_NodeIDs[j];
        m_FEs[i].m_ID = i + 1;
    }
    unsigned p_N_SFE;
    file >> p_N_SFE;
    file >> m_N_NodesinSFE;
    m_SFEs.resize(p_N_SFE);
    for (int i = 0; i < p_N_SFE; ++i) {
        m_SFEs[i].m_NodeIDs.resize(m_N_NodesinSFE);
        file >> m_SFEs[i].m_IDBoundary;
        for (int j = 0; j < m_N_NodesinSFE; ++j)
            file >> m_SFEs[i].m_NodeIDs[j];
        m_SFEs[i].m_ID = i + 1;
    }
}