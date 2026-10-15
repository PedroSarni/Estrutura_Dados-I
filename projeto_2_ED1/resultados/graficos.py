import pandas as pd
import matplotlib.pyplot as plt
import os

"""
Trabalho realizado por:
    Pedro Artur Barberá Sarni (17918418)
    Breno de Sousa Cad (17881714)
    Thales Amaral Gontijo (17925010)
    Murilo Franciscato Ataide (17916308)
"""

# --- CONFIGURAÇÕES ---
ARQUIVOS_CSV = {
    "Encadeada": "medias_encadeada.csv",
    "Sequencial": "medias_sequencial.csv"
} #Associa as implementações aos seus arquivos CSV

PASTA_GRAFICOS = "graficos" 

cores = {
    "Encadeada": "red",
    "Sequencial": "blue"
}

nomes_graficos = {
    "insercoes_inicio": "Inserção no início: LS × LDE",
    "acessos_aleatorios": "Acesso por índice aleatório: LS × LDE",
    "remocoes_inicio": "Remoção no início: LS × LDE",
    "remocoes_meio": "Remoção no meio: LS × LDE",
    "percurso_completo": "Percurso completo: LS × LDE"
}

dados = {
    nome: pd.read_csv(arquivo)
    for nome, arquivo in ARQUIVOS_CSV.items()
} #Lê os dados dos arquivos .csv

# --- GRÁFICOS ---
def plotar_graficos():

    os.makedirs(PASTA_GRAFICOS, exist_ok=True) #Cria a pasta caso não exista

    for experimento, titulo in nomes_graficos.items(): #Percorre os experimentos

        plt.figure(figsize=(14, 6))

        for implementacao, df in dados.items(): #Percorre os dados das duas listas

            dados_experimento = df[
                df["Experimento"] == experimento
            ].sort_values("N") #Seleciona os dados do experimento atual e ordena pelo tamanho da lista

            plt.plot(
                dados_experimento["N"],
                dados_experimento["TempoMedioNs"],
                marker="o",
                linestyle="-",
                color=cores[implementacao],
                label=implementacao
            ) #Plota os tempos médios da implementação atual no mesmo gráfico, gerando o gráfico comparativo

        plt.title(titulo)
        plt.xlabel("Tamanho da lista (N)")
        plt.xticks(dados_experimento["N"].unique()) #Exibe os valores de N utilizados nos testes
        plt.ticklabel_format(style="plain", axis="x") #Evita a notação científica
        plt.ylabel("Tempo médio (ns)")
        plt.yscale("log") # Escala logarítmica no eixo Y
        plt.legend()
        plt.grid(True)

        # --- GERAÇÃO DO ARQUIVO ---
        nome_arquivo = f"grafico_{experimento.lower().replace(' ', '_')}.png"
        caminho = os.path.join(PASTA_GRAFICOS, nome_arquivo) #Define o caminho do arquivo

        plt.savefig(caminho)
        plt.show()

        print(f"Gráfico salvo como '{caminho}'")


# --- CHAMADA DA FUNÇÃO ---
if __name__ == "__main__":
    plotar_graficos()