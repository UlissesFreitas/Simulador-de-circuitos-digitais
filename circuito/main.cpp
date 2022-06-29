#include <iostream>
#include "bool3S.h"
#include "Port.h"
#include "Circuito.h"

using namespace std;

int main()
{
    Circuito *C = new Circuito();
    //Circuito A = Circuito(C);

    //C->setPort(1,"NT",1);
    C->ler("circuito1.txt");
    C->imprimir(cout);

    Port_NOT*    nao =new Port_NOT();
    Port*    an  = new Port_AND;
    Port_NAND   nan = Port_NAND();
    Port_OR     ou = Port_OR();
    Port_NOR    nou = Port_NOR();
    Port_XOR    xou = Port_XOR();
    Port_NXOR   nxou = Port_NXOR();

/*
    nao->digitar();
    nao->imprimir(cout);
    an->digitar();
    an->imprimir(cout);

    cout << an.getName();
    an.digitar();
    cout << nan.getName();
    nan.digitar();
    cout << ou.getName();
    ou.digitar();
    cout << nou.getName();
    nou.digitar();
    cout << xou.getName();
    xou.digitar();
    cout << nxou.getName();
    nxou.digitar();
*/
   // nao.digitar();



    return 0;
}
