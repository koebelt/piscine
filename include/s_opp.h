#ifndef _S_OPP_H_
# define _S_OPP_H_

typedef struct s_opp
{
  char* opp;
  void (*fct)(int, int);
} t_opp;

void my_add(int a, int b);
void my_sub(int a, int b);
void my_div(int a, int b);
void my_mod(int a, int b);
void my_mul(int a, int b);
void my_usage(int a, int b);
#endif