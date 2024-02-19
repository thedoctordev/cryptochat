#include <stdio.h>
#include <math.h>
#include <time.h>

#ifdef _WIN32
    // Caso seja compilado no Windows
    #include <windows.h>
#elif defined(__unix__)
    // Caso sejacompilado pra Mac/Linux
    #include <stdlib.h>
#endif

// A função main está definida no final do código
int main();

// =============================================================================================
//  Esta função limpa a tela do terminal
// =============================================================================================
void limparTela()
{
    // Limpa a tela do terminal
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// =============================================================================================
//  Esta função aguarda que o usuário tecle ENTER para continuar a execução
// =============================================================================================
void pause()
{
    printf("\nClique ENTER para continuar...");
    getchar();getchar(); // Gambiarra :)
}

// =============================================================================================
//  Esta função exibe os créditos
// =============================================================================================
void creditos()
{
    limparTela();
    printf("Obrigado por usar nosso programa :)\n");
    printf("\nDesenvolvido por:\n");
    printf("\tNOME\n");
    printf("\tNOME\n");
    printf("\tNOME\n");
    printf("\tNOME\n");
    printf("\tNOME\n");
    printf("\tNOME\n");
    printf("\tNOME\n");
    pause();
    main();
}

// =============================================================================================
//  Esta função exibe um menu de opções
// =============================================================================================
void menu(char *titulo, char *op1, char *op2, char *op3, char *op4, char *op5, 
     char *op6, char *op7, char *op8, char *op9, char *op10, char *op11, char *op12)
{
    limparTela();
    printf("===================================================================\n");
    printf("|     %-60s|\n", titulo);
    printf("===================================================================\n");
    printf("|  1. %-60s|\n", op1);
    printf("|  2. %-60s|\n", op2);
    printf("|  3. %-60s|\n", op3);
    printf("|  4. %-60s|\n", op4);
    printf("|  5. %-60s|\n", op5);
    printf("|  6. %-60s|\n", op6);
    printf("|  7. %-60s|\n", op7);
    printf("|  8. %-60s|\n", op8);
    printf("|  9. %-60s|\n", op9);
    printf("| 10. %-60s|\n", op10);
    printf("| 11. %-60s|\n", op11);
    printf("| 12. %-60s|\n", op12);
    printf("===================================================================\n");
    printf(">> Escolha uma das opcoes: ");
}

// =============================================================================================
//  Esta função verifica se um número é primo
// =============================================================================================
int primo(int numero, int divisor){

    int resto;

    resto = numero % divisor;

    if (numero <= 1){
        return 0;
    }
    else if (resto == 0 && divisor != 1 && divisor != numero){
        return 0;
    }
    if (divisor < sqrt(numero)){
        return primo(numero, divisor + 1);
    }

    return 1;
}

// =============================================================================================
//  Esta função Verifica números primos e exibe o resultado
// =============================================================================================
void ehPrimo()
{
    int x, resultado;
    limparTela();
    printf("Descubra se um numero eh primo ou nao.\n");
    printf("\nDigite um numero: ");
    scanf("%d", &x);

    resultado = primo(x, 2);

    (resultado == 1) ? printf("Esse numero eh primo.\n\n"): printf("Esse numero nao eh primo.\n\n");

    pause();
    main();
}

// =============================================================================================
//  Esta função lista o máximo de primos em 60 segundos
// =============================================================================================
void listarPrimos()
{
    time_t inicio;
    time(&inicio);
    int primos = 0;
    
    limparTela();

    for (int i = 2; i > 0; i++)
    {
        time_t atual;
        time(&atual);

        if (difftime(atual, inicio) == 60)
        {
            printf("\n\nEm 60 segundos, foram listados %d primos.\n", primos);
            pause();
            main();
        }
        else if (primo(i, 2) == 1)
        {
            primos++;
            printf(" - %d", i);
        }
    }
}

// =============================================================================================
//  Esta função decompoe um número em fatores primos
// =============================================================================================
void fatorar(int numero, int divisor){
    
    int resto;
    resto = numero % divisor;
    
    if (numero != 1){
    
        if (resto == 0){
        
            printf("%d, ", divisor);
        
            return fatorar(numero / divisor, 2);
        }
        else{
            return fatorar(numero, ++divisor);
        }
    }
    else{
        printf("\b\b.\n");
        pause();
        main();
    }
}

void decomporEmPrimos()
{
    limparTela();
    int n;    
    printf("Decomponha um numero em seus fatores primos.\n");
    printf("Digite um numero: ");
    scanf("%d", &n);
    printf("Os fatores primos desse numero sao: ");
    fatorar(n, 2);
}

// =============================================================================================
//  Esta função calcula o MMC e o MDC de dois inteiros, baseando-se em seus fatores primos
// =============================================================================================

int mmc(int numero_1, int numero_2, int resposta, int divisor)
{
        
    if (numero_1 == 1 && numero_2 == 1) return resposta;
    else 
    {
        if ((numero_1 % divisor) == 0 || (numero_2 % divisor) == 0)
        {
            resposta = resposta * divisor;
            if ((numero_1 % divisor) == 0) numero_1 = numero_1 / divisor;
            if ((numero_2 % divisor) == 0) numero_2 = numero_2 / divisor;
            return mmc(numero_1, numero_2, resposta, 2);
        }
        else return mmc(numero_1, numero_2, resposta, divisor + 1);

    }
}

int mdc(int valor1, int valor2, int resposta2, int divisor)
{
    if (valor1 == 1 && valor2 == 1) return resposta2;
    else
    {
        if ((valor1 % divisor) == 0 && (valor2 % divisor) == 0)
        {
            resposta2 = resposta2 * divisor;
            valor1 = valor1 / divisor;
            valor2 = valor2 / divisor;
            return mdc(valor1, valor2, resposta2, 2);
        }
        else
        {
            if (divisor > valor2) return resposta2;
            else return mdc(valor1, valor2, resposta2, divisor + 1);
        }
    }
}

void MMCeMDC(){
    int numero1, numero2, resultadommc, resultadomdc;
    limparTela();
    printf("Encontre o MDC e o MMC de dois inteiros.\n");
    printf("Digite um numero inteiro: ");
    scanf("%d", &numero1);
    printf("Digite outro numero inteiro: ");
    scanf("%d", &numero2);
    
    if (numero1 >= numero2)
    {
        resultadomdc = mdc(numero1, numero2, 1, 2);
        resultadommc = mmc(numero1, numero2, 1, 2);
    }
    else
    {
        resultadomdc = mdc(numero2, numero1, 1, 2);
        resultadommc = mmc(numero2, numero1, 1, 2);
    }
    
    printf("O MDC entre %d e %d eh %d\n", numero1, numero2, resultadomdc);
    printf("O MMC entre %d e %d eh %d\n", numero1, numero2, resultadommc);

    pause();
    main();
}

// =============================================================================================
//  Esta função usa o algoritimo de Euclides para determinar o MDC de dois inteiros
// =============================================================================================
void mdc_euclides(int valor_a, int valor_b, int valor_inicial_a, int valor_inicial_b){


    if (valor_a == valor_b){
        printf("\nO mdc entre %d e %d eh %d\n", valor_inicial_a, valor_inicial_b, valor_a);
    }
    else if (valor_a > valor_b){
        if (valor_b > 1){
            return mdc_euclides(valor_b, valor_a % valor_b, valor_inicial_a, valor_inicial_b);
        }
        else if (valor_b == 1){
            printf("\nO mdc entre %d e %d eh 1\n", valor_inicial_a, valor_inicial_b);
        }
        else if (valor_b == 0){
            printf("\nO mdc entre %d e %d eh %d\n", valor_inicial_a, valor_inicial_b, valor_a);
        }
        else{
            printf("\ntem coisa errada\n");
        }
    }
    else if(valor_a < valor_b){
       return mdc_euclides(valor_b, valor_a, valor_inicial_a, valor_inicial_b);
    }
    else
    {
        printf("\nerrou");
    }
    
    pause();
    main();
}

void mdcEuclides(){
    int a, b, inicial_a, inicial_b;
    limparTela();
    printf("Encontre o MDC de dois inteiros.\n");
    printf("Digite um numero inteiro: ");
    scanf("%d", &a);
    printf("Digite outro numero inteiro: ");
    scanf("%d", &b);

    inicial_a = a;
    inicial_b = b;

    mdc_euclides(a, b, inicial_a, inicial_b);
}

// =============================================================================================
//  Esta função encontra uma combinação linear
// =============================================================================================


void combinacaoLinear(){
    
}

// =============================================================================================
//  Esta função calcula o inverso modular
// =============================================================================================
int inverso(int a, int m)
{
    if (m == 1) return 0;

    for (int i = 0; i < m; i++)
    {
        if((a * i) % m == 1) 
        {
            if (i < 0) return i + m;
            else return i;
        }
    }

    return 0;
}

void inversoModular(){
    int a, m, resultado;
    limparTela();
    printf("Descubra o inverso modular.\n\n");
    printf("Digite o valor de a: ");
    scanf("%d", &a);
    printf("Digite o valor de m: ");
    scanf("%d", &m);
    resultado = inverso(a, m);

    if (resultado != 0)
    {
        printf("\nO invrso de %d mod %d eh: %d\n", a, m, resultado);
    }
    else
    {
        printf("Nao existe um inverso, pois mdc(%d,%d) eh diferente de 1.\n", a, m);
    }

    pause();
    main();
}

// =============================================================================================
//  Esta função calcula uma congruência linear
// =============================================================================================
void congruenciaLinear(){
    int a, b, m, inverse, x;
    limparTela();
    printf("Encontre o solucao de uma congruencia linear.\n");
    printf("Digite o valor de a: ");
    scanf("%d", &a);
    printf("Digite o valor de b: ");
    scanf("%d", &b);
    printf("Digite o valor de m: ");
    scanf("%d", &m);
    inverse = inverso(a, m);
    x = (b * inverse) % m;
    
    if (inverse == 0) printf("Essa congruencia nao tem solucao.\n");
    else printf("A solucao para essa congruencia linear eh %d\n", x);

    pause();
    main();
}

// =============================================================================================
//  Esta função faz uso do Teorema Chinês do Resto
// =============================================================================================
void teoremaChines(){}

// =============================================================================================
//  Esta é a função principal
// =============================================================================================
int main(){
    menu(
        "MATEMATICA DISCRETA", 
        "Verificar se eh primo", 
        "Listar numeros primos", 
        "Decompor em fatores primos",
        "MMC e MDC de dois inteiros",
        "MDC - Algoritmo de Euclides",
        "Encontrar Combinacao linear",
        "Encontrar o Inverso modular",
        "Encontrar a Congruencia linear",
        "Teorema Chines do resto com 3 congruencias",
        "Teorema Chines do resto com N congruencias",
        "Creditos",
        "Encerrar programa");

    int select;
    scanf("%d", &select);

    // Direciona o programa para uma função de acordo com a escolha do usuário
    switch (select)
    {
    case 1:
        ehPrimo();
        break;
    
    case 2:
        listarPrimos();
        break;

    case 3:
        decomporEmPrimos();
        break;

    case 4:
        MMCeMDC();
        break;

    case 5:
        mdcEuclides();
        break;

    case 6:
        combinacaoLinear();
        break;

    case 7:
        inversoModular();
        break;

    case 8:
        congruenciaLinear();
        break;

    case 9:
        teoremaChines();
        break;

    case 10:
        teoremaChines();
        break;
    
    case 11:
        creditos();
        break;

    default:
        limparTela();
        exit(1);
        break;
    }

    return 0;
}