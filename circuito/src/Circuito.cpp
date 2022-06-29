#include "Circuito.h"


bool validType(std::string& Tipo)
{
  if (Tipo.size()!=2) return false;
  Tipo.at(0) = toupper(Tipo.at(0));
  Tipo.at(1) = toupper(Tipo.at(1));
  if (Tipo=="NT" ||
      Tipo=="AN" || Tipo=="NA" ||
      Tipo=="OR" || Tipo=="NO" ||
      Tipo=="XO" || Tipo=="NX") return true;
  return false;
}
ptr_Port allocPort(std::string& Tipo)
{
  if (!validType(Tipo)) return nullptr;

  if (Tipo=="NT") return new Port_NOT;
  if (Tipo=="AN") return new Port_AND;
  if (Tipo=="NA") return new Port_NAND;
  if (Tipo=="OR") return new Port_OR;
  if (Tipo=="NO") return new Port_NOR;
  if (Tipo=="XO") return new Port_XOR;
  if (Tipo=="NX") return new Port_NXOR;

  // Nunca deve chegar aqui...
  return nullptr;
}
///-------------------------------
    /// Inicializacao e Finalizacao
Circuito::Circuito(): Nin(0), id_out(), out_circ(), ports()
{

}
Circuito::Circuito(const Circuito& C){
    Nin = C.getNumInputs();

    /// id_out
    for(unsigned i=0; i<C.getNumOutputs(); i++){
        id_out.push_back(C.getIdOutput(i));
    }
    /// out_circ
    for(unsigned i=0; i<C.getNumOutputs(); i++){
        out_circ.push_back(C.getOutput(i));
    }
    /// ports tem que revisar essa
    /// pode conter erro no C.ports[i]->clone()
    for(unsigned i=0;i<C.getNumPorts();  i++){
        ports.push_back(C.ports[i]->clone());
    }

}
Circuito::~Circuito()
{
    clear();
}
void Circuito::clear(){
    Nin = 0;
    id_out.clear() ;
    out_circ.clear();

    for(unsigned i=0; i<getNumPorts(); i++){
        delete ports[i];
    }
    ports.clear();
}
void Circuito::operator=(const Circuito& C){
    clear();
    /// Nin
    Nin = C.getNumInputs();
    /// id_out
    for(unsigned i=0; i<C.getNumOutputs(); i++){
        id_out.push_back(C.getIdOutput(i));
    }
    /// out_circ
    for(unsigned i=0; i<C.getNumOutputs(); i++){
        out_circ.push_back(C.getOutput(i));
    }
    /// ports
    /// pode conter erro no C.ports[i]->clone()
    for(unsigned i=0;i<C.getNumPorts();  i++){
        ports.push_back(C.ports[i]->clone());
    }
}
void Circuito::resize(unsigned NI, unsigned NO, unsigned NP){
    /// checa parametros
    if(true ){
        clear();
        Nin = NI;
        for(unsigned i=0; i<NO; i++){
            id_out.push_back(0);
            out_circ.push_back(bool3S::UNDEF);
        }
        for(unsigned i=0;i<NP;i++){
            ports.push_back(nullptr);
        }
    }
}
///---------------------------------------------------

bool Circuito::validIdInput(int IdInput) const
{
  return (IdInput<=-1 && IdInput>=-int(getNumInputs()));
}
bool Circuito::validIdOutput(int IdOutput) const
{
  return (IdOutput>=1 && IdOutput<=int(getNumOutputs()));
}
bool Circuito::validIdPort(int IdPort) const
{
  return (IdPort>=1 && IdPort<=int(getNumPorts()));
}
bool Circuito::validIdOrig(int IdOrig) const
{
  return validIdInput(IdOrig) || validIdPort(IdOrig);
}
bool Circuito::definedPort(int IdPort) const
{
  if (!validIdPort(IdPort)) return false;
  if (ports.at(IdPort-1)==nullptr) return false;
  return true;
}
bool Circuito::validPort(int IdPort) const
{
  if (!definedPort(IdPort)) return false;
  for (unsigned j=0; j<getNumInputsPort(IdPort); j++)
  {
    if (!validIdOrig(getId_inPort(IdPort,j))) return false;
  }
  return true;
}

bool Circuito::valid() const
{
  if (getNumInputs()==0) return false;
  if (getNumOutputs()==0) return false;
  if (getNumPorts()==0) return false;
  for (unsigned i=0; i<getNumPorts(); i++)
  {
    if (!validPort(i+1)) return false;
  }
  for (unsigned i=0; i<getNumOutputs(); i++)
  {
    if (!validIdOrig(getIdOutput(i+1))) return false;
  }
  return true;
}

///--------------------------------------------------

unsigned Circuito::getNumInputs() const{
    return Nin;
}
unsigned Circuito::getNumOutputs() const{
    return id_out.size();
}

unsigned Circuito::getNumPorts() const{
    return ports.size();
}

int Circuito::getIdOutput(int IdOutput) const{
    if(validIdOutput(IdOutput)){
        return id_out[IdOutput-1];
    }
    return 0;
}
bool3S Circuito::getOutput(int IdOutput) const{
    if(validIdOutput(IdOutput)){
        return out_circ[IdOutput-1];
    }
    return bool3S::UNDEF;
}

std::string Circuito::getNamePort(int IdPort) const{
    if(definedPort(IdPort)){
        return ports[IdPort-1]->getName();
    }
    return "??";
}
unsigned Circuito::getNumInputsPort(int IdPort) const{
    if(definedPort(IdPort)){
       return ports[IdPort-1]->getNumInputs();
    }
    return 0;
}
int Circuito::getId_inPort(int IdPort, unsigned I) const{
    if(definedPort(IdPort)){
        if(ports[IdPort-1]->validIndex(I)){
            return ports[IdPort-1]->getId_in(I);
        }
    }
    return 0;
}

///Funcoes de modificacao
///--------------------------------------------------------------
void Circuito::setIdOutput(int IdOut, int IdOrig){
    if(validIdOutput(IdOut) && validIdOrig(IdOrig)){
         id_out[IdOut-1] = IdOrig;
    }
}
void Circuito::setPort(int IdPort, std::string Tipo, unsigned NIn){
    if(validIdPort(IdPort) && validType(Tipo) ){
        if(ports[IdPort-1]->validNumInputs(NIn) ){
            delete ports[IdPort-1];
            ports[IdPort-1] = allocPort(Tipo);
            ports[IdPort-1]->setNumInputs(NIn);
        }

    }

}
void Circuito::setId_inPort(int IdPort, unsigned I, int IdOrig) const{
    if(definedPort(IdPort) && validIdOrig(IdOrig)){
        if(ports[IdPort-1]->validIndex(I)){
             ports[IdPort-1]->setId_in(I,IdOrig);
        }
    }
}
///----------------------------------------

void Circuito::digitar(){

    int NI, NO, NP;
    std::string name;

    do{
        try{
            std::cout << "Numero de entradas: ";
            std::cin >> NI;
            if(NI <= 0) throw "entradas";

            std::cout << "Numero de saidas: " ;
            std::cin >> NO;
            if(NO <= 0) throw "saidas";

            std::cout << "Numero de portas: ";
            std::cin >> NP;
            if(NP <= 0) throw "portas";

        }catch(const char* op){
            std::cerr << "Numero de "<< op << " invalido" << std::endl;
            std::cerr << "Tente novamente" << std::endl;
            std::cerr <<  std::endl;
        }

    }while( (NI <= 0) || (NO <= 0) || (NP <= 0) );

    resize(NI, NO, NP);

    for(int i=1; i<=NP; i++){
        std::cin.ignore(std::numeric_limits< std::streamsize>::max(), '\n');
        do{

            std::cout << "Digite o tipo da porta(" << i << "): " << std::endl;
            std::cout << "NT(NOT) \n"<<"AN(AND) \n"<<"NA(NOT AND) \n";
            std::cout <<"OR(OR) \n" << "NO(NOT OR) \n" << "XO(EXCLUSIVE OR) \n";
            std::cout << "NX(NOT EXCLUSIVE OR) \n\n";
            std::getline(std::cin, name);
            if(validType(name)){
                Port* porta = allocPort(name);
                ports.push_back( porta->clone() );
                do{
                    ports[i]->imprimir(std::cout);
                    std::cout << std::endl;
                    ports[i]->digitar();
                    ports[i]->imprimir(std::cout);
                    std::cout << std::endl;


                /// ENQUANTO PORTA I NAO VALIDA REPITA
                }while(!validPort(i));
            } // FIM IF VALIDTYPE


        /// enquanto nome nao valido repita
        }while( !validType(name));

    } // FIM FOR NP

    for(int i=0;i<NO;i++){
        int saidas;
        do{
            std::cout << "Digite de onde vem as saidas: ";
            std::cin >> saidas;
        }while( !validIdOrig(saidas));

    }// FIM FOR NO

}// FIM DIGITAR

///--------------------------------------------
 std::ostream& Circuito::imprimir(std::ostream& O) const{
    O << "CIRCUITO: " << getNumInputs() << ' ' << getNumOutputs() << ' ' << getNumPorts() << std::endl;
    O << "PORTAS:"<<std::endl;
    for(unsigned i=1; i<=getNumPorts(); i++){
        O << i<<')';
        (*ports[i-1]).imprimir(O);
    }

    O << "SAIDAS:"<<std::endl;

    for(unsigned i=1; i<=getNumOutputs(); i++){
        O << i << ") " << getIdOutput(i) << std::endl;
    }

    return O;
}

bool Circuito::ler(const std::string& arq){

        std::ifstream arq_circ(arq.c_str());

    try{
        if(!arq_circ.is_open()) throw 1;

        std::string cabecalho, portas, saidas;
        arq_circ >> cabecalho;
        if (!arq_circ.good() || cabecalho!="CIRCUITO") throw 2;

        unsigned  id_prov, NI, NO, NP;
        /// int NI, NO, NP;
        int id;
        arq_circ>>NI>>NO>>NP;
        std::cout << NI << NO << NP << std::endl;

        /// if( (NI<=0) || (NO <=0) || (NP <=0) ) throw 3
        /// if((NI==0) || (NO==0) || ((NP==0) && (NI>NO))) throw 3;
        if( (NI<=0) || (NO <=0) || (NP <=0) ) throw 3;
        resize(NI,NO,NP);

        arq_circ >> portas; // Recebe "PORTAS"
        if(portas != "PORTAS") throw 4;

    /// Em seguida, para cada porta leh e confere a id e o tipo (validType)
    for(unsigned i = 1; i<=NP; i++){

        arq_circ >> id_prov;
        if(!arq_circ.good()||id_prov!=i) throw 5;

        char p;
        arq_circ >> p;
        if(p != ')') throw 6;

        std::string tipo;
        arq_circ >> tipo;
        if(!validType(tipo)) throw 7;

        Port* porta = allocPort(tipo);
        //porta->imprimir();
        ports[i] = porta->clone() ;

        //if( !( ports[i]->ler(arq_circ) && validPort(i) ))  throw 8;
//std::cout << "entrei aqui" << std::endl;

    } // FIM FOR NP
    }catch(int op){
        std::cout << "Error: "<< op << std::endl;
        arq_circ.close();
        return false;
    }
///************************************************************************
///   	 std::cout << "entrei aqui" << std::endl;

    /// Em seguida, para cada porta leh e confere a id e o tipo (validType)
    /*
    unsigned NP;
    for(unsigned i = 1; i<=NP; i++){

        for(unsigned j=1; j<=getNumInputsPort(i);j++){
            if( (getId_inPort(i,j)>int(getNumPorts())) || (getId_inPort(i,j)<(-int(getNumInputs()))) || (getId_inPort(i,j)==0)){
                arq_circ.close();
                clear();
                return false;
            }
        }
    }

	 //arq_circ.ignore();
	 arq_circ >> saidas;
	 //getline(arq, saidas);
   	 if(saidas != "SAIDAS"){
        	arq_circ.close();
        	clear();
        	return false;
   	 }

	for(unsigned i=1; i<=getNumOutputs(); i++){
        arq_circ >> id_prov;
        if(id_prov!=i){
            arq_circ.close();
            clear();
            return false;
        }
        char p;
        arq_circ >> p ;
        if(p != ')'){
            arq_circ.close();
            clear();
            return false;
        }
        //arq_circ.ignore(std::numeric_limits<std::streamsize>::max(),')');

        // Recebe a id da saida
        arq_circ >> id; // Recebe a id que a saida i retorna
        ///arq_circ.ignore(); // Ignora o
        if( (id>int(getNumPorts())) || (id<(-int(getNumInputs()))) || (id==0)){
            arq_circ.close();
            clear();
            return false;
        }
        setIdOutput(i,id);
    }

       //*this = id_prov;
        arq_circ.close();
        return true;
        */
}

/// SIMULACAO
///---------------------------------------------------------------
bool Circuito::simular(const std::vector<bool3S>& in_circ){
    unsigned i,j;
    bool tudo_def, alguma_def;
    ///SIMULACAO DAS PORTAS
    for( i=0; i< getNumPorts(); i++){
        ports[i]->setOutput(bool3S::UNDEF);
    }

    do{
        tudo_def = true;
        alguma_def = false;

        for( i =0; i < getNumPorts(); i++){
            if(ports[i]->getOutput() == bool3S::UNDEF){

                for(j=0; j < ports[i]->getNumInputs(); j++){
                    /// id = ports[i].id_in[j]
                        // ports[i]->getId_in(i);
                    //setId_inPort(ports[i]->id_in[j], );
                        if( getId_inPort(i,j) >0){

                        }
                }
            }


        }

    }while(!tudo_def && alguma_def);

    return true;
}

