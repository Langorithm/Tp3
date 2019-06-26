#include "ExtremeExorcism.h"

ExtremeExorcism::ExtremeExorcism(Habitacion h, set<Jugador> jugadores, PosYDir f_init,
                list<Accion> acciones_fantasma, Contexto *ctx){
    _hab = h;
    // Me hago el fantasma principal y lo meto en this
    //fantasma inicial = hagoFantasma(acciones_fantasma, f_init);
    for (auto j : jugadores){
        // insertar el trie jugadores
    }
    // Usando contexto defino las posiciones y direcciones de los jugadores
};


void ExtremeExorcism::pasar(){

};


void ExtremeExorcism::ejecutarAccion(Jugador j, Accion a){
    // COMPLETAR
};


list<pair<Jugador, PosYDir>> ExtremeExorcism::posicionJugadores() const {
    // COMPLETAR
};


list<PosYDir> ExtremeExorcism::posicionFantasmas() const {
    // COMPLETAR
};


PosYDir ExtremeExorcism::posicionEspecial() const {
    // COMPLETAR
};


list<PosYDir> ExtremeExorcism::disparosFantasmas() const {
    // COMPLETAR
};


set<Pos> ExtremeExorcism::posicionesDisparadas() const {
    // COMPLETAR
};


bool ExtremeExorcism::jugadorVivo(Jugador j) const {
    // COMPLETAR
};


const Habitacion &ExtremeExorcism::habitacion() const {
    // COMPLETAR
};


PosYDir ExtremeExorcism::posicionJugador(Jugador j) const {
    // COMPLETAR
};


const set<Jugador> &ExtremeExorcism::jugadores() const {
    // COMPLETAR
};


const list<Fantasma> &ExtremeExorcism::fantasmas() const {
    // COMPLETAR
};
