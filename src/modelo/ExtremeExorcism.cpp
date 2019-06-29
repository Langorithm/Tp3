#include <assert.h>
#include "ExtremeExorcism.h"

//--------------------------------------------- Start Funciones Privadas

// Complejidad a Cumplir: O(?)
// Complejidad Actual: O(#jv)
void ExtremeExorcism::_losDemasJugadoresEsperan(Jugador j){
    list< pair<Jugador, PosYDir> >::iterator itPublico = begin(_jvPub);
    for(auto jug : _jvPriv){

        if( itPublico->first != j) {  // Ojo con la complejidad de esto!

            Evento evento_anterior = jug.acciones.back();
            Evento evento_nuevo = Evento(evento_anterior.pos, evento_anterior.dir, false);
            jug.acciones.push_back(evento_nuevo);
        }
        itPublico++;
    }
}

void ExtremeExorcism::_revivirTodosLosJugadores(Contexto *ctx){

    _jvPriv.clear();
    _jvPub.clear();

    map<Jugador, PosYDir> inicial = ctx->localizar_jugadores(
        _jugadores.claves(),
        fantasmas(),
        _hab
    );

    for(Jugador j : _jugadores.claves()){
        infoJugadorPriv jPriv;
        PosYDir pd = inicial.at(j);
        pair<Jugador, PosYDir> jPub(j, PosYDir(pd.pos, pd.dir));

        // Agrego el evento esperar asi funciona ejecAccion
        Evento evento_inicial = Evento(pd.pos, pd.dir, false);
        jPriv.acciones.push_back(evento_inicial);


        _jvPriv.push_back(jPriv);
        _jugadores[j] = &_jvPriv.back();

        _jvPub.push_back(jPub);
    }
}

PosYDir ExtremeExorcism::_aplicarMover(Accion a, PosYDir pd){
    if(a == MABAJO){
        if((_hab.proxima_posValida(pd.pos, ABAJO))){
           pd.pos.second++;
           pd.dir = ABAJO;
        }
    } else if(a == MARRIBA){
        if((_hab.proxima_posValida(pd.pos, ARRIBA))){
            pd.pos.second--;
            pd.dir = ARRIBA;
        }
    } else if(a == MDERECHA){
        if((_hab.proxima_posValida(pd.pos, DERECHA))){
            pd.pos.first++;
            pd.dir = DERECHA;
        }
    } else if(a == MIZQUIERDA){
        if((_hab.proxima_posValida(pd.pos, IZQUIERDA))){
            pd.pos.first--;
            pd.dir = IZQUIERDA;
        }
    }
    return pd;
}

Evento ExtremeExorcism::_crearEvento(Accion a, PosYDir pd){
    if(a == DISPARAR){
        return Evento(pd.pos,pd.dir,true);
    } else if(a == ESPERAR){
        return Evento(pd.pos,pd.dir,false);
    }else{
        PosYDir nuevaPd = _aplicarMover(a, pd);
        return Evento(nuevaPd.pos, nuevaPd.dir, false);
    }
}

list< Evento > ExtremeExorcism::_armarListaDeEventos(const list< Accion > &acciones, PosYDir pd){
    list< Evento > res;
    for(auto a : acciones){
        Evento evento_nuevo = _crearEvento(a, pd);
        pd = evento_nuevo.pos_y_dir(); // Al quitar las referencias, falto actualizar la pos y la dir al nuevo valor
        res.push_back(evento_nuevo);
    }
    return res;
}

Fantasma ExtremeExorcism::_crearFantasmaYHacerloVivir(const list< Accion > &acciones, PosYDir pd){

    Fantasma fantasma = _armarListaDeEventos(acciones, pd);

    _fvPub.push_back(pd);
    _fvPriv.push_back(fantasma);
    _fantasmas.insert(fantasma);

    return fantasma;
}

void ExtremeExorcism::_inicializarMatrizDisparos(const Habitacion &h){
    for(int i=0; i<h.tam(); i++){
        vector<bool> col;
        for(int j=0; j<h.tam(); j++)
            col.push_back(false);
        _matrizDisparos.push_back(col);
    }
}

Evento _iesimo(const list<Evento> &eventos, int indice) {
    int i = 0;
    for(auto e : eventos){
        if(i == indice){
            return e;
        }
        i++;
    }
    assert(false);  // Sí o sí tiene que retornar algo en la línea de arriba
}

// Sospecho error by one
// Cuidado con la complejidad de esto!
Evento ExtremeExorcism::_dameEvento(const list<Evento> &eventos, const int cantPasos) const{
    int tamano = eventos.size();
    int indice = cantPasos % (tamano * 2);
    if(indice < tamano){
        return _iesimo(eventos, indice);
    } else {
        indice = tamano - (indice % tamano);
        return _iesimo(eventos, indice);
    }
}

Pos ExtremeExorcism::_avanzar(Pos pos, Dir dir) const{
    if(dir == ABAJO){
        pos.second--;

    } else if(dir == ARRIBA) {
        pos.second++;

    } else if(dir == DERECHA){
        pos.first++;

    } else if(dir == IZQUIERDA){
        pos.first--;
    }
    return pos;
}
//--------------------------------------- End Funciones Privadas

// -------------------------------------- Start Funciones Publicas

ExtremeExorcism::ExtremeExorcism(Habitacion h, set<Jugador> jugadores, PosYDir f_init,
                list<Accion> acciones_fantasma, Contexto *ctx) : _hab(h), _cantidadPasos(0) , _fantasmas(){

    for (auto j : jugadores){
        _jugadores[j] = NULL;
    }
    _revivirTodosLosJugadores(ctx); // Aca me encargo de meterlos en jvPriv, jvPub

    Fantasma primerFantasma = _crearFantasmaYHacerloVivir(acciones_fantasma, f_init);

    _inicializarMatrizDisparos(h);

    _cantidadPasos = 0;

};


void ExtremeExorcism::pasar(){

    _cantidadPasos++;
};


void ExtremeExorcism::ejecutarAccion(Jugador j, Accion a){

    infoJugadorPriv* jPriv = _jugadores[j];

    assert(jPriv != NULL);  // El jugador tiene que estar vivo!

    assert(jPriv->acciones.size() > 0);
    PosYDir nuevaPosYDir = jPriv->acciones.back().pos_y_dir();
    Evento evento_nuevo = _crearEvento(a, nuevaPosYDir);
    jPriv->acciones.push_back(evento_nuevo);

    // TODO Usar el "vivo?" del diseño para que nos de la complejidad
    for(auto info : _jvPub) { // Modifico j en jvPub
        if (info.first == j){
            info.second.dir = evento_nuevo.dir;
            info.second.pos = evento_nuevo.pos;
        }
    }

     _losDemasJugadoresEsperan(j);

     pasar(); // La funcion en donde se mueven todos los fantasmas
};


list<pair<Jugador, PosYDir>> ExtremeExorcism::posicionJugadores() const {
    return _jvPub;
};


list<PosYDir> ExtremeExorcism::posicionFantasmas() const {
    return _fvPub;
};


PosYDir ExtremeExorcism::posicionEspecial() const {
    return _fvPub.front();
};


list<PosYDir> ExtremeExorcism::disparosFantasmas() const {

    list<PosYDir> res;

    auto fanPub = _fvPub.begin();
    for(auto fanPriv : _fvPriv){

        Evento evento_fantasma = _dameEvento(fanPriv, _cantidadPasos);
        PosYDir pd = evento_fantasma.pos_y_dir();

        if(evento_fantasma.dispara){

            while(_hab.proxima_posValida(pd.pos,pd.dir)){
                pd.pos = _avanzar(pd.pos, pd.dir);
                res.push_back(pd);
            }

        }

        fanPub++;
    }
}


set<Pos> ExtremeExorcism::posicionesDisparadas() const {
    set<Pos> res;
    list<PosYDir> disparos_fantasmas = disparosFantasmas();
    for(auto disp : disparos_fantasmas){
        res.emplace(disp.pos);
    }
    return res;
};


bool ExtremeExorcism::jugadorVivo(Jugador j) const {
    return _jugadores.at(j) != NULL;
};


const Habitacion &ExtremeExorcism::habitacion() const {
    return _hab;
};


PosYDir ExtremeExorcism::posicionJugador(Jugador j) const {
    assert( _jugadores.at(j) != NULL);
    for(auto jug : _jvPub){
        if(jug.first == j){
            return jug.second;
        }
    }
};


const set<Jugador> ExtremeExorcism::jugadores() const {
    set<Jugador> jugs;
    for(auto j : _jugadores.claves()){
        jugs.insert(j);
    }
    return jugs;
};


const list<Fantasma> ExtremeExorcism::fantasmas() const {
    list<Fantasma> res;
    for(auto f : _fantasmas){
        res.push_back(f);
    }
    return res;
};
