package treino;

import java.util.Random;

import rna.RedeNeural;

public class TreinoGenetico{
   int numeroIndividuos = 100;
   Random random = new Random();

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
}
