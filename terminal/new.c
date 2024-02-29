/*
    - ESSA MERDA FUNCIONA
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define LENGTH 999

unsigned long long mdc(unsigned long long a, unsigned long long b)
{
    do {
        unsigned long long r = a % b;
        a = b;
        b = r;
    } while (b != 0);
    return a;
}

// Função para calcular a exponenciação modular
unsigned long long modPow(unsigned long long base, unsigned long long exponent, unsigned long long modulus) {
    if (modulus == 1) return 0;

    unsigned long long result = 1;
    base = base % modulus;

    while (exponent > 0) {
        if (exponent % 2 == 1) {
            result = (result * base) % modulus;
        }
        exponent = exponent / 2;
        base = (base * base) % modulus;
    }

    return result;
}


unsigned long long gerarE(unsigned long long phi)
{
    for (int i = 2; i < phi; i++)
    {
        if (mdc(phi, i) == 1) return i;
    }
}

unsigned long long gerarD(unsigned long long a, unsigned long long m)
{
    for (int i = 2; i < m; i++)
    {
        if ((a * i) % m == 1) return i;
    }
}

void generateKeys()
{
    FILE *file;
    unsigned long long p, q, n, phi, e, d; 
    system("clear");
    printf("Digite dois números primo: ");
    scanf("%llu %llu", &p, &q);
    n = p * q;
    phi = (p - 1) * (q - 1);
    e = gerarE(phi);
    d = gerarD(e, phi);

    // Gera um arquivo com a chave pública
    file = fopen("public.key", "w");
    fprintf(file, "%llu %llu", n, e);
    fclose(file);
    // Gera um arquivo com a chave privada
    file = fopen("private.key", "w");
    fprintf(file, "%llu %llu", n, d);
    fclose(file);

    printf("\nChaves geradas com sucesso.\n");
}

void encrypt()
{
    FILE *file;
    char message[LENGTH];
    strcpy(message, "");
    char line[LENGTH / 10];
    unsigned long long code[LENGTH];
    char archive[30];
    unsigned long long e, n;

    file = fopen("public.key", "r");
    fscanf(file, "%llu %llu", &n, &e);
    fclose(file);

    printf("Digite o nome do arquivo: ");
    scanf("%s", archive);

    file = fopen(archive, "r");
    do {
        fgets(line, LENGTH / 10, file);
        strcat(message, line);
    } while (!feof(file));
    fclose(file);

    file = fopen("encrypted.txt", "w");
    for (int i = 0; i < strlen(message); i++)
    {
        code[i] = modPow((int)message[i], e, n);
        fprintf(file, "%llu ", code[i]);
    }
    fclose(file);
}

void decrypt()
{
    FILE *file;
    unsigned long long code[LENGTH];
    char archive[30];
    unsigned long long d, n;

    file = fopen("private.key", "r");
    fscanf(file, "%llu %llu", &n, &d);
    fclose(file);

    printf("Digite o nome do arquivo: ");
    scanf("%s", archive);

    file = fopen(archive, "r");
    for (int i = 0; !feof(file); i++)
    {
        fscanf(file, "%llu", &code[i]);
    }
    fclose(file);

    file = fopen("decrypted.txt", "w");
    for (int i = 0; code[i] != 0; i++)
    {
        code[i] = (int)modPow(code[i], d, n);
        fprintf(file, "%c", (char)code[i]);
    }
    fclose(file);
}

int main()
{
    system("clear");
    int option;
    printf("RSA\n\n1. Gerar chaves\n2. Criptografar\n3. Descriptografar\n");
    printf("\nEscolha uma opção: ");
    scanf("%d", &option);

    switch (option)
    {
    case 1:
        generateKeys();
        break;
    
    case 2:
        encrypt();
        break;
    
    case 3:
        decrypt();
        break;

    default:
        break;
    }

    return 0;
}