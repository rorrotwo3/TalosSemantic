#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "lexer_automata.h"
#include <QFileDialog>
#include <QMessageBox>
#include <unordered_map>
#include <string>
#include <ctype.h>
#include <set>
#include <QTextStream>
#include "ui_mainwindow.h"
using namespace std;



int matriz[26][31] = {
    {1, 2, 3, 506, 134, 2, 1, 19, 20, 9, 10, 11, 12, 13, 14, 15, 17, 128, 21, 25, 127, 119, 120, 121, 122, 123, 124, 506, 0, 0, 0},          // q0
    {1, 2, 2, 2, 100, 2, 1, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100}, // q1
    {2, 2, 2, 2, 101, 2, 1, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101, 101}, // q2
    {102, 102, 3, 102, 4, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102, 102}, // q3
    {500, 500, 5, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500}, // q4
    {103, 103, 5, 103, 103, 6, 6, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103, 103}, // q5
    {501, 501, 8, 501, 501, 501, 501, 7, 7, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501, 501}, // q6
    {502, 502, 8, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502, 502}, // q7
    {104, 104, 8, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104, 104}, // q8
    {109, 109, 109, 109, 109, 109, 109, 109, 109, 110, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109, 109}, // q9
    {111, 111, 111, 111, 111, 111, 111, 111, 111, 112, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111, 111}, // q10
    {113, 113, 113, 113, 113, 113, 113, 113, 113, 114, 113, 113, 113, 113, 113, 113, 113, 112, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113, 113}, // q11
    {116, 116, 116, 116, 116, 116, 116, 116, 116, 115, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116, 116}, // q12
    {503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 117, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503, 503}, // q13
    {504, 504, 504, 504, 504, 504, 504, 504, 504, 504, 504, 504, 504, 504, 118, 504, 504, 504, 504, 504, 504, 504, 504, 504, 504, 504, 504, 504, 504, 504, 504}, // q14
    {16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 505, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16}, // q15
    {507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 125, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507, 507}, // q16
    {17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 18, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17}, // q17
    {126, 126, 126, 126, 126, 126, 126, 126, 126, 126, 126, 126, 126, 126, 126, 126, 17, 126, 126, 126, 126, 126, 126, 126, 126, 126, 126, 126, 126, 126, 126}, // q18
    {105, 105, 105, 105, 105, 105, 105, 129, 105, 105, 105, 105, 105, 105, 105, 105, 105, 105, 105, 105, 105, 105, 105, 105, 105, 105, 105, 105, 105, 105, 105}, // q19
    {106, 106, 106, 106, 106, 106, 106, 106, 130, 106, 106, 106, 106, 106, 106, 106, 106, 106, 106, 106, 106, 106, 106, 106, 106, 106, 106, 106, 106, 106, 106}, // q20
    {108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 22, 23, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108}, // q21
    {22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 131, 22}, // q22
    {23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 24, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23}, // q23
    {23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 132, 24, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23}, // q24
    {107, 107, 107, 107, 107, 107, 107, 107, 107, 107, 107, 107, 107, 107, 107, 107, 107, 107, 107, 133, 107, 107, 107, 107, 107, 107, 107, 107, 107, 107, 107}  // q25
};




// Mapa de terminales (símbolos de entrada)
std::unordered_map<std::string, int> terminales = {
    {"id", 1000},
    {"include", 1001},
    {"def", 1002},
    {"const", 1003},
    {"function", 1004},
    {"class", 1005},
    {"int", 1006},
    {"float", 1007},
    {"char", 1008},
    {"string", 1009},
    {"bool", 1010},
    {"void", 1011},
    {"cteentera", 1012},
    {"ctereal", 1013},
    {"ctenotacion", 1014},
    {"ctecaracter", 1015},
    {"ctestring", 1016},
    {"return", 1017},
    {"write", 1018},
    {"read", 1019},
    {"if", 1020},
    {"while", 1021},
    {"do", 1022},
    {"for", 1023},
    {"else", 1024},
    {"elseif", 1025},
    {"endclass", 1026},
    {"endfunction", 1027},
    {"dowhile", 1028},
    {"endwhile", 1029},
    {"endfor", 1030},
    {"endif", 1031},
    {"enddo", 1032},
    {"lib", 1033},
    {"of", 1034},
    {"to", 1035},
    {".", 1036},
    {";", 1037},
    {",", 1038},
    {"=", 1039},
    {"(", 1040},
    {")", 1041},
    {"||", 1042},
    {"&&", 1043},
    {"+", 1044},
    {"-", 1045},
    {"!", 1046},
    {"++", 1047},
    {"--", 1048},
    {"==", 1049},
    {"!=", 1050},
    {"<=", 1051},
    {">=", 1052},
    {"<", 1053},
    {">", 1054},
    {"*", 1055},
    {"**", 1056},
    {"/", 1057},
    {"%", 1058},
    {"$", 1059}
};

// Mapa de no terminales (símbolos en el lado izquierdo de las producciones)
std::unordered_map<std::string, int> noTerminales = {
    {"PROGRAM", 1},
    {"DECLARACIONES", 2},
    {"DECLARACIONES2", 3},
    {"DECLARA_CLASS", 4},
    {"DECLARA_LIBRARY", 5},
    {"DECLARACONST", 6},
    {"DECLARAVAR", 7},
    {"ID2", 8},
    {"TIPO_DE_DATO", 9},
    {"VALOR_CONSTANTE", 10},
    {"PARAMETROS", 11},
    {"PARAMETROS2", 12},
    {"ID3", 13},
    {"DECLARA_FUNCTION", 14},
    {"ESTATUTOS", 15},
    {"ESTATUTOS2", 16},
    {"EST_ASIG", 17},
    {"PROD_ID", 18},
    {"PROD_ID2", 19},
    {"EST_WRITE", 20},
    {"EXPRESION", 21},
    {"EST_READ", 22},
    {"PREINCUNARIO", 23},
    {"ID4", 24},
    {"EST_DO", 25},
    {"EST_IF", 26},
    {"LISTA_ELSEIF", 27},
    {"OP_ELSE", 28},
    {"EST_WHILE", 29},
    {"EST_FOR", 30},
    {"EST_RETURN", 31},
    {"EXPR", 32},
    {"EXPR'", 33},
    {"EXPR2", 34},
    {"EXPR2'", 35},
    {"EXPR3", 36},
    {"EXPR4", 37},
    {"EXPR4'", 38},
    {"EXPR5", 39},
    {"EXPR5'", 40},
    {"TERM", 41},
    {"TERM'", 42},
    {"OPREL", 43},
    {"LLAMADA_F", 44},
    {"ID5", 45},
    {"ID5'", 46},
    {"FACT", 47}
};




MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->textEditToken->setTextColor(Qt::darkGreen);
    ui->textEditError->setTextColor(Qt::red);
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_btnAnaliza_clicked() {
    QString codigo = ui->textEditCodigo->toPlainText();
    lexer.reiniciarAnalisis(codigo.toStdString());

    ui->textEditToken->clear();
    ui->textEditError->clear();

    while(lexer.tieneMasTokens()) {
        Token t = lexer.obtenerSiguienteToken();

        if(t.estado == -1) break; // Fin de análisis

        // Usar las nuevas funciones de visualización
        auto errores = LexerAutomata::erroresLexicos();  // <- CAMBIAR AQUÍ
        if(errores.find(t.estado) != errores.end()) {
            mostrarError(t);
        } else {
            mostrarToken(t);
        }
    }
}

void MainWindow::on_btnLimpia_clicked()
{
    //ui->textEditCodigo->clear();
    ui->textEditToken->clear();
    ui->textEditError->clear();
    ui->textEditSintexis->clear();
    ui->textEditCuafrupplos->clear();
    ui->textEditPOper->clear();
    ui->textEditPOperan->clear();
    ui->textEditSaltos->clear();
}

void MainWindow::on_btnSintactico_clicked() {
    //Limpia todo primero
    on_btnLimpia_clicked();

    // Primero realizar analisis lexico
    QString codigo = ui->textEditCodigo->toPlainText();
    lexer.reiniciarAnalisis(codigo.toStdString());

    vector<Token> tokens;
    while (lexer.tieneMasTokens()) {
        Token t = lexer.obtenerSiguienteToken();
        if (t.estado != -1 && t.estado != 131 && t.estado != 132) {
            tokens.push_back(t);
        }
    }
    tokens.push_back(Token{-1, "$", "FIN", 0});

    // Luego analisis sintactico
    if (parser.analizarSintaxis(tokens, codigo.toStdString())) {  // <- CAMBIO AQUÍ
        ui->textEditSintexis->setText("Análisis sintáctico correcto");
    } else {
        ui->textEditSintexis->setText("Errores sintácticos encontrados");
    }

    // 1. Ejecuta el analisis
    bool sintaxisCorrecta = parser.analizarSintaxis(tokens, codigo.toStdString());
    std::vector<std::string> errores = parser.getErroresSemanticos();

    // 2. Muestra los errores semanticos en la UI
    for (const std::string& err : errores) {
        ui->textEditError->append(QString::fromStdString(err));
    }

    // 3. Reporta el estado final
    if (sintaxisCorrecta && errores.empty()) {
        ui->textEditSintexis->setText("Análisis sintáctico y semántico correctos");
    } else if (sintaxisCorrecta && !errores.empty()) {
        ui->textEditSintexis->setText("Análisis sintáctico correcto, pero con errores semánticos.");
    } else {
        ui->textEditSintexis->setText("Errores sintácticos encontrados.");
    }

    // ---------------------------------------------------------
    // 1. MOSTRAR HISTORIAL DE PILAS
    // ---------------------------------------------------------

    // --- Pila de Operandos ---
    QString sOperandos = "[ ";
    std::vector<std::string> hOperandos = parser.getHistorialOperandos();
    for (size_t i = 0; i < hOperandos.size(); ++i) {
        sOperandos += QString::fromStdString(hOperandos[i]);
        if (i < hOperandos.size() - 1) sOperandos += " | ";
    }
    sOperandos += " ]";
    ui->textEditPOperan->setText(sOperandos);

    // --- Pila de Operadores ---
    QString sOperadores = "[ ";
    std::vector<std::string> hOperadores = parser.getHistorialOperadores();
    for (size_t i = 0; i < hOperadores.size(); ++i) {
        sOperadores += QString::fromStdString(hOperadores[i]);
        if (i < hOperadores.size() - 1) sOperadores += " | ";
    }
    sOperadores += " ]";
    ui->textEditPOper->setText(sOperadores);

    // --- Pila de Saltos ---
    QString sSaltos = "[ ";
    std::vector<int> hSaltos = parser.getHistorialSaltos();
    for (size_t i = 0; i < hSaltos.size(); ++i) {
        sSaltos += QString::number(hSaltos[i]);
        if (i < hSaltos.size() - 1) sSaltos += " | ";
    }
    sSaltos += " ]";
    ui->textEditSaltos->setText(sSaltos);

    // ---------------------------------------------------------
    // 2. MOSTRAR TABLA DE CUADRUPLOS
    // ---------------------------------------------------------
    QString tablaCuadruplos;

    tablaCuadruplos += QString("%1 | %2 | %3 | %4 | %5\n")
                           .arg("#", 3)
                           .arg("Oper", 6)
                           .arg("Op1", 10)
                           .arg("Op2", 10)
                           .arg("Res", 10);
    tablaCuadruplos += QString("-").repeated(50) + "\n";

    std::vector<Cuadruplo> cuadruplos = parser.getListaCuadruplos();

    for (size_t i = 0; i < cuadruplos.size(); ++i) {
        // Formateamos cada linea para que se vea alineada
        QString linea = QString("%1 | %2 | %3 | %4 | %5")
                            .arg(i, 3) // Numero del cuádruplo
                            .arg(QString::fromStdString(cuadruplos[i].op), 6)
                            .arg(QString::fromStdString(cuadruplos[i].op1), 10)
                            .arg(QString::fromStdString(cuadruplos[i].op2), 10)
                            .arg(QString::fromStdString(cuadruplos[i].res), 10);

        tablaCuadruplos += linea + "\n";
    }

    // IMPORTANTE: Verificar si la fuente es monoespaciada
    QFont font("Courier New");
    font.setStyleHint(QFont::Monospace);
    ui->textEditCuafrupplos->setFont(font);
    ui->textEditCuafrupplos->setText(tablaCuadruplos);

}


void MainWindow::on_actionAbrir_triggered()
{
    QString archivo = QFileDialog::getOpenFileName(
        this,
        "Abrir archivo",
        "",
        "Todos los archivos (*.*);;Archivos de texto (*.txt)"
        );

    if (!archivo.isEmpty()) {
        QFile file(archivo);

        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);

            QString contenido = in.readAll();

            file.close();

            ui->textEditCodigo->setPlainText(contenido);

            statusBar()->showMessage("Archivo cargado: " + archivo, 3000);
        } else {
            QMessageBox::critical(
                this,
                "Error",
                "No se pudo abrir el archivo: " + archivo
                );
        }
    }
}

void MainWindow::mostrarToken(const Token& t) {
    auto reservadas = LexerAutomata::estadoAsignacion();
    QString resultado;

    // Verificar si es palabra reservada
    auto it = reservadas.find(t.lexema);
    if(it != reservadas.end()) {
        resultado = QString("Token: (%1, %2) (Palabra reservada)\n")
        .arg(it->second)
            .arg(QString::fromStdString(t.lexema));
    }
    else {
        resultado = QString("Token: (%1, %2) (%3)\n")
        .arg(t.estado)
            .arg(QString::fromStdString(t.lexema))
            .arg(QString::fromStdString(t.tipo));
    }

    ui->textEditToken->append(resultado);
}

void MainWindow::mostrarError(const Token& t) {
    auto errores = LexerAutomata::erroresLexicos();
    QString resultado;

    if(errores.find(t.estado) != errores.end()) {
        resultado = QString("Error: (%1): %2 -> \"%3\" (Posición: %4)\n")
                        .arg(t.estado)
                        .arg(QString::fromStdString(errores.at(t.estado)))
                        .arg(QString::fromStdString(t.lexema))
                        .arg(t.posicion);
    }
    else {
        resultado = QString("Error desconocido \"%1\" (Posición: %2)\n")
                        .arg(QString::fromStdString(t.lexema))
                        .arg(t.posicion);
    }

    ui->textEditError->append(resultado);
}




/*int MainWindow::obtenerNumeroLinea(size_t posicion) {
    QString codigo = ui->textEditCodigo->toPlainText();
    int lineas = 1;

    for (int i = 0; i < qMin((int)posicion, codigo.length()); ++i) {
        if (codigo.at(i) == '\n') {
            lineas++;
        }
    }
    return lineas;
}*/


/*void MainWindow::resaltarErrorEnCodigo(size_t posicion) {
    QTextCursor cursor(ui->textEditCodigo->document());
    cursor.setPosition(posicion);
    cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, 1);

    QTextCharFormat formato;
    formato.setBackground(Qt::red);
    formato.setForeground(Qt::white);
    cursor.mergeCharFormat(formato);

    ui->textEditCodigo->setTextCursor(cursor);
    ui->textEditCodigo->setFocus();
}*/



void MainWindow::on_actionGuardar_triggered()
{
    // Abrir cuadro de diálogo para seleccionar la ubicación del archivo
    QString rutaArchivo = QFileDialog::getSaveFileName(
        this,
        "Guardar archivo",
        "",
        "Archivos TLS (*.tls);;Todos los archivos (*)"
        );

    if (rutaArchivo.isEmpty()) {
        return; // El usuario canceló el diálogo
    }

    // Asegurarse de que el archivo tenga la extensión .tls
    if (!rutaArchivo.endsWith(".tls", Qt::CaseInsensitive)) {
        rutaArchivo += ".tls";
    }

    // Crear y abrir el archivo
    QFile archivo(rutaArchivo);
    if (!archivo.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Error", "No se pudo guardar el archivo.");
        return;
    }

    // Escribir el contenido de textEditCodigo en el archivo
    QTextStream salida(&archivo);
    salida << ui->textEditCodigo->toPlainText();
    archivo.close();

    QMessageBox::information(this, "Éxito", "Archivo guardado correctamente.");
}




