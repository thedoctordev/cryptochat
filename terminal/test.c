#include <stdio.h>

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

int main() {
    unsigned long long base; // Defina os valores de base, exponent e modulus conforme necessário
    unsigned long long exponent;
    unsigned long long modulus;
    scanf("%llu %llu %llu", &base, &exponent, &modulus);

    unsigned long long result = modPow(base, exponent, modulus);

    printf("Resultado: %llu\n", result);

    return 0;
}
