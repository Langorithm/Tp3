#ifndef STRING_MAP_H_
#define STRING_MAP_H_

#include <string>

using namespace std;

template<typename T>
class string_map {
public:
    /**
    CONSTRUCTOR
    * Construye un diccionario vacio.
    **/
    string_map();

    /**
    CONSTRUCTOR POR COPIA
    * Construye un diccionario por copia.
    **/
    string_map(const string_map<T>& aCopiar);

    /**
    OPERADOR ASIGNACION
     */
    string_map& operator=(const string_map& d);

    /**
    DESTRUCTOR
    **/
    ~string_map();

    /**
    operator[]
    * Acceso o definición de pares clave/valor
    **/

    T &operator[](const string &key);

    /**
    COUNT
    * Devuelve la cantidad de apariciones de la clave (0 o 1).
    * Sirve para identificar si una clave está definida o no.
    **/

    int count(const string &key) const;

    /**
    AT
    * Dada una clave, devuelve su significado.
    * PRE: La clave está definida.
    --PRODUCE ALIASING--
    -- Versión modificable y no modificable
    **/
    const T& at(const string& key) const;
    T& at(const string& key);

    /**
    ERASE
    * Dada una clave, la borra del diccionario junto a su significado.
    * PRE: La clave está definida.
    --PRODUCE ALIASING--
    **/
    void erase(const string& key);

    /**
     SIZE
     * Devuelve cantidad de claves definidas */
    int size() const;

    /**
     EMPTY
     * devuelve true si no hay ningún elemento en el diccionario */
    bool empty() const;

private:

    struct Nodo {
        vector<Nodo*> siguientes;
        T* definicion;

        Nodo() : siguientes(256, NULL), definicion(NULL) {}
        Nodo(T* def) : siguientes(256, NULL), definicion(def) {}

    };

    int num_hijos(const Nodo& nodo) const {
        int c = 0;
        for(int i=0; i<256; i++)
            if(nodo.siguientes[i])
                c++;
        return c;
    }

    Nodo* copiarNodo(const Nodo *nodo) const {
        if(!nodo)
            return NULL;
        Nodo *ret;
        if(nodo->definicion){
            T* def = new T;
            *def = *(nodo->definicion);
            ret = new Nodo(def);
        }
        else
            ret = new Nodo();
        for(int i=0; i<256; i++){
            ret->siguientes[i] = copiarNodo(nodo->siguientes[i]);
        }
        return ret;
    }

    void destruirNodo(Nodo *nodo) const {
        if(!nodo)
            return;
        for(int i=0; i<256; i++)
            destruirNodo(nodo->siguientes[i]);
        if(nodo->definicion)
            delete nodo->definicion;
        delete nodo;
    }

    int longitudNodo(const Nodo *nodo) const {
        if(!nodo)
            return 0;
        int size = 0;
        if(nodo->definicion)
            size = 1;
        for(int i=0; i<256; i++)
            size += longitudNodo(nodo->siguientes[i]);
        return size;
    }

    void eraseAux(Nodo **nodo, const string& clave, int i);

    Nodo* raiz;
};

#include "string_map.hpp"

#endif // STRING_MAP_H_
