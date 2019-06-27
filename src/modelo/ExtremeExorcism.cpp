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

    _jvPriv.erase(_jvPriv.begin(), _jvPriv.end());
    _jvPub.erase(_jvPub.begin(), _jvPub.end());

    for(auto j : _jugadores.claves()){
        infoJugadorPriv jPriv;
        infoJugadorPriv* jPriv_ptr = &jPriv;


        infoJugadorPub jPub;
        // uso contexto para meter pos, dir e identificador en jPub

        _jugadores[j] = jPriv_ptr;

        _jvPriv.push_back(jPriv);
        _jvPub.push_back(jPub);
    }
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

list< Evento > ExtremeExorcism::_armoListaDeEventos(const list< Accion > &acciones, PosYDir pd){
    list< Evento > res;
    for(auto a : acciones){
        Evento evento_nuevo = _hagoEventoConAccionYPosYDir(a, pd); // pd es pasado por referencia y actualizado
        res.push_back(evento_nuevo);
    }
}

Fantasma ExtremeExorcism::_creoFantasmaYLoHagoVivir(const list< Accion > &acciones, PosYDir pd){

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

    return fantasma;
}

void ExtremeExorcism::_limpiarMatrizDisparos(const Habitacion &h){
    for(int i=0; i<h.tam(); i++){
        vector<bool> col;
        for(int j=0; j<h.tam(); j++)
            col.push_back(false);
        _matrizDisparos.push_back(col);
    }
}
// end Funciones Privadas

// Start Funciones Publicas

ExtremeExorcism::ExtremeExorcism(Habitacion h, set<Jugador> jugadores, PosYDir f_init,
                list<Accion> acciones_fantasma, Contexto *ctx) : _hab(h), _cantidadPasos(0) , _fantasmas(){

    for (auto j : jugadores){
        _jugadores[j] = NULL;
    }
    _revivirTodosLosJugadores(); // Aca me encargo de meterlos en jvPriv, jvPub

    Fantasma primerFantasma = _creoFantasmaYLoHagoVivir(acciones_fantasma, f_init);
    _fantasmas.fast_insert(primerFantasma);

    _limpiarMatrizDisparos(h);

    _cantidadPasos = 0;

};


void ExtremeExorcism::pasar(){

    _cantidadPasos++;
};


void ExtremeExorcism::ejecutarAccion(Jugador j, Accion a){
    // Actualizo los datos del jugador, hay que ver si vive o muere, pues no lo tuve en cuenta.
    // todo lo que sigue podriamos hacerlo en una funcion aparte

    infoJugadorPriv* jPriv = _jugadores[j];

    assert(jPriv != NULL);  // El jugador tiene que estar vivo!

    PosYDir nuevaPosYDir = jPriv->acciones.back().pos_y_dir();
    // Esto de buscar pd en jPriv solo lo podemos hacer cuando ya avanzó por lo menos una vez.
    // Se puede resolver colocando un evento esperar cuando revivimos los jugadores,
    // y despues no darle bola para otra cosa que no sea esta función.
    // O la otra es buscar la pos y la dir en _jvPub siempre, total al revivirlos vamos a inicializarlas en jvPub.
    Evento evento_nuevo = _hagoEventoConAccionYPosYDir(a, nuevaPosYDir);
    jPriv->acciones.push_back(evento_nuevo);

        for(auto i : _jvPub) { // Modifico j en jvPub
            if (i.identificador == j)
                i.dir = evento_nuevo.dir;
            i.pos = evento_nuevo.pos;
        }

     _losDemasJugadoresEsperan(j);
    // Lo mismo que discutimos arriba de como obtener pos y dir se aplica para esta losDemasJugadoresEsperan

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
    } // Esto funciona si eliminamos el nodo de _fvPub y dejamos el puntero de _fvPriv en NULL cada vez que muera
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
    list<PosYDir> res;
    auto fanPub = _fvPub.begin();
    for(auto fanPriv : _fvPriv){
        //Evento evento_fantasma = _dame
        fanPub++;
    }
};


set<Pos> ExtremeExorcism::posicionesDisparadas() const {
    set<Pos> res;
    list<PosYDir> disparos_fantasmas = disparosFantasmas();
    for(auto disp : disparos_fantasmas){
        res.emplace(disp);
    }
    return res;
};


bool ExtremeExorcism::jugadorVivo(Jugador j) const {
    return _jugadores[j] != NULL;
};


const Habitacion &ExtremeExorcism::habitacion() const {
    return _hab;
};


PosYDir ExtremeExorcism::posicionJugador(Jugador j) const {
    assert( _jugadores[j] != NULL);
    for(auto jug : _jvPub){
        if(jug.identificador == j){
            return PosYDir(jug.pos,jug.dir);
        }
    }
};


const set<Jugador> &ExtremeExorcism::jugadores() const {
    set<Jugador> jugs;
    for(auto j : _jugadores.claves()){
        jugs.insert(j);
    }
    return jugs; // La IDE me dice: Reference to stack memory associated with local variable 'jugs' returned
    // Deberiamos poner los jugadores en el heap, no?
};


const list<Fantasma> &ExtremeExorcism::fantasmas() const {
    // COMPLETAR
};
