#define DATA_LEN 60
#define CPF_LEN 15
#define NAME_LEN 60
#define NUMBER_LEN 18
#define EMAIL_LEN 60

#include <stdbool.h>

typedef struct Node {
  char data[DATA_LEN];
  struct Node* next;
} Node;
//sempre o mesmo tamanho de memoria 

typedef struct Contato {
  char cpf[CPF_LEN];
  char nome[NAME_LEN];
  char telefone[NUMBER_LEN];
  char email[EMAIL_LEN];
  struct Node* email_alternativo;
  struct Node* telefone_alternativo;
  struct Contato* next;
} Contato;

Node* Node_create(char* data);
Node* Node_add(Node* head, char* data);
Contato* Contato_create(char* cpf, char* nome, char* telefone, char* email);
Contato* Contato_add(Contato* head, char* cpf, char* nome, char* telefone, char* email);
Contato* Contato_find_by_cpf(Contato* head, char* cpf);
Contato* Contato_load();
void Contato_change_cpf(Contato* contato, char* cpf);
void Contato_delete(Contato* head, Contato* contato);
void Contato_dump(Contato *head);
void Contato_insert_str(Contato* head, char* raw);
void Contato_save(Contato* head, bool ephemeral);

bool is_cpf_valid(char* cpf);
bool is_phone_valid(char* phone);