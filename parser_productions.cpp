#include "parser_productions.h"
using namespace std;

// Inicializar variable estática
MProducciones Productions::producciones;

void Productions::inicializarProducciones() {
    producciones  = {
        {1, {5, 2, 4}}, //program
        {2, {3, 2}}, //declaraciones
        {3, {-1}}, //declaraciones
        {4, {7}}, //desclaraciones 2
        {5, {6}}, //desclaraciones 2
        {6, {14}}, //desclaraciones 2
        {7, {1005, 15, 1026}}, //declara_class
        {8, {1001, 1000, 1036, 1033, 1037, 5}}, //<DECLARA_LIBRARY>
        {9, {-1}}, //<DECLARA_LIBRARY>
        {10, {1003, 1000,2000, 1039, 10, 1037, 6}}, //<DECLARACONST>
        {11, {-1}}, //<DECLARACONST>
        {12, {1002, 1000, 2000, 8, 1034, 9, 2001, 1037, 7}}, //<DECLARAVAR>
        {13, {-1}}, //<DECLARAVAR>
        {14, {1038, 1000, 2000, 8}}, //<ID2>
        {15, {-1}}, //<ID2>
        {16, {1006}}, //<TIPO_DE_DATO>
        {17, {1007}}, //<TIPO_DE_DATO>
        {18, {1008}}, //<TIPO_DE_DATO>
        {19, {1009}}, //<TIPO_DE_DATO>
        {20, {1010}}, //<TIPO_DE_DATO>
        {21, {1011}}, //<TIPO_DE_DATO>
        {22, {1012}}, //<VALOR_CONSTANTE>
        {23, {1013}}, //<VALOR_CONSTANTE>
        {24, {1014}}, //<VALOR_CONSTANTE>
        {25, {1015}}, //<VALOR_CONSTANTE>
        {26, {1016}}, //<VALOR_CONSTANTE>
        {27, {1000, 13, 1039, 9, 12}}, //<PARAMETROS>
        {28, {-1}}, //<PARAMETROS>
        {29, {1038, 11}}, //<PARAMETROS2>
        {30, {-1}}, //<PARAMETROS2>
        {31, {1038, 1000, 2000, 13}}, //ID3
        {32, {-1}}, //ID3
        {33, {1004, 1000, 1039, 9, 1040, 11, 1041, 7, 15, 1027, 14}}, //<DECLARA_FUNCTION>
        {34, {-1}}, //<DECLARA_FUNCTION>
        {35, {16, 1037, 15}}, //<ESTATUTOS>
        {36, {-1}}, //<ESTATUTOS>
        {37, {17}}, //<ESTATUTOS 2>
        {38, {31}}, //<ESTATUTOS 2>
        {39, {20}}, //<ESTATUTOS 2>
        {40, {22}}, //<ESTATUTOS 2>
        {41, {23}}, //<ESTATUTOS 2>
        {42, {26, 15}}, //<ESTATUTOS>
        {43, {29, 15}}, //<ESTATUTOS>
        {44, {25, 15}}, //<ESTATUTOS>
        {45, {30, 15}}, //<ESTATUTOS>
        {46, {18, 19}}, //<EST_ASIG>
        {47, {1000, 2012}}, //<PROD_ID>
        {48, {1039, 2004, 32, 2011}}, //<PROD_ID2>
        {49, {1047}}, //<PROD_ID2>
        {50, {1048}}, //<PROD_ID2>
        {51, {1018, 1040, 32, 21, 1041, 3012}}, //<EST_WRITE>
        {52, {1038, 32, 21}}, //<EXPRESION>
        {53, {-1}}, //<EXPRESION>
        {54, {1019, 1040, 1000, 2002, 24, 1041, 3013}}, //<EST_READ>
        {55, {1047, 1000}}, //<PREINCUNARIO>
        {56, {1048, 1000}}, //<PREINCUNARIO>
        {57, {1038, 1000, 2000, 24}}, //ID4
        {58, {-1}}, //ID4
        {59, {1022, 3006, 15, 1028, 1040, 32, 1041, 3007, 1032}}, //<EST_DO>
        {60, {1020, 1040, 32, 1041, 3000, 15, 27, 28, 3002, 1031}}, //<EST_IF>
        {61, {1025, 1040, 32, 1041, 15, 27}}, //<LISTA_ELSEIF>
        {62, {-1}}, //<LISTA_ELSEIF>
        {63, {3001, 1024, 15}}, //<OP_ELSE>
        {64, {-1}}, //<OP_ELSE>
        {65, {1021, 3006, 1040, 32, 1041, 3004, 15, 3005, 1029}}, //<EST_WHILE>
        {66, {1023, 1000, 2002, 1040, 32, 3008, 3009, 1035, 32, 3010, 1041, 15, 1030, 3011}},//<EST_FOR>
        {67, {1017, 32}}, //<EST_RETURN>
        {68, {34, 2006, 33}}, //<EXPR>
        {69, {1042, 2008, 34}}, //<EXPR'>
        {70, {-1}}, //<EXPR'>
        {71, {36, 2005, 35}}, //<EXPR2>
        {72, {1043, 2007, 36}}, //<EXPR2'>
        {73, {-1}}, //<EXPR2'>
        {74, {37}}, //<EXPR3>
        {75, {1046, 37}}, //<EXPR3>
        {76, {39, 38}}, //<EXPR4>
        {77, {43, 39}}, //<EXPR4'>
        {78, {-1}}, //<EXPR4'>
        {79, {41, 40}}, //<EXPR5>
        {80, {2006, 1044, 2008, 39}}, //<EXPR5'>
        {81, {2006, 1045, 2008, 39}}, //<EXPR5'>
        {82, {-1}}, //<EXPR5'>
        {83, {47, 42}}, //<TERM>
        {84, {2005, 1055, 2007, 41}}, //<TERM'>
        {85, {2005, 1057, 2007, 41}}, //<TERM'>
        {86, {2005, 1058, 2007, 41}}, //<TERM'>
        {87, {2005, 1056, 2007, 41}}, //<TERM'>
        {88, {-1}}, //<TERM'>
        {89, {1049, 2008}}, //<OPREL>
        {90, {1050, 2008}}, //<OPREL>
        {91, {1053, 2008}}, //<OPREL>
        {92, {1051, 2008}}, //<OPREL>
        {93, {1054, 2008}}, //<OPREL>
        {94, {1052, 2008}}, //<OPREL>
        {95, {1040, 45, 1041}}, //<LLAMADA_F>
        {96, {-1}}, //<LLAMADA_F>
        {97, {1000, 2002, 46}}, //<ID5>
        {98, {1038, 1000, 2000, 46}}, //<ID5'>
        {99, {-1}}, //<ID5'>
        {100, {1000, 2002, 44}}, //<FACT>  cambiar el 2002 por 2001
        {101, {10, 2003}}, //<FACT> agregar funcion de constante
        {102, {2009, 1040, 32, 1041, 2010}} //<FACT>
    };
}


Produccion Productions::obtenerProduccionPorIndice(int indice) {
    if (producciones.empty()) {
        inicializarProducciones();
    }

    auto it = producciones.find(indice);
    return (it != producciones.end()) ? it->second : Produccion{};
}
