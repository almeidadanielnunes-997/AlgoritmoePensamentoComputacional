#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX_LETRAS 15
#define NOME_ARQUIVO "resultado_criptografia.txt"

// Enumeração para identificar o tipo de sequência escolhida
typedef enum {
    TIPO_PA = 1,
    TIPO_PG = 2,
    TIPO_FIBONACCI = 3,
    TIPO_PRIMOS = 4
} TipoSequencia;

// Protótipos das funções
void exibirMenu(void);
bool validarPalavra(const char *str);
void limparBuffer(void);
void gerarSequenciaPA(int *vetor, int tamanho, int a1, int r);
void gerarSequenciaPG(int *vetor, int tamanho, int a1, int q);
void gerarSequenciaFibonacci(int *vetor, int tamanho);
void gerarSequenciaPrimos(int *vetor, int tamanho);
bool ehPrimo(int n);
void criptografar(const char *origem, char *destino, int shift, const int *seq, int tam);
void salvarLog(const char *palavraCod, int shift, int tipo, int letras, const char *palavraOriginal, const int *seq);

int main() {
    char palavra[MAX_LETRAS + 2];
    char palavraCodificada[MAX_LETRAS + 2];
    int shift;
    int opcao;
    int sequencia[MAX_LETRAS];
    int tamPalavra;

    printf("=====================================================\n");
    printf("   PROJETO: CRIPTOGRAFIA E MATEMATICA APLICADA       \n");
    printf("=====================================================\n\n");

    // 1. Entrada e validação da palavra (até 15 caracteres alfabéticos)
    do {
        printf("Digite a palavra secreta (maximo %d letras, sem espacos/acentos): ", MAX_LETRAS);
        if (scanf("%16s", palavra) != 1) {
            printf("Erro na leitura da palavra.\n");
            return 1;
        }
        limparBuffer();

        if (!validarPalavra(palavra)) {
            printf("[ERRO] A palavra deve conter apenas letras (A-Z ou a-z) e no maximo %d caracteres.\n\n", MAX_LETRAS);
        }
    } while (!validarPalavra(palavra));

    tamPalavra = strlen(palavra);

    // 2. Entrada do valor de SHIFT (Cifra de César)
    printf("Digite o valor do SHIFT fixo (ex: 3): ");
    while (scanf("%d", &shift) != 1) {
        printf("[ERRO] Entrada invalida. Digite um numero inteiro para o SHIFT: ");
        limparBuffer();
    }
    limparBuffer();

    // 3. Escolha da sequência matemática
    exibirMenu();
    printf("Escolha a opcao da sequencia (1 a 4): ");
    while (scanf("%d", &opcao) != 1 || opcao < 1 || opcao > 4) {
        printf("[ERRO] Opcao invalida. Digite um valor entre 1 e 4: ");
        limparBuffer();
    }
    limparBuffer();

    // 4. Geração dos termos da sequência selecionada
    switch (opcao) {
        case TIPO_PA: {
            int a1, r;
            printf("\n[Configuracao da PA]\n");
            printf("Digite o primeiro termo (a1): ");
            scanf("%d", &a1);
            printf("Digite a razao (r): ");
            scanf("%d", &r);
            limparBuffer();
            gerarSequenciaPA(sequencia, tamPalavra, a1, r);
            break;
        }
        case TIPO_PG: {
            int a1, q;
            printf("\n[Configuracao da PG]\n");
            printf("Digite o primeiro termo (a1): ");
            scanf("%d", &a1);
            printf("Digite a razao (q): ");
            scanf("%d", &q);
            limparBuffer();
            gerarSequenciaPG(sequencia, tamPalavra, a1, q);
            break;
        }
        case TIPO_FIBONACCI:
            gerarSequenciaFibonacci(sequencia, tamPalavra);
            break;
        case TIPO_PRIMOS:
            gerarSequenciaPrimos(sequencia, tamPalavra);
            break;
    }

    // 5. Aplicação da criptografia (Camada 1: César + Camada 2: Sequência)
    criptografar(palavra, palavraCodificada, shift, sequencia, tamPalavra);

    // 6. Exibição do cálculo e resultado no console
    printf("\n------------------- LOG DE EXECUCAO -------------------\n");
    printf("Palavra original:    %s\n", palavra);
    printf("SHIFT fixo:          %d\n", shift);
    printf("Sequencia gerada:    ");
    for (int i = 0; i < tamPalavra; i++) {
        printf("%d%s", sequencia[i], (i == tamPalavra - 1) ? "" : ", ");
    }
    printf("\nDeslocamento total:  ");
    for (int i = 0; i < tamPalavra; i++) {
        printf("%d%s", shift + sequencia[i], (i == tamPalavra - 1) ? "" : ", ");
    }
    printf("\nPalavra codificada:  %s\n", palavraCodificada);
    printf("-------------------------------------------------------\n");

    // 7. Gravação no arquivo
    salvarLog(palavraCodificada, shift, opcao, tamPalavra, palavra, sequencia);

    printf("\nArquivo '%s' gerado com sucesso no diretorio atual.\n", NOME_ARQUIVO);
    return 0;
}

// Apresenta o menu de seleção de técnicas matemáticas
void exibirMenu(void) {
    printf("\n--- Escolha a Sequencia Numerica (Camada 2) ---\n");
    printf("1. Progressao Aritmetica (PA)\n");
    printf("2. Progressao Geometrica (PG)\n");
    printf("3. Serie de Fibonacci (1, 1, 2, 3, 5, 8, ...)\n");
    printf("4. Numeros Primos (2, 3, 5, 7, 11, ...)\n");
    printf("-----------------------------------------------\n");
}

// Garante que a entrada tenha no máximo 15 letras e sem caracteres especiais
bool validarPalavra(const char *str) {
    int len = strlen(str);
    if (len == 0 || len > MAX_LETRAS) {
        return false;
    }
    for (int i = 0; i < len; i++) {
        if (!isalpha((unsigned char)str[i])) {
            return false;
        }
    }
    return true;
}

// Limpa caracteres residuais do buffer do teclado
void limparBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Gera termos da Progressão Aritmética: a_n = a_1 + (n - 1) * r
void gerarSequenciaPA(int *vetor, int tamanho, int a1, int r) {
    for (int i = 0; i < tamanho; i++) {
        *(vetor + i) = a1 + (i * r);
    }
}

// Gera termos da Progressão Geométrica: a_n = a_1 * q^(n - 1)
void gerarSequenciaPG(int *vetor, int tamanho, int a1, int q) {
    int termo = a1;
    for (int i = 0; i < tamanho; i++) {
        *(vetor + i) = termo;
        termo *= q;
    }
}

// Gera a Série de Fibonacci: F_1 = 1, F_2 = 1, F_n = F_(n-1) + F_(n-2)
void gerarSequenciaFibonacci(int *vetor, int tamanho) {
    if (tamanho >= 1) *(vetor + 0) = 1;
    if (tamanho >= 2) *(vetor + 1) = 1;
    for (int i = 2; i < tamanho; i++) {
        *(vetor + i) = *(vetor + i - 1) + *(vetor + i - 2);
    }
}

// Testa se um número é primo
bool ehPrimo(int n) {
    if (n < 2) return false;
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) return false;
    }
    return true;
}

// Gera a sequência de números primos consecutivos a partir de 2
void gerarSequenciaPrimos(int *vetor, int tamanho) {
    int count = 0;
    int num = 2;
    while (count < tamanho) {
        if (ehPrimo(num)) {
            *(vetor + count) = num;
            count++;
        }
        num++;
    }
}

// Aplica a cifra com rotação cíclica no alfabeto (mod 26), preservando caixa alta/baixa
void criptografar(const char *origem, char *destino, int shift, const int *seq, int tam) {
    for (int i = 0; i < tam; i++) {
        char base = isupper((unsigned char)origem[i]) ? 'A' : 'a';
        int deslocamentoTotal = (shift + *(seq + i)) % 26;

        // Trata eventuais deslocamentos negativos no módulo
        if (deslocamentoTotal < 0) {
            deslocamentoTotal += 26;
        }

        *(destino + i) = base + ((origem[i] - base + deslocamentoTotal) % 26);
    }
    *(destino + tam) = '\0';
}

// Grava o resultado final no arquivo conforme o padrão exigido
void salvarLog(const char *palavraCod, int shift, int tipo, int letras, const char *palavraOriginal, const int *seq) {
    FILE *arquivo = fopen(NOME_ARQUIVO, "w");
    if (arquivo == NULL) {
        printf("[ERRO] Nao foi possivel criar o arquivo de log.\n");
        return;
    }

    // Linha de saída idêntica ao modelo da especificação:
    // Palavra codificada: fqvdjqb | SHIFT: 3 | Tipo: 3 | Letras: 7
    fprintf(arquivo, "Palavra codificada: %s | SHIFT: %d | Tipo: %d | Letras: %d\n",
            palavraCod, shift, tipo, letras);

    // Detalhamento complementar do log para conferência acadêmica
    fprintf(arquivo, "\n--- Detalhes da Execucao ---\n");
    fprintf(arquivo, "Entrada original: %s\n", palavraOriginal);
    fprintf(arquivo, "Termos matematicos usados: ");
    for (int i = 0; i < letras; i++) {
        fprintf(arquivo, "%d%s", *(seq + i), (i == letras - 1) ? "" : ", ");
    }
    fprintf(arquivo, "\n");

    fclose(arquivo);
}