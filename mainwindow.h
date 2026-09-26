#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>

// Incluir los nuevos headers
#include "token.h"
#include "lexer_lexer.h"
#include "parser_parser.h"


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

/*struct Token {
    int estado;
    std::string lexema;
    std::string tipo;
    size_t posicion;

    bool esTipo(const std::string& tipoComparar) const {
        return tipo == tipoComparar;
    }

};*/

using Produccion = std::vector<int>;
using MProducciones = std::unordered_map<int, Produccion>;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    /*Token obtenerSiguienteToken();
    void reiniciarAnalisis(const QString &texto);
    bool tieneMasTokens() const;*/

public:
    //Token obtenerSiguienteToken();  // Cambiado de 'token' a 'Token' (el struct)
    //void reiniciarAnalisis(const QString &texto);  // Añadido parámetro
    //bool tieneMasTokens() const;
    //const std::unordered_map<std::string, int> terminales;
    //const std::unordered_map<std::string, int> noTerminales;

private:
   /*
    std::string inputAnalisis;
    size_t posicionActual;

    // Declaraciones de funciones auxiliares y mapas
    int relaciona(char c);
    std::set<int> estadosDiferentes();
    std::unordered_map<int, std::string> estadoAceptacion();
    std::unordered_map<std::string, int> estadoAsignacion();
    std::unordered_map<int, std::string> erroresLexicos();
*/
private slots:
    void on_actionAbrir_triggered();
    void on_btnAnaliza_clicked();
    void on_btnLimpia_clicked();
    void on_btnSintactico_clicked();

    void on_actionGuardar_triggered();

private:

    Ui::MainWindow *ui;
    Lexer lexer;
    Parser parser;

    // Métodos de visualización
    void mostrarToken(const Token &t);
    void mostrarError(const Token &t);
    int obtenerNumeroLinea(size_t posicion);


   /* MProducciones MProducciones;
    Ui::MainWindow *ui;

    // Variables miembro
    std::string inputAnalisis;
    size_t posicionActual;
    std::stack<int> pilaSintactica;
    std::vector<Token> tokensAnalisis;
    size_t tokenActual;

    std::vector<std::vector<int>> matrizPredictiva;
    void inicializarMatrizPredictiva();

    // Mapas (quitamos const para poder modificarlos)
    std::unordered_map<std::string, int> terminales;
    std::unordered_map<std::string, int> noTerminales;
    std::unordered_map<int, int> lexicoToSintactico;
    std::unordered_map<int, std::string> erroresSintactico;
    std::unordered_map<int, std::string> inicializarErroresSintacticos();
    // Métodos
    int tokenToTerminal(const Token& token) const;
    void inicializarLexicoToSintactico();
    int obtenerNumeroLinea(size_t posicion);
    void mostrarErrorSintactico(const Token& token, int codigoError);
    int determinarCodigoError(const Token& token);
    void inicializarParser();
    bool analizarSintaxis();
    void matrizProducciones();
    Produccion obtenerProduccion(int noTerminalNum, int terminal);
    //std::vector<int> obtenerProduccion(const std::string& noTerminal, int terminal);
    //std::vector<int> obtenerProduccionPorIndice(int indice);
    Produccion obtenerProduccionPorIndice(int indice);
    void pushProduccion(const Produccion& prod);

    // Métodos léxicos
    int relaciona(char c);
    std::set<int> estadosDiferentes();
    std::unordered_map<int, std::string> estadoAceptacion();
    std::unordered_map<std::string, int> estadoAsignacion();
    std::unordered_map<int, std::string> erroresLexicos();

    void mostrarToken(const Token &t);
    void mostrarError(const Token &t);
    void inicializarTerminalesYNoTerminales();*/
};
#endif // MAINWINDOW_H
