#include "stdlib.h"
#include "ListaComCursor.h"

struct lista_cursor{
    int *dados;
    int qtd;
    int cap;
    int cursor;
};

static int garantir_capacidade(ListaCursor *l){
    if(!l){
        return 0;
    }
    if (l->cap>l->qtd){
        return 1;
    }
    if(l->cap==0){
        l->dados=malloc(8*sizeof(int));
        if(!l->dados){
            return 0;
        }
        l->cap=8;
        return 1;
    }
    else{
        int novocap=2*l->cap;
        int *novo=realloc(l->dados,sizeof(int)*novocap);
        if(!novo){
            return 0;
        }
        l->dados=novo;
        l->cap=novocap;
        return 1;
    }
}

ListaCursor *criar_lista(){
    ListaCursor *l=malloc(sizeof(ListaCursor));
    if(!l){
        return NULL;
    }
    l->cap=0;
    l->cursor=-1;
    l->qtd=0;
    l->dados=NULL;
    return l;
}

void destruir_lista(ListaCursor *l){
    if(!l){
        return;
    }
    free(l->dados);
    free(l);
    return;
}

void mover_inicio(ListaCursor *l){
    if(l->qtd>0){
        l->cursor=0;
        return;
    }
}

void mover_fim(ListaCursor *l){
    if(l->cursor>=0){
        l->cursor=l->qtd-1;
        return;
    }
}

int avancar_cursor(ListaCursor *l){
    if(!l){
        return 0;
    }
    if((l->cursor==l->qtd-1) || (l->qtd==0)){
        return 0;
    }
    l->cursor++;
    return 1;
}

int retroceder_cursor(ListaCursor *l){
    if(!l){
        return 0;
    }
    if((l->cursor==0) || (l->qtd==0)){
        return 0;
    }
    l->cursor--;
    return 1;
}

int mover_para_indice(ListaCursor *l, int indice){
    if(!l){
        return 0;
    }
    if((indice<0) || (indice>=l->qtd)){
        return 0;
    }
    l->cursor=indice;
    return 1;
}

int obter_atual(ListaCursor *l, int *valor_saida){
    if((!l) || (valor_saida==NULL)){
        return 0;
    }
    if(l->qtd==0){
        return 0;
    }
    *valor_saida=l->dados[l->cursor];
    return 1;
}

int inserir_antes(ListaCursor *l, int valor){
    if(!l){
        return 0;
    }
    if(garantir_capacidade(l)){
        if(l->qtd==0){
            l->dados[0]=valor;
            l->qtd++;
            l->cursor=0;
            return 1;
        }
        else{
            for(int i=l->qtd; i>l->cursor; i--){
                l->dados[i]=l->dados[i-1];
            }
            l->dados[l->cursor]=valor;
            l->qtd++;
            l->cursor++;
            return 1;
        }
    }
    return 0;
}

int inserir_depois(ListaCursor *l, int valor){
    if(!l){
        return 0;
    }
    if(garantir_capacidade(l)){
        if(l->qtd==0){
            l->dados[0]=valor;
            l->qtd++;
            l->cursor=0;
            return 1;
        }
        else{
            for(int i=l->qtd; i>l->cursor+1; i--){
                l->dados[i]=l->dados[i-1];
            }
            l->dados[l->cursor+1]=valor;
            l->qtd++;
            return 1;
        }
    }
    return 0;
}

int remover_atual(ListaCursor *l) {
    if ((!l) || (l->qtd == 0)) {
        return 0;
    }
    for (int i = l->cursor; i < l->qtd - 1; i++) {
        l->dados[i] = l->dados[i + 1];
    }
    l->qtd--;
    if (l->qtd == 0) {
        l->cursor = -1;
    }
    else if (l->cursor >= l->qtd) {
        l->cursor--;
    }
    return 1;
}

int tamanho_lista(ListaCursor *l){
    if(!l){
        return 0;
    }
    return l->qtd;
}