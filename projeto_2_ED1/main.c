
#include <stdio.h>
#include <stdlib.h>
#include "ListaComCursor.h"

int main(void) {
    ListaCursor *lista = criar_lista();

    if (lista == NULL) {
        return 1;
    }

    int opcao, valor;

    while (scanf("%d", &opcao) == 1) {
        switch (opcao) {
            case 0:
                destruir_lista(lista);
                return 0;

            case 1:
                if (scanf("%d", &valor) != 1) {
                    destruir_lista(lista);
                    return 0;
                }
                inserir_antes(lista, valor);
                break;

            case 2:
                if (scanf("%d", &valor) != 1) {
                    destruir_lista(lista);
                    return 0;
                }
                inserir_depois(lista, valor);
                break;

            case 3:
                remover_atual(lista);
                break;

            case 4:
                if (obter_atual(lista, &valor)) {
                    printf("%d\n", valor);
                } else {
                    printf("ERRO\n");
                }
                break;

            case 5:
                mover_inicio(lista);
                break;

            case 6:
                mover_fim(lista);
                break;

            case 7:
                printf("%d\n", avancar_cursor(lista));
                break;

            case 8:
                printf("%d\n", retroceder_cursor(lista));
                break;

            case 9:
                if (scanf("%d", &valor) != 1) {
                    destruir_lista(lista);
                    return 0;
                }
                printf("%d\n", mover_para_indice(lista, valor));
                break;

            case 10:
                printf("%d\n", tamanho_lista(lista));
                break;
        }
    }

    destruir_lista(lista);
    return 0;
}
