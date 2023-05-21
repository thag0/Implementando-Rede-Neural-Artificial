#include <stdio.h>
#include <time.h>
#include "RedeNeural.c"

int main(){
   srand(time(NULL));
   system("cls");

   int qtd_neuronios_entrada = 3;
   int qtd_neuronios_escondida = 4;
   int qtd_neuronios_saida = 3;
   int qtd_camadas_ocultas = 1;

   double dados[3] = {1.2, 3.4, 5.6};

   RedeNeural* rede = RNA_criar_rede(qtd_neuronios_entrada, qtd_neuronios_escondida, qtd_neuronios_saida, qtd_camadas_ocultas);
   RedeNeural* rede2 = RNA_criar_rede(qtd_neuronios_entrada, qtd_neuronios_escondida, qtd_neuronios_saida, qtd_camadas_ocultas);
   //implementar treino da rede
   printf("rede 1\n");
   RNA_calcular_entrada(rede, dados);
   RNA_imprimir_saidas_entrada(rede);
   RNA_imprimir_saidas_ocultas(rede);
   RNA_imprimir_saidas_saida(rede);
   RNA_apagar_rede(rede);

   printf("rede 2\n");
   RNA_calcular_entrada(rede2, dados);
   RNA_imprimir_saidas_entrada(rede2);
   RNA_imprimir_saidas_ocultas(rede2);
   RNA_imprimir_saidas_saida(rede2);
   RNA_apagar_rede(rede2);

   return 0;
}