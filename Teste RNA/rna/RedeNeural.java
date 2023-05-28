package rna;


public class RedeNeural implements Cloneable{
   public Camada entrada;
   public Camada[] ocultas;
   public Camada saida;
   
   public int qtdNeuroniosEntrada;
   public int qtdNeuroniosOcultas;
   public int qtdNeuroniosSaida;

   public int qtdCamadasOcultas;

   int BIAS = 1;
   double TAXA_APRENDIZAGEM = 0.1;

   public RedeNeural(int qtdNeuroniosEntrada, int qtdNeuroniosOcultas, int qtdNeuroniosSaida, int qtdCamadasOcultas){
      if(qtdNeuroniosEntrada < 1 || qtdNeuroniosOcultas < 1 || qtdNeuroniosSaida < 1 || qtdCamadasOcultas < 1){
         throw new IllegalArgumentException("Os valores devem ser maiores ou iguais a um.");
      }

      this.qtdNeuroniosEntrada = qtdNeuroniosEntrada;
      this.qtdNeuroniosOcultas = qtdNeuroniosOcultas;
      this.qtdNeuroniosSaida = qtdNeuroniosSaida;

      this.qtdCamadasOcultas = qtdCamadasOcultas;

      criarRede();
   }


   private void criarRede(){
      //inicializar camada de entrada
      entrada = new Camada();
      entrada.neuronios = new Neuronio[qtdNeuroniosEntrada];
      for(int i = 0; i < qtdNeuroniosEntrada; i++){
         entrada.neuronios[i] = new Neuronio(qtdNeuroniosOcultas);
      }

      //inicializar camadas ocultas
      ocultas = new Camada[qtdCamadasOcultas];
      for (int i = 0; i < qtdCamadasOcultas; i++) {
         Camada novaOculta = new Camada();
         novaOculta.neuronios = new Neuronio[qtdNeuroniosOcultas];
      
         for (int j = 0; j < qtdNeuroniosOcultas; j++) {
            if (i == (qtdCamadasOcultas-1)){
               novaOculta.neuronios[j] = new Neuronio(qtdNeuroniosSaida);
            
            }else{
               novaOculta.neuronios[j] = new Neuronio(qtdNeuroniosOcultas);
            }
         }
         ocultas[i] = novaOculta;
      }

      //inicializar camada de saída
      saida = new Camada();
      saida.neuronios = new Neuronio[qtdNeuroniosSaida];
      for(int i = 0; i < qtdNeuroniosSaida; i++){
         saida.neuronios[i] = new Neuronio(qtdNeuroniosSaida);
      }

   }


   public void calcularSaida(RedeNeural rede, double[] dados){
      if(dados.length != rede.entrada.neuronios.length){
         throw new IllegalArgumentException("As dimensões dos dados de entrada com os neurônios de entrada da rede não são iguais");
      }

      //entrada
      for(int i = 0; i < rede.qtdNeuroniosEntrada; i++){
         rede.entrada.neuronios[i].saida = dados[i];
      }

      //ocultas
      double soma = 0.0;
      for(int i = 0; i < rede.qtdCamadasOcultas; i++){//percorrer camadas ocultas

         Camada camadaAtual = rede.ocultas[i];
         Camada camadaAnterior;
         if(i == 0) camadaAnterior = rede.entrada;
         else camadaAnterior = rede.ocultas[i-1];

         for(int j = 0; j < camadaAtual.neuronios.length; j++){
            soma = 0.0;
            for(int k = 0; k < camadaAnterior.neuronios.length; k++){
               soma += camadaAnterior.neuronios[k].saida * camadaAnterior.neuronios[k].pesos[j];
            }
            soma += BIAS;
            camadaAtual.neuronios[j].saida = relu(soma);
         }
      }

      //saída
      for(int neuronioSaida = 0; neuronioSaida < rede.saida.neuronios.length; neuronioSaida++){
         Camada oculta = rede.ocultas[rede.qtdCamadasOcultas-1];
         soma = 0.0;

         for(int neuronioOculta = 0; neuronioOculta < oculta.neuronios.length; neuronioOculta++){
            soma += oculta.neuronios[neuronioOculta].saida * oculta.neuronios[neuronioOculta].pesos[neuronioSaida]; 
         }
         soma += BIAS;
         rede.saida.neuronios[neuronioSaida].saida = reluDx(soma);
      }
   }

   
   public double calcularPrecisao(RedeNeural rede, double[][] dados){
      double precisao = 0;

      double[] dados_treino = new double[dados[0].length-1];
      double[] classe_treino = new double[1];
      int acertosTotais = 0;
      int acertosSaida = 0;

      for(int i = 0; i < dados.length; i++){
         dados_treino[0] = dados[i][0];
         dados_treino[1] = dados[i][1];
         dados_treino[2] = dados[i][2];
         classe_treino[0]= dados[i][3];

         rede.calcularSaida(rede, dados_treino);

         for(int j = 0; j < rede.saida.neuronios.length; j++){
            if(rede.saida.neuronios[j].saida == classe_treino[j]){
               acertosSaida ++;
            }
         }
         
         if(acertosSaida == (rede.saida.neuronios.length)) acertosTotais++;

      }

      precisao = (double) acertosTotais/dados.length;
      return (precisao*100);
   }


   //funções de ativação
   public double relu(double valor){
      if(valor < 0) return 0;
      return valor;
   }


   public double reluDx(double valor){
      if(valor < 0) return 0;
      return 1;     
   }


   public double sigmoid(double valor){
      return 1 / (1 + Math.exp(-valor));
   }


   public double sigmoidDx(double valor){
      return (sigmoid(valor) * (1-sigmoid(valor)));
   }


   public double tanH(double valor){
      return ((2 * sigmoid(valor) * (2*valor)) - 1);
   }


   //clonar a rede
   @Override
   public RedeNeural clone(){
      try {
         RedeNeural clone = (RedeNeural) super.clone();

         // Clonar dados importantes
         clone.qtdNeuroniosEntrada = this.qtdNeuroniosEntrada;
         clone.qtdNeuroniosOcultas = this.qtdNeuroniosOcultas;
         clone.qtdNeuroniosSaida = this.qtdNeuroniosSaida;
         clone.qtdCamadasOcultas = this.qtdCamadasOcultas;
         clone.BIAS = this.BIAS;
         clone.TAXA_APRENDIZAGEM = this.TAXA_APRENDIZAGEM;

         // Clonar camada de entrada
         clone.entrada = cloneCamada(this.entrada);

         // Clonar camadas ocultas
         clone.ocultas = new Camada[qtdCamadasOcultas];
         for (int i = 0; i < qtdCamadasOcultas; i++) {
            clone.ocultas[i] = cloneCamada(this.ocultas[i]);
         }

         // Clonar camada de saída
         clone.saida = cloneCamada(this.saida);

         return clone;
      } catch (CloneNotSupportedException e){
         throw new RuntimeException(e);
      }
   }


   private Camada cloneCamada(Camada camada){
      Camada clone = new Camada();
      clone.neuronios = new Neuronio[camada.neuronios.length];

      for (int i = 0; i < camada.neuronios.length; i++) {
         clone.neuronios[i] = cloneNeuronio(camada.neuronios[i], camada.neuronios[i].qtdLigacoes, camada.neuronios[i].pesos);
      }

      return clone;
   }


   private Neuronio cloneNeuronio(Neuronio neuronio, int qtdLigacoes, double[] pesos){
      Neuronio clone = new Neuronio(neuronio.pesos.length);

      double pesosClone[] = new double[qtdLigacoes];

      for(int i = 0; i < pesos.length; i++){
         pesosClone[i] = pesos[i];
      }

      clone.pesos = pesosClone;
      clone.qtdLigacoes = qtdLigacoes;

      return clone;
   }
}
