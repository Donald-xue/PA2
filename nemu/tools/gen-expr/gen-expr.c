#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>

// this should be enough
static char buf[65536] = {};
static char code_buf[65536 + 128] = {}; // a little larger than `buf`
static char *code_format =
"#include <stdio.h>\n"
"int main() { "
"  unsigned result = %s; "
"  printf(\"%%u\", result); "
"  return 0; "
"}";

uint32_t choose(uint32_t n){
	int seed = time(0);
	srand(seed);
	int num = rand() % n;
	return num;
}

uint32_t gen_num(){
	uint32_t i;
	i = choose(10);
	sprintf(buf, "%u", i);
	return i;
}

void gen(int lr){
	sprintf(buf, "%c", lr);
}

void gen_rand_op(){
	int seed = time(0);
	srand(seed);
	int i = rand() % 4;
	switch(i){
		case 0: sprintf(buf, "%c", 43); break;
		case 1: sprintf(buf, "%c", 45); break;
		case 2: sprintf(buf, "%c", 42); break;
		case 3: sprintf(buf, "%c", 47); break;
	}
}

static void gen_rand_expr() {
  buf[0] = '\0';
  switch(choose(3)){
  }
}

int main(int argc, char *argv[]) {
  int seed = time(0);
  srand(seed);
  int loop = 1;
  if (argc > 1) {
    sscanf(argv[1], "%d", &loop);
  }
  int i;
  for (i = 0; i < loop; i ++) {
    gen_rand_expr();

    sprintf(code_buf, code_format, buf);

    FILE *fp = fopen("/tmp/.code.c", "w");
    assert(fp != NULL);
    fputs(code_buf, fp);
    fclose(fp);

    int ret = system("gcc /tmp/.code.c -o /tmp/.expr");
    if (ret != 0) continue;

    fp = popen("/tmp/.expr", "r");
    assert(fp != NULL);

    int result;
    fscanf(fp, "%d", &result);
    pclose(fp);

    printf("%u %s\n", result, buf);
  }
  return 0;
}
