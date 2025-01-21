#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 10000

// armazenar dados
typedef struct Registro {
    char nome_completo[100];
    char cargo[100];
    char uorg_lotacao[100];
} Registro;

// nó da árvore binária de busca
typedef struct Nodo {
    Registro dado;
    struct Nodo *esquerda, *direita;
} Nodo;

// função para criar um novo nó da árvore
Nodo* criar_nodo(Registro registro) {
    Nodo *novo = (Nodo*)malloc(sizeof(Nodo));
    if (!novo) {
        fprintf(stderr, "Erro ao alocar memória!\n");
        exit(EXIT_FAILURE);
    }
    novo->dado = registro;
    novo->esquerda = novo->direita = NULL;
    return novo;
}

// registro no arvore
Nodo* inserir(Nodo *raiz, Registro registro) {
    if (raiz == NULL) 
        return criar_nodo(registro);

    if (strcmp(registro.nome_completo, raiz->dado.nome_completo) < 0)
        raiz->esquerda = inserir(raiz->esquerda, registro);
    else if (strcmp(registro.nome_completo, raiz->dado.nome_completo) > 0)
        raiz->direita = inserir(raiz->direita, registro);

    return raiz;
}

// nome na arvore
Nodo* buscar(Nodo *raiz, const char *nome_completo) {
    if (raiz == NULL || strcmp(raiz->dado.nome_completo, nome_completo) == 0)
        return raiz;

    if (strcmp(nome_completo, raiz->dado.nome_completo) < 0)
        return buscar(raiz->esquerda, nome_completo);
    else
        return buscar(raiz->direita, nome_completo);
}

// carregar dados csv na arvore
Nodo* carregar_csv(const char *nome_arquivo, Nodo *raiz) {
    FILE *arquivo = fopen(nome_arquivo, "r");
    if (!arquivo) {
        fprintf(stderr, "Erro ao abrir o arquivo CSV!\n");
        exit(EXIT_FAILURE);
    }

    char linha[MAX_LINE];

    // ignorar cabeçalho
    if (fgets(linha, MAX_LINE, arquivo) == NULL) {
        fprintf(stderr, "Erro: Arquivo CSV está vazio ou mal formatado!\n");
        fclose(arquivo);
        exit(EXIT_FAILURE);
    }

    while (fgets(linha, MAX_LINE, arquivo)) {
        Registro registro = { "", "", "" };
        char *token;

        // extrair os campos relevantes
        token = strtok(linha, ";");
        int coluna = 0;

        while (token != NULL) {
            // tirar as aspas
            if (token[0] == '"') token++;

            size_t len = strlen(token);
            if (token[len - 1] == '"') token[len - 1] = '\0';

            switch (coluna) {
                case 1: 
                    strncpy(registro.nome_completo, token, sizeof(registro.nome_completo) - 1);
                    break;
                case 4: 
                    strncpy(registro.cargo, token, sizeof(registro.cargo) - 1);
                    break;
                case 16: 
                    strncpy(registro.uorg_lotacao, token, sizeof(registro.uorg_lotacao) - 1);
                    break;
            }

            coluna++;
            token = strtok(NULL, ";");
        }

        // ver se registro é válido
        if (strlen(registro.nome_completo) > 0) {
            raiz = inserir(raiz, registro);
        }
    }

    fclose(arquivo);
    return raiz;
}

// liberar memoria na arvore
void liberar_arvore(Nodo *raiz) {
    if (raiz == NULL) return;
    liberar_arvore(raiz->esquerda);
    liberar_arvore(raiz->direita);
    free(raiz);
}

// função principal
int main() {
    Nodo *raiz = NULL;
    char nome_arquivo[] = "dados.csv";

    // carregar csv
    raiz = carregar_csv(nome_arquivo, raiz);

    // pedir nome pra buscar
    char nome_completo[100];
    printf("Digite o nome completo para buscar: ");
    if (fgets(nome_completo, sizeof(nome_completo), stdin) == NULL) {
        fprintf(stderr, "Erro ao ler o nome!\n");
        liberar_arvore(raiz);
        exit(EXIT_FAILURE);
    }
    nome_completo[strcspn(nome_completo, "\n")] = '\0'; // Remover quebra de linha

    // busca na arvore
    Nodo *resultado = buscar(raiz, nome_completo);
    if (resultado) {
        printf("Cargo: %s\n", resultado->dado.cargo);
        printf("UORG Lotacao: %s\n", resultado->dado.uorg_lotacao);
    } else {
        printf("Registro nao encontrado!\n");
    }

    // liberar memoria
    liberar_arvore(raiz);
    return 0;
}
