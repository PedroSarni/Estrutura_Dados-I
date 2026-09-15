#include <stdio.h>
#include <stdlib.h>

void exibicao(int *arr, int n){
    for (int i=0; i< n; i++){
        printf("%d ", arr[i]);
    }
    printf("\n");
}


void bin_rec(int arr[], int l, int r, int target){
    if(l>r){
        //caso não encontre o valor target
        printf("NAO\n");
        return ;
    }
    int meio=l+(r-l)/2;
    if(arr[meio]==target){
        printf("SIM\n");
        return;
    }
    if(arr[meio]<target){
        return bin_rec(arr, meio+1, r, target);
    }
    if(arr[meio]>target){
        return bin_rec(arr, l, meio-1, target);
    }
}
void bin_int(int arr[], int l, int r, int target){
    while(l<=r){
        int meio=l+(r-l)/2;

        if(arr[meio]==target){
            printf("SIM\n");
            return;
        }

        if (arr[meio]<target){
            l=meio+1;
        }

        if(arr[meio]>target){
            r=meio-1;
        }
    }
    //caso finalize o loop, significa que o valor target não foi encontrado
    printf("NAO\n");
    return;
}

void inverter_arr(int arr[] , int n){
    int *arr_temp=malloc(n*sizeof(int));//aloca um vetor temporario
    //for para copiar o vetor original para um vetor temporário
    for(int i = 0; i < n ; i++){
            arr_temp[i] = arr[i];
        }
    //ponteiro no inicio do vetor
    int l=0;
    //ponteiro no final do vetor
    int r=n-1;
    //variavel temporaria para facilitar a troca
    int temp;
    while(l<r){
        temp=arr_temp[l];
        arr_temp[l]=arr_temp[r];
        arr_temp[r]=temp;
        //convergência dos ponteiros para o meio do vetor
        l++;
        r--;
    }
    exibicao(arr_temp, n);
    return;
}

void busca_sequencial(int arr[], int n, int target){
    for (int i=0; i< n; i++){
        if (arr[i]==target){
            printf("SIM\n");
            return;
        }
    }
    //se finzalizou o loop e não achou é pq o target nn consta no vetor
    printf("NAO\n");
    return;
}

int main(){
    //Criação do vetor com n entradas;
    int n;
    scanf("%d", &n);
    int arr[n];
    int target;

    for (int i=0; i<n; i++){
        scanf("%d", &arr[i]);
    }


    int escolha=0;
    while(escolha!=5){
        scanf("%d", &escolha);
        switch (escolha)
        {
        case 1:
            inverter_arr(arr,n);
            break;
        case 2:
            scanf("%d", &target);
            busca_sequencial(arr, n, target);
            break;
        case 3:
            scanf("%d", &target);
            bin_int(arr, 0, n-1, target);
            break;

        case 4:
            scanf("%d", &target);
            bin_rec(arr, 0, n-1, target);
            break;

        case 5:
            break;

        default:
            printf("NAO");
            break;
        }
    }

    return 0;
}