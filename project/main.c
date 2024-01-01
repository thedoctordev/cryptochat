#include <stdio.h>
#include <string.h>

// Em caso de Sistemas baseados em UNIX
#ifdef __unix__
    #include <unistd.h>
    #include <stdlib.h>
// Em caso de Sistemas Windows
#elif defined(_WIN32) || defined(WIN32)
    #define OS_Windows
    #include <windows.h>
#endif

// Declaração das variáveis que serão utilizadas
long int p,
         q,
         n,
         e,
         d,
         phi,
         code[1000],
         encrypted[1000],
         public_key[2],
         private_key;
char message[1000];

// Limpa a tela
int clear_screen()
{
#ifdef OS_Windows
    system("cls");
#else
    system("clear");
#endif
}

// Verifica se um número é primo (caso seja, retorna 1)
int is_prime(long int n)
{
    int result = 1;
    // Verifica se existe algum divisor alem de 1 e n
    for (int i = 2; i < n; i++)
    {
        result = (n % i == 0) ? 0 : result; // Caso exista algum outo divisor, não é primo
    }
    // Caso seja primo retornará 1
    return result;
}

// Retorna o Máximo Divisor Comum (MDC) entre os dois números
long int mdc(long int n1, long int n2)
{
    long int a, b, resto;
    a = n1;
    b = n2;

    do
    {
        resto = a % b;
        a = b;
        b = resto;
    } while (resto != 0);
    
    return a;
}

// Função de potenciação
long int power(long int base, long int expoente)
{
    long int multiplicator = base;
    for (int i = 1; i < expoente; i++)
    {
        base *= multiplicator;
    }
    
    return base;
}

// Converte um Texto para código ASCII
int text_to_ascii()
{
    for (int i = 0; i < 1000; i++)
    {
        code[i] = (int)message[i];
    }

    return 0;
}

// Converte código ASCII para texto
int ascii_to_text()
{
    for (int i = 0; i < 1000; i++)
    {
        message[i] = (char)code[i];
    }

    return 0;
}

// Gera as chaves públia e privada
int keys_generate()
{
    // Chave pública (n, e)
    // Chave privada (d)

    do // Insiste que seja digitado um número primo para (p)
    {
        printf("Digite o primeiro número primo: ");
        scanf("%ld", &p);
    } while (!is_prime(p));

    do // Insiste que seja digitado um número primo para (q)
    {
        printf("Digite o segundo número primo: ");
        scanf("%ld", &q);
    } while (!is_prime(q));

    n = p * q;
    phi = (p - 1) * (q - 1);
    do // Gera o expoente (e)
    {
        e = rand();
    } while (!(e > 1 && e < phi) && mdc(phi, e) != 1);
    do // Gera a chave privada (d)
    {
        d = rand();
    } while ((d * e) % phi != 1);

    public_key[0] = n;
    public_key[1] = e;
    private_key = d;
    printf(">> PUBLIC KEY: %ld %ld\n", public_key[0], public_key[1]);
    printf(">> PRIVATE KEY: %ld\n", private_key);

    return 0;
}

// Criptografa uma mensagem
int encrypt()
{
    long int key_a, key_b;
    printf("Digite a chave pública: ");
    scanf("%ld%ld", &key_a, &key_b);

    printf("Digite a mensagem que deseja criptografar:\n");
    scanf("%s", message);
    text_to_ascii();

    for (int i = 0; i < 1000; i++)
    {
        encrypted[i] = (long int)power(code[i], key_b) % key_a;
        printf("%ld ", encrypted[i]);
    }
    printf("\n");

    return 0;
}

// Descriptografa uma mensagem
int descrypt()
{
    long int key_a, key_b, key_c;
    printf("Digite a chave pública: ");
    scanf("%ld%ld", &key_a, &key_b);
    printf("\nDigite a chave privada: ");
    scanf("%ld", &key_c);

    printf("Digite a mensagem que deseja descriptografar:\n");
    int x = 0;
    long int X;
    while (scanf("%ld", &X)==1)
    {
        encrypted[x] = X;
        x++;
    }

    for (int i = 0; i < 1000; i++)
    {
        code[i] = (long int)power(encrypted[i], d) % n;
        printf("%ld ", code[i]);
    }
    printf("\n");

    return 0;
}

int main()
{
    int select;
    clear_screen();
    printf("=========================================================\n");
    printf("|                   Criptografia RSA                    |\n");
    printf("=========================================================\n");
    printf("| Gerar chaves ...................................... 1 |\n");
    printf("| Criptografar ...................................... 2 |\n");
    printf("| Descriptografar ................................... 3 |\n");
    printf("| Sair .............................................. 4 |\n");
    printf("=========================================================\n");
    scanf("%d", &select);

    switch (select)
    {
    case 1:
        keys_generate();
        break;
    
    case 2:
        encrypt();
        break;

    case 3:
        descrypt();
        break;

    case 4:
        exit(0);
    default:
        main();
        break;
    }

    return 0;
}