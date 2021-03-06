#include "xio.h"
#include "cfglinks.h"

typedef struct {
  int regaddr;
  u32 regval;
} cfglinks_mem;

#define ENDOFTBL			(-1)


cfglinks_mem playbackRAM[]={
  {0x10000000, 0x030201bc},
  {0x10000004, 0x07060504},
  {0x10000008, 0x0b0a0908},
  {0x1000000c, 0x0f0e0d0c},
  {0x10001000, 0x131211bc},
  {0x10001004, 0x17161514},
  {0x10001008, 0x1b1a1918},
  {0x1000100c, 0x1f1e1d1c},
  {0x10002000, 0x232221bc},
  {0x10002004, 0x27262524},
  {0x10002008, 0x2b2a2928},
  {0x1000200c, 0x2f2e2d2c},
  {0x10003000, 0x333231bc},
  {0x10003004, 0x37363534},
  {0x10003008, 0x3b3a3938},
  {0x1000300c, 0x3f3e3d3c},
  {0x10004000, 0x434241bc},
  {0x10004004, 0x47464544},
  {0x10004008, 0x4b4a4948},
  {0x1000400c, 0x4f4e4d4c},
  {0x10005000, 0x535251bc},
  {0x10005004, 0x57565554},
  {0x10005008, 0x5b5a5958},
  {0x1000500c, 0x5f5e5d5c},
  {0x10006000, 0x636261bc},
  {0x10006004, 0x67666564},
  {0x10006008, 0x6b6a6968},
  {0x1000600c, 0x6f6e6d6c},
  {0x10007000, 0x737271bc},
  {0x10007004, 0x77767574},
  {0x10007008, 0x7b7a7978},
  {0x1000700c, 0x7f7e7d7c},
  {0x10008000, 0x838281bc},
  {0x10008004, 0x87868584},
  {0x10008008, 0x8b8a8988},
  {0x1000800c, 0x8f8e8d8c},
  {0x10009000, 0x939291bc},
  {0x10009004, 0x97969594},
  {0x10009008, 0x9b9a9998},
  {0x1000900c, 0x9f9e9d9c},
  {0x1000A000, 0xA3A2A1bc},
  {0x1000A004, 0xA7A6A5A4},
  {0x1000A008, 0xAbAaA9A8},
  {0x1000A00c, 0xAfAeAdAc},
  {0x1000B000, 0xb3b2b1bc},
  {0x1000B004, 0xb7b6b5b4},
  {0x1000B008, 0xbbbab9b8},
  {0x1000B00c, 0xbfbebdbc},
  {0x1000c000, 0x030201bc},
  {0x1000c004, 0x07060504},
  {0x1000c008, 0x0b0a0908},
  {0x1000c00c, 0x0f0e0d0c},
  {0x1000d000, 0x131211bc},
  {0x1000d004, 0x17161514},
  {0x1000d008, 0x1b1a1918},
  {0x1000d00c, 0x1f1e1d1c},
  {0x1000e000, 0x232221bc},
  {0x1000e004, 0x27262524},
  {0x1000e008, 0x2b2a2928},
  {0x1000e00c, 0x2f2e2d2c},
  {0x1000f000, 0x333231bc},
  {0x1000f004, 0x37363534},
  {0x1000f008, 0x3b3a3938},
  {0x1000f00c, 0x3f3e3d3c},
  {0x10010000, 0x434241bc},
  {0x10010004, 0x47464544},
  {0x10010008, 0x4b4a4948},
  {0x1001000c, 0x4f4e4d4c},
  {0x10011000, 0x535251bc},
  {0x10011004, 0x57565554},
  {0x10011008, 0x5b5a5958},
  {0x1001100c, 0x5f5e5d5c},
  {0x10012000, 0x636261bc},
  {0x10012004, 0x67666564},
  {0x10012008, 0x6b6a6968},
  {0x1001200c, 0x6f6e6d6c},
  {0x10013000, 0x737271bc},
  {0x10013004, 0x77767574},
  {0x10013008, 0x7b7a7978},
  {0x1001300c, 0x7f7e7d7c},
  {0x10014000, 0x838281bc},
  {0x10014004, 0x87868584},
  {0x10014008, 0x8b8a8988},
  {0x1001400c, 0x8f8e8d8c},
  {0x10015000, 0x939291bc},
  {0x10015004, 0x97969594},
  {0x10015008, 0x9b9a9998},
  {0x1001500c, 0x9f9e9d9c},
  {0x10016000, 0xA3A2A1bc},
  {0x10016004, 0xA7A6A5A4},
  {0x10016008, 0xAbAaA9A8},
  {0x1001600c, 0xAfAeAdAc},
  {ENDOFTBL, 0}
};


cfglinks_mem config_orsc_ctp6_links[]={
  {0x100f0014, 0x3},
  {0x100f001C, 0x0},
  {0x100f0024, 0x0},
  {0x100f0028, 0x0},
  {0x100f002C, 0x3},
  {0x100f0030, 0xFFFFFF},
  {0x100f0034, 0x0},
  {0x100f0004, 0x1},
  {0x100f0004, 0x0},
  {0x100f0008, 0x1},
  {0x100f0008, 0x0},
  {0x100f0018, 0xFFFFFF},
  {ENDOFTBL, 0}
};


void write_playbackRAM(void) {
  cfglinks_mem *cl = &playbackRAM[0];
  while(cl->regaddr != ENDOFTBL){
    XIo_Out32(cl->regaddr, cl->regval);
    cl++;
  }
}

void write_config_orsc_ctp6_links(void) {
  cfglinks_mem *cl = &config_orsc_ctp6_links[0];
  while(cl->regaddr != ENDOFTBL){
    XIo_Out32(cl->regaddr, cl->regval);
    cl++;
  }
}
