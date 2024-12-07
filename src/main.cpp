#include <cstddef>
#include <cstdio>
#include <getopt.h>
#include <cstdlib>
#include <cstring>
#include "include/branch_sim.h"
#include "include/util.h"

Branch *global_branch;
static char *btrace_file = NULL;
static char *sim_mode = NULL;
void init_btrace(char *btrace_file) {
  if(btrace_file == NULL) panic("need btrace_file to sim");
  global_branch->set_btrace_fp(btrace_file);
  return;
}

static int parse_args(int argc, char *argv[]) {
  const struct option table[] = {
    {"brmode"   , required_argument, NULL, 'm'},
    {"btrace"   , required_argument, NULL, 'b'},
    {"help"     , no_argument      , NULL, 'h'},
    {0          , 0                , NULL, 0 },
  };
  int o;
  while ( (o = getopt_long(argc, argv, "-hb:m:", table, NULL)) != -1) {
    switch (o) {
      case 'm': sim_mode = optarg;                                       break;
      case 'b': btrace_file = optarg;                                    break;
      default:
        printf("Usage: %s [OPTION...] trace [args]\n\n", argv[0]);
        printf("\t-m,--brmode=xxx           set branch sim mode\n");
        printf("\t-m,--btrace=FILE        read btrace FILE to sim\n");
        printf("\n");
        exit(0);
    }
  }
  return 0;
}


void init_sim(int argc, char* argv[]) {
  global_branch = new Branch;
  parse_args(argc,argv);
  if(strcmp(sim_mode,"\0") == 0){
    printf_red("no branch params,use default params to init branch");
  }else{
    global_branch->set_branch_params(sim_mode);
  }
  init_btrace(btrace_file);
  global_branch->init_branch();

  return;
}

void exit_sim(){
  if(global_branch != NULL){
    global_branch->exit_branch();
    delete global_branch;
  }
}

int main(int argc, char* argv[]) {
  init_sim(argc,argv);
  global_branch->run_sim(-1);
  global_branch->display();
  exit_sim();
}