Relatório do Segundo Laboratório de Programação Concorrente - 2025.1

Aluno: Nicholas de Araujo Figueira DRE: 121088218

Compile os arquivos:

   gcc -o geraVetores gera_vetores.c
   gcc -o produtoInterno produto_interno_conc.c 

Execute os arquivos:
   ./geraVetores <nElementos> <arquivoBin>
   ./produtoInterno <arquivoBin> <nThreads>

É necessário gerar primeiro os vetores com o arquivo "geraVetores.c" para, usando o binário criado, poder executar o arquivo de "produtoInterno.c". Os valores encontrados concorrentemente e sequencialmente foram extremamente próximos. Mesmo com um altíssimo número de elementos gerados nos vetores, a variância foi mínima, sendo, inclusive, necessário o uso de float para diminuir a precisão e possiblitar a melhor visualização da margem de erro. O uso de double gerava uma variância literalmente zero, o que impossibilitava uma análise razoável. 
Ademais, precisei de vetores com tamanhos gigantescos (na faixa de milhões) para encontrar uma variância minimamente razoável. Com pequenos vetores o valor é praticamente desprezível. Um outro ponto que poderia ser ponderado é o tempo de execução para verificar se vale a pena o "investimento" na concorrência a partir de que ponto.
