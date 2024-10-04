#ifndef HEADER_LIB
#define HEADER_LIB
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "header.h"

Node* Node_create(char* data) {
  Node* new = (Node*)malloc(sizeof(Node));

  if (new == NULL) {
    // erro
  }

  strncpy(new->data, data, sizeof(new->data) - 1);
  new->data[sizeof(new->data) - 1] = '\0';
  new->next = NULL;
}

Node* Node_add(Node* head, char* data) {
  Node* new = Node_create(data);
  Node* tmp = head;

  while (tmp->next != NULL) {
    tmp = tmp->next;
  }

  tmp->next = new;
}

/*
char cpf[15];
char nome[60];
char numero[20];
char email[60]
struct Node* email_alternativo;
struct Node* telefone_alternativo;
*/

Contato* Contato_create(char* cpf, char* nome, char* telefone, char* email) {
  Contato* new = (Contato*)malloc(sizeof(Contato));

  if (new == NULL) {
    // erro
  }

  strncpy(new->cpf, cpf, sizeof(new->cpf) - 1);
  strncpy(new->nome, nome, sizeof(new->nome) - 1);
  strncpy(new->telefone, telefone, sizeof(new->telefone) - 1);
  strncpy(new->email, email, sizeof(new->email) - 1);

  new->cpf[sizeof(new->cpf) - 1] = '\0';
  new->nome[sizeof(new->nome) - 1] = '\0';
  new->telefone[sizeof(new->telefone) - 1] = '\0';
  new->email[sizeof(new->email) - 1] = '\0';
  new->email_alternativo = NULL;
  new->telefone_alternativo = NULL;
  new->next = NULL;
}

Contato* Contato_add(Contato* head, char* cpf, char* nome, char* telefone, char* email) {
  Contato* new = Contato_create(cpf, nome, telefone, email);
  Contato* tmp = head;

  while (tmp->next != NULL) {
    tmp = tmp->next;
  }

  tmp->next = new;
}

bool is_cpf_valid(char* cpf) {
  if (strlen(cpf) != 14)
    return false;
  if (cpf[3] != '.' || cpf[7] != '.' || cpf[11] != '-' )
    return false;

  return true;
}

bool is_phone_valid(char* phone) {
  // +55 63 98444-4444
  if (strlen(phone) != 17)
    return false;
  if (
    phone[0] != '+' ||
    phone[1] != '5' ||
    phone[2] != '5' ||
    phone[3] != ' ' ||
    phone[6] != ' ' ||
    phone[7] != '9' ||
    phone[12] != '-'
  ) {
    return false;
  }

  return true;
}

void Contato_insert_str(Contato* head, char* raw) {
  //char field[strlen(raw) * 6];
  //char field_index = 0;
  //char subfield_index = 0;
//
  //if (!head) {
  //  head = Contato_create();
  //}
//
  //for (int i = 0; i < strlen(raw)) {
  //  if (raw[i] == '|') {
  //    memset(field, '\0', sizeof(field));
  //    field_index++;
  //  } else {
  //    field[]
  //  }
}

Contato* Contato_load() {
  FILE* fp = fopen("banco-de-dados.txt", "r");
  char buff[1024 * 1024];
  Contato* head = NULL;
  Contato* last;

  while (fgets(buff, 1024 * 1024, fp)) {
    char cpf[CPF_LEN];
    char nome[NAME_LEN];
    char telefone[NUMBER_LEN];
    char email[EMAIL_LEN];
    char telefone_alternativo[1024];
    char email_alternativo[1024];
    sscanf(buff, "%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^\n]", cpf, nome, telefone, email, telefone_alternativo, email_alternativo);

    if (head == NULL) {
      head = Contato_create(cpf, nome, telefone, email);
      last = head;
    } else {
      last = Contato_add(head, cpf, nome, telefone, email);
    }

    char* tel_token = strtok(telefone_alternativo, ";");

    while (tel_token != NULL) {
      if (strcmp(tel_token, "(null)") == 0)
        break;

      if (last->telefone_alternativo == NULL) {
        last->telefone_alternativo = Node_create(tel_token);
      } else {
        Node_add(last->telefone_alternativo, tel_token);
      }

      tel_token = strtok(NULL, ";");
    }

    char* email_token = strtok(email_alternativo, ";");

    while (email_token != NULL) {
      if (strcmp(email_token, "(null)") == 0)
        break;

      if (last->email_alternativo == NULL) {
        last->email_alternativo = Node_create(email_token);
      } else {
        Node_add(last->email_alternativo, email_token);
      }

      email_token = strtok(NULL, ";");
    }
  }

  fclose(fp);

  return head;
}

void Contato_save(Contato* head, bool ephemeral) {
  FILE* fp = fopen(ephemeral == true ? "novos-contatos.txt" : "banco-de-dados.txt", "w");
  fwrite("", 1, 0, fp);
  fclose(fp);

  fp = fopen(ephemeral == true ? "novos-contatos.txt" : "banco-de-dados.txt", "a");
  unsigned long size;
  Contato* tmp = head;

  while (tmp->next != NULL) {
    char buff[1024 * 1024];
    char emails_alt[1024];
    char telefones_alt[1024];
    Node* telefones_alt_ptr = tmp->telefone_alternativo;
    Node* emails_alt_ptr = tmp->email_alternativo;

    strncat(buff, tmp->cpf, strlen(tmp->cpf));
    strncat(buff, "|", 1);
    strncat(buff, tmp->nome, strlen(tmp->nome));
    strncat(buff, "|", 1);
    strncat(buff, tmp->telefone, strlen(tmp->telefone));
    strncat(buff, "|", 1);
    strncat(buff, tmp->email, strlen(tmp->email));
    strncat(buff, "|", 1);

    while (telefones_alt_ptr->next != NULL) {
      strncat(buff, telefones_alt_ptr->data, strlen(telefones_alt_ptr->data));

      if (telefones_alt_ptr->next != NULL)
        strncat(buff, ";", 1);

      telefones_alt_ptr = telefones_alt_ptr->next;
    }

    if (strlen(telefones_alt_ptr->data) > 0) {
      // hacky
      strncat(buff, telefones_alt_ptr->data, strlen(telefones_alt_ptr->data));
    }

    if (buff[strlen(buff) - 1] == ';') {
      buff[strlen(buff) - 1] = '\0';
    }

    strncat(buff, "|", 1);

    while (emails_alt_ptr->next != NULL) {
      strncat(buff, emails_alt_ptr->data, strlen(emails_alt_ptr->data));

      if (emails_alt_ptr->next != NULL && strlen(emails_alt_ptr->next->data) > 0)
        strncat(buff, ";", 1);

      emails_alt_ptr = emails_alt_ptr->next;
    }

    if (buff[strlen(buff) - 1] == ';') {
      buff[strlen(buff) - 1] = '\0';
    }

    strncat(buff, "\n", 1);


    fwrite(buff, 1, strlen(buff), fp);
    tmp = tmp->next;
  }

  // p: in order
}

void Contato_dump(Contato *head) {
  Contato *tmp = head;

  printf("cpf|nome|telefone|email|tel_alt_1|tel_alt_2|email_alt_1|email_alt_2\r\n");

  while (tmp != NULL) {
    char tel_alt_1[NUMBER_LEN] = "(null)";
    char tel_alt_2[NUMBER_LEN] = "(null)";
    char email_alt_1[EMAIL_LEN] = "(null)";
    char email_alt_2[EMAIL_LEN] = "(null)";

    if (tmp->telefone_alternativo != NULL) {
      memset(tel_alt_1, '\0', NUMBER_LEN);
      strncpy(tel_alt_1, tmp->telefone_alternativo->data, strlen(tmp->telefone_alternativo->data) - 1);
      if (tmp->telefone_alternativo->next != NULL) {
        memset(tel_alt_2, '\0', NUMBER_LEN);
        strncpy(tel_alt_2, tmp->telefone_alternativo->next->data, strlen(tmp->telefone_alternativo->next->data) - 1);
      } else {
        strncpy(tel_alt_2, "(null)", NUMBER_LEN);
      }
    } else {
      strncpy(tel_alt_1, "(null)", NUMBER_LEN);
    }

    if (tmp->email_alternativo != NULL) {
      memset(email_alt_1, '\0', EMAIL_LEN);
      strncpy(email_alt_1, tmp->email_alternativo->data, strlen(tmp->email_alternativo->data) - 1);
      if (tmp->email_alternativo->next != NULL) {
        memset(email_alt_2, '\0', EMAIL_LEN);
        strncpy(email_alt_2, tmp->email_alternativo->next->data, strlen(tmp->email_alternativo->next->data) - 1);
      } else {
        strncpy(email_alt_2, "(null)", EMAIL_LEN);
      }
    } else {
      strncpy(email_alt_1, "(null)", EMAIL_LEN);
    }

    printf("%s|", tmp->cpf);
    printf("%s|", tmp->nome);
    printf("%s|", tmp->telefone);
    printf("%s|", tmp->email);
    printf("%s|", tel_alt_1);
    printf("%s|", tel_alt_2);
    printf("%s|", email_alt_1);
    printf("%s\r\n", email_alt_2);

    tmp = tmp->next;
  }
}

Contato* Contato_find_by_cpf(Contato* head, char* cpf) {
  Contato* tmp = head;

  while (tmp != NULL) {
    if (strcmp(tmp->cpf, cpf) == 0) {
      return tmp;
    }

    tmp = tmp->next;
  }

  return NULL;
}

void Contato_delete(Contato* head, Contato* contato) {
  Contato* tmp = head;

  while (tmp->next != NULL) {
    if (tmp->next == contato) {
      tmp->next = contato->next;
      free(contato);
      break;
    }

    tmp = tmp->next;
  }

  if (head == contato) {
    head = head->next;
  }

  // p: first
}

void Contato_change_cpf(Contato* contato, char* cpf) {
  memset(contato->cpf, '\0', CPF_LEN);
  strncpy(contato->cpf, cpf, strlen(cpf));
}

#endif