/*=================================================================================
                        PROJETO DE CRIPTOGRAFIA RSA
=================================================================================*/
// Bibliotecas nescessárias
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <gmp.h>
// Identifica o SO
#ifdef _WIN32 // Em caso de Sistemas Windows
    #define OS_Windows
    #include <windows.h>
#elif defined(__unix__) // Em caso de Sistemas baseados em UNIX (Mac/Linux)
    #include <stdlib.h>
#endif

#define MAX_LEN 999 // Aqui é definido o tamanho máximo tamanho máximo de uma mensagem

/*=================================================================================
                Função que limpa a tela do terminal
=================================================================================*/
void clearScreen()
{
    #ifdef OS_Windows
        system("cls");
    #else
        system("clear");
    #endif
}

/*=================================================================================
                Função que aguarda o usuário pressionar uma tecla
    OBS: Só funciona no windows
=================================================================================*/
void pause(){
    #ifdef OS_Windows
        system("pause");
    #endif
}

/*=================================================================================
                        Função que exibe um cabeçalho
=================================================================================*/
void header(char* title)
{
    clearScreen();
    printf("===========================================================\n");
    printf("|   %s\n", title);
    printf("===========================================================\n");
}

/*=================================================================================
                    Função que exibe um menu de opções
=================================================================================*/
int menu(char *title, char *opcao1, char *opcao2, char *opcao3, char *opcao4, char *opcao5)
{
    int selected;
    header(title);
    printf("| 1. %s\n", opcao1);
    printf("| 2. %s\n", opcao2);
    printf("| 3. %s\n", opcao3);
    printf("| 4. %s\n", opcao4);
    printf("| 5. %s\n", opcao5);
    printf("===========================================================\n");
    printf(" Selecione uma opção: ");
    scanf("%d", &selected);

    return selected;
}

/*=================================================================================
            Função que exibe uma o conteúdo de um arquivo de texto
=================================================================================*/
void printArchive(char *name)
{
    char text[MAX_LEN];
    FILE *file;
    file = fopen(name, "r");
    clearScreen();
    fscanf(file, "%[^EOF]s", text);
    printf("%s\n", text);
    fclose(file);
    pause();
}

/*=================================================================================
    Função responsável por verificar se um número é primo (caso seja, retorna 1)
=================================================================================*/
int isPrime(unsigned long n) {
		if (n <= 1) {
				return 0;
		}
		for (int i = 2; i < n; i++) {
				if (n % i == 0) {
						return 0;
				}
		}
		return 1;
}

/*=================================================================================
 ALGORÍTIMO DE EUCLIDES - Retorna o Máximo Divisor Comum (MDC) entre os dois número
=================================================================================*/
int mdc(unsigned long n1, unsigned long n2) {
    unsigned long a, b, resto;
    a = n1;
    b = n2;
    do {
        resto = a % b;
        a = b;
        b = resto;
    } while (b != 0);

    return a;
}

/*=================================================================================
            Função responsável por converter um texto para código ASCII
=================================================================================*/
void stringToAscii(char *text, int *ascii)
{
    for (int i = 0; i < MAX_LEN; i++)
    {
        ascii[i] = i < strlen(text) ? (int)text[i] : 0;
    }
}

/*=================================================================================
            Função responsável por gerar as chaves pública e privada
=================================================================================*/
unsigned long generateE(unsigned long phi)
{
    for (unsigned long i = 2; i < phi; i++)
    {
        if (mdc(phi, i) == 1) return i;
    }
    return 0;
}

unsigned long generateD(unsigned long a, unsigned long m)
{
    for (unsigned long i = 2; i < m; i++)
    {
        if((a * i) % m == 1) return i;
    }
    return 0;
}

void keysGenerate()
{
    // Variáveis nescessárias
    FILE *file;
    unsigned long p, q, n, phi, e, d;
    p = 0;
    q = 0;

    header("GERAR CHAVES PÚBLICA E PRIVADA");

    do // Solicita que o usuário digite um valor primo para 'p'
    {
        header("GERAR CHAVES PÚBLICA E PRIVADA"); // Imprime o cabeçalho
        (p == 0) ? printf("Digite um número primo: ") : printf("Este número não é primo, tente outro: ");
        scanf("%lu", &p); // Lê um valor par 'p'
    } while (!isPrime(p));
    
    do // Solicita que o usuário digite um valor primo para 'q'
    {
        header("GERAR CHAVES PÚBLICA E PRIVADA"); // Imprime o cabeçalho
        (q == 0) ? printf("Digite outro número primo: ") : printf("Este número não é primo, tente outro: ");
        scanf("%lu", &q); // Lê um valor par 'p'
    } while (!isPrime(q));

    n = p * q; // Calcula o valor de 'n'
    phi = (p - 1) * (q - 1); // Calcula o valor de φ(n) | Função totiente de Euler

    // Escolhe o valor aleatório para 'e' | mdc(e, φ(n)) = 1
    e = generateE(phi);

    // Escolhe o valor aleatório para 'd' | de ≡ 1 mod(φ(n))
    d = generateD(e, phi);

    // Gera um arquivo com a chave pública
    file = fopen("public.key", "w");
    fprintf(file, "%lu %lu", n, e);
    fclose(file);
    // Gera um arquivo com a chave privada
    file = fopen("private.key", "w");
    fprintf(file, "%lu %lu", n, d);
    fclose(file);
}

/*=================================================================================
                Função responsável por encriptar uma mensagem
=================================================================================*/
void encrypt(char input)
{
    FILE *file;
    int ascii[MAX_LEN];
    char message[MAX_LEN]; // Variável que guardará a mensagem a ser encriptada
    unsigned long n, e;

    if (input == 'k')
    {
        header("ENCRIPTAR MENSSAGEM"); // Imprime o cabeçalho
        printf("| Digite a mensagem que deseja encriptar:\n| >>> ");
        scanf("%[^EOF]", message); // Lê a mensagem digitada pelo usuário
    }
    else if (input == 'f')
    {
        char archive[30];

        header("ENCRIPTAR MENSSAGEM"); // Imprime o cabeçalho
        printf("Digite o nome do arquivo que deseja encriptar:\n >>> ");
        scanf("%s", archive); // Lê o nome do arquivo digitado pelo usuário
        file = fopen(archive, "r"); // Abre o arquivo indicado no modo Somente leitura
        fscanf(file, "%[^EOF]s", message); // Lê o conteúdo do arquivo e o copia para a string 'message'
        fclose(file); // Fecha o arquivo
    }

    // Lê a chave pública
    file = fopen("public.key", "r"); // Abre o arquivo que contém a chave pública no modo Somente leitura
    fscanf(file, "%lu %lu", &n, &e); // Lê os valores contidos no arquivo
    fclose(file); // Fecha o aquivo

    stringToAscii(message, ascii); // Converte o texto para número
    
    // Escreve a mensagem encriptada em um arquivo
    file = fopen("encrypted.txt", "w"); // Abre o arquivo no modo escrita
    for (int i = 0; i < strlen(message); i++)
    {
        mpz_t encryptedCode, base, expoent, modulus; // Cria variáveis do tipo "Inteiro de precisão multipla"
        mpz_inits(encryptedCode, base, expoent, modulus, NULL); // Inicializa as variáveis
        mpz_set_ui(base, ascii[i]); // Copia o valor ASCII para a variável 'base'
        mpz_set_ui(expoent, e); // Copia o valor de 'e' para a variável 'expoent'
        mpz_set_ui(modulus, n); // Copia o valor da 'n' para a variável 'modulus'
        mpz_powm(encryptedCode, base, expoent, modulus); // Faz o cálculo da potência modular | b^e mod n
        gmp_fprintf(file, "%Zd ", encryptedCode); // Escreve o código encriptado no arquivo
        mpz_clears(encryptedCode, base, expoent, modulus, NULL); // Elimia as variáveis para liberar memória
    }
    fclose(file); // Fecha o arquivo
}

/*=================================================================================
                Função responsável por decriptar uma mensagem
=================================================================================*/
void decrypt(char input)
{
    FILE *file;
    char message[MAX_LEN];
    char code[20];
    unsigned long n, d, num;
    int j = 0;

    if (input == 'k')
    {
        header("DENCRIPTAR MENSSAGEM"); // Imprime o cabeçalho
        printf("| Digite a mensagem que deseja encriptar:\n| >>> ");
        scanf("%[^EOF]s", message); // Lê a mensagem digitada pelo usuário
    }
    else if (input == 'f')
    {
        char archive[30];

        header("DECRIPTAR MENSSAGEM"); // Imprime o cabeçalho
        printf("Digite o nome do arquivo que deseja decriptar:\n >>> ");
        scanf("%s", archive); // Lê o nome do arquivo digitado pelo usuário
        file = fopen(archive, "r"); // Abre o arquivo indicado no modo Somente leitura
        fscanf(file, "%[^EOF]s", message); // Lê o conteúdo do arquivo e o copia para a string 'message'
        fclose(file); // Fecha o arquivo
    }

    // Lê a chave privada
    file = fopen("private.key", "r"); // Abre o arquivo que contém a chave privada
    fscanf(file, "%lu %lu", &n, &d); // Lê os valores contidos no arquivo
    fclose(file); // Fecha o arquivo

    // Escreve a mensagem decriptada em um arquivo
    file = fopen("decrypted.txt", "w");
    for (int i = 0; i < strlen(message); i++) // Passa por todos os caracteres do arquivo
    {
        if (message[i] != ' ')
        {
            // Enquanto não for um espaco, concatena na variável 'code'
            code[j] = message[i];
            j++;
        }
        else    
        {
            // Sendo um espaço, temos um código na variável 'code'
            mpz_t decryptedCode, base, expoent, modulus; // Cria variáveis do tipo "Inteiro de precisão multipla"
            mpz_inits(decryptedCode, base, expoent, modulus, NULL); // Inicializa as variáveis
            num = strtoul(code, NULL, 10); // Convete a string 'code' para inteiro
            mpz_set_ui(base, num); // Copia o valor de 'num' para a variável 'base'
            mpz_set_ui(expoent, d); // Copia o valor de 'd' para a variável 'expoent'
            mpz_set_ui(modulus, n); // Copia o valor de 'n' para a variável 'modulus'
            mpz_powm(decryptedCode, base, expoent, modulus);  // Faz o cálculo da potência modular | b^d mod n
            num = mpz_get_ui(decryptedCode); // Converte o número de Precisão Multipla para Inteiro
            mpz_clears(decryptedCode, base, expoent, modulus, NULL); // Elimina as variáveis para liberar memória
            fprintf(file, "%c", (int)num); // Escreve o caractere correspodente no arquivo
            strcpy(code, ""); // Limpa a variável 'code' para a próxima iteração
            j = 0; // Reseta o valor de 'j'
            num = 0;
        }
    }
    fclose(file); // Fecha o arquivo
}
/*=================================================================================
        Função principal - responsável por conectar todas as funcionalidades
=================================================================================*/
int main()
{
    int option;
    option = menu("CRIPTOGRAFIA RSA", 
                  "Como o RSA funciona?", 
                  "Gerar chaves Pública/Privada", 
                  "Encriptar/Decriptar mensagem", 
                  "Créditos", 
                  "Encerrar programa");

    switch (option)
    {
    case 1: // Como o RSA funciona?
        printArchive("about.txt");
        break;
    
    case 2: // Gerar chave Púbica/Privada
        keysGenerate();
        break;
    
    case 3: // Encriptar/Decriptar mensagem
        int new_option = menu("ENCRIPTAR/DECRIPTAR MENSAGEM", 
                              "[ENCRIPTAR] Digitar mensagem (Em desevolvimento...)", 
                              "[ENCRIPTAR] Ler arquivo", 
                              "[DECRIPTAR] Digitar mensagem (Em desevolvimento...)", 
                              "[DECRIPTAR] Ler arquivo", 
                              "Voltar\n|\n| Digite qualquer outro caractere para encerrar o programa.");
        
        switch (new_option)
        {
        case 1: // [ENCRIPTAR] Digitar mensagem
            encrypt('k');
            break;
        
        case 2: // [ENCRIPTAR] Ler arquivo
            encrypt('f');
            break;
        
        case 3: // [DECRIPTAR] Digitar mensagem
            decrypt('k');
            break;
        
        case 4: // [DECRIPTAR] Ler arquivo
            decrypt('f');
            break;
        
        case 5:
            main();
            break;
        
        default:
            break;
        }

        break;
    
    case 4: // Créditos
        printArchive("credits.txt");
        break;
    
    case 5: // Encerrar programa
        header("\n| \tObrigado por usar nosso programa, espero que \n| tenha suprido sua nescessidade.\n|");
        pause();
        exit(0);
        break;
    
    default:
        main();
        break;
    }

    return 0;
}
