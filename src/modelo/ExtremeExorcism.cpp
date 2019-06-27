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

void ExtremeExorcism::_revivirTodosLosJugadores(){
    // Completar
}

void ExtremeExorcism::_aplicarMover(Accion a, PosYDir &pd){
    if(a == MABAJO){
        if((_hab.posValida(pd.pos))){
           pd.pos.second++;
           pd.dir = ABAJO;
        }
    } else if(a == MARRIBA){
        if((_hab.posValida(pd.pos))){
            pd.pos.second--;
            pd.dir = ARRIBA;
        }
    } else if(a == MDERECHA){
        if((_hab.posValida(pd.pos))){
            pd.pos.first++;
            pd.dir = DERECHA;
        }
    } else if(a == MIZQUIERDA){
        if((_hab.posValida(pd.pos))){
            pd.pos.first--;
            pd.dir = IZQUIERDA;
        }
    }
}

Evento ExtremeExorcism::_hagoEventoConAccionYPosYDir(Accion a, PosYDir &pd){
    if(a != DISPARAR && a != ESPERAR){

        _aplicarMover(a, pd);
        return Evento(pd.pos,pd.dir,false);

    } else if(a == DISPARAR){

        return  Evento(pd.pos,pd.dir,true);

    } else if(a == ESPERAR){

        return  Evento(pd.pos,pd.dir,false);
    }
}

list< Evento > ExtremeExorcism::_armoListaDeEventos(list< Accion > &acciones, PosYDir pd){
    list< Evento > res;
    for(auto a : acciones){
        Evento evento_nuevo = _hagoEventoConAccionYPosYDir(a, pd); // pd es pasado por referencia y actualizado
        res.push_back(evento_nuevo);
    }
}

void ExtremeExorcism::_creoFantasmaYLoHagoVivir(list< Accion > acciones, PosYDir pd){

    PosYDir posDir_original = pd;
    PosYDir* ptr_fPub;
    ptr_fPub = &posDir_original;

    Fantasma fantasma = _armoListaDeEventos(acciones, pd);

    infoFantasmaPriv info;
    info.vivo = ptr_fPub;
    info.f = fantasma;

    _fvPriv.push_back(info);
    _fvPub.push_back(posDir_original);
    _fantasmas.insert(fantasma);
}
// end Funciones Privadas

// Start Funciones Publicas

ExtremeExorcism::ExtremeExorcism(Habitacion h, set<Jugador> jugadores, PosYDir f_init,
                list<Accion> acciones_fantasma, Contexto *ctx) : _hab(h) {
    for (auto j : jugadores){
        _jugadores[j] = NULL;
    }
    _revivirTodosLosJugadores();

    _creoFantasmaYLoHagoVivir(acciones_fantasma, f_init);
    // TODO Crear eventos


    for(int i=0; i<h.tam(); i++){
        vector<bool> col;
        for(int j=0; j<h.tam(); j++)
            col.push_back(false);
        _matrizDisparos.push_back(col);
    }

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
    PosYDir* fPub = fPriv.vivo;
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
