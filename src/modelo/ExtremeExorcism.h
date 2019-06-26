#ifndef EXTREME_EXORCISM_H
#define EXTREME_EXORCISM_H



#include "Contexto.h"
#include "Habitacion.h"
#include "TiposJuego.h"
#include "modulos_basicos/string_map.h"
#include <list>
#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>
#include <modulos_basicos/linear_set.h>

using namespace std;

class ExtremeExorcism {
public:
  ExtremeExorcism(Habitacion h, set<Jugador> jugadores, PosYDir f_init,
                  list<Accion> acciones_fantasma, Contexto *ctx);

  void pasar();

  void ejecutarAccion(Jugador j, Accion a);

  list<pair<Jugador, PosYDir>> posicionJugadores() const;

  list<PosYDir> posicionFantasmas() const;

  PosYDir posicionEspecial() const;

  list<PosYDir> disparosFantasmas() const;

  set<Pos> posicionesDisparadas() const;

  bool jugadorVivo(Jugador j) const;

  const Habitacion &habitacion() const;

  PosYDir posicionJugador(Jugador j) const;

  const set<Jugador> &jugadores() const;

  const list<Fantasma> &fantasmas() const;

private:
  // Empiezo por declarar todas las estructuras que estan en la representacion del modulo juego.md del tp2
  // Nótese que algunos structs (como Evento) estan en TiposJuego.h

    struct infoJugadorPub{
      Jugador identificador;
      Pos pos;
      Dir dir;
  };
    struct infoJugadorPriv{
        list<Evento> acciones;
    };
    struct infoFantasmaPub{
        Pos pos;
        Dir dir;
    };
    struct infoFantasmaPriv{
        Fantasma f;
        infoFantasmaPub** vivo;
    };

  list< infoJugadorPub > _jvPub;

  list< infoJugadorPriv > _jvPriv;

  string_map _jugadores;

  list< infoFantasmaPriv > _fvPriv;
  list< infoFantasmaPub > _fvPub;
  algo2::linear_set _fantasmas;
  Habitacion _hab;
  vector< vector<bool> > _matrizDisparos;
  int _cantidad_Pasos;
};

#endif
