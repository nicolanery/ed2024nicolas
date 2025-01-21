#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEN 10000

// armazenar dados
typedef struct Pessoa {
    char nome[MAX_LEN];
    char cargo[MAX_LEN];
    char uorgLotacao[MAX_LEN];
    struct Pessoa* esquerda;
    struct Pessoa* direita;
} Pessoa;

// criar n[o
Pessoa* criarNo(char* nome, char* cargo, char* uorgLotacao) {
    Pessoa* novaPessoa = (Pessoa*)malloc(sizeof(Pessoa));
    strcpy(novaPessoa->nome, nome);
    strcpy(novaPessoa->cargo, cargo);
    strcpy(novaPessoa->uorgLotacao, uorgLotacao);
    novaPessoa->esquerda = novaPessoa->direita = NULL;
    return novaPessoa;
}

// inserir pessoa na arvore de busca
Pessoa* inserir(Pessoa* raiz, Pessoa* novaPessoa) {
    if (raiz == NULL) {
        return novaPessoa;
    }
    if (strcmp(novaPessoa->nome, raiz->nome) < 0) {
        raiz->esquerda = inserir(raiz->esquerda, novaPessoa);
    } else if (strcmp(novaPessoa->nome, raiz->nome) > 0) {
        raiz->direita = inserir(raiz->direita, novaPessoa);
    }
    return raiz;
}

// buscar nome
Pessoa* buscar(Pessoa* raiz, char* nome) {
    if (raiz == NULL) {
        return NULL;
    }
    if (strcmp(nome, raiz->nome) == 0) {
        return raiz;
    }
    if (strcmp(nome, raiz->nome) < 0) {
        return buscar(raiz->esquerda, nome);
    } else {
        return buscar(raiz->direita, nome);
    }
}

// ler csv e inserir na arvore
void lerCSV(Pessoa** raiz) {
    FILE* arquivo = fopen("cadastro.csv", "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    char linha[MAX_LEN];
    char nome[MAX_LEN], cargo[MAX_LEN], uorgLotacao[MAX_LEN];

    // ignorar o cabeçalho
    fgets(linha, sizeof(linha), arquivo);

    while (fgets(linha, sizeof(linha), arquivo)) {
        // remover aspas
        linha[strcspn(linha, "\n")] = '\0';

        // ler dados
        sscanf(linha, "%[^,],%[^,],%[^\n]", nome, cargo, uorgLotacao);

        // inserir nova pessoa na arvore
        Pessoa* novaPessoa = criarNo(nome, cargo, uorgLotacao);
        *raiz = inserir(*raiz, novaPessoa);
    }

    fclose(arquivo);
}

int main() {
    Pessoa* raiz = NULL;
    char nomeBusca[MAX_LEN];

    // ler csv e inserir dados na arvore
    lerCSV(&raiz);

    //  nome para busca
    printf("Entre com nome para busca: ");
    fgets(nomeBusca, sizeof(nomeBusca), stdin);
    nomeBusca[strcspn(nomeBusca, "\n")] = '\0';  // tirar a quebra de linha

    // buscar  pessoa na árvore
    Pessoa* pessoaEncontrada = buscar(raiz, nomeBusca);

    // exibir resultados
    if (pessoaEncontrada != NULL) {
        printf("\nNome: %s\n", pessoaEncontrada->nome);
        printf("Cargo: %s\n", pessoaEncontrada->cargo);
        printf("UORG Lotação: %s\n", pessoaEncontrada->uorgLotacao);
    } else {
        printf("\nPessoa não encontrada.\n");
    }

    return 0;
}
