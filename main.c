#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct fruta {
		char nome[15];
		int codigo;
		int quantidade;
		float preco;
} Fruta;

typedef struct lista {
		Fruta f;
		struct lista *prox;
} Lista;

Lista* criar_fruta();
Lista* cadastro(Lista* lista);
void imprimir_frutas(Lista* lista);
Lista* buscar_fruta(Lista* lista, int codigo);
Lista* alterar_fruta(Lista* lista, int codigo);
Lista* excluir_fruta(Lista* lista, int codigo);
Lista* vender_fruta(Lista* lista, int codigo);
void registrar_venda(Fruta f, int quantidade);
void obter_horario_atual(char *buffer, size_t buffer_size) {
		time_t agora = time(NULL); 
		struct tm *horario_local = localtime(&agora);
		strftime(buffer, buffer_size, "%Y/%m/%d %H:%M:%S", horario_local);
}

Lista* criar_fruta() {
		Lista *novo = (Lista*)malloc(sizeof(Lista));
		return novo;
}

Lista* cadastro(Lista* lista) {
		Lista *nova_fruta = criar_fruta();

		printf("Nome: ");
		scanf("%s", nova_fruta->f.nome);
		printf("Código: ");
		scanf("%d", &nova_fruta->f.codigo);
		printf("Quantidade: ");
		scanf("%d", &nova_fruta->f.quantidade);
		printf("Preço: ");
		scanf("%f", &nova_fruta->f.preco);

		nova_fruta->prox = lista;
		return nova_fruta;
}

void imprimir_frutas(Lista* lista) {
		Lista *aux = lista;

		while(aux != NULL) {
				printf("---------------\n");
				printf("Nome: %s\nCódigo: %d\nQuantidade: %d\nPreço: %.2f\n\n", aux->f.nome, aux->f.codigo, aux->f.quantidade, aux->f.preco);
				printf("---------------\n");
				aux = aux->prox;
		}
}

Lista* buscar_fruta(Lista* lista, int codigo) {
		Lista *aux = lista;
		while(aux != NULL) {
				if(aux->f.codigo == codigo) {
						return aux;
				}
				aux = aux->prox;
		}
		return NULL;
}

Lista* alterar_fruta(Lista* lista, int codigo) {
		Lista *fruta = buscar_fruta(lista, codigo);
		if(fruta != NULL) {
				printf("Nome: ");
				scanf("%s", fruta->f.nome);
				printf("Quantidade: ");
				scanf("%d", &fruta->f.quantidade);
				printf("Preço: ");
				scanf("%f", &fruta->f.preco);
		} else {
				printf("Fruta não encontrada.\n");
		}
		return lista;
}

Lista* excluir_fruta(Lista* lista, int codigo) {
		Lista *aux = lista;
		Lista *anterior = NULL;

		while(aux != NULL) {
				if(aux->f.codigo == codigo) {
						if(anterior == NULL) {
								lista = aux->prox;
						} else {
								anterior->prox = aux->prox;
						}
						free(aux);
						printf("Fruta excluída.\n");
						return lista;
				}
				anterior = aux;
				aux = aux->prox;
		}
		printf("Fruta não encontrada.\n");
		return lista;
}

Lista* vender_fruta(Lista* lista, int codigo) {
		Lista *fruta = buscar_fruta(lista, codigo);
		if(fruta != NULL) {
				int quantidade;
				printf("Quantidade a vender: ");
				scanf("%d", &quantidade);

				if(quantidade > fruta->f.quantidade) {
						printf("Quantidade insuficiente.\n");
				} else {
						fruta->f.quantidade -= quantidade;
						registrar_venda(fruta->f, quantidade);
				}
		} else {
				printf("Fruta não encontrada.\n");
		}
		return lista;
}

void registrar_venda(Fruta f, int quantidade) {
		FILE *file = fopen("registro.txt", "a");
		if(file != NULL) {
				char buffer[20];
				obter_horario_atual(buffer, sizeof(buffer));
				fprintf(file, "Nome: %s\nCódigo: %d\nQuantidade vendida: %d\nPreço unitário: R$%.2f\nTotal: R$%.2f\nData e Hora: %s\n\n", 
								f.nome, f.codigo, quantidade, f.preco, quantidade * f.preco, buffer);
				fclose(file);
		} else {
				printf("Erro ao abrir o arquivo de registro.\n");
		}
}

int main(void) {
		Lista* lista = NULL;
		int escolha;
		int codigo;

		while(1) {
				printf("1 - Cadastrar fruta\n2 - Listar frutas\n3 - Buscar fruta\n4 - Alterar fruta\n5 - Excluir fruta\n6 - Vender fruta\n7 - Sair\n");
				scanf("%d", &escolha);

				switch (escolha) {
						case 1:
								lista = cadastro(lista);
								break;
						case 2:
								imprimir_frutas(lista);
								break;
						case 3:
								printf("Código da fruta: ");
								scanf("%d", &codigo);
								Lista* fruta = buscar_fruta(lista, codigo);
								if(fruta != NULL) {
										printf("Fruta encontrada: Nome: %s, Código: %d, Quantidade: %d, Preço: %.2f\n", fruta->f.nome, fruta->f.codigo, fruta->f.quantidade, fruta->f.preco);
								} else {
										printf("Fruta não encontrada.\n");
								}
								break;
						case 4:
								printf("Código da fruta: ");
								scanf("%d", &codigo);
								lista = alterar_fruta(lista, codigo);
								break;
						case 5:
								printf("Código da fruta: ");
								scanf("%d", &codigo);
								lista = excluir_fruta(lista, codigo);
								break;
						case 6:
								printf("Código da fruta: ");
								scanf("%d", &codigo);
								lista = vender_fruta(lista, codigo);
								break;
						case 7:
								exit(0);
								break;
						default:
								printf("Opção inválida.\n");
				}
		}

		return 0;
}
