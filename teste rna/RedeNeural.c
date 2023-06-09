#include <stdlib.h>
#include <math.h>

#define BIAS 1
#define TAXA_APRENDIZAGEM 0.1

#define FUNCAO_ATIVACAO(valor) RNA_funcao_ativacao_sigmoid(valor)
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
   Camada* ocultas;
   int quantidade_oculta;
   Camada saida;

}RedeNeural;


RedeNeural* RNA_criar_rede(int qtd_neuronios_entrada, int qtd_neuronios_escondida, int qtd_neuronios_saida, int qtd_camadas_escondidas);
void RNA_apagar_rede(RedeNeural* rede);
void RNA_inicializar_neuronio(Neuronio* neuronio, int quantidade_ligacoes);

double RNA_funcao_ativacao_relu(double valor);
double RNA_funcao_ativacao_reluDx(double valor);
double RNA_funcao_ativacao_sigmoid(double valor);


void RNA_calcular_saida(RedeNeural* rede, double dados[]);
void RNA_treinar(RedeNeural* rede, double entrada[], double alvo[]);

void RNA_imprimir_saidas_entrada(RedeNeural* rede);
void RNA_imprimir_saidas_ocultas(RedeNeural* rede);
void RNA_imprimir_saidas_saida(RedeNeural* rede);

//------------------------------------------------------------------
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


RedeNeural* RNA_criar_rede(int qtd_neuronios_entrada, int qtd_neuronios_ocultas, int qtd_neuronios_saida, int qtd_camadas_ocultas){
   RedeNeural* nova_rede = (RedeNeural*) malloc(sizeof(RedeNeural));

   //camadas de entrada
   nova_rede->entrada.quantidade_neuronios = qtd_neuronios_entrada;
   nova_rede->entrada.neuronios = (Neuronio*) malloc(qtd_neuronios_entrada * sizeof(Neuronio));
   for(int i = 0; i < qtd_neuronios_entrada; i++){
      RNA_inicializar_neuronio(&nova_rede->entrada.neuronios[i], qtd_neuronios_ocultas);
   }

   //camadas ocultas
   nova_rede->ocultas = (Camada*) malloc(qtd_camadas_ocultas * sizeof(Camada));
   for(int i = 0; i < qtd_camadas_ocultas; i++){
      nova_rede->ocultas[i].quantidade_neuronios = qtd_neuronios_ocultas;
      nova_rede->ocultas[i].neuronios = (Neuronio*) malloc(qtd_neuronios_ocultas * sizeof(Neuronio));


      for(int j = 0; j < qtd_neuronios_ocultas; j++){
         if(i == qtd_camadas_ocultas-1){
            RNA_inicializar_neuronio(&nova_rede->ocultas[i].neuronios[j], qtd_neuronios_saida);
         }else{
            RNA_inicializar_neuronio(&nova_rede->ocultas[i].neuronios[j], qtd_neuronios_ocultas);
         }
      }
   }
   nova_rede->quantidade_oculta = qtd_camadas_ocultas;


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
   for(int i = 0; i < rede->quantidade_oculta; i++){
      free(rede->ocultas[i].neuronios);
   }
   free(rede->saida.neuronios);
   free(rede->ocultas);
   free(rede);   
}


void RNA_inicializar_neuronio(Neuronio* neuronio, int quantidade_ligacoes){
   neuronio->quantidade_ligacoes = quantidade_ligacoes;
   neuronio->peso = (double*) malloc(quantidade_ligacoes * sizeof(double));
   int limite_peso = 100;//adaptavel conforme o uso

   for(int i = 0; i < quantidade_ligacoes; i++){
      neuronio->peso[i] = (rand() % (limite_peso*2)) - limite_peso;
   }

   neuronio->saida = 0;
}


void RNA_calcular_saida(RedeNeural* rede, double dados[]){

   //camada de entrada
   int qtd_neuronios_entrada = rede->entrada.quantidade_neuronios;
   double soma = 0;
   for(int i = 0; i < qtd_neuronios_entrada; i++){
      rede->entrada.neuronios[i].saida = dados[i];
   }

   //camadas ocultas
   for (int camada_oculta = 0; camada_oculta < rede->quantidade_oculta; camada_oculta++){
      /*referencias para as camadas atual e anterior
       * Atualiza as referencias com base na camada oculta atual
      */
      Camada* camada_atual = &(rede->ocultas[camada_oculta]);
      Camada* camada_anterior;
      if (camada_oculta == 0) camada_anterior = &(rede->entrada);
      else camada_anterior = &(rede->ocultas[camada_oculta - 1]);

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
      Camada* oculta = &rede->ocultas[rede->quantidade_oculta-1];

      //percorrer cada neuronio da camada oculta anterior
      double soma = 0;
      for(int neuronio_oculta = 0; neuronio_oculta < oculta->quantidade_neuronios; neuronio_oculta++){
         soma += (oculta->neuronios[neuronio_oculta].saida) * (rede->saida.neuronios[neuronio_saida].peso[neuronio_oculta]);
      }
      soma += BIAS;
      rede->saida.neuronios[neuronio_saida].saida = FUNCAO_ATIVACAO_SAIDA(soma);
   }
}


void RNA_treinar(RedeNeural* rede, double entrada[], double classe[]){
   double erro_total = 0.0;

   // Etapa de propagação direta (forward propagation)
   RNA_calcular_saida(rede, entrada);

   // Cálculo do erro na camada de saída
   for(int i = 0; i < rede->saida.quantidade_neuronios; i++){
      double saida = rede->saida.neuronios[i].saida;
      double erro = classe[i] - saida;
      erro_total += pow(erro, 2);

      // Cálculo do gradiente local na camada de saída
      double gradiente_local = erro * FUNCAO_ATIVACAO(saida);

      // Atualização dos pesos na camada de saída
      for(int j = 0; j < rede->quantidade_oculta; j++){
         Camada* oculta = &rede->ocultas[rede->quantidade_oculta - 1];
         double saida_oculta = oculta->neuronios[j].saida;
         double delta_peso = gradiente_local * saida_oculta * TAXA_APRENDIZAGEM;
         rede->saida.neuronios[i].peso[j] += delta_peso;
      }
   }

   // Cálculo do erro nas camadas ocultas e atualização dos pesos
   for(int i = rede->quantidade_oculta - 1; i >= 0; i--){
      Camada* camada_atual = &rede->ocultas[i];
      Camada* camada_anterior;

      if(i == 0) camada_anterior = &rede->entrada;
      else camada_anterior = &rede->ocultas[i - 1];

      for(int j = 0; j < camada_atual->quantidade_neuronios; j++){
         double gradiente_local = 0.0;

         // Cálculo do gradiente local na camada oculta
         for(int k = 0; k < rede->saida.quantidade_neuronios; k++){
            double erro = classe[k] - rede->saida.neuronios[k].saida;
            double peso = rede->saida.neuronios[k].peso[j];
            gradiente_local += erro * FUNCAO_ATIVACAO_SAIDA(rede->saida.neuronios[k].saida) * peso;
         }

         // Atualização dos pesos na camada oculta
         for(int l = 0; l < camada_anterior->quantidade_neuronios; l++){
            double saida_anterior = camada_anterior->neuronios[l].saida;
            double delta_peso = gradiente_local * saida_anterior * TAXA_APRENDIZAGEM;
            camada_atual->neuronios[j].peso[l] += delta_peso;
         }
      }
   }
}


//impressões
void RNA_imprimir_saidas_entrada(RedeNeural* rede){
   printf("--Entrada--\n");
   for(int i = 0; i < rede->entrada.quantidade_neuronios; i++){
      printf("[neuronio(%d): %.2f]\n", i, rede->entrada.neuronios[i].saida);
   }
   printf("\n"); 
}


void RNA_imprimir_saidas_ocultas(RedeNeural* rede){
   printf("--Camadas ocultas--\n");
   for(int camada_escondida = 0; camada_escondida < (rede->quantidade_oculta); camada_escondida++){
      printf("Camada %d\n", camada_escondida);
      for(int i = 0; i < rede->ocultas[camada_escondida].quantidade_neuronios; i++){
         printf("[neuronio(%d): %.2f]\n", i, rede->ocultas[camada_escondida].neuronios[i].saida);
      }
   }
   printf("\n");
}


void RNA_imprimir_saidas_saida(RedeNeural* rede){
   printf("--Saida--\n");
   for(int i = 0; i < rede->saida.quantidade_neuronios; i++){
      printf("[neuronio(%d): %.2f]\n", i, rede->saida.neuronios[i].saida);
   }
   printf("\n");
}