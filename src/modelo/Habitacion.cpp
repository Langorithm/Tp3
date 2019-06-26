#include "Habitacion.h"
#include "assert.h"
#include <istream>
#include <sstream>
#include <string>

Habitacion string_to_hab(std::istream& is) {
    int row = 0;
    int col = 0;
    int max_col = 0;
    int width;
    int height;

    set<Pos> occupied;

    char c;
    while (is.get(c)) {
        if (c == '#') {
            occupied.insert(Pos(col, row));
            col++;
        } else if (c == '\n') {
            row++;
            max_col = std::max(col, max_col);
            col = 0;
        } else {
            col++;
        }
    }
    width = max_col;
    height = row;

    assert(height == width);

    return Habitacion(height, occupied);
}

Habitacion string_to_hab(string s) {
    std::istringstream is(s);
    int row = 0;
    int col = 0;
    int max_col = 0;
    int width;
    int height;

    set<Pos> occupied;

    char c;
    while (is.get(c)) {
        if (c == '#') {
            occupied.insert(Pos(col, row));
            col++;
        } else if (c == '\n') {
            row++;
            max_col = std::max(col, max_col);
            col = 0;
        } else {
            col++;
        }
    }
    width = max_col;
    height = row;

    assert(height == width);

    return Habitacion(height, occupied);
}

Habitacion::Habitacion(unsigned int tam, set<Pos> ocupadas){
    for(int i=0; i<tam; i++){
        vector<bool> col(tam);
        for(int j=0; j<tam; j++)
            if(ocupadas.count(make_pair(i, j)))
                col.push_back(true);
            else
                col.push_back(false);
        matriz.push_back(col);
    }
}

unsigned int Habitacion::tam() const {
    return matriz.size();
}

bool Habitacion::ocupado(Pos pos) const {
    return pos.first >= 0 &&
        pos.first < tam() &&
        pos.second >= 0 &&
        pos.second < tam() &&
	    matriz[pos.first][pos.second];
};

bool Habitacion::operator==(const Habitacion& hab2) const{
    return matriz == hab2.matriz;
};
