/** Criptografia RSA
 * 
 * Este programa está sendo desenvolvido por alunos do primeiro período de Ciência/Engenharia da Computação (UFAL), como projeto
 * para a disciplina de Matemática Discreta. Com o intuito de aplicar o conteúdo abordado durante as aulas.
 * 
 * ! Os números primos para gerar chaves devem ser de dois ou mais dígitos
 * TODO: Verificar se os números digitados são primos (ADICIONAR UMA FUNÇÃO)
 * TODO: Adicionar a verificação do sistema para usar a biblioteca <windows.h>
 * TODO: Adicionar o suporte a acentuação
 * TODO: Manter o usuário no programa (ADICIONAR FUNÇÃO)
 * TODO: Adicionar as opções "Sobre" e "Creditos"
*/

// Bibliotecas necessárias
#include <stdio.h> // Entrada e saída padrão
#include <stdlib.h> // Algumas funcionalidades úteis
#include <string.h> // Manipulação de strings ("Textos")

#define LENGTH 999 // Tamanho limite da mensagem a ser manipulada
                   //*Cada linha deve ter no máximo 1 décimo desse valor

//===============================================================================================================================
/**
 * A função "mdc" é uma implementação do algoritmo de Elclides que calcula o máximo divisor comum (MDC) entre dois números. 
 * @param a É um número inteiro não negativo
 * @param b É um número inteiro não negativo
 * @return O maior número que divide os dois números (a,b) sem deixar resto.
*/
unsigned long long mdc(unsigned long long a, unsigned long long b) {
    do { // Dentro do loop, calculamos o resto da divisão de "a" por "b" e armazenamos em "r".
        unsigned long long r = a % b;
        a = b; // Em seguida, "b" é atribuído a "a", e o valor do resto "r" é atribuído a "b".
        b = r; // Isto é equivalente a "andar" um passo para a direita na sequência de divisões.
    } while (b != 0); // Este processo é repetido até que "b" seja igual a 0. Este é o critério de parada.
    return a; // A função retorna "a", que é o MDC dos números iniciais.
}

//===============================================================================================================================
/** Função que realiza a operação de exponenciação modular.
 * A operação de exponenciação modular é uma operação matemática que calcula (base^exponent) % modulus.
 * É frequentemente usada em algoritmos de criptografia.
 * 
 * @param b É a base da operação da exponenciação.
 * @param e É o expoente da operação da exponenciação.
 * @param m É o número no qual o resultado da operação da exponenciação será reduzido.
 * @return O resultado da operação (base^exponent) % modulus. 
 */
unsigned long long modPow(unsigned long long b, unsigned long long e, unsigned long long m) {
    unsigned long long result = 1; // A função começa inicializando um resultado como 1.
    b = b % m; //então reduz a base usando o operador de módulo.
    while (e > 0) { // Então, entra em um loop enquanto o expoente é maior que zero.
        if (e % 2 == 1) {
            // Se o expoente é ímpar, ele multiplica o resultado pela base (reduzida) e novamente reduz usando o módulo.
            result = (result * b) % m;
        }
        e = e / 2; // O expoente é dividido por 2 (efetivamente reduzindo pela metade).
        b = (b * b) % m; // A base é então quadrada e reduzida.
    }
    return result; // 6. O resultado final é retornado.
}

//===============================================================================================================================
/** Função para gerar o valor 'e' usado na chave pública
 * @param phi Valor limite para e
 * @return Retorna o valor coprimo a phi
 */
unsigned long long generateE(unsigned long long phi) {
    for (int i = 2; i < phi; i++) { // Inicia um loop com i indo de 2 até phi-1 
        if (mdc(phi, i) == 1) return i; // Retorna i | mdc(i, phi) = 1 ∧ 0 < i < phi
    }
}

/** Função para gerar o valor 'd' usado na chave privada
 * Encontra o inverso modular
 * @param a Coeficiente da operação
 * @param m Módulo da operação
 * @return inverso de a mod m
 */
unsigned long long generateD(unsigned long long a, unsigned long long m) {
    for (int i = 2; i < m; i++) { // Inicia um loop com i indo de 2 até m
        if ((a * i) % m == 1) return i; // Retorna i | ai ≡ 1 mod m
    }
}

/** Função responsável por gerar a chave pública e privada
 * Gera um dois arquivos contendo as chaves
 */
void generateKeys() {
    FILE *file; // Ponteiro para direionar à arquivos
    unsigned long long p, q, n, phi, e, d; // Variáveis necessária

    //! Só funciona em Unix
    system("clear"); // Limpa a tela 
    // TODO: Adicionar um altenativa para Windows

    printf("Digite um número primos: ");
    scanf("%llu", &p); // Solicita que o usuário digite um número primo para p
    printf("Digite outro número primos: ");
    scanf("%llu", &q); // Solicita que o usuário digite um número primo para q
    // TODO: Verificar se os números digitados são primos

    n = p * q; // n é usado em ambas as chaves
    phi = (p - 1) * (q - 1); // Função Totiente de Euler
    e = generateE(phi); // Escolhe 'e' tal que mdc(e, phi) = 1 e 0 < e < phi
    d = generateD(e, phi); // Calcula 'd' tal que e*d ≡ 1 (mod phi)

    // Gera um arquivo com a chave pública
    file = fopen("public.key", "w"); // Abre o arquivo no modo escrita (Cria, caso não exista)
    fprintf(file, "%llu %llu", n, e); // Escreve os valores da chave publica no arquivo
    fclose(file); // Fecha o arquivo

    // Gera um arquivo com a chave privada
    file = fopen("private.key", "w"); // Abre o arquivo no modo escrita (Cria, caso não exista)
    fprintf(file, "%llu %llu", n, d); // Escreve os valores da chave privada no arquivo
    fclose(file); // Fecha o arquivo

    printf("\nChaves geradas com sucesso.\n");
}

//===============================================================================================================================
/** Função responsável por criptografar uma mensagem
 * - Converte os caracteres para números
 * - Criptografa cada caractere
 */
void encrypt()
{
    FILE *file; // Ponteiro para direionar à arquivos
    char message[LENGTH]; // String em que será armazenada a mensagem
    strcpy(message, ""); // Inicializa a string como vazia
    char line[LENGTH / 10]; // String auxiliar para a leirtura
    unsigned long long code[LENGTH]; // Array em que seram armazenados os caracteres criptografados
    char archive[30]; // Usada para armazenar o nome do arquivo a ser tratado
    unsigned long long e, n; // Amazenará os valores da chave pública

    // Lê a chave pública do arquivo
    //! Caso o arquivo não exista, resultará em erro
    // TODO: Tratar esse erro
    file = fopen("public.key", "r"); // Abre o arquivo no modo leitura
    fscanf(file, "%llu %llu", &n, &e); // Lê os valores da chave pública e os armazena nas variáveis
    fclose(file); // Fecha o arquivo

    printf("Digite o nome do arquivo: ");
    scanf("%s", archive); // Solicita o nome do arquivo a ser criptografado

    // Lê o conteúdo do arquivo e o armazena na string message
    //* Quando o aqruivo termina com uma quebra de linha, ele lê a última linha duas vezes
    //! Caso o arquivo não exista, resultará em erro
    // TODO: Tratar esse erro
    file = fopen(archive, "r"); // Abre o arquivo no modo leitura
    do { // Entra no loop para ler o conteúdo
        fgets(line, LENGTH / 10, file); // Lê uma linha por vez (No máximo 1 décimo do tamanho limite da mensagem)
        strcat(message, line); // Concatena a linha na string message
    } while (!feof(file)); // Mantém o loop até o fim do arquivo
    fclose(file); // Fecha o arquivo

    // Criptografa a mensagem (o arquivo)
    file = fopen(archive, "w"); // Abre o arquivo no modo escrita
    for (int i = 0; i < strlen(message); i++) { // Inicia um loop que passa por todos os caracteres
        code[i] = modPow((int)message[i], e, n); // Criptografa o caractere
        //* Caracteres acentuados são convertidos em um inteiro fora da tabela ASCII
        fprintf(file, "%llu ", code[i]); // Escreve o caracter criptografado no arquivo
    }
    fclose(file); // Fecha o arquivo

    printf("\nMensagem criptografada com sucesso!\n");
}

//===============================================================================================================================
/** Função responsável por descriptografar uma mensagem
 * - Descriptografa cada caractere
 * - Converte os números para caracteres
 */
void decrypt() {
    FILE *file; // Ponteiro para direionar à arquivos
    unsigned long long code[LENGTH]; // Array em que seram armazenados os caracteres criptografados e descriptografados
    char archive[30]; // Usada para armazenar o nome do arquivo a ser tratado
    unsigned long long d, n; // Amazenará os valores da chave da

    // Lê a chave privada do arquivo
    //! Caso o arquivo não exista, resultará em erro
    // TODO: Tratar esse erro
    file = fopen("private.key", "r"); // Abre o arquivo no modo leitura
    fscanf(file, "%llu %llu", &n, &d); // Lê os valores da chave privada e os armazena nas variáveis
    fclose(file); // Fecha o arquivo

    // Solicita o nome do arquivo criptografado
    printf("Digite o nome do arquivo com extensão: ");
    scanf("%s", archive);

    // Lê a mensagem criptografada do arquivo
    //! Caso o arquivo não exista, resultará em erro
    // TODO: Tratar esse erro
    file = fopen(archive, "r"); // Abre o arquivo no modo leitura
    for (int i = 0; !feof(file); i++) { // Inicia um loop que só tem sua execução interrompida qundo chegar ao fim do arquivo
        fscanf(file, "%llu", &code[i]); // Lê um caractere criptografado e o armazena na posição atual do array
    }
    fclose(file); // Fecha o arquivo

    // Descriptografa a mensagem e grava no arquivo
    file = fopen(archive, "w"); // Abre o arquivo no modo escrita
    for (int i = 0; code[i] != '\0'; i++) { // Inicia um loop que se encerra ao encontra um '\0' que indica o fim do arquivo (mensagem)
        code[i] = (int)modPow(code[i], d, n); // Descriptografa o caractere
        fprintf(file, "%c", (char)code[i]); // Converte o número para caractere e o imprime no arquivo
    }
    fclose(file); // Fecha o arquivo

    printf("\nMensagem descriptografada com sucesso!\n");
}

//===============================================================================================================================
/** Função principal
 * Primeira função a ser executada ao iniciar o programa
 */
int main() {

    //! Só funciona em Unix
    system("clear"); // Limpa a tela 
    // TODO: Adicionar um altenativa para Windows
    
    int option; // Armazena a escolha do usuário
    printf("RSA\n\n1. Gerar chaves\n2. Criptografar\n3. Descriptografar\n4. Encerrar o programa\n");
    printf("\nEscolha uma opção: ");
    scanf("%d", &option); // Lê a escolha do usuário

    // Executa a opção escolhida pelo usuário
    switch (option) {
        case 1: // Gerar chaves
            generateKeys();
            break;
        case 2: // Criptografar
            encrypt();
            break;
        case 3: // Descriptografar
            decrypt();
            break;
        case 4: // Encerrar programa
            exit(0);
            break;
        default:
            break;
    }

    return 0;
}