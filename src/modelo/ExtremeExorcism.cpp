#include "ExtremeExorcism.h"

ExtremeExorcism::ExtremeExorcism(Habitacion h, set<Jugador> jugadores, PosYDir f_init,
                list<Accion> acciones_fantasma, Contexto *ctx){
    _hab = h;
    // Me hago el fantasma principal y lo meto en this
    //fantasma inicial = hagoFantasma(acciones_fantasma, f_init);
    for (auto j : jugadores){
        // insertar en el trie jugadores
        // insertar en jvPriv y jvPub
        // uso _revivirTodosLosJugadores
    }
    // Usando contexto defino las posiciones y direcciones de los jugadores
};


void ExtremeExorcism::pasar(){

};


void ExtremeExorcism::ejecutarAccion(Jugador j, Accion a){
    infoJugadorPriv jPriv = _jugadores.at(j);
    Evento evento_anterior = jPriv.acciones.back();
    Evento evento_nuevo = evento_anterior;
    if (a == DISPARAR){ // esDisparo?(a);
        evento_nuevo.dispara = true;

            // if _matar_fantasmas(evento_nuevo.pos, evento_nuevo.pos) then nuevaROnda
    } else if( a != DISPARAR && a != ESPERAR ){ // esMover?(a)
        Pos nuevaPosicion = evento_anterior.pos;
        Dir nuevaDireccion = evento_anterior.dir;
        if(){ // hayVecinoLibre(avanzarCasillero(pos, dir)) then nuevaPosicion = avanzarCasillero, same with nuevaDir
              // Propongo hacer la funcion privada hayVecinoLibre en la clase ExtremeExorcism.
        }
        // Ademas, falta la logica en donde al moverse el jugador en este paso, muere
        evento_nuevo.pos = nuevaPosicion; //(nuevaPosicion, nuevaDireccion, false)
        evento_nuevo.dir = nuevaDireccion;
        evento_nuevo.dispara = false;
        /* Aca se supone que actualizamos la pos y dir del jugador en la lista publica, pero la forma que tenemos
         * de hacerlo es iterando sobre jvPub entero. que es O(#jv), chequear que no rompa la complejidad.
         */
    } else if(a == ESPERAR){ //esEsperar(a)
        Evento evento_nuevo = evento_anterior;
        evento_nuevo.dispara = false;
    }
    jPriv.acciones.push_back(evento_nuevo);
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
