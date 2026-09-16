import pandas as pd
import matplotlib.pyplot as plt
import os

# --- CONFIGURAÇÕES ---
ARQUIVO_CSV = "resultados.csv"
PASTA_GRAFICOS = "graficos"

cores = {
    "Inversao": "red",
    "Busca Sequencial": "blue",
    "Busca Binaria Recursiva": "orange",
    "Busca Binaria Iterativa": "green"
}

nomes_graficos = {
    "Inversao": "Inversão de Vetor",
    "Busca Sequencial": "Busca Sequencial",
    "Busca Binaria Recursiva": "Busca Binária Recursiva",
    "Busca Binaria Iterativa": "Busca Binária Iterativa"
}

# --- GRÁFICOS ---
def plotar_graficos():
    dados = pd.read_csv(ARQUIVO_CSV) #Lê os dados do resultados.csv

    os.makedirs(PASTA_GRAFICOS, exist_ok=True) #Cria a pasta para os gráficos caso ela não exista

    for algoritmo in dados["Algoritmo"].unique(): #Percorre todos os algoritmos presentes no arquivo CSV

        dados_algoritmo = dados[dados["Algoritmo"] == algoritmo] #Seleciona somente os dados do algoritmo atual

        plt.figure(figsize=(14, 6)) #Define o tamanho do gráfico

        plt.plot(
            dados_algoritmo["N"],
            dados_algoritmo["TempoMedio"],
            marker="o",
            linestyle="-",
            color=cores[algoritmo]
        )

        plt.title(f"Tempo de Execução - {nomes_graficos[algoritmo]}")
        plt.xlabel("Tamanho do vetor (N)")
        plt.ylabel("Tempo médio (nanosegundos)")

        plt.xticks(dados_algoritmo["N"])
        plt.ticklabel_format(style="plain", axis="x")

        # Escala logarítmica no eixo Y
        escalas_y = {
            "Inversao": [1000, 10000, 100000, 1000000],
            "Busca Sequencial": [1000, 10000, 100000, 1000000],
            "Busca Binaria Recursiva": [60, 70, 80, 90, 100],
            "Busca Binaria Iterativa": [50, 60]
        }
        
        plt.yscale("log")
        plt.yticks(escalas_y[algoritmo])
        plt.grid(True, which="major", axis="both", linestyle="--") #Adiciona linhas de grade nos valores principais dos eixos X e Y

        plt.tight_layout()

# --- GERAÇÃO DO ARQUIVO ---
        nome_arquivo = f"grafico_{algoritmo.lower().replace(' ', '_')}.png"
        caminho = os.path.join(PASTA_GRAFICOS, nome_arquivo)

        plt.savefig(caminho)
        plt.show()

        print(f"Gráfico salvo como '{caminho}'")


if __name__ == "__main__":
    plotar_graficos()