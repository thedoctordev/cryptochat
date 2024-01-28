# cryptochat
Projeto de criptografia RSA.

## Versão terminal (Linguagem C)
A versão em C faz uso da biblioteca GNU Multiple Precision (GMP), que pose ser instalada seguindo os passos indicados na [documentação](https://gmplib.org/manual).

Para compilar corretamente é necessário adicionar `-lgmp` ao fim do comando, para indicar ao compilador que a biblioteca GMP estás sendo utilizada.

```
gcc main.c -o main -lgmp
```

### Pontos a serem trabalhados

- Instalação da biblioteca GMP no Windows ;
- Exibição do arquivo _about.txt_ na opção 1 ;
- Entrada de dados nas opções 1 e 3 do menu _Encriptar/Decriptar mensagem_ (opção 3 do menu principal).

### Observações
- Alguns caracteres não estão sendo encriptados (ou decriptados) corretamente ;
- Caso necessário, começar do zero.

## Versão web (JavaScript)

A versão web pode ser acessada [aqui](https://cryptozap.000webhostapp.com/).
