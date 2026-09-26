#include "semantic_analyzer.h"
#include <QDebug> // Para imprimir en la consola de Qt

SemanticAnalyzer::SemanticAnalyzer() : cont_direcc(0), pListaErrores(nullptr), contadorTemporales(1) {

}

// Implementacion del Setter
void SemanticAnalyzer::setReferenciaErrores(std::vector<std::string>* refLista) {
    pListaErrores = refLista;
}

/*
 * Accion 1 (Push ID) | Accion 2000: Apilar ID para Declaracion
 * Lallama cuando el parser ve un ID en una declaracion
 */
void SemanticAnalyzer::accionDeclararId(const std::string& lexema) {
    // 1. Revisar si ya existe (Regla de no-redeclaración)
    if (tablaSimbolos.count(lexema)) {
        std::string errorMsg = "ERROR SEMÁNTICO: El identificador '" + lexema + "' ya ha sido declarado (Duplicidad).";
        qDebug().noquote() << "Acción (2000): " << QString::fromStdString(errorMsg); // Dejamos el debug
        if (pListaErrores) {
            pListaErrores->push_back(errorMsg); // Se añade a la lista
        }
    } else {
        qDebug().noquote() << "Acción (2000): Apilando ID '" << QString::fromStdString(lexema) << "'";
        pilaLexemas.push(lexema);
    }
}

/*
 * Accion 2 (Pop y registrar)
 * Lallama cuando el parser ve el tipo de datos (of int, of float...)
 */
void SemanticAnalyzer::accionAsignarTipo(const std::string& tipo) {
    qDebug() << "Acción (2001): Registrando variables con tipo '" << QString::fromStdString(tipo) << "'";
    // 1. Sacar todos los IDs de la pila temporal
    while (!pilaLexemas.empty()) {
        std::string lexema_act = pilaLexemas.top();
        pilaLexemas.pop();
        int nuevaDirecc = cont_direcc++;
        Simbolo nuevoSimbolo = { tipo, nuevaDirecc };
        tablaSimbolos.emplace(lexema_act, nuevoSimbolo);
        qDebug() << "  -> Registrado '" << QString::fromStdString(lexema_act) << "' (Tipo: " << QString::fromStdString(tipo) << ")";
    }
}

// (2002)
void SemanticAnalyzer::accionVerificarIdYPushTipo(const std::string& lexema) {
    //qDebug() << "Acción (2002): Verificando ID '" << QString::fromStdString(lexema) << "'";
    auto it = tablaSimbolos.find(lexema);
    if (it == tablaSimbolos.end()) {
        // 1. Mandar el error original
        std::string errorMsg = "ERROR SEMÁNTICO: Variable no declarada '" + lexema + "'";
        qDebug().noquote() << errorMsg;
        if (pListaErrores) {
            pListaErrores->push_back(errorMsg);
        }
        // 2. se parchea con un real
        pilaTipos.push(T_FLOAT);
        qDebug().nospace().noquote() << "Push Tipo: [" << T_FLOAT << "]";
    } else {
        qDebug().noquote() << "Acción (2002/12): Verificando ID '" << QString::fromStdString(lexema) << "'";
        pilaTipos.push(it->second.tipo);
        pilaOperandos.push(lexema); //para guardar el nombre
        historialOperandos.push_back(lexema); //para imprimir pila
        qDebug().nospace().noquote() << "Push Tipo: [" << QString::fromStdString(it->second.tipo) << "]";
    }
    debugImprimirPilaTipos(pilaTipos);
}

// (Accion 1 -> 2003)
void SemanticAnalyzer::accionPushTipoConstante(int tokenEstado, const std::string& lexema) {
    std::string tipo = getTipoConstanteFromToken(tokenEstado);
    qDebug().noquote() << "Acción (2003): Push Tipo Constante -> " << QString::fromStdString(tipo);
    pilaTipos.push(tipo);
    pilaOperandos.push(lexema);
    historialOperandos.push_back(lexema);//imprimir pila
    //qDebug().nospace().noquote() << "Push Tipo: [" << QString::fromStdString(tipo) << "]";
    debugImprimirPilaTipos(pilaTipos);
}

// (Accion 2, 5, 6 -> 2004, 2007, 2008)
void SemanticAnalyzer::accionPushOperador(int opTokenEstado) {
    //qDebug().noquote() << "Acción (200x): Push Op -> " << opTokenToString(opTokenEstado);
    pilaOperadores.push(opTokenEstado);
    historialOperadores.push_back(opTokenToString(opTokenEstado)); // para imprimir pila (guardamos simbolos)
    qDebug().nospace().noquote() << "Push Op: [" << opTokenToString(opTokenEstado) << "]";
    debugImprimirPilaTipos(pilaTipos);
}

// Funcion interna reutilizable
void SemanticAnalyzer::resolverUnaOperacion() {
    // 1. Sacar operandos y operador
    int op = pilaOperadores.top(); pilaOperadores.pop();
    std::string tipo2 = pilaTipos.top(); pilaTipos.pop();
    std::string tipo1 = pilaTipos.top(); pilaTipos.pop();

    // 1.5 Sacar OPERANDOS (Nombres/Valores)
    std::string op2_val = pilaOperandos.top(); pilaOperandos.pop();
    std::string op1_val = pilaOperandos.top(); pilaOperandos.pop();

    std::string opStr = opTokenToString(op); // Convertir 109 a "="

    // 2. Log de la operación
    qDebug().nospace().noquote() << "  -> Resolviendo: [" << tipo1 << " " << opStr << " " << tipo2 << "]";

    // 3. Consultar Cubo Semántico
    std::string tipoResultado = CuboSemantico::verificar(op, tipo1, tipo2);

    // 4. Generar Cuadruplo
    if (op == OP_ASIG) { // Asignacion (=)
        // Cuadruplo: (=, op2, , op1) -> Asigna op2 a op1
        generarCuadruplo("=", op1_val, "null", op2_val);
        // En asignacion no se genera temporal
        pilaOperandos.push(op1_val); // Dejamos el resultado por si acaso (a = b = c)
    } else {
        // Operación Aritmetica/Logica (+, -, *, <, >)
        std::string temporal = nuevoTemporal();
        //generarCuadruplo(opStr, op1_val, op2_val, temporal);
        generarCuadruplo(opStr, op1_val, op2_val, temporal);

        pilaOperandos.push(temporal); // Metemos el temporal T1 a la pila
        historialOperandos.push_back(temporal); // para hacer cuadruplos en interfaz
    }

    // 4.5 Reportar Error (si existe)
    if (tipoResultado == T_ERROR) {
        std::string errorMsg;

        // Checamos si el operador es una asignacion
        if (op == OP_ASIG) { // token 109 (=)
            errorMsg = "ERROR SEMÁNTICO: Tipos incompatibles en asignación (" + tipo1 + " " + opStr + " " + tipo2 + ")";
        } else {
            errorMsg = "ERROR SEMÁNTICO: Operación entre tipos incompatible (" + tipo1 + " " + opStr + " " + tipo2 + ")";
        }

        qDebug().noquote() << "      " << QString::fromStdString(errorMsg); // Indentado
        if (pListaErrores) {
            pListaErrores->push_back(errorMsg);
        }

        // Se parchea ponemos un real en lugar del error
        tipoResultado = T_FLOAT;
        qDebug().nospace().noquote() << "  -> PATCH: Result Type: [" << tipoResultado << "]";

    } else{
        qDebug().nospace().noquote() << "      = " << tipoResultado;
    }

    // 5. Meter el resultado de vuelta a la pila de tipos
    //qDebug().nospace().noquote() << "  -> Result Type: [" << tipoResultado << "]";
    pilaTipos.push(tipoResultado);

    debugImprimirPilaTipos(pilaTipos);
    debugImprimirPilaOperadores(pilaOperadores);
}

// (Accion 3, 4 -> 2005, 2006)
// Usamos un truco: 2005 (Accion 3) es para precedencia alta (*, /)
// 2006 (Accion 4) es para precedencia baja (+, -)
void SemanticAnalyzer::accionResolverOpsPrecedencia(int precedencia) {
    qDebug().noquote() << "Acción (2005/2006): Checando Precedencia (Nivel " << precedencia << ")";
    while (!pilaOperadores.empty()) {
        int op = pilaOperadores.top();

        // 1. Definir Grupos de Operadores
        // Alta: *, /, %, **
        bool esPrecAlta = (op == OP_MULT || op == OP_DIV || op == OP_MOD || op == OP_POT);

        // Baja: +, -
        bool esPrecBaja = (op == OP_SUMA || op == OP_RESTA);

        // Relacional: >, <, >=, <=, ==, !=, &&, ||
        // (Los agregamos aquí para que se resuelvan cuando se pida precedencia baja o general)
        bool esRelacional = (op == OP_MAYOR || op == OP_MENOR || op == OP_MAYOR_IGUAL ||
                             op == OP_MENOR_IGUAL || op == OP_IGUAL || op == OP_DIFERENTE ||
                             op == OP_AND || op == OP_OR);

        // 2. Decidir si resolver
        if (precedencia == 1 && esPrecAlta) {
            resolverUnaOperacion();
        }
        // CAMBIO AQUÍ: Agregamos 'esRelacional' al chequeo de nivel 2
        else if (precedencia == 2 && (esPrecBaja || esRelacional)) {
            resolverUnaOperacion();
        } else {
            break; // No hay mas operaciones de esta precedencia
        }
    }
}

// (Accion 7 -> 2009)
void SemanticAnalyzer::accionPushFondoFalso() {
    //qDebug().noquote() << "Acción (2009): Push Op -> MFF";
    pilaOperadores.push(FONDO_FALSO);
    historialOperadores.push_back("MFF"); //para la pila en interfaz
    qDebug().nospace().noquote() << "Push Op: [MFF]";
    debugImprimirPilaOperadores(pilaOperadores);
}

// (Accion 8 -> 2010)
void SemanticAnalyzer::accionPopFondoFalso() {
    qDebug().noquote() << "Acción (2010): Pop MFF (Resolving inside '()')";
    // Resolver todas las operaciones pendientes dentro del parentesis
    while (!pilaOperadores.empty() && pilaOperadores.top() != FONDO_FALSO) {
        resolverUnaOperacion();
    }

    if (pilaOperadores.empty()) {
        qDebug().noquote() << "  -> ERROR SINTÁCTICO: Falta '('";
    } else {
        pilaOperadores.pop(); // Sacar el FONDO_FALSO
        qDebug().nospace().noquote() << "Pop Op: [MFF]";
    }
    qDebug().noquote() << "  -> MFF Resuelto.";
    debugImprimirPilaTipos(pilaTipos);
    debugImprimirPilaOperadores(pilaOperadores);
}

// (Accion 9 -> 2011)
void SemanticAnalyzer::accionResolverAsignacion() {
    qDebug().noquote() << "Acción (2011): Resolviendo asignación...";
    resolverUnaOperacion(); // La asignacion es solo otra operacion
}


// --- IF
    // Produccion: if ( EXPR ) <3000> ESTATUTOS <3001> else ESTATUTOS endif <3002>

    void SemanticAnalyzer::accionIf_Inicio() { // 3000
    // 1. Generar SF (Salto Falso)
    std::string cond = pilaOperandos.top(); pilaOperandos.pop();
    pilaTipos.pop(); // Sacar el tipo bool

    generarCuadruplo("SF", cond, "", "?"); // "?" es pendiente
    pilaSaltos.push_back(listaCuadruplos.size() - 1); // Guardar indice del SF
    historialSaltos.push_back(listaCuadruplos.size() - 1);
}

void SemanticAnalyzer::accionIf_Else() { // 3001
    // 1. Generar SI (Salto Incondicional) para saltar el ELSE
    generarCuadruplo("SI", "", "", "?");

    // 2. Rellenar el SF anterior (el del IF) para que caiga aqui (al inicio del else)
    int falso = pilaSaltos.back(); pilaSaltos.pop_back();
    rellenar(falso, listaCuadruplos.size()); // Apunta al siguiente cuadruplo actual

    // 3. Guardar el indice del SI para rellenarlo al final
    pilaSaltos.push_back(listaCuadruplos.size() - 1);
    historialSaltos.push_back(listaCuadruplos.size() - 1);
}

void SemanticAnalyzer::accionIf_Fin() { // 3002
    // 1. Rellenar el SI (el que salta el else) o el SF (si no hubo else)
    int fin = pilaSaltos.back(); pilaSaltos.pop_back();
    rellenar(fin, listaCuadruplos.size());
}

// --- WHILE
// Produccion: while <3003> ( EXPR ) <3004> ESTATUTOS endwhile <3005>

void SemanticAnalyzer::accionWhile_Inicio() { // 3003
    // 1. Guardar donde empieza la condicion para volver a evaluar
    pilaSaltos.push_back(listaCuadruplos.size());
    historialSaltos.push_back(listaCuadruplos.size() - 1);
}

void SemanticAnalyzer::accionWhile_FinExpr() { // 3004
    // 1. Generar SF si la condicion falla
    std::string cond = pilaOperandos.top(); pilaOperandos.pop();
    pilaTipos.pop();

    generarCuadruplo("SF", cond, "", "?");
    pilaSaltos.push_back(listaCuadruplos.size() - 1); // Guardar indice SF
    historialSaltos.push_back(listaCuadruplos.size() - 1);
}

void SemanticAnalyzer::accionWhile_Fin() { // 3005
    int falso = pilaSaltos.back(); pilaSaltos.pop_back(); // Sacar SF
    int retorno = pilaSaltos.back(); pilaSaltos.pop_back(); // Sacar Retorno

    // 1. Generar SI para volver a evaluar
    generarCuadruplo("SI", "", "", std::to_string(retorno));

    // 2. Rellenar el SF para que salga acá
    rellenar(falso, listaCuadruplos.size());
}

// --- DO-WHILE (REPEAT)

// Se llama justo después del token 'do' (1022)
void SemanticAnalyzer::accionDo_Inicio() { // 3006
    // Guardamos la direccion actual (el inicio del bucle) en la pila de saltos
    int direccionInicio = listaCuadruplos.size();
    pilaSaltos.push_back(direccionInicio);
    historialSaltos.push_back(listaCuadruplos.size() - 1);
    qDebug().noquote() << "Acción (3006): Inicio Do-While en cuadruplo: " << direccionInicio;
}

// Se llama al final, después de 'dowhile ( EXPR )'
void SemanticAnalyzer::accionDo_Fin() { // 3007
    // 1. Sacar el resultado de la condicion (debe ser bool)
    std::string cond = pilaOperandos.top(); pilaOperandos.pop();
    pilaTipos.pop(); // Sacar el tipo (deberia ser BOOL, se puede validar)

    // 2. Sacar la dirección de retorno de la pila de saltos
    int retorno = pilaSaltos.back(); pilaSaltos.pop_back();

    // 3. Generar SV (Salto Verdadero)
    // Si la condicion es VERDADERA, volvemos al inicio (retorno)
    generarCuadruplo("SV", cond, "", std::to_string(retorno));

    qDebug().noquote() << "Acción (3007): Fin Do-While (SV a " << retorno << ")";
}


// --- FOR
    // Produccion: for id = EXPR to EXPR <3008> do ESTATUTOS endfor <3011>

void SemanticAnalyzer::accionFor_Asignacion() { // 3008
    std::string valInicial = pilaOperandos.top(); pilaOperandos.pop();
    pilaTipos.pop();

    // OJO: No sacamos el ID de la pila todavía, lo necesitaremos para la comparación
    // y el incremento. Solo lo "leemos" (top) o lo dejamos ahí.
    // Estrategia: Dejaremos el ID en la pilaOperandos durante TODO el ciclo.
    std::string id = pilaOperandos.top();
    // (No hacemos pop del ID ni de su tipo)

    // Generar Cuádruplo: id = valInicial
    generarCuadruplo("=", valInicial, "", id);
    qDebug().noquote() << "Acción (3008): For - Asignación Inicial (" << QString::fromStdString(id) << "=" << QString::fromStdString(valInicial) << ")";
}

    // Se llama inmediatamente después de la 3008
    void SemanticAnalyzer::accionFor_Inicio() { // 3009
    // Guardar el retorno (donde empezará la comparación en cada vuelta)
    int direccionInicio = listaCuadruplos.size();
    pilaSaltos.push_back(direccionInicio);
    historialSaltos.push_back(listaCuadruplos.size() - 1);

    /* Generar SF (Salirse si condición falsa)
    generarCuadruplo("SF", cond, "", "?");
    pilaSaltos.push_back(listaCuadruplos.size() - 1);*/
}

    // Se llama después de: ... to EXPR2 <AQUI> )
    void SemanticAnalyzer::accionFor_Comparacion() { // 3010
        // Pilas: [..., id, EXPR2]
        std::string valFinal = pilaOperandos.top(); pilaOperandos.pop();
        pilaTipos.pop();

        std::string id = pilaOperandos.top(); // El ID sigue ahí abajo

        // 1. Generar Comparación: id < valFinal
        std::string temporal = nuevoTemporal();
        generarCuadruplo("<", id, valFinal, temporal);

        // 2. Generar SF (Salto Falso) para salir si no se cumple
        generarCuadruplo("SF", temporal, "", "?");

        // 3. Guardar índice del SF para rellenar luego
        pilaSaltos.push_back(listaCuadruplos.size() - 1);
        historialSaltos.push_back(listaCuadruplos.size() - 1);

        qDebug().noquote() << "Acción (3010): For - Comparación y SF";
    }

void SemanticAnalyzer::accionFor_Fin() { // 3011
    // 1. Incrementar el ID: id = id + 1
                                    // Necesitamos un '1' constante.
                                    std::string id = pilaOperandos.top(); // Sigue ahí!
    std::string temporalInc = nuevoTemporal();

    // Generamos: T_inc = id + 1
    generarCuadruplo("+", id, "1", temporalInc);
    // Generamos: id = T_inc
    generarCuadruplo("=", temporalInc, "", id);

    // 2. Salto Incondicional (SI) al inicio (acción 3009)
    int falso = pilaSaltos.back(); pilaSaltos.pop_back(); // Sacamos el SF
    int retorno = pilaSaltos.back(); pilaSaltos.pop_back(); // Sacamos el Inicio

    generarCuadruplo("SI", "", "", std::to_string(retorno));

    // 3. Rellenar el SF para que apunte aquí (al final del loop)
    rellenar(falso, listaCuadruplos.size());

    // 4. ¡LIMPIEZA! Ahora sí sacamos el ID que guardamos todo este tiempo
    pilaOperandos.pop(); // Adiós id
    pilaTipos.pop();     // Adiós tipo id

    qDebug().noquote() << "Acción (3011): For - Incremento y Fin";
}

// --- READ (Leer) ---
// Producción: read ( id <2002> ) <3012>
// Nota: 2002 ya verificó que la variable existe.
void SemanticAnalyzer::accionRead() { // 3012
    // El ID está en la pila de operandos (lo metió la acción 2002)
    std::string id = pilaOperandos.top(); pilaOperandos.pop();
    pilaTipos.pop(); // Sacamos su tipo

    // Generar: (READ, , , id)
    generarCuadruplo("READ", "", "", id);
    qDebug().noquote() << "Acción (3012): Generar READ para " << QString::fromStdString(id);
}

// --- WRITE (Escribir) ---
// Producción: write ( EXPR ) <3013>
void SemanticAnalyzer::accionWrite() { // 3013
    // El resultado de la expresión está en la pila
    std::string resultado = pilaOperandos.top(); pilaOperandos.pop();
    pilaTipos.pop();

    // Generar: (WRITE, , , result)
    generarCuadruplo("WRITE", "", "", resultado);
    qDebug().noquote() << "Acción (3013): Generar WRITE de " << QString::fromStdString(resultado);
}



/*
 * Funcion Auxiliar
 * Convierte el estado lexico de un tipo (ej 140) a un string (ej int)
 */
std::string SemanticAnalyzer::getTipoConstanteFromToken(int tokenEstado) {
    switch(tokenEstado) {
    case 102: return T_INT;
    case 103: return T_FLOAT;
    case 125: return T_CHAR;
    case 126: return T_STRING;
    default:  return T_ERROR;
    }
}

std::string SemanticAnalyzer::getTipoFromToken(int tokenEstado) {
    switch(tokenEstado) {
    case 140: return "int";
    case 141: return "float";
    case 142: return "char";
    case 143: return "string";
    case 144: return "bool";
    case 145: return "void";
    default:  return "tipo_desconocido";
    }
}

void SemanticAnalyzer::imprimirTablaSimbolos() {
    qDebug() << "--- TABLA DE SÍMBOLOS FINAL ---";
    for (const auto& par : tablaSimbolos) {
        qDebug() << "Lexema: " << QString::fromStdString(par.first)
        << "\t Tipo: " << QString::fromStdString(par.second.tipo)
        << "\t Dir: " << par.second.direccion;
    }
}

std::string SemanticAnalyzer::opTokenToString(int opTokenEstado) const{
    switch (opTokenEstado) {
        case OP_SUMA: return "+";
        case OP_RESTA: return "-";
        case OP_MULT: return "*";
        case OP_DIV: return "/";
        case OP_MOD: return "%";
        case OP_POT: return "**";
        case OP_ASIG: return "=";
        case OP_IGUAL: return "==";
        case OP_DIFERENTE: return "!=";
        case OP_MENOR: return "<";
        case OP_MENOR_IGUAL: return "<=";
        case OP_MAYOR: return ">";
        case OP_MAYOR_IGUAL: return ">=";
        case OP_AND: return "&&";
        case OP_OR: return "||";
        default: return std::to_string(opTokenEstado);
    }
}


void SemanticAnalyzer::reset() {
    qDebug() << "****************************************"
                "--- Reseteando Analizador Semántico ---"
                "****************************************";
    tablaSimbolos.clear();

    // Vaciar todas las pilas
    while (!pilaLexemas.empty()) pilaLexemas.pop();
    while (!pilaTipos.empty()) pilaTipos.pop();
    while (!pilaOperadores.empty()) pilaOperadores.pop();

    cont_direcc = 0;
    // pListaErrores no se toca, se limpia desde el Parser

    // Limpia lo nuevo
    while(!pilaOperandos.empty()) pilaOperandos.pop();
    pilaSaltos.clear();
    listaCuadruplos.clear();
    contadorTemporales = 1;

    historialOperandos.clear();
    historialOperadores.clear();
    historialSaltos.clear();
}



/*
 * Funcion para imprimir el estado actual de la pila tipos
 * Recibe la pila para valor (copia) para no afectar la original
 */
void SemanticAnalyzer::debugImprimirPilaTipos(std::stack<std::string> pila) const {
    QDebug debug = qDebug().nospace().noquote();
    debug << "  [TIPOS] (Top ->): [";
    while (!pila.empty()) {
        debug << pila.top();
        pila.pop();
        if (!pila.empty()) debug << ", ";
    }
    debug << "]";
}


/*
 * Imprime el estado actual de la pila operadores
 * recibe la pila por valor y traduce los tokens a strings
 */
void SemanticAnalyzer::debugImprimirPilaOperadores(std::stack<int> pila) const {
    QDebug debug = qDebug().nospace().noquote();
    debug << "  [OPS]   (Top ->): [";
    while (!pila.empty()) {
        int op = pila.top();
        if (op == FONDO_FALSO) {
            debug << "MFF"; // Marca de Fondo Falso
        } else {
            // Reutilizamos tu función 'opTokenToString' para ver '+', '*', etc.
            debug << opTokenToString(op);
        }
        pila.pop();
        if (!pila.empty()) debug << ", ";
    }
    debug << "]";
}


//funciones axiliares
void SemanticAnalyzer::generarCuadruplo(std::string op, std::string op1, std::string op2, std::string res) {
    listaCuadruplos.emplace_back(op, op1, op2, res);
    qDebug().noquote() << "GEN_Cuadruplos: (" << op.c_str() << ", " << op1.c_str() << ", " << op2.c_str() << ", " << res.c_str() << ")";
}

std::string SemanticAnalyzer::nuevoTemporal() {
    return "R" + std::to_string(contadorTemporales++);
}

void SemanticAnalyzer::rellenar(int direccion, int valor) {
    if (direccion >= 0 && static_cast<size_t>(direccion) < listaCuadruplos.size()) {
        listaCuadruplos[direccion].res = std::to_string(valor);
        qDebug().noquote() << "RELLENAR: Cuadruplo " << direccion << " con salto a " << valor;
    }
}
