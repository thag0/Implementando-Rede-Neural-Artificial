import rna.RedeNeural;

class Main{
   public static void main(String[] args){
      limparConsole();

      //porta lógica xor em cascata com outra xor
      double[][] dados = new double[][]{
         {0, 0, 0, 0},
         {0, 0, 1, 1},
         {0, 1, 0, 1},
         {0, 1, 1, 0},
         {1, 0, 0, 1},
         {1, 0, 1, 0},
         {1, 1, 0, 0},
         {1, 1, 1, 1}
      };

      double[] dados_teste = new double[3];
      double[] classe_teste = new double[1];

      RedeNeural rede = new RedeNeural(3, 3, 1, 2);

      dados_teste[0] = 1;
      dados_teste[1] = 0;
      dados_teste[2] = 1;
      System.out.println("Resultado antes do treino (" + dados_teste[0] + ") (" + dados_teste[1] + ") (" + dados_teste[2] + ")");
      rede.calcularSaida(rede, dados_teste);


      Auxiliares.imprimirarApenasSaidasRede(rede);
   
      rede.backPropagation(rede, dados, classe_teste, 1000);


      System.out.println("------------------------------------------------------------");
      System.out.println("Precisao " + rede.calcularPrecisao(rede, dados, classe_teste));

      System.out.println("Resultado depois do treino (" + dados_teste[0] + ") (" + dados_teste[1] + ") (" + dados_teste[2] + ")");
      rede.calcularSaida(rede, dados_teste);
      Auxiliares.imprimirarApenasSaidasRede(rede);
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