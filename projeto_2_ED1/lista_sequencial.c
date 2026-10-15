#include "stdlib.h"
#include "ListaComCursor.h"


/*
Trabalho realizado por:
    Pedro Artur Barberá Sarni (17918418)
    Breno de Sousa Cad (17881714)
    Thales Amaral Gontijo (17925010)
    Murilo Franciscato Ataide (17916308)
*/

struct lista_cursor{
    int *dados; //iremos fazer malloc depois
    int qtd;
    int cap;
    int cursor;
};

//Função para garantir que tem espaço o suficiente no vetor, se não tiver, ele realoca o vetor ocm o dobro de espaço
static int garantir_capacidade(ListaCursor *l){
    if(!l){
        //Falha na inicialização
        return 0;
    }
    //Verifica se tem espaço
    if (l->cap>l->qtd){
        //Se tiver espaço, não precisa fazer nada
        return 1;
    }
    //Vamos alocar o espaço necessário
    if(l->cap==0){
        //Caso o vetor não tenha sido alocado ainda
        l->dados=malloc(8*sizeof(int));
        if(!l->dados){
            //Erro na alocação
            return 0;
        }
        //Se a alocação due certo, atualizamos a capacidade
        l->cap=8;
        return 1;
    }
    //Caso o vetor esteja lotado, vamos dobrar sua capacidade
    else{
        int novocap=2*l->cap;
        //Alocação de segurança, caso ela falhe, não perdemos nosso vetor atual
        int *novo=realloc(l->dados,sizeof(int)*novocap);
        if(!novo){
            //Falha na realocação
            return 0;
        }
        //Atualiza o vetor e a nova capacidade
        l->dados=novo;
        l->cap=novocap;
        return 1;
    }
}

ListaCursor *criar_lista(){
    ListaCursor *l=malloc(sizeof(ListaCursor));
    if(!l){
        //Falha na alocação do vetor
        return NULL;
    }
    l->cap=0;
    l->cursor=-1; //Como a lista esta vázia, inválidamos o ponteiro
    l->qtd=0;
    l->dados=NULL; //Não iremos alocar o vetor ainda
    return l;
}

void destruir_lista(ListaCursor *l){
    if(!l){
        //Falha na inicialização
        return;
    }
    //Desaloca o vetor de dados
    free(l->dados);
    //Desaloca a struct
    free(l);
    return;
}

void mover_inicio(ListaCursor *l){
    //Verifica se o vetor não esta vazio
    if(l->qtd>0){
        l->cursor=0;
        return;
    }
}

void mover_fim(ListaCursor *l){
    //Verifica se o vetor não esta vazio
    if(l->qtd>0){
        l->cursor=l->qtd-1;
        return;
    }
}

int avancar_cursor(ListaCursor *l){
    if(!l){
        //Falha na inicialização do vetor
        return 0;
    }
    //Verifica se o cursor não esta na ultima posição ou se o vetor está vazio
    if((l->cursor==l->qtd-1) || (l->qtd==0)){
        return 0;
    }
    //Move o cursor para o proximo elemento
    l->cursor++;
    return 1;
}

int retroceder_cursor(ListaCursor *l){
    if(!l){
        //Falha na inicialização do vetor
        return 0;
    }
    //Verifica se o cursor não esta na última posição ou se o vetor esta vazio
    if((l->cursor==0) || (l->qtd==0)){
        return 0;
    }
    //Move o cursor para o elemento anterior
    l->cursor--;
    return 1;
}

int mover_para_indice(ListaCursor *l, int indice){
    if(!l){
        //Falha na inicialização
        return 0;
    }
    //Verifica se o índice escolhido é válido
    //Índice valido <=> 0 <= indice <= l->qtd-1
    if((indice<0) || (indice>=l->qtd)){
        return 0;
    }
    //Move o cursor para o índice escolhido
    l->cursor=indice;
    return 1;
}

int obter_atual(ListaCursor *l, int *valor_saida){
    if((!l) || (valor_saida==NULL)){
        //Falha na alocação ou ponteiro de saída nulo
        return 0;
    }
    if(l->qtd==0){
        //Se o vetor esta vazio o curso aponta para ninguém
        return 0;
    }
    //Retorna o elemento apontado pelo cursor
    *valor_saida=l->dados[l->cursor];
    return 1;
}

int inserir_antes(ListaCursor *l, int valor){
    if(!l){
        //Falha na inicialização do vetor
        return 0;
    }
    //verifica se o vetor tem espaço
    if(garantir_capacidade(l)){
        //Lista vazia: insere o primeiro elemento e inicializa o cursor
        if(l->qtd==0){
            l->dados[0]=valor;
            l->qtd++;
            l->cursor=0;
            return 1;
        }
        else{
            //Desloca os elementos à direita para abrir espaço na posição do cursor
            for(int i=l->qtd; i>l->cursor; i--){
                l->dados[i]=l->dados[i-1];
            }
            //Insere o elemento antes do atual e mantém o cursor no elemento original
            l->dados[l->cursor]=valor;
            l->qtd++;
            l->cursor++;
            return 1;
        }
    }
    //Falha ao garantir capacidade para a inserção
    return 0;
}

int inserir_depois(ListaCursor *l, int valor){
    if(!l){
        //Falha na inicialização
        return 0;
    }
    //Verifica se há capacidade para inserir um novo elemento
    if(garantir_capacidade(l)){
        //Lista vazia: insere o primeiro elemento e inicializa o cursor
        if(l->qtd==0){
            l->dados[0]=valor;
            l->qtd++;
            l->cursor=0;
            return 1;
        }
        //Desloca os elementos à direita para abrir espaço após o cursor
        else{
            for(int i=l->qtd; i>l->cursor+1; i--){
                l->dados[i]=l->dados[i-1];
            }
            //Insere o elemento após o atual, mantendo o cursor na posição original
            l->dados[l->cursor+1]=valor;
            l->qtd++;
            return 1;
        }
    }
    //Falha ao garantir capacidade para a inserção
    return 0;
}

int remover_atual(ListaCursor *l) {
    if ((!l) || (l->qtd == 0)) {
        //Lista não inicializada ou vazia
        return 0;
    }
    //Desloca os elementos à esquerda para sobrescrever o elemento atual
    for (int i = l->cursor; i < l->qtd - 1; i++) {
        l->dados[i] = l->dados[i + 1];
    }
    //Atualiza a quantidade de elementos da lista
    l->qtd--;
    if (l->qtd == 0) {
        //Lista ficou vazia: invalida o cursor
        l->cursor = -1;
    }
    else if (l->cursor >= l->qtd) {
        //Se o último elemento foi removido, move o cursor para o anterior
        l->cursor--;
    }
    return 1;
}

int tamanho_lista(ListaCursor *l){
    if(!l){
        //Falha na incialização do vetor
        return 0;
    }
    //Retorna a quantidade de elementos no vetor
    return l->qtd;
}