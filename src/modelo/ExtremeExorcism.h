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

  const set<Jugador> jugadores() const;

  const list<Fantasma> fantasmas() const;

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
    struct infoFantasmaPriv{
        Fantasma f;
        PosYDir* vivo;
    };

  list< infoJugadorPub > _jvPub;
  list< infoJugadorPriv > _jvPriv;

  string_map<infoJugadorPriv*> _jugadores;

  list< infoFantasmaPriv > _fvPriv;
  list< PosYDir > _fvPub;

  algo2::linear_set<Fantasma> _fantasmas;

  Habitacion _hab;

  vector< vector<bool> > _matrizDisparos;

  int _cantidadPasos;

  // Funciones Privadas

  void _losDemasJugadoresEsperan(Jugador j);
  void _revivirTodosLosJugadores();
  list< Evento > _armoListaDeEventos(const list< Accion > &, PosYDir);
  Evento _hagoEventoConAccionYPosYDir(Accion a, PosYDir &pd);
  void _aplicarMover(Accion a, PosYDir &pd);
  Fantasma _creoFantasmaYLoHagoVivir(const list< Accion > &acciones, PosYDir pd);
  void _limpiarMatrizDisparos(const Habitacion &h);
  Evento _dameEvento(const list<Evento> &eventos, const int cantPasos) const;
  Evento _iesimo(const list<Evento> &eventos, int indice) const;
  Pos _avanzar(Pos, Dir) const;

};

#endif
