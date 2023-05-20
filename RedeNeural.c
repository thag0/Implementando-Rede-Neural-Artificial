#include <stdlib.h>
#include <math.h>

#define BIAS 1

#define FUNCAO_ATIVACAO(valor) RNA_funcao_ativacao_relu(valor)
#define FUNCAO_ATIVACAO_SAIDA(valor) RNA_funcao_ativacao_reluDx(valor)

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


RedeNeural* RNA_criar_rede(int qtd_neuronios_entrada, int qtd_neuronios_escondida, int qtd_neuronios_saida, int qtd_camadas_escondidas);
void RNA_apagar_rede(RedeNeural* rede);
double RNA_funcao_ativacao_relu(double valor);
double RNA_funcao_ativacao_reluDx(double valor);
double RNA_funcao_ativacao_sigmoid(double valor);
void RNA_inicializar_neuronio(Neuronio* neuronio, int quantidade_ligacoes);
void RNA_imprimir_pesos_entrada(RedeNeural* rede);
void RNA_calcular_entrada(RedeNeural* rede, double dados[]);
void RNA_imprimir_saida_neuronio_saida(RedeNeural* rede);


double RNA_funcao_ativacao_relu(double valor){
   if(valor < 0) return 0;
   else return valor;
}


double RNA_funcao_ativacao_reluDx(double valor){
   if(valor < 0) return 0;
   return 1; 
}


double RNA_funcao_ativacao_sigmoid(double valor){
   return 1 / (1 + exp(-valor));
}


RedeNeural* RNA_criar_rede(int qtd_neuronios_entrada, int qtd_neuronios_escondida, int qtd_neuronios_saida, int qtd_camadas_escondidas){
   RedeNeural* nova_rede = (RedeNeural*) malloc(sizeof(RedeNeural));

   //camadas de entrada
   nova_rede->entrada.quantidade_neuronios = qtd_neuronios_entrada;
   nova_rede->entrada.neuronios = (Neuronio*) malloc(qtd_neuronios_entrada * sizeof(Neuronio));
   for(int i = 0; i < qtd_neuronios_entrada; i++){
      RNA_inicializar_neuronio(&nova_rede->entrada.neuronios[i], qtd_neuronios_escondida);
   }

   //camadas escondidas
   nova_rede->escondidas = (Camada*) malloc(qtd_camadas_escondidas * sizeof(Camada));
   for(int i = 0; i < qtd_camadas_escondidas; i++){
      nova_rede->escondidas[i].quantidade_neuronios = qtd_neuronios_escondida;
      nova_rede->escondidas[i].neuronios = (Neuronio*) malloc(qtd_neuronios_escondida * sizeof(Neuronio));

      for(int j = 0; j < qtd_neuronios_escondida; j++){
         RNA_inicializar_neuronio(&nova_rede->escondidas[i].neuronios[j], qtd_neuronios_escondida);
      }
   }
   nova_rede->quantidade_escondidas = qtd_camadas_escondidas;


   //cada de saída
   nova_rede->saida.quantidade_neuronios = qtd_neuronios_saida;
   nova_rede->saida.neuronios = (Neuronio*) malloc(qtd_neuronios_saida * sizeof(Neuronio));
   for(int i = 0; i < qtd_neuronios_saida; i++){
      RNA_inicializar_neuronio(&nova_rede->saida.neuronios[i], qtd_neuronios_saida);
   }

   return nova_rede;
}


void RNA_apagar_rede(RedeNeural* rede){
   free(rede->entrada.neuronios);
   for(int i = 0; i < rede->quantidade_escondidas; i++){
      free(rede->escondidas[i].neuronios);
   }
   free(rede->saida.neuronios);
   free(rede->escondidas);
   free(rede);   
}


void RNA_inicializar_neuronio(Neuronio* neuronio, int quantidade_ligacoes){
   neuronio->quantidade_ligacoes = quantidade_ligacoes;
   neuronio->peso = (double*) malloc(sizeof(double));

   (*neuronio->peso) = (rand() % 200) - 100;

   neuronio->saida = 0;
}


void RNA_calcular_entrada(RedeNeural* rede, double dados[]){

   //camada de entrada
   int qtd_neuronios_entrada = rede->entrada.quantidade_neuronios;
   double soma = 0;
   for(int i = 0; i < qtd_neuronios_entrada; i++){
      soma += dados[i] * (*rede->entrada.neuronios[i].peso);
      soma += BIAS;
      rede->entrada.neuronios[i].saida = FUNCAO_ATIVACAO(soma);
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
         soma += BIAS;
         camada_atual->neuronios[neuronio].saida = FUNCAO_ATIVACAO(soma);
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
      soma += BIAS;
      rede->saida.neuronios[neuronio_saida].saida = FUNCAO_ATIVACAO_SAIDA(soma);
   }
}


void RNA_imprimir_pesos_entrada(RedeNeural* rede){
   int neuronios_entrada = rede->entrada.quantidade_neuronios;

   //pesos dos neuronios para a primeira camada escondida
   for(int i = 0; i < neuronios_entrada; i++){
      for(int j = 0; j < rede->escondidas[0].quantidade_neuronios; j++){
         printf("[n_entrada %d][n_escondida %d][ %.2f ]\n", i, j, rede->entrada.neuronios[i].peso[j]);
      }
   }
   printf("\n");
}


void RNA_imprimir_saida_neuronio_entrada(RedeNeural* rede){
   printf("--Entrada--\n");
   for(int i = 0; i < rede->entrada.quantidade_neuronios; i++){
      printf("[neuronio(%d): %.2f]\n", i, rede->entrada.neuronios[i].saida);
   }
   printf("\n"); 
}


void RNA_imprimir_saida_neuronio_ocultas(RedeNeural* rede){
   printf("--Camadas ocultas--\n");
   for(int camada_escondida = 0; camada_escondida < (rede->quantidade_escondidas); camada_escondida++){
      printf("Camada %d\n", camada_escondida);
      for(int i = 0; i < rede->escondidas[camada_escondida].quantidade_neuronios; i++){
         printf("[neuronio(%d): %.2f]\n", i, rede->escondidas[camada_escondida].neuronios[i].saida);
      }
   }
   printf("\n");
}


void RNA_imprimir_saida_neuronio_saida(RedeNeural* rede){
   printf("--Saida--\n");
   for(int i = 0; i < rede->saida.quantidade_neuronios; i++){
      printf("[neuronio(%d): %.2f]\n", i, rede->saida.neuronios[i].saida);
   }
   printf("\n");
}