#pragma once

#include "cls_MeshLoader.h"

class cls_AneuMeshLoader :
        public cls_MeshLoader {
public:
    virtual void LoadMesh(const std::string &fileName);
};

