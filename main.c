#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define BIAS 1

typedef struct{
   double* peso;
   double saida;

   int quantidade_ligacoes;
}Neuronio;


typedef struct{
   Neuronio* neuronios;
   int quantidade_neuronios;
}Camada;


typedef struct{
   Camada entrada;
   Camada* escondidas;
   int quantidade_escondidas;
   Camada saida;

}RedeNeural;

RedeNeural* criar_rede(int qtd_neuronios_entrada, int qtd_neuronios_escondida, int qtd_neuronios_saida, int qtd_camadas_escondidas);
void apagar_rede(RedeNeural* rede);
double funcao_ativacao_relu(double valor);
double funcao_ativacao_reluDx(double valor);
double funcao_ativacao_sigmoid(double valor);
void inicializar_neuronio(Neuronio* neuronio, int quantidade_ligacoes);
void imprimir_pesos_entrada(RedeNeural* rede);
void calcular_entrada(RedeNeural* rede, double dados[]);
void imprimir_saida_neuronio_saida(RedeNeural* rede);


int main(){
   srand(time(NULL));
   system("cls");

   int qtd_neuronios_entrada = 3;
   int qtd_camadas_escondidas = 2;
   int qtd_neuronios_escondida = 2;
   int qtd_neuronios_saida = 3;

   double dados[3] = {1.2, 3.4, 4.5};

   RedeNeural* rede = criar_rede(qtd_neuronios_entrada, qtd_neuronios_escondida, qtd_neuronios_saida, qtd_camadas_escondidas);
   //implementar treino da rede
   calcular_entrada(rede, dados);
   imprimir_saida_neuronio_saida(rede);
   apagar_rede(rede);

   return 0;
}


double funcao_ativacao_relu(double valor){
   if(valor < 0) return 0;
   return valor;
}


double funcao_ativacao_reluDx(double valor){
   if(valor < 0) return 0;
   return 1; 
}


double funcao_ativacao_sigmoid(double valor){
   return 1 / (1 + exp(-valor));
}


RedeNeural* criar_rede(int qtd_neuronios_entrada, int qtd_neuronios_escondida, int qtd_neuronios_saida, int qtd_camadas_escondidas){
   RedeNeural* nova_rede = (RedeNeural*) malloc(sizeof(RedeNeural));

   //camadas de entrada
   nova_rede->entrada.quantidade_neuronios = qtd_neuronios_entrada;
   nova_rede->entrada.neuronios = (Neuronio*) malloc(qtd_neuronios_entrada * sizeof(Neuronio));
   for(int i = 0; i < qtd_neuronios_entrada; i++){
      inicializar_neuronio(&nova_rede->entrada.neuronios[i], qtd_neuronios_escondida);
   }

   //camadas escondidas
   nova_rede->escondidas = (Camada*) malloc(qtd_camadas_escondidas * sizeof(Camada));
   for(int i = 0; i < qtd_camadas_escondidas; i++){
      nova_rede->escondidas[i].quantidade_neuronios = qtd_neuronios_escondida;
      nova_rede->escondidas[i].neuronios = (Neuronio*) malloc(qtd_neuronios_escondida * sizeof(Neuronio));

      for(int j = 0; j < qtd_neuronios_escondida; j++){
         inicializar_neuronio(&nova_rede->escondidas[i].neuronios[j], qtd_neuronios_escondida);
      }
   }
   nova_rede->quantidade_escondidas = qtd_camadas_escondidas;


   //cada de saída
   nova_rede->saida.quantidade_neuronios = qtd_neuronios_saida;
   nova_rede->saida.neuronios = (Neuronio*) malloc(qtd_neuronios_saida * sizeof(Neuronio));
   for(int i = 0; i < qtd_neuronios_saida; i++){
      inicializar_neuronio(&nova_rede->saida.neuronios[i], qtd_neuronios_saida);
   }

   return nova_rede;
}


void apagar_rede(RedeNeural* rede){
   free(rede->entrada.neuronios);
   for(int i = 0; i < rede->quantidade_escondidas; i++){
      free(rede->escondidas[i].neuronios);
   }
   free(rede->saida.neuronios);
   free(rede->escondidas);
   free(rede);   
}


void inicializar_neuronio(Neuronio* neuronio, int quantidade_ligacoes){
   neuronio->quantidade_ligacoes = quantidade_ligacoes;
   neuronio->peso = (double*) malloc(quantidade_ligacoes * sizeof(double));

   for(int i = 0; i < quantidade_ligacoes; i++){
      neuronio->peso[i] = (rand() % 200) - 100;
   }

   neuronio->saida = 0;
}


void calcular_entrada(RedeNeural* rede, double dados[]){

   //camada de entrada
   int tamanho = rede->entrada.quantidade_neuronios;
   for(int i = 0; i < tamanho; i++){
      rede->entrada.neuronios[i].saida = ((rede->entrada.neuronios[i].peso[i]) * dados[i]) + BIAS;
      rede->entrada.neuronios[i].saida = funcao_ativacao_relu(rede->entrada.neuronios[i].saida);
   }

   //camadas ocultas
   for (int camada_oculta = 0; camada_oculta < rede->quantidade_escondidas; camada_oculta++){
      /*referencias para as camadas atual e anterior
       * Atualiza as referencias com base na camada oculta atual
      */
      Camada* camada_atual = &(rede->escondidas[camada_oculta]);
      Camada* camada_anterior;
      if (camada_oculta == 0) camada_anterior = &(rede->entrada);
      else camada_anterior = &(rede->escondidas[camada_oculta - 1]);

      /*percorre os neuronios da camada atual
       * e para cada neuronio da camada atual, faz a soma
       * por todos os neuronios da camada anterior
      */ 
      for(int neuronio = 0; neuronio < (camada_atual->quantidade_neuronios); neuronio++){
         double soma = 0;

         for (int i = 0; i < (camada_anterior->quantidade_neuronios); i++){
            soma += camada_anterior->neuronios[i].saida * camada_atual->neuronios[neuronio].peso[i];
         }

         camada_atual->neuronios[neuronio].saida = funcao_ativacao_relu(soma + BIAS);
      }
   }

   //camada de saída
   //percorrer cada neuronio de saída
   for(int neuronio_saida = 0; neuronio_saida < (rede->saida.quantidade_neuronios); neuronio_saida++){
      Camada* oculta = &rede->escondidas[rede->quantidade_escondidas-1];

      //percorrer cada neuronio da camada oculta anterior
      double soma = 0;
      for(int neuronio_oculta = 0; neuronio_oculta < oculta->quantidade_neuronios; neuronio_oculta++){
         soma += (oculta->neuronios[neuronio_oculta].saida) * (rede->saida.neuronios[neuronio_saida].peso[neuronio_oculta]);
      }
      rede->saida.neuronios[neuronio_saida].saida = funcao_ativacao_reluDx(soma + BIAS);
   }
}


void imprimir_pesos_entrada(RedeNeural* rede){
   int neuronios_entrada = rede->entrada.quantidade_neuronios;

   //pesos dos neuronios para a primeira camada escondida
   for(int i = 0; i < neuronios_entrada; i++){
      for(int j = 0; j < rede->escondidas[0].quantidade_neuronios; j++){
         printf("[n_entrada %d][n_escondida %d][ %.2f ]\n", i, j, rede->entrada.neuronios[i].peso[j]);
      }
   }
   printf("\n");
}


void imprimir_saida_neuronio_saida(RedeNeural* rede){
   printf("--Saida--\n");
   for(int i = 0; i < rede->saida.quantidade_neuronios; i++){
      printf("[neuronio(%d): %.2f]\n", i, rede->saida.neuronios[i].saida);
   }
}