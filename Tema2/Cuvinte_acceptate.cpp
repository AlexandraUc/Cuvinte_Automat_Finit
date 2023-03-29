#include <iostream>
#include <fstream>
#include <string.h>
#include <string>
using namespace std;

/*
    Format fisier: afn-lambda(?), lungime maxima cuvinte, nr stari, card alfabet, alfabet, stare initiala, 
    nr stari finale, stari finale, nr tranzitii, tranzitii
    Format linie matrice: stare de plecare, stare in care ajungem, litera (nimic pentru lambda) 
*/

class Automat{
    int lungimeCuvinte, nrStari, cardAlfabet, nrStariFinale, tranzitii;
    char stareInitiala;
    char* stariFinale;
    char* alfabet;
    char** matriceAutomat;
    string* lambdaInchideri;
    string drum;
    bool ok, AFN_lambda;

    void citireAutomat(const string&);
    void toateCuvintele(int, char, string, string);
    void cautaLambdaInchideri(string&, char);
    void seteazaLambdaInchideri();
    void genereazaCuvintele(string cuvant = "");
public:
    Automat(const string&);
    Automat(const Automat&);
    Automat& operator = (const Automat&);

    void afisareMatriceAutomat();
    void afisLambdaInchideri();

    void cheamaFunctie();
    bool verificaAcceptare(string);

    ~Automat();
};

// Constructor
Automat::Automat(const string& numeFisier = "in.txt"){
    this->drum = "";
    this->ok = 0;
    this->citireAutomat(numeFisier);
    if(this->AFN_lambda == 1)
        this->seteazaLambdaInchideri();
    else 
        this->lambdaInchideri = NULL;
}

// Copy constructor
Automat::Automat(const Automat& obj){
    this->AFN_lambda = obj.AFN_lambda;
    this->nrStari = obj.nrStari;
    this->cardAlfabet = obj.cardAlfabet;
    this->nrStariFinale = obj.nrStariFinale;
    this->tranzitii = obj.tranzitii;
    this->lungimeCuvinte = obj.lungimeCuvinte;
    this->stareInitiala = obj.stareInitiala;
    this->drum = obj.drum;
    this->ok = obj.ok;

    this->alfabet = new char[obj.cardAlfabet];
    strcpy(this->alfabet, obj.alfabet);

    if(obj.lambdaInchideri != NULL){
        this->lambdaInchideri = new string[obj.nrStari];
        for(int i = 0; i < obj.nrStari; i++)
            this->lambdaInchideri = obj.lambdaInchideri;
    }
    else
        this->lambdaInchideri = NULL;

    this->stariFinale = new char[strlen(obj.stariFinale) + 1];
    strcpy(this->stariFinale, obj.stariFinale);

    this->matriceAutomat = new char*[this->tranzitii];
    for(int i = 0; i < obj.tranzitii; i++){
        this->matriceAutomat[i] = new char[4];
        strcpy(this->matriceAutomat[i], obj.matriceAutomat[i]);
    }
}

// Operatorul de atribuire 
Automat& Automat::operator = (const Automat& obj){
    if(this != &obj){
        this->AFN_lambda = obj.AFN_lambda;
        this->nrStari = obj.nrStari;
        this->cardAlfabet = obj.cardAlfabet;
        this->nrStariFinale = obj.nrStariFinale;
        this->tranzitii = obj.tranzitii;
        this->lungimeCuvinte = obj.lungimeCuvinte;
        this->stareInitiala = obj.stareInitiala;
        this->drum = obj.drum;
        this->ok = obj.ok;

        this->alfabet = new char[obj.cardAlfabet];
        strcpy(this->alfabet, obj.alfabet);

        if(obj.lambdaInchideri != NULL){
            this->lambdaInchideri = new string[obj.nrStari];
            for(int i = 0; i < obj.nrStari; i++)
                this->lambdaInchideri = obj.lambdaInchideri;
        }
        else
            this->lambdaInchideri = NULL;

        this->stariFinale = new char[strlen(obj.stariFinale) + 1];
        strcpy(this->stariFinale, obj.stariFinale);

        this->matriceAutomat = new char*[this->tranzitii];
        for(int i = 0; i < obj.tranzitii; i++){
            this->matriceAutomat[i] = new char[4];
            strcpy(this->matriceAutomat[i], obj.matriceAutomat[i]);
        }
    }
    return *this;
}

// Citeste din fisier
void Automat::citireAutomat(const string& numeFisier){
    ifstream f(numeFisier);
    if(!f){
        cout << "Nu exista fisierul\n";
        exit(1);
    }

    f >> this->AFN_lambda >> this->lungimeCuvinte >> this->nrStari >> this->cardAlfabet;

    this->alfabet = new char[this->cardAlfabet];
    for(int i = 0; i < this->cardAlfabet; i++)
        f >> this->alfabet[i];
    this->alfabet[this->cardAlfabet] = '\0';

    f >> this->stareInitiala >> this->nrStariFinale;
    this->drum += this->stareInitiala;

    this->stariFinale = new char[this->nrStariFinale];
    for(int i = 0; i < this->nrStariFinale; i++)
        f >> this->stariFinale[i];
    this->stariFinale[this->nrStariFinale] = '\0';

    f >> this->tranzitii;
    this->matriceAutomat = new char*[this->tranzitii];
    for(int i = 0; i < this->tranzitii; i++)
        this->matriceAutomat[i] = new char[4];

    string linie;
    getline(f, linie); // Se citeste linia goala
    for(int i = 0; i < this->tranzitii; i++){
        getline(f, linie);
        this->matriceAutomat[i][0] = linie[0];
        this->matriceAutomat[i][1] = linie[2];
        if(linie.size() > 3)
            this->matriceAutomat[i][2] = linie[4];
        else
            this->matriceAutomat[i][2] = ' ';
        this->matriceAutomat[i][3] = '\0';
    }
    f.close();
}

// Cheama functia de afisare a cuvintelor in functie
void Automat::cheamaFunctie(){
    this->ok = 0;

    if(this->AFN_lambda == 0)
        this->toateCuvintele(this->lungimeCuvinte, this->stareInitiala, "", this->drum);
    else{
        if(this->verificaAcceptare(" ") == 1){    // Verifica cuvantul vid
            this->ok = 1;
            cout << ' ' << endl;
        }
        this->genereazaCuvintele();
    }

    if(this->ok == 0)
        cout << "Nu se accepta niciun cuvant de lungime " << this->lungimeCuvinte << endl;
}

// Afiseaza toate cuvintele (+drumul) de o lungime maxima data (pentru AFD si AFN)
void Automat::toateCuvintele(int lungimeCuvinte, char stareCurenta, string cuvant, string drum){
    if(lungimeCuvinte >= 0){
        for(int i = 0; i < this->nrStariFinale; i++)
            if(stareCurenta == this->stariFinale[i]){
                this->ok = 1;
                cout << cuvant << ' ' << drum << endl;
            }
    }
    if(lungimeCuvinte > 0){
        for(int i = 0; i < this->tranzitii; i++){
            if(stareCurenta == this->matriceAutomat[i][0])
                if(this->matriceAutomat[i][2] != ' ')
                    toateCuvintele(lungimeCuvinte - 1, this->matriceAutomat[i][1], 
                        cuvant + this->matriceAutomat[i][2], drum + this->matriceAutomat[i][1]);
                else
                    toateCuvintele(lungimeCuvinte, this->matriceAutomat[i][1], 
                        cuvant, drum + this->matriceAutomat[i][1]);
        }
    } 
}

// Genereaza toata cuvintele de lungime maxima si cheama functia de verificare (pentru AFN-lambda)
void Automat::genereazaCuvintele(string cuvant){
    for(int i = 0; i < this->cardAlfabet; i++){
        if(cuvant.size() < this->lungimeCuvinte){
            this->genereazaCuvintele(cuvant + this->alfabet[i]);
            if(this->verificaAcceptare(cuvant + this->alfabet[i]) == 1){
                this->ok = 1;
                cout << cuvant + this->alfabet[i] << endl;
            }
        }
    }
}

// Lambda inchideri pentru o stare
void Automat::cautaLambdaInchideri(string& inchideri, char stare){
    for(int i = 0; i < this->tranzitii; i++)
        if(stare == this->matriceAutomat[i][0] && this->matriceAutomat[i][2] == ' '){
            bool ok = 1;
            for(int j = 0; j < inchideri.size() && ok; j++)
                if(inchideri[j] == this->matriceAutomat[i][1])
                    ok = 0;
            // Se opreste daca gaseste din nou starea pentru a nu intra intr-o bucla infinita
            if(ok == 1){
                inchideri += this->matriceAutomat[i][1];
                this->cautaLambdaInchideri(inchideri, this->matriceAutomat[i][1]);
            }
        }
}

// Seteaza lambda inchiderile
void Automat::seteazaLambdaInchideri(){
    this->lambdaInchideri = new string[this->nrStari];
    for(int i = 0; i < this->nrStari; i++){
        this->lambdaInchideri[i] = "";
        char stare = (char)(i + 48);
        this->lambdaInchideri[i] += stare;
        this->cautaLambdaInchideri(this->lambdaInchideri[i], stare);
    }
}

// Verifica daca un cuvant este acceptat de AFN-lambda
bool Automat::verificaAcceptare(string cuvant){
    // stari - starile din care plecam
    // stariCurente - starile in care ajungem
    string stari = this->lambdaInchideri[(int)(this->stareInitiala - 48)];   // Inchideri lambda pentru starea initiala
    
    for(int i = 0; i < cuvant.size(); i++){
        // Citim o litera
        string stariCurente = "";
        for(int j = 0; j < stari.size(); j++){
            for(int k = 0; k < this->tranzitii; k++)
                if(this->matriceAutomat[k][0] == stari[j] && this->matriceAutomat[k][2] == cuvant[i]){
                    bool ok = 1;
                    for(int l = 0; l < stariCurente.size(); l++)
                        if(stariCurente[l] == this->matriceAutomat[k][1])
                            ok = 0;
                    if(ok == 1)
                        stariCurente += this->matriceAutomat[k][1];
                }
        }

        // Completam cu lambda inchiderile
        stari = stariCurente;
        stariCurente = "";
        
        for(int i = 0; i < stari.size(); i++){
            string temp = this->lambdaInchideri[(int)(stari[i] - 48)];
            for(int j = 0; j < temp.size(); j++){
                bool ok = 1;
                for(int k = 0; k < stariCurente.size(); k++)
                    if(stariCurente[k] == temp[j])
                        ok = 0;
                if(ok == 1)
                    stariCurente += temp[j];
            }
        }
        stari = stariCurente;
    }

    bool ok = 0;
    for(int i = 0; i < stari.size() && ok == 0; i++)
        for(int j = 0; j < this->nrStariFinale && ok == 0; j++)
            if(stari[i] == this->stariFinale[j])    
                return 1;
    return 0;
}

// Afiseaza matricea de tranzitii
void Automat::afisareMatriceAutomat(){
    for(int i = 0; i < this->tranzitii; i++){
        for(int j = 0; j < 3; j++)
            cout << this->matriceAutomat[i][j] << ' ';
        cout << endl;
    }
}

// Afiseaza lambda inchiderile
void Automat::afisLambdaInchideri(){
    if(this->lambdaInchideri != NULL){
        for(int i = 0; i < this->nrStari; i++)
            cout << this->lambdaInchideri[i] << endl;
    }
    else
        cout << "Nu exista\n";
}

// Destructor
Automat::~Automat(){
    if(this->stariFinale != NULL){
        delete[] this->stariFinale;
        this->stariFinale = NULL;
    }

    if(this->alfabet != NULL){
        delete[] this->alfabet;
        this->alfabet = NULL;
    }

    for(int i = 0; i < this->tranzitii; i++)
        if(this->matriceAutomat[i] != NULL){
            delete[] this->matriceAutomat[i];
            this->matriceAutomat[i] = NULL;
        }
    if(this->matriceAutomat != NULL){
        delete[] this->matriceAutomat;
        this->matriceAutomat = NULL;
    }

    if(this->lambdaInchideri != NULL){
        delete[] this->lambdaInchideri;
        this->lambdaInchideri = NULL;
    }
    
}

int main(){
    string numeFisier2 = "in4.txt";
    Automat a, a2(numeFisier2);
    //a.cheamaFunctie();
    a2.cheamaFunctie();
    return 0;
}