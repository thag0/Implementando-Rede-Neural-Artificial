package rna;


public class RedeNeural{
   public Camada entrada;
   public Camada[] ocultas;
   public Camada saida;
   
   public int qtdNeuroniosEntrada;
   public int qtdNeuroniosOcultas;
   public int qtdNeuroniosSaida;

   public int qtdCamadasOcultas;

   int BIAS = 1;
   final double TAXA_APRENDIZAGEM = 0.2;

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
      for(int camadaOculta = 0; camadaOculta < rede.qtdCamadasOcultas; camadaOculta++){//percorrer camadas ocultas

         Camada camadaAtual = rede.ocultas[camadaOculta];
         Camada camadaAnterior;
         if(camadaOculta == 0) camadaAnterior = rede.entrada;
         else camadaAnterior = rede.ocultas[camadaOculta-1];

         for(int neuronio = 0; neuronio < camadaAtual.neuronios.length; neuronio++){
            double soma = 0.0;
            for(int i = 0; i < camadaAnterior.neuronios.length; i++){
               soma += camadaAnterior.neuronios[i].saida * camadaAnterior.neuronios[i].pesos[neuronio];
            }
            soma += BIAS;
            camadaAtual.neuronios[neuronio].saida = sigmoid(soma);
         }
      }

      //saída
      for(int neuronioSaida = 0; neuronioSaida < rede.saida.neuronios.length; neuronioSaida++){
         Camada oculta = rede.ocultas[rede.qtdCamadasOcultas-1];
         double soma = 0.0;

         for(int neuronioOculta = 0; neuronioOculta < oculta.neuronios.length; neuronioOculta++){
            soma += oculta.neuronios[neuronioOculta].saida * oculta.neuronios[neuronioOculta].pesos[neuronioSaida]; 
         }
         soma += BIAS;
         rede.saida.neuronios[neuronioSaida].saida = reluDx(soma);
      }
   }


   public void backPropagation(RedeNeural rede, double[][] dados, double[] classe, int epochs){
      double[] dados_treino = new double[dados[0].length-1];
      double[] classe_treino = new double[1];

      for(int i = 0; i < epochs; i++){

         for(int j = 0; j < dados[1].length-1; j++){
            for(int k = 0; k < dados_treino.length; k++){
               dados_treino[k] = dados[j][k];
            }
            classe_treino[0] = dados[j][dados[0].length-1];
            rede.backPropagation(rede, dados_treino, classe_treino);
         }
      }
   }
   /**
    * teste
    */
   private void backPropagation(RedeNeural rede, double[] entrada, double[] classe){

      //calcular saída
      calcularSaida(rede, entrada);
      //comparar o valor desejado da saída com o resultado da rede (erro)

      double erro = 0.0;
      //percorrer saída
      for(int i = 0; i < rede.saida.neuronios.length; i++){
         //para cada neuronio da saida, percorrer todos os neuronios da camada anterior
         //que se relacionam com ele
         //novo_peso = peso_anterior + TAXA_APRENDIZAGEM * erro * entrada
         Camada camadaAnterior = rede.ocultas[rede.ocultas.length-1];//ultima camada oculta
         double saida = rede.saida.neuronios[i].saida;
         erro = classe[i] - saida;
         erro = Math.pow(erro, 2);

         //percorrer os neuronios da camada anterior para reajustar os pesos
         for(int j = 0; j < camadaAnterior.neuronios.length; j++){

            //percorrer os pesos do neuronio
            for(int k = 0; k < camadaAnterior.neuronios[j].pesos.length; k++){
               
               if(i == k){//peso relativo ao neuronio de saída
                  double saidaNeuronioOculta = camadaAnterior.neuronios[j].saida;
                  double pesoNeuronioOculta = camadaAnterior.neuronios[j].pesos[k];
                  camadaAnterior.neuronios[j].pesos[k] = pesoNeuronioOculta + (TAXA_APRENDIZAGEM * erro * saidaNeuronioOculta);
               }
            }
         }
      }

      //percorrer camadas ocultas até a entrada
      for(int i = rede.ocultas.length-2; i >= 0; i--){
         Camada camadaAtual = rede.ocultas[i];
         Camada camadaAnterior;
         
         //se estiver na ultima camada oculta, a anterior vai ser a entrada
         //se não, vai ser a camada anterior relativa a camada atual
         if(i == rede.ocultas.length-1) camadaAnterior = rede.entrada;
         else camadaAnterior = rede.ocultas[rede.ocultas.length-1];


         //percorrer os neuronios da camada atual
         for(int j = 0; j < camadaAtual.neuronios.length; j++){

            //percorrer os neuronios da camada anterior
            for(int k = 0; k < camadaAnterior.neuronios.length; k++){

               //percorrer os pesos dos neuronios
               for(int l = 0; l < camadaAnterior.neuronios[k].pesos.length; l++){
                  //novo_peso = peso_anterior + TAXA_APRENDIZAGEM * erro * entrada
                  if(l == j){
                     double pesoNeuronioOculta = camadaAtual.neuronios[k].pesos[l];
                     double saidaNeuronioOculta = camadaAtual.neuronios[k].saida;
                     camadaAnterior.neuronios[k].pesos[l] = pesoNeuronioOculta + (TAXA_APRENDIZAGEM * erro * saidaNeuronioOculta);
                  }
               }
            }
         }
      }
   }


   public double calcularPrecisao(RedeNeural rede, double[][] dados, double[] classe){
      double precisao = 0;

      double[] dados_treino = new double[3];
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
}
