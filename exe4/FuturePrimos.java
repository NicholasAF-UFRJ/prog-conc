/* 

Aluno: Nicholas de Araujo

Verifica a primalidade de números de 1 a N usando Future.

Compilação: javac FuturePrimos.java
Execução: java FuturePrimos [N] [nThreads]
N é o número máximo a ser verificado (padrão 50000).
Se nThreads não for especificado, o padrão é 10.
Exemplo de execução: java FuturePrimos 100 5

*/

import java.util.concurrent.Callable;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;

import java.util.ArrayList;
import java.util.List;

class VerificaPrimo implements Callable<Boolean> {
  private final long n;

  public VerificaPrimo(long n) {
    this.n = n;
  }

  private boolean ehPrimo(long n) {
    if (n <= 1) return false;
    if (n == 2) return true;
    if (n % 2 == 0) return false;
    for (long i = 3; i * i <= n; i += 2) {
      if (n % i == 0) return false;
    }
    return true;
  }

  public Boolean call() {
    return ehPrimo(n);
  }
}

public class FuturePrimos {
  public static void main(String[] args) {
    // Valores padrão
    int N = 50000;
    int nThreads = 10;

    // Leitura dos argumentos da linha de comando
    if (args.length >= 1) {
      try {
        N = Integer.parseInt(args[0]);
      } catch (NumberFormatException e) {
        System.err.println("Primeiro argumento inválido. Usando N=50000");
      }
    }

    if (args.length >= 2) {
      try {
        nThreads = Integer.parseInt(args[1]);
      } catch (NumberFormatException e) {
        System.err.println("Segundo argumento inválido. Usando 10 threads.");
      }
    }

    ExecutorService executor = Executors.newFixedThreadPool(nThreads);
    List<Future<Boolean>> list = new ArrayList<>();

    for (int i = 1; i <= N; i++) {
      Callable<Boolean> worker = new VerificaPrimo(i);
      Future<Boolean> submit = executor.submit(worker);
      list.add(submit);
    }

    int qtdPrimos = 0;
    for (int i = 0; i < list.size(); i++) {
      try {
        boolean ehPrimo = list.get(i).get();
        if (ehPrimo) {
          qtdPrimos++;
        }
      } catch (InterruptedException | ExecutionException e) {
        e.printStackTrace();
      }
    }

    System.out.println("Total de primos, com " + nThreads + " threads, entre 1 e " + N + ": " + qtdPrimos);
    executor.shutdown();
  }
}
