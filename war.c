#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define QTD_TERRITORIOS 5 

// Estrutura que representa um território
typedef struct {
    char nome[50];
    char cor[20];
    int tropas;
    int conquistado;  
} Territorio;

// Declaração das funções usadas no programa
void inicializarTerritorios(Territorio *mapa);
void exibirMapa(const Territorio *mapa);
void atacar(Territorio *mapa);
void verificarMissao(const Territorio *mapa, const char *missao);
int todosConquistados(const Territorio *mapa, const char *corAlvo);
int contarConquistados(const Territorio *mapa);

int main() {
    srand(time(NULL));
    Territorio *mapa = calloc(QTD_TERRITORIOS, sizeof(Territorio));
    if (!mapa) {
        printf("Erro ao alocar memoria.\n");
        return 1;
    }

    // preenche o mapa com dados iniciais
    inicializarTerritorios(mapa); 

    // cria uma lista de missões possíveis
    const char *missoes[] = {
        "Destruir o exercito Verde",
        "Conquistar 3 territorios"
    };
    // escolhe uma missão aleatoriamente
    const char *missaoAtual = missoes[rand() % 2];

    int opcao;
    do {
        // menu principal
        printf("\n===== MENU =====\n");
        printf("1 - Atacar\n");
        printf("2 - Verificar Missao\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);
        getchar(); 

        switch (opcao) {
            case 1:
                atacar(mapa);
                break;
            case 2:
                verificarMissao(mapa, missaoAtual);
                break;
            case 0:
                printf("Encerrando o jogo...\n");
                break;
            default:
                printf("Opcao invalida!\n");
        }
    } while (opcao != 0);

    free(mapa);
    return 0;
}

// função que cria os territórios iniciais
void inicializarTerritorios(Territorio *mapa) {
    const char *nomes[] = {"Brasil", "Argentina", "Chile", "Peru", "Colombia"};
    const char *cores[] = {"Vermelho", "Verde", "Azul", "Amarelo", "Preto"};

    for (int i = 0; i < QTD_TERRITORIOS; i++) {
        strcpy(mapa[i].nome, nomes[i]);
        strcpy(mapa[i].cor, cores[i]);
        mapa[i].tropas = rand() % 6 + 3;
        mapa[i].conquistado = 0;
    }
    exibirMapa(mapa); 
}

// mostra o estado atual do mapa
void exibirMapa(const Territorio *mapa) {
    printf("\n=== ESTADO DO MAPA ===\n");
    for (int i = 0; i < QTD_TERRITORIOS; i++) {
        printf("%d. %-10s | Cor: %-8s | Tropas: %d\n",
                i + 1, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
    printf("=======================\n");
}

// função que simula uma batalha entre dois territórios
void atacar(Territorio *mapa) {
    int atk, def;
    exibirMapa(mapa);

    printf("Escolha o territorio atacante (1-5): ");
    scanf("%d", &atk);
    printf("Escolha o territorio defensor (1-5): ");
    scanf("%d", &def);

    // valida se a escolha é válida
    if (atk < 1 || atk > 5 || def < 1 || def > 5 || atk == def) {
        printf("Escolha invalida!\n");
        return;
    }

    Territorio *atacante = &mapa[atk - 1];
    Territorio *defensor = &mapa[def - 1];

    if (atacante->tropas <= 1) {
        printf("O atacante nao possui tropas suficientes!\n");
        return;
    }

    // gera dados aleatórios de ataque e defesa
    int dadoAtq = rand() % 6 + 1;
    int dadoDef = rand() % 6 + 1;

    printf("\nAtaque de %s (%s) contra %s (%s)\n", 
        atacante->nome, atacante->cor, defensor->nome, defensor->cor);
    printf("Dado atacante: %d | Dado defensor: %d\n", dadoAtq, dadoDef);

    // compara os dados para decidir o resultado
    if (dadoAtq >= dadoDef) {
        defensor->tropas--;
        printf("Defensor perdeu 1 tropa!\n");

        // se o defensor ficar sem tropas, o território é conquistado
        if (defensor->tropas <= 0) {
            defensor->tropas = 1;
            strcpy(defensor->cor, atacante->cor);
            defensor->conquistado = 1;
            printf("Territorio %s conquistado por %s!\n", defensor->nome, atacante->cor);
        }
    } else {
        atacante->tropas--;
        printf("Atacante perdeu 1 tropa!\n");
    }

    // mostra o mapa atualizado
    exibirMapa(mapa); 
}

// verifica se a missão foi cumprida
void verificarMissao(const Territorio *mapa, const char *missao) {
    printf("\nMissao atual: %s\n", missao);

    // missão 1: destruir o exército verde
    if (strcmp(missao, "Destruir o exercito Verde") == 0) {
        if (todosConquistados(mapa, "Verde"))
            printf("Missao concluida! O exercito Verde foi destruido!\n");
        else
            printf("Missao ainda nao concluida.\n");
    } 
    // missão 2: conquistar 3 territórios
    else if (strcmp(missao, "Conquistar 3 territorios") == 0) {
        int conquistados = contarConquistados(mapa);
        if (conquistados >= 3)
            printf("Missao concluida! Voce conquistou 3 territorios!\n");
        else
            printf("Voce conquistou %d territorios. Continue!\n", conquistados);
    }
}

// retorna 1 se todos os territórios da corAlvo foram conquistados
int todosConquistados(const Territorio *mapa, const char *corAlvo) {
    for (int i = 0; i < QTD_TERRITORIOS; i++) {
        if (strcmp(mapa[i].cor, corAlvo) == 0)
            return 0; 
    }
    return 1; 
}

// conta quantos territórios já foram conquistados
int contarConquistados(const Territorio *mapa) {
    int total = 0;
    for (int i = 0; i < QTD_TERRITORIOS; i++) {
        if (mapa[i].conquistado)
            total++;
    }
    return total;
}
