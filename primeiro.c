#include <stdio.h>
#include <stdlib.h>
#include <math.h>


// Função para ordenar o array (usando ordenação por seleção)
void ordenar(double *valores, int n) {
    for (int i = 0; i < n - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < n; j++) {
            if (valores[j] < valores[min_idx]) {
                min_idx = j;
            }
        }
        // Troca os elementos
        double temp = valores[min_idx];
        valores[min_idx] = valores[i];
        valores[i] = temp;
    }
}

// Função para calcular a média
double calcular_media(double *valores, int n) {
    double soma = 0.0;
    for (int i = 0; i < n; i++) {
        soma += valores[i];
    }
    return soma / n;
}

// Função para calcular o desvio padrão
double calcular_desvio_padrao(double *valores, int n, double media) {
    double soma_diferencas = 0.0;
    for (int i = 0; i < n; i++) {
        soma_diferencas += pow((valores[i] - media), 2);
    }
    return sqrt(soma_diferencas / n);
}

// Função para calcular a mediana
double calcular_mediana(double *valores, int n) {
    // Fazendo uma cópia do array original para ordenar sem modificar o original
    double *valores_copia = (double *)malloc(n * sizeof(double));
    if (valores_copia == NULL) {
        printf("Erro de alocação de memória.\n");
        exit(1);
    }
    for (int i = 0; i < n; i++) {
        valores_copia[i] = valores[i];
    }

    ordenar(valores_copia, n); // Ordena a cópia do array

    // Calcula a mediana na cópia ordenada
    double mediana;
    if (n % 2 == 0) {
        mediana = (valores_copia[n / 2 - 1] + valores_copia[n / 2]) / 2.0;
    } else {
        mediana = valores_copia[n / 2];
    }

    free(valores_copia); // Libera a memória da cópia
    return mediana;
}

int main() {
    int n;
    printf("Quantos números na sua série (N): ");
    scanf("%d", &n);

    // Alocação dinâmica do array
    double *valores = (double *)malloc(n * sizeof(double));
    if (valores == NULL) {
        printf("Erro de alocação de memória.\n");
        return 1;
    }

    // Entrada dos valores
    printf("Entre com os números:\n");
    for (int i = 0; i < n; i++) {
        printf("> ");
        scanf("%lf", &valores[i]);
    }

    // Calculando os valores mínimo e máximo
    double min = valores[0], max = valores[0];
    for (int i = 1; i < n; i++) {
        if (valores[i] < min) min = valores[i];
        if (valores[i] > max) max = valores[i];
    }

    // Cálculos dos outros parâmetros
    double media = calcular_media(valores, n);
    double mediana = calcular_mediana(valores, n);
    double desvio_padrao = calcular_desvio_padrao(valores, n, media);

    // Exibindo os resultados
    printf("Valor mínimo: %.2lf\n", min);
    printf("Valor máximo: %.2lf\n", max);
    printf("Média: %.2lf\n", media);
    printf("Mediana: %.2lf\n", mediana);
    printf("Desvio padrão: %.13lf\n", desvio_padrao);

    // Liberando a memória alocada
    free(valores);
    return 0;
}
