#include <assert.h>
#include "ExtremeExorcism.h"
// Start Funciones Privadas

void ExtremeExorcism::_losDemasJugadoresEsperan(Jugador j){
    list< infoJugadorPub >::iterator itPublico = begin(_jvPub);
    for(auto jug : _jvPriv){

        if( itPublico->identificador != j) {

            Evento evento_anterior = jug.acciones.back();
            Evento evento_nuevo = Evento(evento_anterior.pos, evento_anterior.dir, false);
            jug.acciones.push_back(evento_nuevo);
        }
        itPublico++;
    }
}
// end Funciones Privadas

// Start Funciones Publicas

ExtremeExorcism::ExtremeExorcism(Habitacion h, set<Jugador> jugadores, PosYDir f_init,
                list<Accion> acciones_fantasma, Contexto *ctx) : _hab(h) {
    // inicializar todas las estructuras privadas

    // fantasma PrimerFantasma = hacer un fantasma a partir del contexto y meterlo en _fvPriv y Pub
    //fantasma inicial = hagoFantasma(acciones_fantasma, f_init);

    for (auto j : jugadores){
        infoJugadorPriv jPriv; // Hago al jugador privado
        infoJugadorPriv* jPriv_ptr = &jPriv; // Hago el pointer a donde esta la infoPriv del jugador
        list < Evento > vacia; // inicializo las acciones;
        jPriv.acciones = vacia; // meto las acciones en la infoPriv del jugador

        _jugadores.at(j) = jPriv_ptr;
        _jvPriv.push_back(jPriv);
        // _jvPub.push_back(); meter en esto la posicion inicial y la posicion siguiente
        // Problema posible: el pointer apunta a la informacion del jugador directamente, no al nodo del contenedo
        // Esto puede resultarnos en no poder eliminar el nodo de la lista en O(1), ver si rompe complejidad
        // Segundo Problema: Como la funcion ejecutarAccion() toma el evento pasado del jugador, deberiamos pushear
        // como primer Evento un evento (posInicial, dirInicial, ESPERAR).

        // No uso _revivirTodosLosJugadores pues es necesario que cuando llamo al significado del trie con at(clave),
        // pueda en el momento asignarle la clave, que es del tipo puntero(infoPriv)
    }
    // Usando contexto defino las posiciones y direcciones de los jugadores
};


void ExtremeExorcism::pasar(){
    // Es la funcion en donde avanzan todos los fantasmas
    _cantidadPasos++;
};


void ExtremeExorcism::ejecutarAccion(Jugador j, Accion a){
    _cantidadPasos = 0;
    infoJugadorPriv* jPriv = _jugadores.at(j); // O(#jv)
    assert(jPriv != NULL);  // El jugador tiene que estar vivo!
    Evento evento_anterior = jPriv->acciones.back(); // Encuentro su ultimo evento
    Evento evento_nuevo = evento_anterior; // Preparo el evento que voy a meter en su lista de acciones
    if (a == DISPARAR){ // esDisparo?(a)
        evento_nuevo.dispara = true;

            // if _matar_fantasmas(evento_nuevo.pos, evento_nuevo.pos) then nuevaROnda
    } else if( a != DISPARAR && a != ESPERAR ){ // esMover?(a)
        Pos nuevaPosicion = evento_anterior.pos;
        Dir nuevaDireccion = evento_anterior.dir;
        if(0){ // hayVecinoLibre(avanzarCasillero(pos, dir)) then nuevaPosicion = avanzarCasillero, same with nuevaDir
              // Propongo hacer la funcion privada hayVecinoLibre en la clase ExtremeExorcism.
        }
        // Ademas, falta la logica en donde al moverse el jugador en este paso muere.
        // En el caso en que muera, no deberiamos permitir que el flow del programa siga su rumbo
        evento_nuevo.pos = nuevaPosicion; //(nuevaPosicion, nuevaDireccion, false)
        evento_nuevo.dir = nuevaDireccion;
        evento_nuevo.dispara = false;
        // Actualizamos la pos y dir del jugador en la lista publica
        for(auto i : _jvPub){
            if(i.identificador == j)
                i.dir = nuevaDireccion;
                i.pos = nuevaPosicion;
        }
    } else if(a == ESPERAR){ //esEsperar(a)
        evento_nuevo.dispara = false;
    }
    jPriv->acciones.push_back(evento_nuevo);
     _losDemasJugadoresEsperan(j);
     pasar(); // La funcion en donde se mueven todos los fantasmas
};


list<pair<Jugador, PosYDir>> ExtremeExorcism::posicionJugadores() const {
    list<pair<Jugador, PosYDir>> res;
    for( auto j : _jvPub){
        PosYDir posicionYDireccion = PosYDir(j.pos,j.dir);
        pair<Jugador, PosYDir> id_PosYDir = make_pair(j.identificador,posicionYDireccion);

        res.push_back(id_PosYDir);
    }
    return res;
};


list<PosYDir> ExtremeExorcism::posicionFantasmas() const {
    list<PosYDir> posicionesFantasmas;
    for(auto f : _fvPriv) {
        if (f.vivo != NULL) {
            PosYDir posYDirFantasma = PosYDir(f.vivo->pos,f.vivo->dir);
            posicionesFantasmas.push_back(posYDirFantasma);
        }
    } // Esto depende de que eliminemos el nodo de _fvPub y dejemos el puntero de _fvPriv en NULL cada vez que muera
      // un fantasma.
};


PosYDir ExtremeExorcism::posicionEspecial() const {
    infoFantasmaPriv fPriv = _fvPriv.front();
    infoFantasmaPub* fPub = fPriv.vivo;
    Pos posicion = fPub->pos;
    Dir direccion = fPub->dir;
    return PosYDir(posicion,direccion);
};


list<PosYDir> ExtremeExorcism::disparosFantasmas() const {
    // COMPLETAR
};


set<Pos> ExtremeExorcism::posicionesDisparadas() const {
    // COMPLETAR
};


bool ExtremeExorcism::jugadorVivo(Jugador j) const {
    for(auto jug : _jvPub) {
        if (j == jug.identificador) {
            return true;
        }
    }
    return false;
};


const Habitacion &ExtremeExorcism::habitacion() const {
    return _hab;
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
