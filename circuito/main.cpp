#include <iostream>
#include "bool3S.h"
#include "Port.h"
#include "Circuito.h"

using namespace std;
void gerarTabela(Circuito& C);
int main()
{


    Circuito *C = new Circuito();
    Circuito A = Circuito();

    //C->setPort(1,"NT",1);
    //C->ler("circuito1.txt");


    A.ler("cc.txt");
   //A.digitar();
    A.imprimir(cout);
    cout << "*****************" << endl;
    //gerarTabela(A);

    cout << "IN: " <<A.getNumInputs() << endl;
    cout << "Out: " <<A.getNumOutputs() << endl;
    cout << "ports: " <<A.getNumPorts() << endl;
    cout << "valid: " <<A.valid() << endl;

    cout << "IdOut: " <<A.getIdOutput(-2) << endl;






    return 0;
}

void gerarTabela(Circuito& C)
{
  vector<bool3S> in_circ(C.getNumInputs());
  int i;

  // Comeca com todas as entradas indefinidas
  for (i=0; i<(int)C.getNumInputs(); i++)
  {
    in_circ.at(i) = bool3S::UNDEF;
  }

  cout << "ENTRADAS" << '\t' << "SAIDAS" << endl;
  do
  {

    // Simulacao
    C.simular(in_circ);

    // Impressao das entradas
    for (i=0; i<(int)C.getNumInputs(); i++)
    {
      cout << in_circ.at(i);
      if (i<(int)C.getNumInputs()-1) cout << ' ';
      else
      {
        cout <<'\t';
        if (C.getNumInputs()<=2) cout <<'\t';
      }
    }

    // Impressao das saidas
    for (i=0; i<(int)C.getNumOutputs(); i++)
    {
      cout << C.getOutput(i+1);
      if (i<(int)C.getNumOutputs()-1) cout << ' ';
      else cout << '\n';
    }

    // Determina qual entrada deve ser incrementada na proxima linha
    // Incrementa a ultima possivel que nao for TRUE
    // Se a ultima for TRUE, faz essa ser UNDEF e tenta incrementar a anterior
    i = int(C.getNumInputs())-1;
    while (i>=0 && in_circ.at(i)==bool3S::TRUE)
    {
      in_circ.at(i)++;
      i--;
    };
    // Incrementa a input selecionada
    if (i>=0) in_circ.at(i)++;
  } while (i>=0);
}

