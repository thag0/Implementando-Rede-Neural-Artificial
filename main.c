#include <stdio.h>
#include <time.h>
#include "RedeNeural.c"


#define QTD_TREINO 10000


void treino_sequencial(RedeNeural* rede, double dados[][3], double* entrada_teste, double* classe){
   for(int i = 0; i < QTD_TREINO; i++){
      if(i%4 == 0){
         entrada_teste[0] = dados[0][0];
         entrada_teste[1] = dados[0][1];
         *classe = dados[0][2];

      }else if(i%3 == 0){
         entrada_teste[0] = dados[1][0];
         entrada_teste[1] = dados[1][1];
         *classe = dados[1][2];

      }else if(i%2 == 0){
         entrada_teste[0] = dados[2][0];
         entrada_teste[1] = dados[2][1];
         *classe = dados[2][2];

      }else{
         entrada_teste[0] = dados[3][0];
         entrada_teste[1] = dados[3][1];
         *classe = dados[3][2];
      }

      RNA_treinar(rede, entrada_teste, classe);
   }
}


void treino_aleatorio(RedeNeural* rede, double dados[][3], double* entrada_teste, double* classe){
   int valor = 0;
   for(int i = 0; i < QTD_TREINO; i++){
      valor = rand() % 4;
      if(valor == 0){
         entrada_teste[0] = dados[0][0];
         entrada_teste[1] = dados[0][1];
         *classe = dados[0][2];

      }else if(valor == 1){
         entrada_teste[0] = dados[1][0];
         entrada_teste[1] = dados[1][1];
         *classe = dados[1][2];

      }else if(valor == 2){
         entrada_teste[0] = dados[2][0];
         entrada_teste[1] = dados[2][1];
         *classe = dados[2][2];

      }else if(valor == 3){
         entrada_teste[0] = dados[3][0];
         entrada_teste[1] = dados[3][1];
         *classe = dados[3][2];
      }

      RNA_treinar(rede, entrada_teste, classe);
   }
}


void impressao_resultados(RedeNeural* rede){
   RNA_imprimir_saidas_entrada(rede);
   RNA_imprimir_saidas_ocultas(rede);
   RNA_imprimir_saidas_saida(rede);
}


int main(){
   srand(time(NULL));
   system("cls");

   int qtd_neuronios_entrada = 2;
   int qtd_neuronios_oculta = 3;
   int qtd_neuronios_saida = 1;
   int qtd_camadas_ocultas = 2;

   int linhas_dados = 4;
   int colunas_dados = 3;
   double dados[4][3] = { 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 0};

   double* classe = (double*) malloc(sizeof(double));
   double* entrada_teste = (double*) malloc(2 * sizeof(double));
   double dados_predict[] = {0, 1};

   RedeNeural* rede = RNA_criar_rede(qtd_neuronios_entrada, qtd_neuronios_oculta, qtd_neuronios_saida, qtd_camadas_ocultas);
   printf("\t-Antes do treino-\n");
   RNA_calcular_saida(rede, dados_predict);
   impressao_resultados(rede);

   treino_aleatorio(rede, dados, entrada_teste, classe);

   printf("\t-Depois do treino-\n");
   RNA_calcular_saida(rede, dados_predict);
   impressao_resultados(rede);

   char resposta = ' ';
   printf("Fazer uma predicao ? [s/n]: ");
   scanf(" %c", &resposta);
   if(resposta == 's' || resposta == 'S'){
      int loop = 1;
      while(loop == 1){
         system("cls");
         printf("\t-Predicao-\n");
         printf("valor 00: ");
         scanf("%lf", &dados_predict[0]);
         printf("valor 01: ");
         scanf("%lf", &dados_predict[1]);

         RNA_calcular_saida(rede, dados_predict);
         RNA_imprimir_saidas_saida(rede);


         printf("\nDigite qualquer coisa para continuar ou x para sair: ");
         scanf(" %c", &resposta);
         if(resposta == 'x' || resposta == 'X') loop = 0;
      }


   }

   RNA_apagar_rede(rede);
   free(classe);
   free(entrada_teste);

   return 0;
}