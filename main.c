#include <stdio.h>
#include <time.h>
#include "RedeNeural.c"

int main(){
   srand(time(NULL));
   system("cls");

   int qtd_neuronios_entrada = 3;
   int qtd_neuronios_escondida = 5;
   int qtd_neuronios_saida = 2;
   int qtd_camadas_escondidas = 1;

   double dados[3] = {1.2, 3.4, 5.6};

   RedeNeural* rede = RNA_criar_rede(qtd_neuronios_entrada, qtd_neuronios_escondida, qtd_neuronios_saida, qtd_camadas_escondidas);
   //implementar treino da rede
   printf("rede criada\n");
   RNA_calcular_entrada(rede, dados);

   RNA_imprimir_saida_neuronio_entrada(rede);
   RNA_imprimir_saida_neuronio_ocultas(rede);
   RNA_imprimir_saida_neuronio_saida(rede);

   RNA_apagar_rede(rede);

   return 0;
}