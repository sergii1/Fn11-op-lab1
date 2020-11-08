#include <iostream>
#include <iterator>
#include <fstream>
#include <algorithm>
#include <string>
#include <vector>
#include <list>
#include <set>
#include <exception>
#include "Types.h"

class cls_MeshLoader {
protected:
    class BadFileFormatException : public std::exception {
        std::string Error;
    public:
        BadFileFormatException() {
            Error.assign("Unknown or unsupported file format!");
        }

        explicit BadFileFormatException(const std::string &file_format) {
            Error.assign("File format \"" + file_format + "\" is not supported!");
        }

        const char *what() const noexcept {
            return Error.c_str();
        }
    };

    class NoFileFoundException : public std::exception {
        std::string error;
    public:
        NoFileFoundException() {
            error.assign("Cannot open file! Probably it does not exist!");
        }

        explicit NoFileFoundException(const std::string &file) {
            error.assign("Cannot open file: \"" + file + "\"! Probably it does not exist!");
        }

        const char *what() const noexcept {
            return error.c_str();
        }
    };

    //размерность пространства
    unsigned m_Dim;
    //количество узлов в одном конечном элементе
    unsigned m_N_NodesinFE;
    //количество узлов в одном поверхностном элементе
    unsigned m_N_NodesinSFE;
    //узлы
    std::vector<Node> m_Nodes;
    //конечные элементы
    std::vector<FiniteElement> m_FEs;
    //поверхностные элементы
    std::vector<BoundaryFiniteElement> m_SFEs;
public:
    //загрузка сетки
    virtual void LoadMesh(const std::string &fileName) = 0;

    //получение контейнера с узлами
    std::vector<Node> getNodes() const;

    //получение контейнера с конечными элементами
    std::vector<FiniteElement> getFiniteElements() const;

    //получение контейнера с поверхностными элементами
    std::vector<BoundaryFiniteElement> getSurfaceFiniteElements() const;

    //нахождение конечного элемента по ID трех узлов
    std::vector<FiniteElement> findFE_byNodes(unsigned p_ID1, unsigned p_ID2, unsigned p_ID3) const;

    //нахождение конечного элемента по ребру (по двум узлам)
    std::vector<FiniteElement> findFE_byEdge(unsigned p_ID1, unsigned p_ID2) const;

    //метод, возвращающий контейнер поверхностных узлов по ID поверхности;
    std::vector<Node> getSFNs_byIDSurface(int p_IDSurface) const;

    //метод, возвращающий контейнер КЭ с заданным ID материала;
    std::vector<FiniteElement> getFEs_byTypeID(int p_IDType) const;

    //метод, возвращающий контейнер поверхностных КЭ с заданным ID поверхности;
    std::vector<BoundaryFiniteElement> getSFEs_byIDSurface(int p_ID) const;

    //метод, создающий контейнер, n-ый элемент которого хранит контейнер всех «соседей» по ребрам для узла n;
    std::vector<std::vector<Node>> Get_Nodes_Neighbors() const;

    /*метод, преобразующий симплексные тетраэдральные КЭ в квадратичные
    путем внесения новых узлов в загруженную КЭ сетку.
    Новые узлы должны располагаться в середине ребер существующих КЭ*/
    void Transform_to_Quadratic();
};
