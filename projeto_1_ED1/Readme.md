# 📊Projeto 1 — Estruturas de Dados I 

Implementação, em C, de operações sobre vetores inteiros e de uma análise experimental de desempenho. O projeto contempla inversão de vetor, busca sequencial e busca binária nas versões iterativa e recursiva.

## ✍️Integrantes

- Pedro Artur Barberá Sarni — 17918418
- Breno de Sousa Cad — 17881714
- Thales Amaral Gontijo — 17925010
- Murilo Fransciscato Ataide — 17916308

## ✨Funcionalidades

O programa principal (`prj_1.c`) lê um vetor e aceita as seguintes opções até receber `5`:

| Opção | Operação | Saída |
| --- | --- | --- |
| `1` | Inverte a ordem do vetor | Vetor invertido |
| `2 x` | Busca sequencial pelo valor `x` | `SIM` ou `NAO` |
| `3 x` | Busca binária iterativa pelo valor `x` | `SIM` ou `NAO` |
| `4 x` | Busca binária recursiva pelo valor `x` | `SIM` ou `NAO` |
| `5` | Encerra o programa | — |

> As opções `3` e `4` pressupõem que o vetor informado esteja ordenado em ordem crescente. A opção `1` exibe uma cópia invertida e não altera o vetor usado nas buscas posteriores.

## Como compilar e executar

No diretório `projeto_1_ED1`, use um compilador C compatível com C99 ou superior:

```bash
gcc -std=c99 -Wall -Wextra -o prj_1 prj_1.c
./prj_1
```

### Formato de entrada

1. Informe o tamanho `n` do vetor;
2. informe seus `n` elementos;
3. informe as opções do menu, e o valor procurado quando aplicável.

Exemplo:

```text
5
1 3 5 7 9
2 5
3 7
4 4
1
5
```

Saída esperada:

```text
SIM
SIM
NAO
9 7 5 3 1
```

## 📈Análise de desempenho

O arquivo `medidor_tempo.c` executa cada algoritmo 100 vezes para vetores de tamanhos `1000`, `50000`, `100000`, `500000` e `1000000`. Ele mede o tempo médio em nanossegundos e contabiliza atribuições e comparações.

Para gerar o CSV, compile o medidor e execute-o dentro da pasta `output`, pois o arquivo `resultados.csv` é criado no diretório atual:

```bash
gcc -std=c99 -Wall -Wextra -o output/medidor_tempo medidor_tempo.c
cd output
./medidor_tempo
```

Nas implementações de busca, os testes do medidor usam o pior caso: o valor buscado não pertence ao vetor.

## 📊Gráficos

O script `output/graficos.py` lê `resultados.csv` e salva gráficos PNG em `output/graficos/`.

Pré-requisitos:

```bash
python3 -m pip install pandas matplotlib
```

Execução, a partir de `output`:

```bash
python3 graficos.py
```

Os arquivos gerados incluem gráficos individuais de tempo para cada algoritmo e gráficos comparativos de tempo médio, atribuições e comparações.

## 🛠️Estrutura

```text
projeto_1_ED1/
├── prj_1.c             # Programa interativo
├── medidor_tempo.c     # Benchmark dos algoritmos
└── output/
    ├── resultados.csv  # Dados obtidos pelo benchmark
    ├── graficos.py     # Geração dos gráficos
    └── graficos/       # Imagens geradas
```
