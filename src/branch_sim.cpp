#include "include/branch_sim.h"
#include "include/common.h"
#include "include/util.h"
#include <cstdint>
#include <cstdio>
#include <cstdlib>

void Branch::init_branch(){

}

void Branch::exit_branch(){

}

void Branch::jump(branch_type_t type,addr_t pc,addr_t dnpc,bool result){
  total_count ++;
  if(type == TYPE_BR  ) br_count ++;
  if(type == TYPE_JALR) jalr_count ++;
  if(type == TYPE_JAL ) jal_count ++;
  //留给未来的开发空间
  //NOTE:每一次可以把所有算法的结果都算出来
  //NOTE:STATIC_JUMP
  if(type == TYPE_BR){
    if(result == 0){
      static_jump.br_miss_predict_count ++;
    }
  }
  //NOTE:STATIC_NO_JUMP
  if(type == TYPE_BR){
    if(result == 1){
      static_no_jump.br_miss_predict_count ++;
    }
  }

  return;
}

void Branch::run_sim(uint64_t n){
  addr_t pc;
  addr_t dnpc;
  int result;
  char type[16];
  branch_type_t branch_type;
  // addr_t pc;
  for(uint64_t i = 0; i < n; i++){
    if(fscanf(btrace_fp,"type:%s pc:%x dnpc:%x result:%d\n", type,&pc, &dnpc, &result)==EOF){
      printf_green("sim end\n");
      fclose(btrace_fp);
      break;
    }
    branch_type = get_branch_type(type);
    jump(branch_type, pc, dnpc, result);
  }  
  return;
}

void Branch::display(){
  printf("====branch counts====\n");
  printf("br:%ld jal:%ld jalr:%ld \n\
total:%ld\n",
  br_count, jal_count, jalr_count, total_count);
  //NOTE:STATIC_JUMP
  printf("==== static_jump ====\n");
  printf("br_miss_predict_count:%ld\n",static_jump.br_miss_predict_count);
  printf("br_miss_predict_rate:%.3f\n",(double)static_jump.br_miss_predict_count/br_count);
  //NOTE:STATIC_NO_JUMP
  printf("===static_no_jump === \n");
  printf("br_miss_predict_count:%ld\n",static_no_jump.br_miss_predict_count);
  printf("br_miss_predict_rate:%.3f\n",(double)static_no_jump.br_miss_predict_count/br_count);
  return;
}