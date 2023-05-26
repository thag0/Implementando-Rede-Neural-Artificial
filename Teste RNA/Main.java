import rna.RedeNeural;
import treino.Individuo;
import treino.TreinoGenetico;

class Main{
   public static void main(String[] args){
      limparConsole();


      int nEntrada = 3;
      int nOcultas = 3;
      int nSaida = 3;
      int qtdOcultas = 1;

      int qtdPopulacao = 10;
      TreinoGenetico treinoGenetico = new TreinoGenetico(qtdPopulacao);

      for(int i = 0; i < qtdPopulacao; i++){
         Individuo novoIndividuo = gerarIndividuo(nEntrada, nOcultas, nSaida, qtdOcultas);
         treinoGenetico.carregarIndividuo(novoIndividuo);
         System.out.println(novoIndividuo);
      }

      treinoGenetico.individuos[4].batidasParede = 3;
      treinoGenetico.individuos[4].flechaAcertada = 1;
      treinoGenetico.individuos[4].ouroColetado = 1;
      treinoGenetico.individuos[4].passosDados = 50;
      treinoGenetico.individuos[4].batidasParede = 60;


      treinoGenetico.calcularFitness(treinoGenetico.individuos[4]);
      

      
      Individuo melhoreIndividuo = treinoGenetico.escolherMelhorIndividuo();
      System.out.println(melhoreIndividuo);
   }


   public static Individuo gerarIndividuo(int qtdNeuroniosEntrada, int qtdNeuroniosOcultas, int qtdNeuroniosSaida, int qtdOcultas){
      Individuo individuo = new Individuo();
      individuo.vivo = true;
      individuo.rede = new RedeNeural(qtdNeuroniosEntrada, qtdNeuroniosOcultas, qtdNeuroniosSaida, qtdOcultas);
      return individuo;
   }


   public static void limparConsole(){
      try{
         String nomeSistema = System.getProperty("os.name");

         if(nomeSistema.contains("Windows")){
            new ProcessBuilder("cmd", "/c", "cls").inheritIO().start().waitFor();
         }
         
      }catch(Exception e){
         System.out.println(e);
      }
   }
}