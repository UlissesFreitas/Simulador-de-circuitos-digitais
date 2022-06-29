#ifndef CIRCUITO_H
#define CIRCUITO_H

#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <fstream>
#include "bool3S.h"
#include "Port.h"

class Circuito
{
private:
    unsigned Nin;

    std::vector<int> id_out;
    std::vector<bool3S> out_circ;
    std::vector<ptr_Port> ports;

public:
    /// Inicializacao e Finalizacao
    Circuito(); /// ok
    Circuito(const Circuito& C); /// ok
    ~Circuito(); /// ok

    void clear(); /// OK
    void operator=(const Circuito& C); /// OK
    void resize(unsigned NI, unsigned NO, unsigned NP); /// quase OK

    /// Funcoes de testagem
    /// Estao todas feitas
    bool validIdInput(int IdInput) const;
    bool validIdOutput(int IdOutput) const;
    bool validIdPort(int IdPort) const;
    bool validIdOrig(int IdOrig) const;
    bool definedPort(int IdPort) const;
    bool validPort(int IdPort) const;
    bool valid() const;

    /// Funcoes de consulta
    unsigned getNumInputs() const; /// ok
    unsigned getNumOutputs() const; /// ok
    unsigned getNumPorts() const; /// ok
    int getIdOutput(int IdOutput) const; /// ok
    bool3S getOutput(int IdOutput) const; ///ok
    std::string getNamePort(int IdPort) const; /// ok
    unsigned getNumInputsPort(int IdPort) const; /// ok
    int getId_inPort(int IdPort, unsigned I) const; /// OK

    ///Funcoes de modificacao
    void setIdOutput(int IdOut, int IdOrig); /// OK
    void setPort(int IdPort, std::string Tipo, unsigned NIn); /// OK
    void setId_inPort(int IdPort, unsigned I, int IdOrig) const; /// OK

    /// E/S de dados
    void digitar(); /// NAO OK
    bool ler(const std::string& arq); /// NAO OK
    std::ostream& imprimir(std::ostream& O=std::cout) const; /// quase ok
    bool salvar(const std::string& arq) const; /// quase OK


    /// SIMULACAO
    bool simular(const std::vector<bool3S>& in_circ);

};
std::ostream& operator<<(std::ostream& O, const Circuito& C);

#endif // CIRCUITO_H
