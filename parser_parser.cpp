#include "parser_parser.h"
#include "parser_productions.h"
#include <QDebug>
#include <QString>
#include <algorithm>
using namespace std;

Parser::Parser() : tokenActual(0) {
    erroresSintactico = SyntaxTable::inicializarErroresSintacticos();

    SyntaxTable::inicializarLexicoToSintactico();
    SyntaxTable::inicializarMatrizPredictiva();
    SyntaxTable::inicializarTerminalesYNoTerminales();
    Productions::inicializarProducciones();
    semantico.setReferenciaErrores(&listaErroresSemanticos);
}

void Parser::inicializarParser() {
    while (!pilaSintactica.empty()) pilaSintactica.pop();
    pilaSintactica.push(1059);  // "$"
    pilaSintactica.push(1);     // PROGRAM inicial
    tokenActual = 0;
    semantico.reset();
}


bool Parser::analizarSintaxis(const vector<Token>& tokens, const std::string& codigo) {
    tokensAnalisis = tokens;
    codigoFuente = codigo;
    inicializarParser();
    listaErroresSemanticos.clear();

    try {
        while (!pilaSintactica.empty()) {
            int tope = pilaSintactica.top(); //ve el top de la pila
            Token token;
            do { //buclue para sig token valid
                token = tokenActual < tokensAnalisis.size() ?
                            tokensAnalisis[tokenActual] :
                            Token{-1, "$", "FIN", 0};

                // Saltar comentarios (ya deberían estar filtrados por el lexer)
                if (token.estado == 131 || token.estado == 132) {
                    tokenActual++;
                } else {
                    break;
                }
            } while (true);

            int terminal = SyntaxTable::tokenToTerminal(token);

            qDebug() << "Pila:" << tope << "| Token:" << token.lexema << "(" << terminal << ")";

            if (tope == 1059 && terminal == 1059) {
                qDebug() << "Análisis sintáctico exitoso.";
                semantico.imprimirTablaSimbolos();
                return true;
            }

            //manejo de noTerminales
            if (tope >= 1 && tope <= 47) {
                int produccionIdx = SyntaxTable::obtenerProduccionIdx(tope, terminal);
                qDebug() << "Producción obtenida para NoTerminal" << tope << "y Terminal" << terminal;
                if (produccionIdx > 0) {

                    //
                    // --- LOGICA DE ACCIONES SEMANTICAS ---
                    //

                    // Acción 1: Guardar ID (Reglas 12 y 14)
                    Produccion produccion = Productions::obtenerProduccionPorIndice(produccionIdx);
                    if (!produccion.empty()) {
                        pilaSintactica.pop();
                        pushProduccion(produccion);
                        continue;
                    }
                }
                mostrarErrorSintactico(token, 600 + (tope - 1));
                return false;

            } else if (tope >= 1000 && tope <= 1059) {
                // Es un TERMINAL
                if (tope == terminal) {
                    qDebug() << "Salio: " << tope << terminal;
                    pilaSintactica.pop();
                    tokenActual++; // Avanzamos al siguiente token
                } else {
                    mostrarErrorSintactico(token, 700);
                    return false;
                }

            } else if (tope >= 2000 && tope <= 4000) {  //ajustar el rango cando se agreguen acciones
                // ---- Es una ACCION SEMANTICA

                // 1. Llama a la funcion que tiene el SWITCH adentro
                ejecutarAccionSemantica(tope);

                // 2. Saca la accion de la pila.
                //    NO avanza el token (tokenActual++ NO va aqui).
                pilaSintactica.pop();

            }
            else {
                mostrarErrorSintactico(token, 800);
                return false;
            }
        }
    }
    catch (const std::exception& e) {
        // Error fatal durante el análisis
        return false;
    }
    //si sale del bucle sin $
    return false;
}


void Parser::pushProduccion(const Produccion& prod) {

    //qDebug() << "Aplicando producción: [";
    for (int elem : prod) {
        // Mapear numeros a simbolos (terminales/no terminales)
        if (elem >= 1 && elem <= 47) {
            qDebug() << "NoTerminal:" << elem;
        }
        else if (elem >= 1000 && elem <= 1059) {
            qDebug() << "Terminal:" << elem; // Muestra el numero del terminal
        }
        else if (elem == -1) {
            qDebug() << "ε (vacio)";
        }
    }
    qDebug() << "]";


    for (auto it = prod.rbegin(); it != prod.rend(); ++it) {
        if (*it != -1) {
            pilaSintactica.push(*it);
        }
    }
}


void Parser::mostrarErrorSintactico(const Token& token, int codigoError) {
    QString mensaje;

    auto it = erroresSintactico.find(codigoError);
    if (it != erroresSintactico.end()) {
        mensaje = QString("Error sintáctico (%1) en línea %2: %3\n")
                      .arg(codigoError)
                      .arg(obtenerNumeroLinea(token.posicion))
                      .arg(QString::fromStdString(it->second));
    } else {
        mensaje = QString("Error sintáctico desconocido (%1) en línea %2\n")
                      .arg(codigoError)
                      .arg(obtenerNumeroLinea(token.posicion));
    }

    qDebug() << mensaje;

    //resaltarErrorEnCodigo(token.posicion);
}



int Parser::obtenerNumeroLinea(size_t posicion) {
    int lineas = 1;

    // Cambiar el for loop para evitar warning de signo
    for (size_t i = 0; i < posicion && i < codigoFuente.length(); ++i) {
        if (codigoFuente[i] == '\n') {
            lineas++;
        }
    }
    return lineas;
}


void Parser::ejecutarAccionSemantica(int numAccion) {
    // Obtenemos el token ANTERIOR, que es el relevante para la accion
    // (excepto para acciones de precedencia, que no miran el token)
    Token tokenPrevio = (tokenActual > 0) ?
                            tokensAnalisis[tokenActual - 1] :
                            Token{};


    // (A veces necesitamos el token actual, para acciones "pre-token")
    Token tokenSiguiente = (tokenActual < tokensAnalisis.size()) ?
                               tokensAnalisis[tokenActual] :
                               Token{};

    switch (numAccion) {
        // --- Acciones de Declaracion
        case 2000: // PUSH_ID_DECLARAR
            semantico.accionDeclararId(tokenPrevio.lexema);
            break;
        case 2001: // ASIGNAR_TIPO
            semantico.accionAsignarTipo(semantico.getTipoFromToken(tokenPrevio.estado));
            break;

            // --- Acciones de Expresion

        case 2002: // VERIFICAR_ID_Y_PUSH_TIPO
            semantico.accionVerificarIdYPushTipo(tokenPrevio.lexema);
            break;

            // (Accion 1)
        case 2003: // PUSH_TIPO_CONSTANTE
            semantico.accionPushTipoConstante(tokenPrevio.estado, tokenPrevio.lexema);
            break;

            // (Accion 2)
        case 2004: // PUSH_OPERADOR (=)
            semantico.accionPushOperador(tokenPrevio.estado); // OP_ASIG (109)
            break;

            // (Accion 3)
        case 2005: // RESOLVER_PRECEDENCIA_ALTA (*, /)
            semantico.accionResolverOpsPrecedencia(1);
            break;

            // (Accion 4)
        case 2006: // RESOLVER_PRECEDENCIA_BAJA (+, -)
            semantico.accionResolverOpsPrecedencia(2);
            break;

            // (Accion 5)
        case 2007: // PUSH_OPERADOR (*, /, etc.)
            semantico.accionPushOperador(tokenPrevio.estado);
            break;

        // (Accion 6)
        case 2008: // PUSH_OPERADOR (+, -, ||)
            semantico.accionPushOperador(tokenPrevio.estado);
            break;

        // (Accion 7)
        case 2009: // PUSH_FONDO_FALSO
            semantico.accionPushFondoFalso();
            break;

        // (Accion 8)
        case 2010: // POP_FONDO_FALSO
            semantico.accionPopFondoFalso();
            break;

        // (Accion 9)
        case 2011: // RESOLVER_ASIGNACION
            semantico.accionResolverAsignacion();
            break;

        case 2012: // VERIFICAR_ID_Y_PUSH_TIPO (L-Value, para asignacion) Por ahora, hace lo mismo que 2002, pero es po si la logica cambia
            semantico.accionVerificarIdYPushTipo(tokenPrevio.lexema);
            break;

        case 3000: semantico.accionIf_Inicio(); break;
        case 3001: semantico.accionIf_Else(); break;
        case 3002: semantico.accionIf_Fin(); break;

        case 3003: semantico.accionWhile_Inicio(); break;
        case 3004: semantico.accionWhile_FinExpr(); break;
        case 3005: semantico.accionWhile_Fin(); break;

        case 3006: semantico.accionDo_Inicio(); break;
        case 3007: semantico.accionDo_Fin(); break;

        case 3008: semantico.accionFor_Asignacion(); break;
        case 3009: semantico.accionFor_Inicio(); break;
        case 3010: semantico.accionFor_Comparacion(); break;
        case 3011: semantico.accionFor_Fin(); break;

        case 3012: semantico.accionRead(); break;
        case 3013: semantico.accionWrite(); break;
    }
}


std::vector<std::string> Parser::getErroresSemanticos() const {
    return listaErroresSemanticos;
}

std::vector<std::string> Parser::getHistorialOperandos() const { return semantico.getHistorialOperandos(); }
std::vector<std::string> Parser::getHistorialOperadores() const { return semantico.getHistorialOperadores(); }
std::vector<int> Parser::getHistorialSaltos() const { return semantico.getHistorialSaltos(); }
std::vector<Cuadruplo> Parser::getListaCuadruplos() const { return semantico.getListaCuadruplos(); }
