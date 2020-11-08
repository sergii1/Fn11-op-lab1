#include <cstring>
#include "cls_AneuMeshLoader.h"

int main(int argc, char** argv) {
  std::string fileName = "cube.aneu";
  if(argc>1){
    std::cout<<argv[1];
    fileName = argv[1];
  }

  setlocale(0, "RUSSIAN");
  cls_AneuMeshLoader A;
  A.LoadMesh(fileName);
  std::ofstream file("Output.txt");

  std::vector <Node> Nodes = A.getNodes();
  file << "Список узлов" << std::endl;
  std::for_each(Nodes.begin(), Nodes.end(), [&](Node &A) { file << A << std::endl; });

  std::vector <FiniteElement> FEs = A.getFiniteElements();
  file << "Список КЭ" << std::endl;
  std::for_each(FEs.begin(), FEs.end(), [&](FiniteElement &A) { file << A << std::endl;});

  std::vector <BoundaryFiniteElement> SFEs = A.getSurfaceFiniteElements();
  file << "Список поверхностных КЭ" << std::endl;
  std::for_each(SFEs.begin(), SFEs.end(), [&](BoundaryFiniteElement &A) { file << A << std::endl; });
  file << std::endl;

  std::vector <FiniteElement> FEsbyNode = A.findFE_byNodes(2, 4, 8);
  file << "Список КЭ с узлами 2,4,8" << std::endl;
  std::for_each(FEsbyNode.begin(), FEsbyNode.end(), [&](FiniteElement &A) { file << A << std::endl;; });
  file << std::endl;

  std::vector <FiniteElement> FEsbyEdge = A.findFE_byEdge(1, 2);
  file << "Список КЭ с ребром из узлов 1, 2" << std::endl;
  std::for_each(FEsbyEdge.begin(), FEsbyEdge.end(), [&](FiniteElement &A) { file << A << std::endl;; });
  file << std::endl;

  std::vector <Node> SFNsbyIDSurface = A.getSFNs_byIDSurface(4);
  file << "Список узлов поверхностных КЭ с ID поверхности = 4" << std::endl;
  std::for_each(SFNsbyIDSurface.begin(), SFNsbyIDSurface.end(), [&](Node &A) { file << A << std::endl;; });
  file << std::endl;

  std::vector <FiniteElement> FEsbyTypeID = A.getFEs_byTypeID(1);
  file << "Список КЭ с ID типа материала = 1" << std::endl;
  std::for_each(FEsbyTypeID.begin(), FEsbyTypeID.end(), [&](FiniteElement &A) { file << A << std::endl;; });

  std::vector <BoundaryFiniteElement> SFEsbyIDSurface = A.getSFEs_byIDSurface(4);
  file << "Список поверхностных КЭ с ID поверхности = 4" << std::endl;
  std::for_each(SFEsbyIDSurface.begin(), SFEsbyIDSurface.end(),
                [&](BoundaryFiniteElement &A) { file << A << std::endl; });
  file << std::endl;

  std::vector <std::vector<Node>> Neighbors = A.Get_Nodes_Neighbors();
  for (int i = 1; i < Neighbors.size(); ++i) {
    file << "Соседи вершины :" << i << std::endl;
    std::for_each(Neighbors[i].begin(), Neighbors[i].end(), [&](Node &A) { file << A << std::endl; });
  }

  A.Transform_to_Quadratic();
  file << std::endl << "После преобразования симплексных тетраэдральные КЭ в квадратичные" << std::endl;

  Nodes = A.getNodes();
  file << "Список узлов" << std::endl;
  std::for_each(Nodes.begin(), Nodes.end(), [&](Node &A) { file << A << std::endl;; });

  FEs = A.getFiniteElements();
  file << "Список КЭ" << std::endl;
  std::for_each(FEs.begin(), FEs.end(), [&](FiniteElement &A) { file << A << std::endl;; });

  SFEs = A.getSurfaceFiniteElements();
  file << "Список поверхностных КЭ" << std::endl;
  std::for_each(SFEs.begin(), SFEs.end(), [&](BoundaryFiniteElement &A) { file << A << std::endl; });
  file << std::endl;

  system("pause");
  return 0;
}