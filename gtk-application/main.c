#include <gtk/gtk.h>
#include <glib/gstdio.h>

#define LENGTH 999

unsigned long long p, q;
char archive[30]; // Usada para armazenar o nome do arquivo a ser tratado

//===============================================================================================================================
float raizquadrada(float num) {
    // Algoritmo de Newton-Raphson, método da aproximação quadrática
    float x = num;
    float y = 1;
    float epsilon = 0.000001; // precisão desejada

    while (x - y > epsilon) {
        x = (x + y) / 2;
        y = num / x;
    }
    return x;
}

int isPrime(float numero) {
    int i; // divisor

    for (i = 2; i <= raizquadrada(numero); i++) {
        if ((int)numero % i == 0) {
            return 0; // Se for divisível por algum número, não é primo
        }
    }
    return 1; // Se não encontrou nenhum divisor, é primo
}

static void define_p(GtkWidget *widget, gpointer data)
{
	GtkEntryBuffer *buffer = gtk_entry_get_buffer(GTK_ENTRY(data));
	const char *text = gtk_entry_buffer_get_text(buffer);
	p = atoi(text);
}

static void define_q(GtkWidget *widget, gpointer data)
{
	GtkEntryBuffer *buffer = gtk_entry_get_buffer(GTK_ENTRY(data));
	const char *text = gtk_entry_buffer_get_text(buffer);
	q = atoi(text);
}

static void define_archive(GtkWidget *widget, gpointer data)
{
	GtkEntryBuffer *buffer = gtk_entry_get_buffer(GTK_ENTRY(data));
	const char *text = gtk_entry_buffer_get_text(buffer);
	strcpy(archive, text);
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
static void generateKeys(GtkWidget *widget, gpointer data) {
    FILE *file; // Ponteiro para direcionar à arquivos
    unsigned long long n, phi, e, d; // Variáveis necessária

    n = p * q; // n é usado em ambas as chaves
    phi = (p - 1) * (q - 1); // Função Totiente de Euler
	p = 0; // Limpa a variável
	q = 0; // Limpa a variável
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
	
	g_print("Chaves geradas com sucesso!\n");
	gtk_window_destroy(data);
}

//===============================================================================================================================
/** Função responsável por criptografar uma mensagem
 * - Converte os caracteres para números
 * - Criptografa cada caractere
 */
void encrypt(GtkWidget *widget, gpointer data)
{
    FILE *file; // Ponteiro para direcionar à arquivos
    char message[LENGTH]; // String em que será armazenada a mensagem
    strcpy(message, ""); // Inicializa a string como vazia
    char line[LENGTH / 10]; // String auxiliar para a leitura
    unsigned long long code[LENGTH]; // Array em que serão armazenados os caracteres criptografados
    unsigned long long e, n; // Armazenará os valores da chave pública

    // Lê a chave pública do arquivo
    //! Caso o arquivo não exista, resultará em erro
    // TODO: Tratar esse erro
    file = fopen("public.key", "r"); // Abre o arquivo no modo leitura
    fscanf(file, "%llu %llu", &n, &e); // Lê os valores da chave pública e os armazena nas variáveis
    fclose(file); // Fecha o arquivo

    // Lê o conteúdo do arquivo e o armazena na string message
    //* Quando o arquivo termina com uma quebra de linha, ele lê a última linha duas vezes
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
        fprintf(file, "%llu ", code[i]); // Escreve o caractere criptografado no arquivo
    }
    fclose(file); // Fecha o arquivo

    g_print("\nMensagem criptografada com sucesso!\n");
	gtk_window_destroy(data);
}

//===============================================================================================================================
/** Função responsável por descriptografar uma mensagem
 * - Descriptografa cada caractere
 * - Converte os números para caracteres
 */
void decrypt(GtkWidget *widget, gpointer data) {
    FILE *file; // Ponteiro para direcionar à arquivos
    unsigned long long code[LENGTH]; // Array em que serão armazenados os caracteres criptografados e descriptografados
    unsigned long long d, n; // Armazenará os valores da chave da

    // Lê a chave privada do arquivo
    //! Caso o arquivo não exista, resultará em erro
    // TODO: Tratar esse erro
    file = fopen("private.key", "r"); // Abre o arquivo no modo leitura
    fscanf(file, "%llu %llu", &n, &d); // Lê os valores da chave privada e os armazena nas variáveis
    fclose(file); // Fecha o arquivo

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

    g_print("\nMensagem descriptografada com sucesso!\n");
    gtk_window_destroy(data);
}

static void generate_keys_window(GtkWidget *widget, gpointer data)
{
	/* Construct a GtkBuilder instance and load our UI description */
	GtkBuilder *builder = gtk_builder_new();
	gtk_builder_add_from_file(builder, "generate_keys.ui", NULL);

	/* Connect signal handlers to the constructed widgets. */
	GObject *window = gtk_builder_get_object(builder, "window");
	gtk_window_set_application(GTK_WINDOW(window), data);

	GObject *entry_p = gtk_builder_get_object(builder, "entry_p");
	g_signal_connect(entry_p, "changed", G_CALLBACK(define_p), entry_p);

	GObject *entry_q = gtk_builder_get_object(builder, "entry_q");
	g_signal_connect(entry_q, "changed", G_CALLBACK(define_q), entry_q);

	GObject *button = gtk_builder_get_object(builder, "button");
	g_signal_connect(button, "clicked", G_CALLBACK(generateKeys), window);

	gtk_widget_set_visible(GTK_WIDGET(window), TRUE);

	g_object_unref(builder);
}

static void encrypt_window(GtkWidget *widget, gpointer app)
{
	GtkBuilder *builder = gtk_builder_new();
	gtk_builder_add_from_file(builder, "encrypt.ui", NULL);

	GObject *window = gtk_builder_get_object(builder, "window");
	gtk_window_set_application(GTK_WINDOW(window), app);

	GObject *entry = gtk_builder_get_object(builder, "entry");
	g_signal_connect(entry, "changed", G_CALLBACK(define_archive), entry);

	GObject *button = gtk_builder_get_object(builder, "button");
	g_signal_connect(button, "clicked", G_CALLBACK(encrypt), window);

	gtk_widget_set_visible(GTK_WIDGET(window), TRUE);

	g_object_unref(builder);
}

static void decrypt_window(GtkWidget *widget, gpointer app)
{
	GtkBuilder *builder = gtk_builder_new();
	gtk_builder_add_from_file(builder, "decrypt.ui", NULL);

	GObject *window = gtk_builder_get_object(builder, "window");
	gtk_window_set_application(GTK_WINDOW(window), app);

	GObject *entry = gtk_builder_get_object(builder, "entry");
	g_signal_connect(entry, "changed", G_CALLBACK(define_archive), entry);

	GObject *button = gtk_builder_get_object(builder, "button");
	g_signal_connect(button, "clicked", G_CALLBACK(decrypt), window);

	gtk_widget_set_visible(GTK_WIDGET(window), TRUE);

	g_object_unref(builder);
}

static void
activate(GtkApplication *app, gpointer user_data)
{
	GtkBuilder *builder = gtk_builder_new();
	gtk_builder_add_from_file(builder, "main.ui", NULL);

	GObject *window = gtk_builder_get_object(builder, "window");
	gtk_window_set_application(GTK_WINDOW(window), app);

	GObject *button_1 = gtk_builder_get_object(builder, "button_1");
	g_signal_connect(button_1, "clicked", G_CALLBACK(generate_keys_window), app);

	GObject *button_2 = gtk_builder_get_object(builder, "button_2");
	g_signal_connect(button_2, "clicked", G_CALLBACK(encrypt_window), app);

	GObject *button_3 = gtk_builder_get_object(builder, "button_3");
	g_signal_connect(button_3, "clicked", G_CALLBACK(decrypt_window), app);

	gtk_widget_set_visible(GTK_WIDGET(window), TRUE);

	g_object_unref(builder);
}

static void
cryptochat_application_about_action(GSimpleAction *action,
									GVariant *parameter,
									gpointer user_data)
{
	static const char *authors[] = {"Alison Bruno Martires Soares",
    								"Diego Neves da Silva",
    								"Jayme Vinicius Esteves Pedroza Melo",
    								"João Gabriel Seixas Santos",
    								"José Cícero de Oliveira Rodrigues",
    								"Neilton Gabriel Gonçalves Luciano",
    								"Zilderlan Naty dos Santos", NULL};
	GtkApplication *self = user_data;
	GtkWindow *window = NULL;

	g_assert(GTK_APPLICATION(self));

	window = gtk_application_get_active_window(GTK_APPLICATION(self));

	gtk_show_about_dialog(window,
						  "program-name", "Cryptochat",
						  "authors", authors,
						  "version", "0.0.1",
						  "copyright", "© 2024 José Cícero",
						  NULL);
}

static void
cryptochat_application_quit_action(GSimpleAction *action,
								   GVariant *parameter,
								   gpointer user_data)
{
	GtkApplication *self = user_data;

	g_assert(GTK_APPLICATION(self));

	g_application_quit(G_APPLICATION(self));
}

static const GActionEntry app_actions[] = {
	{"quit", cryptochat_application_quit_action},
	{"about", cryptochat_application_about_action},
};

static void
init(GtkApplication *app)
{
	g_action_map_add_action_entries(G_ACTION_MAP(app),
									app_actions,
									G_N_ELEMENTS(app_actions),
									app);
	gtk_application_set_accels_for_action(GTK_APPLICATION(app),
										  "app.quit",
										  (const char *[]){"<primary>q", NULL});
}

int main(int argc, char *argv[])
{
	GtkApplication *app = gtk_application_new("com.thedoctor.cryptochat", G_APPLICATION_FLAGS_NONE);
	g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
	g_signal_connect(app, "startup", G_CALLBACK(init), NULL);

	int status = g_application_run(G_APPLICATION(app), argc, argv);

	g_object_unref(app);

	return status;
}