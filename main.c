#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "header.h"

/*
db struct
cpf|nome|telefone|email|tel_alt1;tel_alt2;tel_alt3....|email_alt1;email_alt2....
cpf|nome|telefone|email|tel_alt1;tel_alt2;tel_alt3....|email_alt1;email_alt2....
*/

/*
CPF (padr˜ao xxx-xxx-xxx-xx), n´umero
de telefone principal (padr˜ao +55 DDD 9xxxx-xxx)
*/

// o gerente deve opder adicionar contatos via arquivo
// deixar linha em branco por ultimo nos txt

int main() {
  Contato* contato = Contato_create("0123456789", "abc", "638848448", "adaasudhsuib");
  Contato* contato2 = Contato_add(contato, "789456123", "fgh", "638848448", "adaasudhsuib");
  Contato* contato3 = Contato_add(contato, "951959195", "def", "638848448", "adaasudhsuib");
  Contato* lista = Contato_load();
  // valide com is_cpf_valid e is_phone_valid ANTES de contato_add
  // Contato_save(lista, true); // true escreve em novos-contatos e false em banco-de-dados
  // Contato_dump(lista); // lista completa
  // Contato* f = Contato_find_by_cpf(lista, "111.111.111-11"); // NULL ou Contato*
  // Contato_delete(lista, f);
  // Contato_change_cpf(f, "222.222.222-23");
  // Contato_dump(lista);
  return 0;
}






