#include <iostream>
#include <fstream>
#include <cstring>
#include <stdlib.h>
#include <thread>   // std::thread 
using namespace std;

const int MAX = 1000; //número máximo de linhas e colunas que a imagem pode ter
const int MAXI = 50;  //tamanho máximo que o nome do arquivo pode ter

struct image{  // contém os dados do arquivo que vai ser lido
    char crip[3];  // nome para saber se o conteúdo do arquivo é uma imagem de criptografia P2
    int data[MAX][MAX];  //guarda os pixels da imagem
    int numberc,numberow,verif; //numero de colunas, de linhas e o valor do pixel de maior valor;
};

////////VARIÁVEIS GLOBAIS PARA QUE TODAS AS FUNÇÕES TENHAM ACESSO SEM SER REFERENCIADAS//////
image pic; // OBJETO IMAGEM
int final_image[MAX][MAX]; //OBJETO QUE GUARDA O RESULTADO DA FUNÇÃO ALARGAMENTO
int final_image2[MAX][MAX]; // OBJETO QUE GUARDA O RESULTADO DA FUNÇÃO DE EQUALIZAÇÃO
char nomearq[MAXI]; // NOME DO ARQUIVO A SER ABERTO
/////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////FUNÇÕES\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\/
void get_data();               // PEGA OS DADOS DO ARQUIVO
void alargamento_contraste();  // FAZ A MUDANÇA NO ALARGAMENTO DO CONTRASTE E SALVA UM ARQUIVO COM OS DADOS MODIFICADOS
void equaliza_histograma();          // FAZ A MUDANÇA NA EQUALIZAÇÃO DO HISTOGRAMA E SALVA UM ARQUIVO COM OS DADOS MODIFICADOS
/////////////////////////////////////////////////////////////////////////////////////////////

int main(){

    get_data();

    std::thread first(alargamento_contraste); // set the first thread to be the 
                                              // function Alargamento de Contraste.

    std::thread second(equaliza_histograma);        // set the second thread to be the 
                                              // function Equalização de Histograma.

    first.join();                             // pauses until first finishes
    second.join();                            // pauses until second finishes

    return 0;

}


void get_data(){

    ifstream arquivo;                   // CRIA O OBJETO ARQUIVO DA BIBLIOTECA - LEITURA DO ARQUIVO
    cout<<"Type the file name"<<endl;
    cin.getline(nomearq,MAXI);          // RECEBE O NOME DO ARQUIVO

    arquivo.open(nomearq);              // ABRE O ARQUIVO

if(!arquivo.is_open()){                 // SE O ARQUIVO NÃO FOI ABERTO POR QUE NÃO EXISTE NO DIRETÓRIO
     cout<<"\n ERROR: NOT FOUND"<<endl; // MENSAGEM DE ERRO SERÁ ESCRITA
     exit(0);                           // E ACABA A EXECUÇÃO DO PROGRAMA
}
    arquivo>>pic.crip;                  // RECEBE O TIPO DE CRIPTOGRAFIA DA IMAGEM

while(strcmp(pic.crip,"P2")){           // CASO A CRIP SEJA DIFERENTE DA DETERMINADA, É SOLICITADO A ENTRADA DE OUTRO ARQUIVO E FAZ A VAIDAÇÃO NOVAMENTE
    cout<<"Type the file name that is compatible"<<endl;
    cin.getline(nomearq,MAXI);
    arquivo.open(nomearq);
    if(!arquivo.is_open()){
     cout<<"\n ERROR: NOT FOUND"<<endl;
     exit(0);
    }
    arquivo>>pic.crip;
}

    arquivo>>pic.numberc>>pic.numberow>>pic.verif; // RECEBE O NúMERO DE COLUNAS, LINHA E O VALOR MáXIMO DOS PIXELS
    for(int i=0;i<pic.numberow;i++){               // RECEBE OS DADOS DE PIXELS DA IMAGEM E ARMAZENA NA MATRIZ "data"
        for(int j=0;j<pic.numberc;j++){
            arquivo>>pic.data[i][j];
        }
    }

arquivo.close(); // FECHA O ARQUIVO
}

void equaliza_histograma(){

    int HIST[256] = {};

    for(int i=0;i<256;i++) HIST[i] = 0;

    for(int i=0;i<pic.numberow;i++){
        for(int j=0;j<pic.numberc;j++){
            HIST[pic.data[i][j]]++;
        }
    }

    for(int i=1;i<256;i++){
      HIST[i] += HIST[i-1];
    }

    for(int i=0;i<pic.numberow;i++){
        for(int j=0;j<pic.numberc;j++){
            final_image2[i][j] = (int)(255*HIST[pic.data[i][j]])/(pic.numberc*pic.numberow); // ARMAZENAMENTO DO RESULTADO DA EQUA
        }
    }

    ofstream arq; // ABRE O OBJETO ARQUIVO PARA ARMAZENAMENTO - ESCRITA NO ARQUIVO DA FUNÇÃO EQUALIZAÇÃO DE HISTOGRAMA
    arq.open("ResultingPhoto_SOThread2.pgm");  // CRIA E ABRE UM ARQUIVO COM O NOME DESCRITO NA FUNÇÃO
    arq<<pic.crip<<endl<<pic.numberc<<" "<<pic.numberow<<endl<<pic.verif<<endl; // ARMAZENA CRIPTOGRAFIA, NÓ DE COLUNAS, DE LINHAS E MAIOR PIXEL
     for(int i=0;i<pic.numberow;i++){
        for(int j=0;j<pic.numberc;j++){
            arq<<final_image2[i][j]<<" "; // ADICIONA NO ARQUIVO A MATRIZ DE DADOS DA IMAGEM
        }
    }
    arq.close(); // FECHA O ARQUIVO

}


void alargamento_contraste(){

    for(int i=0;i<pic.numberow;i++){
        for(int j=0;j<pic.numberc;j++){
            final_image[i][j] = pic.data[i][j];
        }
    }

    int IMAX = final_image[0][0];
    int IMIN = final_image[0][0];

    for(int i=0;i<pic.numberow;i++){
        for(int j=0;j<pic.numberc;j++){
            if(final_image[i][j]>IMAX) IMAX=final_image[i][j];
            if(final_image[i][j]<IMIN) IMIN=final_image[i][j];
        }
    }


    for(int i=0;i<pic.numberow;i++){
        for(int j=0;j<pic.numberc;j++){
            final_image[i][j] = (int)(255/(IMAX-IMIN))*(pic.data[i][j]-IMIN);
        }
    }

    ofstream arq;                               // ABRE O OBJETO ARQUIVO PARA ARMAZENAMENTO 
                                                // ESCRITA NO ARQUIVO DA FUNÇÃO ALARGAMENTO DE CONTRASTE
    arq.open("ResultingPhoto_SOThread1.pgm");   // CRIA E ABRE UM ARQUIVO COM O NOME DESCRITO NA FUNÇÃO
    arq<<pic.crip<<endl<<pic.numberc<<" "<<pic.numberow<<endl<<pic.verif<<endl; // MESMO DA FUNÇÃO ANTERIOR
    for(int i=0;i<pic.numberow;i++){
        for(int j=0;j<pic.numberc;j++){
            arq<<final_image[i][j]<<" "; // ARMAZENA NO ARQUIVO OS DADOS DA IMAGEM
        }
    }
    arq.close(); // FECHA O ARQUIVO

}
