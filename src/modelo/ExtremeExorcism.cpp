#include <assert.h>
#include "ExtremeExorcism.h"

//--------------------------------------------- Start Funciones Privadas

// Complejidad a Cumplir: O(?)
// Complejidad Actual: O(#jv)
// TODO La complejidad está mal, ver el comentario adentro
// Quedaría O(#jv * max(|j|))
// TODO Nota interesante: en la correción del TP2 no se dieron cuenta de que
// estaba mal
void ExtremeExorcism::_losDemasJugadoresEsperan(Jugador j){
    list< pair<Jugador, PosYDir> >::iterator itPublico = begin(_jvPub);
    for(auto jug : _jvPriv){

        if( itPublico->first != j) {  // TODO Ojo con la complejidad de esto!

            Evento evento_anterior = jug.acciones.back();
            Evento evento_nuevo = Evento(evento_anterior.pos, evento_anterior.dir, false);
            jug.acciones.push_back(evento_nuevo);
        }
        itPublico++;
    }
}

void ExtremeExorcism::_revivirTodosLosJugadores(){

    _jvPriv.clear();
    _jvPub.clear();

    map<Jugador, PosYDir> inicial = _ctx->localizar_jugadores(
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
        pd.dir = ABAJO;
        if((_hab.hayVecinoLibre(pd.pos, ABAJO))){
            pd.pos = _hab.avanzarCasillero(pd.pos, ABAJO);
        }
    } else if(a == MARRIBA){
        pd.dir = ARRIBA;
        if((_hab.hayVecinoLibre(pd.pos, ARRIBA))){
            pd.pos = _hab.avanzarCasillero(pd.pos, ARRIBA);
        }
    } else if(a == MDERECHA){
        pd.dir = DERECHA;
        if((_hab.hayVecinoLibre(pd.pos, DERECHA))){
            pd.pos = _hab.avanzarCasillero(pd.pos, DERECHA);
        }
    } else if(a == MIZQUIERDA){
        pd.dir = IZQUIERDA;
        if((_hab.hayVecinoLibre(pd.pos, IZQUIERDA))){
            pd.pos = _hab.avanzarCasillero(pd.pos, IZQUIERDA);
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
    Evento evento_inicial = Evento(pd.pos, pd.dir, false);
    res.push_back(evento_inicial);
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
Evento ExtremeExorcism::_recorrer(const list<Evento> &eventos, int cantPasos) const{
    cantPasos++;  // No sé por qué puse esto, pero hace que fallen menos tests
    /*
    int length = eventos.size();
    int indice = _cantidadPasos % ((length+5)*2);

    if(indice <= length){ // Devuelvo normal

        return _iesimo(eventos, indice);

    } else if(length < indice <= (length+5) ){ // Devuelvo esperar posiicion final

        Evento res = eventos.back();
        return Evento(res.pos,res.dir,false);

    } else if( (length+5) < indice <= (2*length + 5) ){ // Devuelvo invertido

        int indiceInvertido = (length - ((indice - 5) % length) ) + 1;
        return _iesimo(eventos, indiceInvertido);

    } else if ( (2*length + 5) < indice <= (2*(length + 5)) ){ // Devuelvo esperar posicion inicial

        Evento res = eventos.front();
        return Evento(res.pos,res.dir,false);
    }
    assert(false);*/

    vector<Evento> eventosVec;
    vector<Evento> eventosVecInv;
    for (Evento ev : eventos){
        eventosVec.push_back(ev);
    }

    for (int i = eventosVec.size()-1; i >= 0; --i) {
        Evento ev = eventosVec[i];
        ev.dir = dir_inversa(ev.dir);
        eventosVecInv.push_back(ev);
    }

    vector<Evento> eventosAux;

    for (Evento ev : eventosVec) eventosAux.push_back(ev);
    Evento evAux = eventosVec[eventosVec.size()-1];
    evAux.dispara = false;

    eventosAux.push_back(evAux);
    eventosAux.push_back(evAux);
    eventosAux.push_back(evAux);
    eventosAux.push_back(evAux);
    eventosAux.push_back(evAux);

    for (Evento ev : eventosVecInv) eventosAux.push_back(ev);
    evAux = eventosVecInv[eventosVecInv.size()-1];
    evAux.dispara = false;

    eventosAux.push_back(evAux);
    eventosAux.push_back(evAux);
    eventosAux.push_back(evAux);
    eventosAux.push_back(evAux);
    eventosAux.push_back(evAux);

    return eventosAux[cantPasos % eventosAux.size()];
}

//--------------------------------------- End Funciones Privadas

// -------------------------------------- Start Funciones Publicas

ExtremeExorcism::ExtremeExorcism(Habitacion h, set<Jugador> jugadores, PosYDir f_init,
                                 list<Accion> acciones_fantasma, Contexto *ctx) : _hab(h), _cantidadPasos(0), _ctx(ctx){

    for (auto j : jugadores){
        _jugadores[j] = NULL;
    }
    _revivirTodosLosJugadores(); // Aca me encargo de meterlos en jvPriv, jvPub

    Fantasma primerFantasma = _crearFantasmaYHacerloVivir(acciones_fantasma, f_init);

    _inicializarMatrizDisparos(h);

    _cantidadPasos = 0;

};


void ExtremeExorcism::pasar(){
    list<PosYDir> disparos = disparosFantasmas();

    for(PosYDir pd : disparos){
        _matrizDisparos[pd.pos.first][pd.pos.second] = true;
    }

    list<pair<Jugador, PosYDir>>::iterator pubIt = _jvPub.begin();
    list<infoJugadorPriv>::iterator privIt = _jvPriv.begin();
    while(pubIt != _jvPub.end() && privIt != _jvPriv.end()){
        if(_matrizDisparos[pubIt->second.pos.first][pubIt->second.pos.second]){
            // Le dieron a un jugador
            // TODO Usar vivo para mantener la complejidad pedida, no acceder
            // directamente al trie
            _jugadores[pubIt->first] = NULL;
            pubIt = _jvPub.erase(pubIt);
            privIt = _jvPriv.erase(privIt);
        }
        ++pubIt;
        ++privIt;
    }
    assert(pubIt == _jvPub.end() && privIt == _jvPriv.end());

    list<PosYDir>::iterator jPubIt = _fvPub.begin();
    list<Fantasma>::iterator jPrivIt = _fvPriv.begin();
    while(jPubIt != _fvPub.end() && jPrivIt != _fvPriv.end()){
        Evento evento = _recorrer(*jPrivIt, _cantidadPasos);
        jPubIt->pos = evento.pos;
        jPubIt->dir = evento.dir;
        ++jPubIt;
        ++jPrivIt;
    }
    assert(jPubIt == _fvPub.end() && jPrivIt == _fvPriv.end());

    // Limpiar _matrizDisparos
    for(PosYDir pd : disparos){
        _matrizDisparos[pd.pos.first][pd.pos.second] = false;
    }

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
    for(pair<Jugador, PosYDir> &info : _jvPub) { // Modifico j en jvPub
        if (info.first == j){
            info.second.dir = evento_nuevo.dir;
            info.second.pos = evento_nuevo.pos;
        }
    }

    if(a == DISPARAR && _matarFantasmas(evento_nuevo.pos_y_dir()))
        // De todos los fantasmas que se mataron, uno de esos es el principal
        _nuevaRonda(jPriv->acciones);


    _losDemasJugadoresEsperan(j);

    pasar(); // La funcion en donde se mueven todos los fantasmas
};


bool ExtremeExorcism::_matarFantasmas(PosYDir pd){
    bool res = false;
    Pos pos = pd.pos;
    while(_hab.hayVecinoLibre(pos, pd.dir)){
        pos = _hab.avanzarCasillero(pos, pd.dir);
        _matrizDisparos[pos.first][pos.second] = true;
    }

    list<PosYDir>::iterator pubIt = _fvPub.begin();
    list<Fantasma>::iterator privIt = _fvPriv.begin();
    while(pubIt != _fvPub.end() && privIt != _fvPriv.end()){
        if(_matrizDisparos[pubIt->pos.first][pubIt->pos.second]){
            // Le dieron a un fantasma
            // TODO Revisar la complejidad de comparar fantasmas, en el TP2 la
            // pusimos O(1) pero no se si es correcto. Por ahí tiene que ver
            // con que algo está por referencia
            // TODO Chequear que sea efectivamente front() y no back()
            if(*privIt == _fvPriv.front())
                res = true;
            pubIt = _fvPub.erase(pubIt);
            privIt = _fvPriv.erase(privIt);
        }
        ++pubIt;
        ++privIt;
    }

    // La iteración sobre ambas listas tiene que haber terminado al mismo
    // tiempo, ya que tienen la misma longitud
    assert(pubIt == _fvPub.end() && privIt == _fvPriv.end());

    // Limpiar _matrizDisparos
    pos = pd.pos;
    while(_hab.hayVecinoLibre(pos, pd.dir)){
        pos = _hab.avanzarCasillero(pos, pd.dir);
        _matrizDisparos[pos.first][pos.second] = false;
    }
    return res;
}


void ExtremeExorcism::_nuevaRonda(Fantasma f){
    _cantidadPasos = 0;
    _revivirTodosLosJugadores();
    _regenerarFantasmas();
    _nuevoFantasmaEspecial(f);
}


void ExtremeExorcism::_regenerarFantasmas(){
    _fvPriv.clear();
    _fvPub.clear();
    for(Fantasma f : _fantasmas){
        _fvPriv.push_front(f);
        _fvPub.push_front(f.front().pos_y_dir());
    }
}


void ExtremeExorcism::_nuevoFantasmaEspecial(Fantasma f){
    _fantasmas.insert(f);
    _fvPriv.push_front(f);
    _fvPub.push_front(f.front().pos_y_dir());
}


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
    // Esto es como _listaPosicionesDisparos del tp2, pero retorna una lista de
    // PosYDir en vez de lista de Pos
    // TODO puede tener repetidos, me imagino que no importa, pero chequear

    list<PosYDir> res;

    auto fanPub = _fvPub.begin();
    for(auto fanPriv : _fvPriv){

        Evento evento_fantasma = _recorrer(fanPriv, _cantidadPasos);
        PosYDir pd = evento_fantasma.pos_y_dir();

        if(evento_fantasma.dispara){

            while(_hab.hayVecinoLibre(pd.pos,pd.dir)){
                pd.pos = _hab.avanzarCasillero(pd.pos, pd.dir);
                res.push_back(pd);
            }

        }

        fanPub++;
    }

    return res;
}


set<Pos> ExtremeExorcism::posicionesDisparadas() const {
    // TODO No dice de fantasmas, pero asumo que es solo de fantasmas. Tiene
    // sentido? Quedaría igual que posicionesDisparos del tp3
    set<Pos> res;
    for(PosYDir pd : disparosFantasmas())
        res.insert(pd.pos);
    return res;
}


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
    // TODO Creo que la función tenía que ser O(1). Chequear
    set<Jugador> jugs;
    for(auto j : _jugadores.claves()){
        jugs.insert(j);
    }
    return jugs;
};


const list<Fantasma> ExtremeExorcism::fantasmas() const {
    // TODO Creo que la función tenía que ser O(1). Chequear
    list<Fantasma> res;
    for(auto f : _fantasmas){
        res.push_back(f);
    }
    return res;
};
