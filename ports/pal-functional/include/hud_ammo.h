// src/hud_ammo.h
#ifndef HUD_AMMO_H
#define HUD_AMMO_H

#include "types.h"
#include <stdint.h>  // uintptr_t
#include <stdarg.h>  // va_list

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------------------------------------------
 * External engine / SDK functions (defined in another unit of the
 * project). Their original header is not available, so they are declared
 * here so the compiler knows their real signature.
 * ------------------------------------------------------------------------ */
void  sys_assert_dispatch(const char* p_file, s32 line, const char* p_assertion,
          long p4, long p5, long p6, long p7, long p8); // Signature confirmed against its real definition in kernel_sys.c (not variadic)
s32   txt_vsnprintf_internal(char* p_dest_buffer, const char* p_format_str, va_list args_list);

/* Declared but currently unused in hud_ammo.c; kept in case
 * another unit of the project needs them. Otherwise they can be
 * removed safely. */
void* hud_allocate_or_get_node(int* source, ...);
u32   hud_initialize_subsystem(u32 size, long address);

/* ------------------------------------------------------------------------
 * hud_ammo.c functions used before their own definition in the .c
 * (hence the prototypes). The signature here is the one of the real
 * DEFINITION; "MISMATCH" comments mark cases where the original
 * prototype in the .c did not agree with the definition.
 * ------------------------------------------------------------------------ */

/* Inventory / weapons */
u8    inv_get_active_weapon_id(void);
s32   inv_count_unlocked_weapons(void);
s32   inv_get_quick_select_remaining_space(void);

void  inv_set_weapon_slot_data(u32 p_asset_ptr, u32 current_ammo, u32 max_ammo, u32 weapon_id,
          u32 experience_val, void* p_matrix_base, s32 slot_index);
void  inv_update_weapon_visual_pointers(u32 p_primary_asset, u32 p_secondary_asset,
          void* p_array_base, s32 slot_index);
void  inv_set_extended_ammo_slot_data(u32 ammo_type, u32 current_ammo, u32 max_ammo,
          u32 upgrade_state, void* p_matrix_base, s32 slot_index, s32 group_index);

void  inv_set_weapon_inventory_visibility(u32* p_inventory_base, long visibility_state);
void  inv_set_quick_select_open_state(u32* p_inventory_base, u32 open_state);   // MISMATCH: antes "long state"
void  inv_set_animation_factor(u32* p_inventory_base, f32 animation_val);
void  inv_set_active_weapon_slot(u32* p_inventory_base, u32 slot_index);        // MISMATCH: antes "s32 slot"
void  inv_set_weapon_inventory_mode(u32* p_inventory_base, u32 inventory_mode);
void  inv_reset_weapon_inventory(void* p_inventory_base);                      // MISMATCH: antes "u32* p_inventory_base"

/* HUD widgets */
void  hud_set_widget_position_2d(u32* p_widget, s32 x_coord, s32 y_coord);
void  hud_set_widget_scale_y(u32* p_widget, s32 y_scale);
void  hud_set_widget_render_mode_alt(u32* p_widget, u32 render_flags);
void  hud_set_widget_color_alt(u32* p_widget, u32 color_rgba);
void  hud_set_widget_context_2d_ext(u32* p_widget, u32 val_z, u32 val_w);
void  hud_set_widget_context_2d(u32* p_widget, u32 val_x, u32 val_y);
void  hud_set_widget_visibility(u32* p_widget, long enable);
void  hud_update_widget_context(u32* p_widget, u32* p_new_resource);

void  hud_init_slider_widget(u32* p_widget, u32 value_id, u32 p_data_source, uintptr_t asset_name_ptr,
          uintptr_t p_hud_pool, long p6, long p7, long p8);
void  hud_link_widget_text(u32* p_widget, const char* text_ptr, long param_3,
          long p4, long p5, long p6, long p7, long p8);
void  hud_init_meter_widget(u32* p_widget, const char* text_ptr, long p_hud_pool,
          long p4, long p5, long p6, long p7, long p8);
void  hud_register_widget_asset(u32* p_widget, const char* asset_name_ptr, long p_hud_pool,
          long p4, long p5, long p6, long p7, long p8); // MISMATCH: previously variadic and with "void*" as the first parameter
void  hud_clear_widget_matrices(u32* p_widget_transform, const char* text_ptr, long p_hud_pool,
          long p4, long p5, long p6, long p7, long p8); // MISMATCH: there was also an incompatible variadic redeclaration
void inv_set_weapon_inventory_transition_flag(u32* p_inventory_base, u32 transition_flag);

/* Memory / node pool */
int*  hud_allocate_node(int* p_hud_pool, long p2, long p3, long p4, long p5, long p6, long p7, long p8);
void  hud_free_node(int* p_hud_pool, int* p_node_to_free);
void* core_identity_stub(long param_1, void* p_node);
void* ee_memset(void* dest, u8 value, u32 size); // MISMATCH: previously also declared with "s32 value"

void hud_set_state_from_lookup(u8* p_hudState, u8* table_ptr, s32 target_id);
void math_set_vector4(u32 val1, u32 val2, u32 val3, u32 val4, u32* p_targetDestination);

#ifdef __cplusplus
}
#endif

#endif // HUD_AMMO_H
