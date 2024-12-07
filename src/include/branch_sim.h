#ifndef __CACHE_MEM_H__
#define __CACHE_MEM_H__
#include <cstdint>
#include <cstdio>
#include <cstring>
#include "common.h"
#include "util.h"
enum branch_mode_t{
  STATIC_JUMP, STATIC_NO_JUMP
};
enum branch_type_t{
  TYPE_JALR, TYPE_JAL, TYPE_BR //TODO:还有jr伪指令，待添加
};

typedef struct StaticJumpCount{
  uint64_t br_miss_predict_count  ; //branch miss predict的次数
  uint64_t jalr_miss_predict_count; //branch miss predict的次数
  uint64_t jal_miss_predict_count ; //branch miss predict的次数
}static_jump_count_t;

typedef struct StaticNoJumpCount{
  uint64_t br_miss_predict_count  ; //branch miss predict的次数
  uint64_t jalr_miss_predict_count; //branch miss predict的次数
  uint64_t jal_miss_predict_count ; //branch miss predict的次数
}static_no_jump_count_t;


class Branch{
private:
  branch_mode_t branch_mode = STATIC_JUMP; 
  FILE *btrace_fp;
  uint64_t total_count = 0; //总共的次数
  uint64_t br_count = 0; //branch类型的次数
  uint64_t jalr_count = 0; //jalr类型的次数
  uint64_t jal_count = 0; //jar类型的次数
  static_jump_count_t static_jump={
    .br_miss_predict_count = 0,
    .jalr_miss_predict_count = 0,
    .jal_miss_predict_count = 0
  };
  static_no_jump_count_t static_no_jump{
    .br_miss_predict_count = 0,
    .jalr_miss_predict_count = 0,
    .jal_miss_predict_count = 0
  };

  void jump(branch_type_t type,addr_t pc,addr_t dnpc,bool result);
  bool scan_traces();
public:
  void init_branch();
  void exit_branch();
  void run_sim(uint64_t n);
  void display();
  branch_type_t get_branch_type(char *type){
    if(strcmp(type,"jalr") == 0)     return TYPE_JALR;
    else if(strcmp(type,"jal") == 0) return TYPE_JAL;
    else if(strcmp(type,"br") == 0)  return TYPE_BR;
    else panic("unknown type");
  }
  void set_branch_params(char *mode){
    if(strcmp(mode,"STATIC_JUMP") == 0) branch_mode = STATIC_JUMP;
    else if(strcmp(mode,"STATIC_NO_JUMP") == 0) branch_mode = STATIC_NO_JUMP;
    else panic("branch_mode is not valid");
  }
  void set_btrace_fp(char *btrace_file){
    btrace_fp = fopen(btrace_file,"r");
    if(btrace_fp == NULL) panic("btrace_file is NULL");
    printf("The file is %s\n", btrace_file);
  }
};

extern Branch *global_branch;
#endif /* __CACHE_MEM_H__ */