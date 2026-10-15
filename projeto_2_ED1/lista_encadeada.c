
#include <stdlib.h>
#include "ListaComCursor.h"

//Estrutura que representa cada nó da lista duplamente encadeada
typedef struct node {
    int data;
    struct node *next;
    struct node *previous;
} Node;

//Estrutura que armazena as informações da lista e o cursor atual
struct lista_cursor {
    int qtd;
    Node *head;
    Node *cursor;
    Node *tail;
};

//Função para criar e inicializar uma lista vazia
ListaCursor *criar_lista(void) {
    //Aloca o espaço necessário para a estrutura da lista
    ListaCursor *p = malloc(sizeof(ListaCursor));

    if (!p) {
        //Falha na alocação da lista
        return NULL;
    }

    //Inicializa a quantidade e todos os ponteiros da lista
    p->qtd = 0;
    p->head = NULL;
    p->cursor = NULL;
    p->tail = NULL;

    return p;
}

void destruir_lista(ListaCursor *l) {
    if (!l) {
        //Falha na inicialização
        return;
    }

    //Começa a percorrer a lista a partir do primeiro nó
    Node *temp = l->head;

    while (temp != NULL) {
        //Guarda o nó atual antes de avançar para o próximo
        Node *rem = temp;
        temp = temp->next;
        //Desaloca o nó guardado
        free(rem);
    }

    //Desaloca a estrutura da lista
    free(l);
}

void mover_inicio(ListaCursor *l) {
    //Verifica se a lista esta vazia
    if (l->qtd == 0) {
        return;
    }

    //Faz o cursor apontar para o início da lista
    l->cursor = l->head;
}

void mover_fim(ListaCursor *l) {
    //Verifica se a lista esta vazia
    if (l->qtd == 0) {
        return;
    }

    //Faz o cursor apontar para o final da lista
    l->cursor = l->tail;
}

int avancar_cursor(ListaCursor *l) {
    //Verifica se a lista esta vazia ou se o cursor já esta no final
    if (l->qtd == 0 || l->cursor->next == NULL) {
        return 0;
    }

    //Move o cursor para o próximo nó
    l->cursor = l->cursor->next;
    return 1;
}

int retroceder_cursor(ListaCursor *l) {
    //Verifica se a lista esta vazia ou se o cursor já esta no início
    if (l->qtd == 0 || l->cursor->previous == NULL) {
        return 0;
    }

    //Move o cursor para o nó anterior
    l->cursor = l->cursor->previous;
    return 1;
}

int mover_para_indice(ListaCursor *l, int index) {
    //Verifica se o índice escolhido é válido
    //Índice valido <=> 0 <= index <= l->qtd-1
    if (index < 0 || index >= l->qtd) {
        return 0;
    }

    //Ponteiro auxiliar usado para percorrer a lista
    Node *p;

    //Se o índice esta na primeira metade, percorre a partir do início
    if (index < l->qtd / 2) {
        p = l->head;

        for (int i = 0; i < index; i++) {
            p = p->next;
        }
    }
    //Se o índice esta na segunda metade, percorre a partir do final
    else {
        p = l->tail;

        for (int i = l->qtd - 1; i > index; i--) {
            p = p->previous;
        }
    }

    //Move o cursor para o nó encontrado
    l->cursor = p;
    return 1;
}

int obter_atual(ListaCursor *l, int *valor_saida) {
    //Verifica se a lista esta vazia ou se o ponteiro de saída é nulo
    if (l->qtd == 0 || valor_saida == NULL) {
        return 0;
    }

    //Retorna o valor armazenado no nó apontado pelo cursor
    *valor_saida = l->cursor->data;
    return 1;
}

int inserir_antes(ListaCursor *l, int valor) {
    //Aloca o espaço necessário para o novo nó
    Node *new = malloc(sizeof(Node));

    if (!new) {
        //Falha na alocação do novo nó
        return 0;
    }

    //Armazena o valor no novo nó
    new->data = valor;

    // Lista vazia
    if (l->qtd == 0) {
        new->next = NULL;
        new->previous = NULL;

        l->head = new;
        l->tail = new;
        l->cursor = new;
    }
    else {
        // Novo nó fica antes do cursor
        new->previous = l->cursor->previous;
        new->next = l->cursor;

        if (l->cursor->previous == NULL) {
            // Inserção no início
            l->head = new;
        }
        else {
            // Inserção no meio
            l->cursor->previous->next = new;
        }

        l->cursor->previous = new;
    }

    //Atualiza a quantidade de elementos da lista
    l->qtd++;
    return 1;
}

int inserir_depois(ListaCursor *l, int valor) {
    //Aloca o espaço necessário para o novo nó
    Node *new = malloc(sizeof(Node));

    if (!new) {
        //Falha na alocação do novo nó
        return 0;
    }

    //Armazena o valor no novo nó
    new->data = valor;

    // Lista vazia
    if (l->qtd == 0) {
        new->next = NULL;
        new->previous = NULL;

        l->head = new;
        l->tail = new;
        l->cursor = new;
    }
    else {
        // Novo nó fica depois do cursor
        new->previous = l->cursor;
        new->next = l->cursor->next;

        if (l->cursor->next == NULL) {
            // Inserção no final
            l->tail = new;
        }
        else {
            // Inserção no meio
            l->cursor->next->previous = new;
        }

        l->cursor->next = new;
    }

    //Atualiza a quantidade de elementos da lista
    l->qtd++;
    return 1;
}

int remover_atual(ListaCursor *l) {
    //Verifica se a lista esta vazia
    if (l->qtd == 0) {
        return 0;
    }

    //Guarda o nó atual para ajustar os ponteiros e desalocá-lo
    Node *rem = l->cursor;

    // Ajusta o ponteiro do nó anterior
    if (rem->previous == NULL) {
        l->head = rem->next;
    }
    else {
        rem->previous->next = rem->next;
    }

    // Ajusta o ponteiro do próximo nó
    if (rem->next == NULL) {
        l->tail = rem->previous;
    }
    else {
        rem->next->previous = rem->previous;
    }

    // Cursor vai para o sucessor, se existir;
    // caso contrário, vai para o predecessor.
    if (rem->next != NULL) {
        l->cursor = rem->next;
    }
    else {
        l->cursor = rem->previous;
    }

    //Desaloca o nó removido e atualiza a quantidade de elementos
    free(rem);
    l->qtd--;

    return 1;
}

int tamanho_lista(ListaCursor *l) {
    //Retorna a quantidade de elementos
    return l->qtd;
}
