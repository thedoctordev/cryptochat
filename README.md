# Cryptochat

Este projeto foi desenvolvido por alunos de ciência da computação da UFAL (Universidade Federal de Alagoas), durante o primeriro período, só pra ficar claro.

Na pasta `lista-MD` pode-se encontrar algumas funções relacionadas a Teoria dos números.

## Versão terminal (Linguagem C)

Através do terminal o usuário tem as opções de Gerar chave, Criptografar e Descriptografar um arquivo.

### Principais pontos a serem trabalhados

- Tratamento de erros em casos de inexistência no momento da leitura;

### Observações

- Alguns caracteres não estão sendo encriptados (ou decriptados) corretamente - mais especificamente, caracteres acentuados;
- A versão com GTK ainda tem muito a ser trabalhado.

## Versão GTK (Linguagem C)

Para compilar esta versão do código é necessário instalar o gtk primeiro. Verifique a [documentação](https://www.gtk.org/docs/getting-started/).

Para compilar corretamente use o seguinte comando:

```
gcc $(pkg-config --cflags gtk4) -o Cryptochat main.c $(pkg-config --libs gtk4)
```

### Importante

No Windows, podem haver dificuldades para preparar o ambiente necessário para o uso do gtk. (Mas é possível).

Pode também levar em consideração o uso do wsl.

## Versão web (JavaScript)

A versão web pode ser acessada [aqui](https://cryptozap.000webhostapp.com/). _**No momento está offline**_.

Esta versão pode ser testada em um servidor local como o [XAMPP](https://www.apachefriends.org/pt_br/index.html).

## Referências

- Todo ícone presente no repositório pertence ao pacote [Material Design Icons](https://pictogrammers.com/library/mdi/);
- [Artigo: "Analysis of Exponentiation Cryptosystems" por Jean-Jacques Quisquater e Claude Couvreur](https://www.quora.com/How-do-I-evaluate-sqrt-45-using-the-Newton-Raphson-method).
