package treino;

import java.util.Random;

import rna.RedeNeural;

public class TreinoGenetico{
   public int qtdPopulacao;
   public float TAXA_MUTACAO;
   public Individuo[] individuos;


   Random random = new Random();

   public TreinoGenetico(int qtdPopulacao){
      this.qtdPopulacao = qtdPopulacao;

      individuos = new Individuo[qtdPopulacao];
   }


   public void carregarIndividuo(Individuo individuo){
      for(int i = 0; i < individuos.length; i++){
         if(individuos[i] == null){
            individuos[i] = individuo;
         }
      }
   }


   public void ajustarPesos(RedeNeural rede){
      //percorrer camada de entrada
      //percorrer neuronios da camada de entrada
      for(int i = 0; i < rede.entrada.neuronios.length; i++){
         //percerrer pesos de cada neuronio da camada de entrada
         for(int j = 0; j < rede.entrada.neuronios[i].pesos.length; j++){
            rede.entrada.neuronios[i].pesos[j] += novoValorAleatorio();
         }
      }

      //percorrer camadas ocultas
      for(int i = 0; i < rede.ocultas.length; i++){
         //percorrer neuronios da camada oculta
         for(int j = 0; j < rede.ocultas[i].neuronios.length; j++){
            //percorrer pesos de cada neuronio da camada oculta
            for(int k = 0; k < rede.ocultas[i].neuronios[j].pesos.length; k++){
               rede.ocultas[i].neuronios[j].pesos[k] += novoValorAleatorio();
            }
         }
      }
   }


   private double novoValorAleatorio(){
      //tentar gerar valores bem pequenos
      double valor = random.nextDouble();
      valor /= 4;

      //inverter valor aleatoriamente
      if(random.nextInt(11) % 2 == 0) valor *= -1;
      return valor;
   }


   public void calcularFitness(Individuo individuo){
      int fitness = 0;

      fitness -= (individuo.batidasParede * 0.125);
      fitness += (individuo.flechaAcertada * 5);
      fitness += (individuo.ouroColetado * 10);
      fitness += (individuo.passosDados * 0.45);
      fitness += (individuo.jogoGanho * 15);

      individuo.fitness = fitness;
   }


   public Individuo escolherMelhorIndividuo(){
      int melhorValor = 0;
      int indice = 0;
   
      for (int i = 0; i < individuos.length; i++){
         if(i == 0){
            melhorValor = individuos[0].fitness;
            indice = 0;
         
         }else if(individuos[i].fitness > melhorValor){
            indice = i;
            melhorValor = individuos[i].fitness;
         }
      }
      return individuos[indice].clone();
   }
}
