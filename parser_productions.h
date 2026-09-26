#ifndef PARSER_PRODUCTIONS_H
#define PARSER_PRODUCTIONS_H

#include <vector>
#include <unordered_map>

using Produccion = std::vector<int>;
using MProducciones = std::unordered_map<int, Produccion>;

class Productions {
private:
    static MProducciones producciones;

public:
    static void inicializarProducciones();
    static Produccion obtenerProduccionPorIndice(int indice);
};

#endif // PARSER_PRODUCTIONS_H
