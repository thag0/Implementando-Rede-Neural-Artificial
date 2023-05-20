#include <stdio.h>
#include "RedeNeural.c"

int main(){
   srand(time(NULL));
   system("cls");

   int qtd_neuronios_entrada = 3;
   int qtd_camadas_escondidas = 2;
   int qtd_neuronios_escondida = 2;
   int qtd_neuronios_saida = 3;

   double dados[3] = {1.2, 3.4, 4.5};

   RedeNeural* rede = RNA_criar_rede(qtd_neuronios_entrada, qtd_neuronios_escondida, qtd_neuronios_saida, qtd_camadas_escondidas);
   //implementar treino da rede
   RNA_calcular_entrada(rede, dados);
   RNA_imprimir_saida_neuronio_saida(rede);
   RNA_apagar_rede(rede);

   return 0;
}