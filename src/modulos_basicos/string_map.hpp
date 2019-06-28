template <typename T>
string_map<T>::string_map(){
    raiz = NULL;
}

template <typename T>
string_map<T>::string_map(const string_map<T>& aCopiar) : string_map() { *this = aCopiar; } // Provisto por la catedra: utiliza el operador asignacion para realizar la copia.

template <typename T>
string_map<T>& string_map<T>::operator=(const string_map<T>& d) {
    destruirNodo(raiz);
    raiz = copiarNodo(d.raiz);
}

template <typename T>
string_map<T>::~string_map(){
    destruirNodo(raiz);
}

template <typename T>
T& string_map<T>::operator[](const string& clave){
    /* return *(new T()); */

    if(!raiz)
        raiz = new Nodo();

    Nodo** nodo = &raiz;

    for(int i=0; i<clave.length(); i++){
        int c = int(clave[i]);
        nodo = &((*nodo)->siguientes[c]);
        if(*nodo == NULL)
            *nodo = new Nodo();
    }

    T** pdef = &((*nodo)->definicion);
    if(!*pdef){
        *pdef = new T;
    }
    return **pdef;
}


template <typename T>
int string_map<T>::count(const string& clave) const {
    Nodo* nodo = raiz;
    if(!nodo)
        return 0;
    int i;
    for(i = 0; i<clave.length() && nodo; i++){
        int c = int(clave[i]);
        nodo = nodo->siguientes[c];
    }
    if (i == clave.length() && nodo && nodo->definicion)
        return 1;
    else
        return 0;
}

template <typename T>
const T& string_map<T>::at(const string& clave) const {
    Nodo *nodo = raiz;
    for(int i=0; i<clave.length(); i++)
        nodo = nodo->siguientes[int(clave[i])];
    return *(nodo->definicion);
}

template <typename T>
T& string_map<T>::at(const string& clave) {
    Nodo *nodo = raiz;
    for(int i=0; i<clave.length(); i++)
        nodo = nodo->siguientes[int(clave[i])];
    return *(nodo->definicion);
}

template <typename T>
void string_map<T>::erase(const string& clave) {
    eraseAux(&raiz, clave, 0);
}

template <typename T>
void string_map<T>::eraseAux(Nodo** nodo, const string& clave, int i) {
    if(!*nodo)
        return;
    if(i < clave.length()){
        int c = int(clave[i]);
        i++;
        eraseAux(&((*nodo)->siguientes[c]), clave, i);
    }else{
        assert((*nodo)->definicion); // es una hoja
        delete (*nodo)->definicion;
        (*nodo)->definicion = NULL;
    }
    if(num_hijos(**nodo) == 0){
        destruirNodo(*nodo);
        *nodo = NULL;
    }
}

template <typename T>
int string_map<T>::size() const{
    return longitudNodo(raiz);
}

template <typename T>
bool string_map<T>::empty() const{
    return !raiz;
}

template <typename T>
algo2::linear_set<string> string_map<T>::claves() const {
    algo2::linear_set<string> res;
    clavesRecursiva(raiz, res, "");
    return res;
};
