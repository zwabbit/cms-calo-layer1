#ifndef ECL_CONFIG_H_
#define ECL_CONFIG_H_

#include "xil_types.h"
typedef enum {CLK=0, RST, BX0, L1A} mux_reg_field_t;

void bang_bits(u8 toggle, u32 address);
void ecl_set_muxreg_raw_z(u8 value);
void ecl_set_delay_raw_z(u8 value, int stb_index);
void ecl_set_delay_z(u8 value, int index);
void ecl_set_mux_fields_z(mux_reg_field_t field, u8 value);
void ecl_config_init_z();

#endif
