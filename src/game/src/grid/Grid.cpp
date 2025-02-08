// Grid.cpp
#include "GridObject.cpp"
#include <string>

class Grid {
public:
    Grid(std::string *param_1,bool param_2,char **param_3);

    void addGridLayer(int param_1);
    int addGridObject(GridObject *param_1,int param_2,int param_3);
    bool areGridsOccupied(GridObject *param_1,int param_2,int param_3);
    void areGridsOccupiedByDynamicObject(int param_1,int param_2,GridObject *param_3);
};