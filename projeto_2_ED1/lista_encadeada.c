
#include <stdlib.h>
#include "ListaComCursor.h"

typedef struct node {
    int data;
    struct node *next;
    struct node *previous;
} Node;

struct lista_cursor {
    int qtd;
    Node *head;
    Node *cursor;
    Node *tail;
};

ListaCursor *criar_lista(void) {
    ListaCursor *p = malloc(sizeof(ListaCursor));

    if (!p) {
        return NULL;
    }

    p->qtd = 0;
    p->head = NULL;
    p->cursor = NULL;
    p->tail = NULL;

    return p;
}

void destruir_lista(ListaCursor *l) {
    if (!l) {
        return;
    }

    Node *temp = l->head;

    while (temp != NULL) {
        Node *rem = temp;
        temp = temp->next;
        free(rem);
    }

    free(l);
}

void mover_inicio(ListaCursor *l) {
    if (l->qtd == 0) {
        return;
    }

    l->cursor = l->head;
}

void mover_fim(ListaCursor *l) {
    if (l->qtd == 0) {
        return;
    }

    l->cursor = l->tail;
}

int avancar_cursor(ListaCursor *l) {
    if (l->qtd == 0 || l->cursor->next == NULL) {
        return 0;
    }

    l->cursor = l->cursor->next;
    return 1;
}

int retroceder_cursor(ListaCursor *l) {
    if (l->qtd == 0 || l->cursor->previous == NULL) {
        return 0;
    }

    l->cursor = l->cursor->previous;
    return 1;
}

int mover_para_indice(ListaCursor *l, int index) {
    if (index < 0 || index >= l->qtd) {
        return 0;
    }

    Node *p;

    if (index < l->qtd / 2) {
        p = l->head;

        for (int i = 0; i < index; i++) {
            p = p->next;
        }
    }
    else {
        p = l->tail;

        for (int i = l->qtd - 1; i > index; i--) {
            p = p->previous;
        }
    }

    l->cursor = p;
    return 1;
}

int obter_atual(ListaCursor *l, int *valor_saida) {
    if (l->qtd == 0 || valor_saida == NULL) {
        return 0;
    }

    *valor_saida = l->cursor->data;
    return 1;
}

int inserir_antes(ListaCursor *l, int valor) {
    Node *new = malloc(sizeof(Node));

    if (!new) {
        return 0;
    }

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

    l->qtd++;
    return 1;
}

int inserir_depois(ListaCursor *l, int valor) {
    Node *new = malloc(sizeof(Node));

    if (!new) {
        return 0;
    }

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

    l->qtd++;
    return 1;
}

int remover_atual(ListaCursor *l) {
    if (l->qtd == 0) {
        return 0;
    }

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

    free(rem);
    l->qtd--;

    return 1;
}

int tamanho_lista(ListaCursor *l) {
    return l->qtd;
}
