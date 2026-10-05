// src/hud_ammo.c
#include "hud_ammo.h"
#include <math.h> // Required to call sinf() natively

// Physical address of the global weapon state table in PS2 RAM
#define INVENTORY_WEAPONS_DATA_PTR     ((const u8*)0x0019B2F8)
#define MAX_WEAPONS_LIMIT_CONFIG       28 // 0x1B + 1 base slot positions

// Global PS2 RAM address that stores the ID of the active weapon
#define GLOBAL_ACTIVE_WEAPON_ID_PTR    ((const u8*)0x001396C8)

// Definition of the main canvas static resources
#define RECURSO_HUD_CANVAS          ((const char*)0x001AE6D8) // "HudBase"
#define RECURSO_HEALTH_OUTLINE      ((const char*)0x001AE6E8) // "HealthBarOutline"
#define RECURSO_HEALTH_FILL         ((const char*)0x001AE6F8) // "HealthBarFill"

// Definitions of the extended set of HUD static resources
#define RECURSO_WEAPON_NAME         ((const char*)0x001AE700) // "WeaponName"
#define RECURSO_WEAPON_XP           ((const char*)0x001AE710) // "WeaXP"
#define RECURSO_AMMO_ICON           ((const char*)0x001AE720) // "AmmoIcon"
#define RECURSO_AMMO_ICON_BACK      ((const char*)0x001AE730) // "AmmoIconBack"
#define RECURSO_BOLT_TEXT           ((const char*)0x001AE740) // "BoltText"
#define RECURSO_BOLT_ICON           ((const char*)0x001AE750) // "BoltIcon"
#define RECURSO_QSEL_BACK         ((const char*)0x001AE058) // "QSEL_BACK"
#define RECURSO_QSEL_BORD         ((const char*)0x001AE068) // "QSelBI%d"
#define FORMATO_SLOT_RADIAL         ((const char*)0x001AE088) // "QSelBI%d"
#define FORMATO_ICONO_RADIAL    ((const char*)0x001AE098) // "QSelIco%d"

/**
 * @brief Returns the unique identifier (ID) of the weapon the player currently has equipped, in real time.
 * Used by the ammo and HUD rendering subsystem to synchronize the visual counters.
 * Original Ghidra address: 0x002B18D8 (PAL)
 *
 * @return u8 Numeric ID of the active weapon (e.g. 0 = wrench, 1 = Lancer, etc.).
 */
u8 inv_get_active_weapon_id(void) {
	// Returns the global RAM status byte directly
	return *GLOBAL_ACTIVE_WEAPON_ID_PTR;
}

/**
 * @brief Computes the spatial offset or remaining slots in the radial menu from the equipped weapon.
 * Uses the global count and the active ID to coordinate the rotation limits of the Quick Select interface.
 * Original Ghidra address: 0x002B18E8 (PAL)
 *
 * @return s32 Indexed distance or remaining slots (clamped between 0 and 40).
 */
s32 inv_get_quick_select_remaining_space(void) {
	// 1. Retrieve the total of unlocked weapons and the ID of the weapon in hand
	s32 total_weapons = inv_count_unlocked_weapons();
	u8 active_weapon_id = inv_get_active_weapon_id();

	// 2. Compute the differential distance in the selection ring
	s32 remaining_slots = total_weapons - (s32)active_weapon_id;

	// Safeguard rule against negative overflows
	if (remaining_slots < 0) {
		remaining_slots = 0;
	}

	// Apply the standard 40-position (0x28) clamp of the Insomniac HUD
	s32 clamped_offset = 0x28;
	if (remaining_slots < 0x29) {
		clamped_offset = remaining_slots;
	}

	return clamped_offset;
}

/**
 * @brief Counts the total number of valid weapons currently unlocked in the player's inventory.
 * Scans the global data array applying control limits to define the size of the HUD interface.
 * Original Ghidra address: 0x002B1930 (PAL)
 *
 * @return s32 Final number of active weapon slots ready to render (clamped between 0 and 40).
 */
s32 inv_count_unlocked_weapons(void) {
	s32 total_active_elements = 0;
	s32 memory_offset = 0;

	// General loop that sequentially scans the 28 weapon slots of the Insomniac engine
	for (s32 weapon_idx = 1; weapon_idx <= MAX_WEAPONS_LIMIT_CONFIG; weapon_idx++) {
		const u8* p_weapon_bytes = INVENTORY_WEAPONS_DATA_PTR + memory_offset;

		// Each weapon slot stores a contiguous 4-byte block of state flags
		for (s32 byte_idx = 0; byte_idx < 4; byte_idx++) {
			u8 flag_byte = p_weapon_bytes[byte_idx];

			// If the flag holds valid data, it is counted as an active component
			if (flag_byte != 0) {
				total_active_elements++;
			}
		}

		// Compute the indexed memory alignment step for the next slot (weapon_idx * 4)
		memory_offset = weapon_idx * 4;
	}

	// Safeguard rule: the count cannot be below absolute zero
	if (total_active_elements < 0) {
		total_active_elements = 0;
	}

	// Apply a strict mathematical clamp: the retail PS2 HUD supports up to 40 graphic slots (0x28)
	s32 clamped_count = 0x28;
	if (total_active_elements < 0x29) {
		clamped_count = total_active_elements;
	}

	return clamped_count;
}

/**
 * @brief Changes the physical anchor position of the HUD components according to the video mode (4:3 or 16:9/PAL).
 * Injects the corresponding packed pixel coordinates to correct screen distortion.
 * Original Ghidra address: 0x0034EC58 (PAL)
 *
 * @param p_hud_main_struct Base address of the central interface structure (param_1).
 * @param video_mode ID of the active video mode (0 for NTSC/4:3, 1 for PAL/16:9) (param_2).
 */
void hud_update_layout_aspect_ratio(u32* p_hud_main_struct, s32 video_mode) {
	if (p_hud_main_struct == NULL) {
		return;
	}

	// Offset 0x15A4 is index 1385 in 32-bit integers (1385 * 4 = 5540 bytes)
	u8* p_base = (u8*)p_hud_main_struct;
	*(s32*)(p_base + 0x15A4) = video_mode;

	u32* p_sub_widget = (u32*)(p_base + 0x2A0);

	// Case A: standard NTSC / 4:3 mode (values extracted from mask 0x7567)
	if (video_mode == 0) {
		s16 x_pos = 103; // 0x67 in hexadecimal (horizontal pixels)
		s16 y_pos = 117; // 0x75 in hexadecimal (vertical pixels)
		hud_set_widget_position_2d(p_sub_widget, (s32)x_pos, (s32)y_pos);
	}
	// Case B: widescreen 16:9 / PAL mode (values extracted from mask 0xEAA2)
	else if (video_mode == 1) {
		s16 x_pos = 162; // 0xA2 in hexadecimal
		s16 y_pos = 234; // 0xEA in hexadecimal
		hud_set_widget_position_2d(p_sub_widget, (s32)x_pos, (s32)y_pos);
	}
}

/**
 * @brief Configures the capacity multiplier or base modifier of the ammo subsystem at offset 0x18.
 * Original Ghidra address: 0x0034BCE0 (PAL)
 *
 * @param multiplier_val Control value or scale address to inject (param_1).
 * @param p_extended_ammo_struct Base memory address of the ammo substructure (param_2).
 */
void inv_set_ammo_capacity_multiplier(u32 multiplier_val, u32* p_extended_ammo_struct) {
	if (p_extended_ammo_struct != NULL) {
		// Offset 0x18 is index 6 in an array of 32-bit integers (6 * 4 = 24 bytes)
		// Note: Ghidra swapped the argument order in the original decompiler (param_1 is the value, param_2 the pointer)
		p_extended_ammo_struct[0x06] = multiplier_val;
	}
}

/**
 * @brief Returns the pointer to the main transformation (position) vector of a HUD widget.
 * Reads the physical address stored at offset +0 of the structure directly.
 * Original Ghidra address: 0x00337AF0 (PAL)
 *
 * @param p_widget Base address of the interface component structure (param_1).
 * @return f32* Pointer to the widget's position vector (X, Y, Z, W), or NULL if unassigned.
 */
f32* hud_get_widget_position_vector_ptr(u32* p_widget) {
	if (p_widget == NULL) {
		return NULL;
	}

	// Returns the pointer stored at index 0 (+0 bytes) directly
	return (f32*)(*p_widget);
}

/**
 * @brief Configures a state or attribute in an indexed 4-byte burst at offset 0x8C of the ammo subsystem.
 * Original Ghidra address: 0x0034BC70 (PAL)
 *
 * @param p_extended_ammo_struct Base address of the ammo substructure (param_1).
 * @param group_index Index of the slot or group to modify, in 4-byte steps (param_2).
 * @param attribute_val State value, flag or attribute to inject (param_3).
 */
void inv_set_ammo_matrix_group_state(void* p_extended_ammo_struct, s32 group_index, u32 attribute_val) {
	if (p_extended_ammo_struct == NULL) {
		return;
	}

	// Compute the exact offset applying the 4-byte step shifted to 0x8C
	u8* p_target_slot = (u8*)p_extended_ammo_struct + (group_index * 4) + 0x8C;

	// Inject the control value directly
	*(u32*)p_target_slot = attribute_val;
}

/**
 * @brief Configures a group pointer or control index in an indexed 4-byte burst at offset 0x20 of the ammo subsystem.
 * Original Ghidra address: 0x0034BC28 (PAL)
 *
 * @param p_extended_ammo_struct Base address of the ammo substructure (param_1).
 * @param group_index Index of the slot or group to modify, in 4-byte steps (param_2).
 * @param p_group_data Memory address of the data block or parameter to link (param_3).
 */
void inv_set_ammo_matrix_group_ptr(void* p_extended_ammo_struct, s32 group_index, u32 p_group_data) {
	if (p_extended_ammo_struct == NULL) {
		return;
	}

	// Compute the exact offset applying the 4-byte step shifted to 0x20
	u8* p_target_slot = (u8*)p_extended_ammo_struct + (group_index * 4) + 0x20;

	// Inject the address or control value directly
	*(u32*)p_target_slot = p_group_data;
}

/**
 * @brief Initializes to defaults and zeroes the state of the extended ammo array subsystem.
 * Configures the floating-point scale factors (1.0f) and fine frame interpolation (0.0666f) of the HUD.
 * Original Ghidra address: 0x0034BB38 (PAL)
 *
 * @param p_extended_ammo_struct Base address of the ammo substructure (param_1).
 */
void inv_reset_extended_ammo_subsystem(u32* p_extended_ammo_struct) {
	if (p_extended_ammo_struct == NULL) {
		return;
	}

	// 1. Clear the secondary state variables
	p_extended_ammo_struct[4] = 0;
	u32 zero_token = p_extended_ammo_struct[4];

	p_extended_ammo_struct[7] = 0;
	p_extended_ammo_struct[8] = 0;
	p_extended_ammo_struct[9] = 0;
	p_extended_ammo_struct[10] = 0;

	// 2. Inject the scale factors (1.0f) and animation rate (0x3d88882f = 0.0666667f)
	p_extended_ammo_struct[3] = 0x3F800000; // 1.0f
	p_extended_ammo_struct[6] = 0x3D88882F; // 0.0666667f (interpolation rate for 60 FPS)

	p_extended_ammo_struct[0] = 0x3F800000; // X scale = 1.0f
	p_extended_ammo_struct[1] = 0x3F800000; // Y scale = 1.0f
	p_extended_ammo_struct[2] = 0x3F800000; // Z scale = 1.0f

	// 3. Initial bulk clear of the combined three-dimensional grid (slots 0-1, groups 0-1)
	inv_set_extended_ammo_slot_data(zero_token, zero_token, zero_token, zero_token, (void*)p_extended_ammo_struct, 0, 0);
	inv_set_extended_ammo_slot_data(zero_token, zero_token, zero_token, zero_token, (void*)p_extended_ammo_struct, 0, 1);
	inv_set_extended_ammo_slot_data(zero_token, zero_token, zero_token, zero_token, (void*)p_extended_ammo_struct, 1, 0);
	inv_set_extended_ammo_slot_data(zero_token, zero_token, zero_token, zero_token, (void*)p_extended_ammo_struct, 1, 1);
}

/**
 * @brief Configures the ammo statistics in the extended inventory array (multidimensional steps of 0x10 and 0x30).
 * Injects the bullet parameters contiguously, computing the exact offset per group and slot.
 * Original Ghidra address: 0x0034BC38 (PAL)
 *
 * @param ammo_type Assigned ammo ID or type (param_1).
 * @param current_ammo Current number of bullets (param_2).
 * @param max_ammo Maximum magazine capacity (param_3).
 * @param upgrade_state Upgrade state or level of the component (param_4).
 * @param p_matrix_base Base memory address of the inventory structure (param_5).
 * @param slot_index Index of the secondary slot (param_6).
 * @param group_index Index of the upper weapon category or group (param_7).
 */
void inv_set_extended_ammo_slot_data(u32 ammo_type, u32 current_ammo, u32 max_ammo, u32 upgrade_state,
	void* p_matrix_base, s32 slot_index, s32 group_index) {
	if (p_matrix_base == NULL) {
		return;
	}

	// Compute the physical cell address applying the indexed steps of 16 and 48 bytes
	u8* p_data_cell = (u8*)p_matrix_base + (slot_index * 0x10) + (group_index * 0x30);

	// Store the statistics contiguously at the indicated offsets
	*(u32*)(p_data_cell + 0x2C) = ammo_type;     // Ammo ID/type
	*(u32*)(p_data_cell + 0x30) = current_ammo;  // Current bullets
	*(u32*)(p_data_cell + 0x34) = max_ammo;      // Maximum capacity
	*(u32*)(p_data_cell + 0x38) = upgrade_state; // State or multiplier
}

/**
 * @brief Configures a pair of contiguous 32-bit values (X, Y) in the ammo control structure (offset +8).
 * Used by the live update loop to inject burst coordinates or HUD limits.
 * Original Ghidra address: 0x0034D1B0 (PAL)
 *
 * @param val_x First component or control value (param_1).
 * @param val_y Second, contiguous component or control value (param_2).
 * @param p_dest_struct Base address of the containing structure (param_3).
 */
void hud_set_ammo_widget_context_2d(u32 val_x, u32 val_y, void* p_dest_struct) {
	if (p_dest_struct != NULL) {
		// Retrieve the real physical pointer stored at offset +8
		u32** pp_context_target = (u32**)((u8*)p_dest_struct + 8);
		u32* p_context = *pp_context_target;

		if (p_context != NULL) {
			p_context[0] = val_x;   // Stores at offset +0 of the pointed block
			p_context[1] = val_y;   // Stores at offset +4 of the pointed block
		}
	}
}

/**
 * @brief Fully initializes the visual ammo layout and the weapon inventory arrays.
 * Orchestrates widget registration (background, border, text, slider) and configures the base slot statistics.
 * Original Ghidra address: 0x0034BF20 (PAL)
 */
void hud_init_ammo_layout(void* p_hud_main_struct, long param_2, long p_hud_pool, long p4, long p5, long p6, long p7, long p8) {
	u8* p_base = (u8*)p_hud_main_struct;

	// 1. Initial reservation and clearing of transformation nodes in the pool
	*(u32*)(p_base + 0x13C) = (u32)p_hud_pool;
	if (p_hud_pool != 0) {
		int* node0 = hud_allocate_node((int*)p_hud_pool, param_2, p_hud_pool, p4, p5, p6, p7, p8);
		u32* vec0 = (u32*)core_identity_stub(0x10, node0);
		*(u32**)(p_base + 8) = vec0;
		vec0[0] = 0; vec0[1] = 0; vec0[2] = 0; vec0[3] = 0;

		int* node1 = hud_allocate_node((int*)*(u32*)(p_base + 0x13C), 0, 0, 0, 0, 0, 0, 0);
		u32* vec1 = (u32*)core_identity_stub(0x10, node1);
		*(u32**)(p_base + 0x1C8) = vec1;
		vec1[0] = 0; vec1[1] = 0; vec1[2] = 0; vec1[3] = 0;
	}

	// 2. Registration of the ammo visual components (widgets)
	hud_register_widget_asset((u32*)(p_base + 0x10), (const char*)0x001AE668, p_hud_pool, p4, p5, p6, p7, p8); // "AmmoBack"
	hud_register_widget_asset((u32*)(p_base + 0x5C), (const char*)0x001AE678, p_hud_pool, p4, p5, p6, p7, p8); // "AmmoOutline"
	hud_register_widget_asset((u32*)(p_base + 0xA8), (const char*)0x001AE680, p_hud_pool, p4, p5, p6, p7, p8); // "AmmoText"

	// 3. Initialization and aesthetic configuration of the bullet slider/meter
	u32* p_slider = (u32*)(p_base + 0xF4);
	hud_init_slider_widget(p_slider, 0x92, 0, (const char*)0x001AE688, p_hud_pool, p4, p5, p6); // "AmmoBar"

	hud_set_widget_context_2d(p_slider, 0x8049c1ff, 0x80001eff);
	hud_set_widget_context_2d_ext(p_slider, 0x50f0c070, 0x50f0c070);
	hud_set_widget_color_alt(p_slider, 0x60442d00);
	hud_set_widget_render_mode_alt(p_slider, 100);

	// Vertical Y scaling of the dynamic meter
	f32 scale_y = 1.5f; // Value estimated from the original interpolation
	hud_set_widget_scale_y(p_slider, (s32)scale_y);

	// 4. Sequential configuration of the weapon inventory slots (layer A)
	void* p_inv_a = (void*)(p_base + 0x140);
	inv_reset_weapon_inventory(p_inv_a);
	inv_set_weapon_inventory_mode(p_inv_a, 2);
	inv_set_active_weapon_slot(p_inv_a, (p_base + 0x1CC));
	inv_set_animation_factor(p_inv_a, 0.005f);
	inv_set_quick_select_open_state(p_inv_a, 2);
	inv_set_weapon_slot_data(0, 0x80f0c070, 0x42480000, 0, 0, p_inv_a, 0); // Capacity 50.0f
	inv_update_weapon_visual_pointers(0, 0, p_inv_a, 0);
	inv_set_weapon_inventory_visibility(p_inv_a, 1);

	// 5. Sequential configuration of the weapon inventory slots (layer B)
	void* p_inv_b = (void*)(p_base + 0x1D4);
	inv_reset_weapon_inventory(p_inv_b);
	inv_set_weapon_inventory_mode(p_inv_b, 3);
	inv_set_weapon_slot_data(0, 0x442d00, 0, 0, 0, p_inv_b, 0);
	inv_set_weapon_slot_data(0x3E99999A, 0x60442d00, 0, 0, 0, p_inv_b, 1);
	inv_update_weapon_visual_pointers(0, 0x40000000, p_inv_b, 1);
	inv_set_quick_select_open_state(p_inv_b, 0);
	inv_set_active_weapon_slot(p_inv_b, (p_base + 0x1CC));
	inv_set_weapon_inventory_visibility(p_inv_b, -1);

	// Initialization of the final secondary control flags
	*(u32*)(p_base + 0x3F4) = 0;
	*(u32*)(p_base + 0x400) = 0xFFFFFFFF;
	*(u32*)(p_base + 0x3F8) = 0;
}

/**
 * @brief Swaps the value of the inventory animation lock (offset 0x28) and returns its previous state.
 * Used by the engine to release transitions and check synchronization states in the HUD.
 * Original Ghidra address: 0x0034B790 (PAL)
 *
 * @param p_inventory_base Base memory address of the global inventory structure (param_1).
 * @param new_lock_state New control value injected into the flag (param_2).
 * @return u32 The previous state of the flag before it was overwritten.
 */
u32 inv_swap_animation_lock(u32* p_inventory_base, u32 new_lock_state) {
	if (p_inventory_base == NULL) {
		return 0;
	}

	// Offset 0x28 is index 10 in an array of 32-bit integers (10 * 4 = 40 bytes)
	u32 old_lock_state = p_inventory_base[0x0A];

	// Overwrite the flag with the requested new state
	p_inventory_base[0x0A] = new_lock_state;

	// Return the old value to the rendering pipeline
	return old_lock_state;
}

/**
 * @brief Configures and dispatches the visibility transition (fade in/out) of the weapon inventory interface.
 * Sets the floating-point animation limits at offset 0x18 depending on the requested state.
 * Original Ghidra address: 0x0034B7F8 (PAL)
 *
 * @param p_inventory_base Base memory address of the global inventory structure (param_1).
 * @param visibility_state New visibility state (1 for visible, 0 to hide) (param_2).
 */
void inv_set_weapon_inventory_visibility(u32* p_inventory_base, long visibility_state) {
	if (p_inventory_base == NULL) {
		return;
	}

	// Offset 0x1C is index 7 in an array of 32-bit integers (7 * 4 = 28 bytes)
	p_inventory_base[0x07] = (s32)visibility_state;

	// Offset 0x28 is index 10 (10 * 4 = 40 bytes), controls the interpolation lock
	if (p_inventory_base[0x0A] == 0) {
		// If the state is 1 (appear), start the animation from 0.0f
		if (visibility_state == 1) {
			p_inventory_base[0x06] = 0; // Offset 0x18 (index 6)
		}
		// Otherwise (hide), start the fade-out animation from 1.0f (0x3f800000)
		else {
			p_inventory_base[0x06] = 0x3F800000; // Offset 0x18 (index 6)
		}

		p_inventory_base[0x0A] = 1; // Sets the animation-cycle start flag
	}
}

/**
 * @brief Configures the visibility or open state of the quick-select radial menu at offset 0x20.
 * Original Ghidra address: 0x0034B838 (PAL)
 *
 * @param p_inventory_base Base memory address of the global inventory structure (param_1).
 * @param open_state New control value (1 for open/visible, 0 for closed/hidden) (param_2).
 */
void inv_set_quick_select_open_state(u32* p_inventory_base, u32 open_state) {
	if (p_inventory_base != NULL) {
		// Offset 0x20 is index 8 in an array of 32-bit integers (8 * 4 = 32 bytes)
		p_inventory_base[0x08] = open_state;
	}
}

/**
 * @brief Configures the interpolation speed or animation factor of the interface at inventory offset 0x24.
 * Original Ghidra address: 0x0034B840 (PAL)
 *
 * @param animation_val Floating-point or control factor for the interface speed (param_1).
 * @param p_inventory_base Base memory address of the global inventory structure (param_2).
 */
void inv_set_animation_factor(u32* p_inventory_base, f32 animation_val) {
	if (p_inventory_base != NULL) {
		// Offset 0x24 is index 9 in an array of 32-bit integers (9 * 4 = 36 bytes)
		p_inventory_base[9] = *(u32*)&animation_val;
	}
}

/**
 * @brief Configures the slot index of the active or selected weapon at inventory offset 0x80.
 * Original Ghidra address: 0x0034B788 (PAL)
 *
 * @param p_inventory_base Base memory address of the global inventory structure (param_1).
 * @param slot_index Slot index or identification of the weapon to equip (param_2).
 */
void inv_set_active_weapon_slot(u32* p_inventory_base, u32 slot_index) {
	if (p_inventory_base != NULL) {
		// Offset 0x80 is index 32 in an array of 32-bit integers (32 * 4 = 128 bytes)
		p_inventory_base[0x20] = slot_index;
	}
}

/**
 * @brief Configures the transition flag or loading state of the weapon inventory at offset 0x2C.
 * Original Ghidra address: 0x0034B758 (PAL)
 *
 * @param p_inventory_base Base memory address of the global inventory structure (param_1).
 * @param transition_flag New control value or state flag to inject (param_2).
 */
void inv_set_weapon_inventory_transition_flag(u32* p_inventory_base, u32 transition_flag) {
	if (p_inventory_base != NULL) {
		// Offset 0x2C is index 11 in an array of 32-bit integers (11 * 4 = 44 bytes)
		p_inventory_base[0x0B] = transition_flag;
	}
}

/**
 * @brief Configures the display mode or secondary state of the weapon inventory at offset 0x84.
 * Original Ghidra address: 0x0034B7F0 (PAL)
 *
 * @param p_inventory_base Base memory address of the global inventory structure (param_1).
 * @param inventory_mode New state value or game mode to inject (param_2).
 */
void inv_set_weapon_inventory_mode(u32* p_inventory_base, u32 inventory_mode) {
	if (p_inventory_base != NULL) {
		// Offset 0x84 is index 33 in an array of 32-bit integers (33 * 4 = 132 bytes)
		p_inventory_base[33] = inventory_mode;
	}
}

/**
 * @brief Resets the global weapon inventory state to its factory defaults.
 * Clears the dynamic properties and initializes the first slots of the arsenal array.
 * Original Ghidra address: 0x0034B698 (PAL)
 *
 * @param p_inventory_base Base memory address of the global inventory structure (param_1).
 */
void inv_reset_weapon_inventory(void* p_inventory_base) {
	if (p_inventory_base == NULL) {
		return;
	}

	u8* p_inv = (u8*)p_inventory_base;

	// 1. Clear the state flags and initialize the base variables
	*(u32*)(p_inv + 0x18) = 0;
	*(u32*)(p_inv + 0x28) = 0;
	u32 zero_token = *(u32*)(p_inv + 0x18);

	*(u32*)(p_inv + 0x2C) = 0;
	*(u32*)(p_inv + 0x80) = 0;

	// 2. Inject the fine animation factor (0x3ba3d70a = 0.005f)
	*(f32*)(p_inv + 0x24) = 0.005f;

	// 3. Conditional initialization of the array slots with the helper functions
	inv_set_weapon_slot_data(zero_token, zero_token, zero_token, zero_token, zero_token, p_inventory_base, 0);

	// Inject the floating-point value 1.0f (0x3f800000) in bulk into slot 1
	u32 float_one_token = 0x3F800000;
	inv_set_weapon_slot_data(float_one_token, float_one_token, float_one_token, float_one_token, float_one_token, p_inventory_base, 1);

	// 4. Configuration of secondary flags and initialization of visual pointers
	*(u32*)(p_inv + 0x84) = 2; // Default secondary selector or counter
	inv_update_weapon_visual_pointers(zero_token, zero_token, p_inventory_base, 0);

	*(u32*)(p_inv + 0x20) = 0;
	*(u32*)(p_inv + 0x1C) = 1; // Initial activation or visibility flag
}

/**
 * @brief Updates the pointers to the graphic resources (assets/textures) linked to a specific arsenal slot.
 * Writes the addresses in indexed 4-byte bursts for the inventory rendering pipeline.
 * Original Ghidra address: 0x0034B7D8 (PAL)
 *
 * @param p_primary_asset Pointer to the weapon's base graphic resource or main texture (param_1).
 * @param p_secondary_asset Pointer to the secondary graphic resource or interface mask (param_2).
 * @param p_array_base Base memory address of the visual pointer array (param_3).
 * @param slot_index Index of the slot or column to modify (param_4).
 */
void inv_update_weapon_visual_pointers(u32 p_primary_asset, u32 p_secondary_asset,
	void* p_array_base, s32 slot_index) {
	if (p_array_base == NULL) {
		return;
	}

	// Compute the exact pointer to the selected slot (4-byte step)
	u32* p_slot_target = (u32*)((u8*)p_array_base + slot_index * 4);

	p_slot_target[0] = p_primary_asset;   // Injects at base offset +0
	p_slot_target[3] = p_secondary_asset; // Injects at offset +12 bytes (index 3 as u32)
}

/**
 * @brief Initializes and injects the statistical and visual parameters of a weapon in the indexed inventory array.
 * Computes the contiguous 16-byte (0x10) offsets that store ammo, IDs and HUD references.
 * Original Ghidra address: 0x0034B7A0 (PAL)
 *
 * @param p_asset_ptr Pointer to the name or visual resource of the weapon widget (param_1).
 * @param current_ammo Current number of bullets (param_2).
 * @param max_ammo Maximum magazine capacity (param_3).
 * @param weapon_id Unique identifier of the weapon/ammo type (param_4).
 * @param experience_val Experience progress or level of the weapon (param_5).
 * @param p_matrix_base Base memory address of the inventory data table (param_6).
 * @param slot_index Index of the slot or column to modify (param_7).
 */
void inv_set_weapon_slot_data(u32 p_asset_ptr, u32 current_ammo, u32 max_ammo, u32 weapon_id,
	u32 experience_val, void* p_matrix_base, s32 slot_index) {

	// Compute the physical address of the weapon data structure (16-byte step)
	u8* p_data_row = (u8*)p_matrix_base + ((s32)p_matrix_base + slot_index * 0x10);

	// Inject the dynamic magazine statistics contiguously at offsets +0x30
	*(u32*)(p_data_row + 0x30) = current_ammo;    // Current ammo
	*(u32*)(p_data_row + 0x34) = max_ammo;        // Maximum ammo
	*(u32*)(p_data_row + 0x38) = weapon_id;       // Weapon ID
	*(u32*)(p_data_row + 0x3C) = experience_val;  // XP value / modifier

	// Link the widget's graphic resource or asset in the visual pointer section (+0x70)
	u8* p_visual_row = (u8*)p_matrix_base + slot_index * 4;
	*(u32*)(p_visual_row + 0x70) = p_asset_ptr;
}

/**
 * @brief Configures in isolation the vertical Y component (scale/orientation) of a widget's secondary vector.
 * Converts the integer to floating point and injects it at offset +4 of the vector at offset +4.
 * Original Ghidra address: 0x00338A20 (PAL)
 *
 * @param p_widget Base address of the interface component structure (param_1).
 * @param y_scale Integer scale factor or vertical dimension converted to floating point (param_2).
 */
void hud_set_widget_scale_y(u32* p_widget, s32 y_scale) {
	if (p_widget != NULL) {
		// Offset +4 of the base structure stores the pointer to the transformation vector
		f32** pp_vector_target = (f32**)((u8*)p_widget + 4);
		f32* p_vector = *pp_vector_target;

		if (p_vector != NULL) {
			// Offset +4 inside the vector itself is the Y component (index 1 as f32)
			p_vector[1] = (f32)y_scale;
		}
	}
}

/**
 * @brief Configures the render flags or blend mode (alternate alias of hud_set_widget_render_mode).
 * Writes directly to the graphics control property at offset 0x40.
 * Original Ghidra address: 0x003388E8 (PAL)
 *
 * @param p_widget Base address of the interface component structure (param_1).
 * @param render_flags Bit mask with the drawing and transparency options (param_2).
 */
void hud_set_widget_render_mode_alt(u32* p_widget, u32 render_flags) {
	if (p_widget != NULL) {
		// Offset 0x40 is index 0x10 in an array of 32-bit integers (16 * 4 = 64 bytes)
		p_widget[0x10] = render_flags;
	}
}

/**
 * @brief Configures the colour or opacity of a visual component (alternate alias of hud_set_widget_color).
 * Writes directly to the property at offset 0x44 of the structure.
 * Original Ghidra address: 0x00338A38 (PAL)
 *
 * @param p_widget Base address of the interface component structure (param_1).
 * @param color_rgba 32-bit value encoding the colour and opacity (param_2).
 */
void hud_set_widget_color_alt(u32* p_widget, u32 color_rgba) {
	if (p_widget != NULL) {
		// Offset 0x44 is index 17 in an array of 32-bit integers (17 * 4 = 68 bytes)
		p_widget[0x11] = color_rgba;
	}
}

/**
 * @brief Initializes a widget structure intended for logical sliders or value selectors in the interface.
 * Configures the initial scale parameters and sets the default upper limit to 100%.
 * Original Ghidra address: 0x003380E8 (PAL)
 *
 * @param p_widget Base address of the interface widget structure (param_1).
 * @param value_id Identifier or initial value assigned to the slider (param_2).
 * @param p_data_source Control pointer or data source of the element (param_3).
 */
void hud_init_slider_widget(u32* p_widget, u32 value_id, u32 p_data_source, uintptr_t asset_name_ptr,
	uintptr_t p_hud_pool, long p6, long p7, long p8) {

	// 1. Call the base constructor to initialize the spatial matrices
	// The parameter casts are adjusted to match the original math call
	hud_clear_widget_matrices(p_widget, (void*)asset_name_ptr, (void*)p_hud_pool,
		(long)asset_name_ptr, p_hud_pool, p6, p7, p8);

	// 2. Inject the state parameters and control links
	p_widget[0x0F] = value_id;      // Offset 0x3C
	p_widget[0x0D] = p_data_source; // Offset 0x34

	// 3. Set the engine's default initial limits and flags
	p_widget[0x11] = 0x80000000;    // Offset 0x44 (update mask)
	p_widget[0x10] = 100;           // Offset 0x40 (maximum limit of 100%)
}

/**
 * @brief Configures the second pair of 32-bit values (offsets +8 and +12) in the widget context structure (offset 0x0C).
 * Usually used to define UV clipping limits or the secondary dimensions of a texture.
 * Original Ghidra address: 0x00338190 (PAL)
 *
 * @param p_widget Base address of the interface widget structure (param_1).
 * @param val_z Third component or control value (param_2).
 * @param val_w Fourth, contiguous component or control value (param_3).
 */
void hud_set_widget_context_2d_ext(u32* p_widget, u32 val_z, u32 val_w) {
	if (p_widget != NULL) {
		// Offset 0x0C is index 3 in 32-bit integers (3 * 4 = 12 bytes)
		u32** pp_context_target = (u32**)((u8*)p_widget + 0x0C);
		u32* p_context = *pp_context_target;

		if (p_context != NULL) {
			p_context[2] = val_z;   // Stores at offset +8 of the context block
			p_context[3] = val_w;   // Stores at offset +12 of the context block
		}
	}
}

/**
 * @brief Configures a pair of contiguous 32-bit values (X, Y) in the widget context structure (offset 0x0C).
 * Original Ghidra address: 0x00338178 (PAL)
 *
 * @param p_widget Base address of the interface widget structure (param_1).
 * @param val_x First component or control value (param_2).
 * @param val_y Second, contiguous component or control value (param_3).
 */
void hud_set_widget_context_2d(u32* p_widget, u32 val_x, u32 val_y) {
	if (p_widget != NULL) {
		// Offset 0x0C is index 3 in 32-bit integers (3 * 4 = 12 bytes)
		u32** pp_context_target = (u32**)((u8*)p_widget + 0x0C);
		u32* p_context = *pp_context_target;

		if (p_context != NULL) {
			p_context = val_x;   // Stores at offset +0 of the context block
			p_context = val_y;   // Stores at offset +4 of the context block
		}
	}
}

/**
 * @brief Dynamically updates the transformation vector assigned at offset +4 of a widget.
 * Automatically frees the previous node with hud_free_node if it detects a position or scale change.
 * Original Ghidra address: 0x00337B90 (PAL)
 *
 * @param p_widget Base address of the interface component structure (param_1).
 * @param p_new_vector Pointer to the new 4-component math vector to render (param_2).
 */
void hud_update_widget_vector(u32* p_widget, u32* p_new_vector) {
	if (p_widget == NULL) {
		return;
	}

	// Offset +4 is index 1 in an array of 32-bit integers (1 * 4 = 4 bytes)
	u32** pp_current_vector = (u32**)((u8*)p_widget + 4);

	// Check whether the new vector differs from the one already loaded in the structure
	if (p_new_vector != *pp_current_vector) {

		// Offset 0x2C is index 11 (11 * 4 = 44 bytes), which stores the pool pointer
		u32* p_hud_pool = (u32*)p_widget[0x0B];

		if (p_hud_pool == 0) {
			*pp_current_vector = p_new_vector;
		}
		// Offset 0x18 is index 6 (6 * 4 = 24 bytes), secondary refresh/cycle flag
		else if (p_widget[0x06] == 0) {
			// Call the recycling routine to free the previous memory node
			hud_free_node((int*)p_hud_pool, (int*)*pp_current_vector);

			p_widget[0x06] = 1; // Sets the refresh/redraw flag of the vector layout
			*pp_current_vector = p_new_vector;
		}
		else {
			*pp_current_vector = p_new_vector;
		}
	}
}

/**
 * @brief Returns the pointer to the secondary transformation (scale/orientation) vector of a HUD widget.
 * Reads the physical address stored at offset +4 of the structure directly.
 * Original Ghidra address: 0x00337AF8 (PAL)
 *
 * @param p_widget Base address of the interface component structure (param_1).
 * @return f32* Pointer to the widget's scale vector (X, Y, Z, W), or NULL if unassigned.
 */
f32* hud_get_widget_vector_ptr(u32* p_widget) {
	if (p_widget == NULL) {
		return NULL;
	}

	// Offset +4 is index 1 in an array of 32-bit integers (1 * 4 = 4 bytes)
	return (f32*)((long)p_widget[1]);
}

/**
 * @brief Computes the mathematical cosine with hardware optimization (originally through VU0, coprocessor 2).
 * Portably replaces the PS2's vector microcode calls and _vcallms instructions.
 * Original Ghidra address: 0x00283A58 (PAL)
 *
 * @param radians Previously normalized angle in radians (param_1).
 * @return f32 The cosine computed natively by the hardware.
 */
f32 math_vu0_cos(f32 radians) {
	// On the original PS2 hardware the microroutine 0xC90 in VU0 was called.
	// Portably on modern systems, the PC FPU computes it instantly:
	return cosf(radians);
}

/**
 * @brief Computes the mathematical sine with hardware optimization (originally through VU0, coprocessor 2).
 * Portably replaces the PS2's vector microcode calls and _vcallms instructions.
 * Original Ghidra address: 0x00283A40 (PAL)
 *
 * @param radians Previously normalized angle in radians (param_1).
 * @return f32 The sine computed natively by the hardware.
 */
f32 math_vu0_sin_cos(f32 radians) {
	// On the original hardware the angle is injected into coprocessor 2 through _qmtc2,
	// the microroutine 0xC80 in VU0 is called and the result extracted with _qmfc2.
	// Portably on modern systems, the PC FPU computes it instantly:
	return sinf(radians);
}

#define MATH_PI 3.14159265358979323846f

/**
 * @brief Normalizes the sum of two angles in radians to keep the result in the range [-PI, PI].
 * Corrects circular angle overflows by adding or subtracting a full revolution (2*PI).
 * Original Ghidra address: 0x00284458 (PAL)
 *
 * @param angle_alpha First angular component in radians (param_1).
 * @param angle_beta Second angular component in radians to add (param_2).
 * @return f32 The resulting normalized angle.
 */
f32 math_normalize_angle_rad(f32 angle_alpha, f32 angle_beta) {
	f32 result_angle = angle_alpha + angle_beta;

	// If the angle is greater than or equal to PI, subtract a full revolution (2 * PI)
	if (result_angle >= (f32)MATH_PI) {
		result_angle = (result_angle - (f32)MATH_PI) - (f32)MATH_PI;
	}
	// If the angle is less than -PI, add a full revolution (2 * PI)
	if (result_angle < -(f32)MATH_PI) {
		result_angle = result_angle + (f32)MATH_PI + (f32)MATH_PI;
	}

	return result_angle;
}

/**
 * @brief Initializes and registers a widget structure intended to hold a visual resource or texture (asset).
 * Configures the base matrices with a uniform initial scale of 1.0f (100%) to avoid distortion.
 * Original Ghidra address: 0x003381B0 / approximate line 274 (PAL)
 */
void hud_register_widget_asset(u32* p_widget, const char* asset_name_ptr, long p_hud_pool,
	long p4, long p5, long p6, long p7, long p8) {

	// 1. Call the base constructor to clear and initialize the spatial matrices
	hud_clear_widget_matrices(p_widget, asset_name_ptr, p_hud_pool, p4, p5, p6, p7, p8);

	// Offset 0x0B is index 11; it stores the memory pool pointer
	if ((int*)p_widget[0x0B] == NULL) {
		p_widget[0x12] = 0; // Zeroes the secondary state flag (offset 0x48)
	}
	else {
		u32* p_vector_node;

		// 2. Reserve and clear the first auxiliary node at offset 0x0D
		int* p_node1 = hud_allocate_node((int*)p_widget[0x0B], 0, 0, p4, p5, p6, p7, p8);
		p_vector_node = (u32*)core_identity_stub(0x10, p_node1);
		p_widget[0x0D] = (u32)p_vector_node;

		p_vector_node[0] = 0;
		p_vector_node[1] = 0;
		p_vector_node[2] = 0;
		p_vector_node[3] = 0;

		// 3. Reserve and clear the second auxiliary node at offset 0x0E
		int* p_node2 = hud_allocate_node((int*)p_widget[0x0B], 0, 0, 0, 0, 0, 0, 0);
		p_vector_node = (u32*)core_identity_stub(0x10, p_node2);
		p_widget[0x0E] = (u32)p_vector_node;

		p_vector_node[0] = 0;
		p_vector_node[1] = 0;
		p_vector_node[2] = 0;
		p_vector_node[3] = 0;

		// 4. Set the default base scale to 1.0f (0x3f800000) in the second indexed vector (param_1[1])
		f32* p_scale_vector = *(f32**)(&p_widget[1]);
		if (p_scale_vector != NULL) {
			p_scale_vector[0] = 1.0f; // X scale
			p_scale_vector[1] = 1.0f; // Y scale
			p_scale_vector[2] = 1.0f; // Z scale
		}

		p_widget[0x12] = 0;
	}

	// Zero the property at offset 0x11 (44 bytes)
	p_widget[0x11] = 0;
}

/**
 * @brief Dynamically updates the data resource or context assigned to a HUD widget.
 * Automatically frees the previous node with hud_free_node if it detects a change of visual resource.
 * Original Ghidra address: 0x00337C00 (PAL)
 *
 * @param p_widget Base address of the interface component structure (param_1).
 * @param p_new_resource Pointer to the new data, string or graphic icon to render (param_2).
 */
void hud_update_widget_context(u32* p_widget, u32* p_new_resource) {
	if (p_widget == NULL) {
		return;
	}

	// Offset 0x10 is index 4 in an array of 32-bit integers (4 * 4 = 16 bytes)
	u32** pp_current_resource = (u32**)((u8*)p_widget + 0x10);

	// Check whether the new resource differs from the one already on screen
	if (p_new_resource != *pp_current_resource) {

		// Offset 0x2C is index 11 (11 * 4 = 44 bytes), which stores the pool pointer
		u32* p_hud_pool = (u32*)p_widget[0x0B]; // Using indexed index 11

		if (p_hud_pool == 0) {
			*pp_current_resource = p_new_resource;
		}
		// Offset 0x24 is index 9 (9 * 4 = 36 bytes), refresh/cycle flag
		else if (p_widget[0x09] == 0) {
			// Call the HUD routine to recycle and free the previous memory node
			hud_free_node((int*)p_hud_pool, (int*)*pp_current_resource);

			p_widget[0x09] = 1; // Sets the layout refresh/redraw flag
			*pp_current_resource = p_new_resource;
		}
		else {
			*pp_current_resource = p_new_resource;
		}
	}
}

/**
 * @brief Frees a HUD memory node and returns it to the reusable list (free list).
 * Decrements the active widget counter and restructures the pool header pointers.
 * Original Ghidra address: 0x00338C28 (PAL)
 *
 * @param p_hud_pool Header structure of the interface memory pool (param_1).
 * @param p_node_to_free Pointer to the memory block of the node to free (param_2).
 */
void hud_free_node(int* p_hud_pool, int* p_node_to_free) {
	if (p_hud_pool == NULL || p_node_to_free == NULL) {
		return;
	}

	// Offset 0x14 is index 5 in 32-bit integers (5 * 4 = 20 bytes)
	int* p_current_free_head = (int*)p_hud_pool[5];

	// Link the freed node in front of the previous reusable list
	*p_node_to_free = (int)p_current_free_head;

	// Make the freed node the new head of available elements in the pool
	p_hud_pool[5] = (int)p_node_to_free;

	// Offset 0x10 (index 4, 16 bytes) decrements the counter of HUD widgets in use
	p_hud_pool[4] = p_hud_pool[4] - 1;
}

/**
 * @brief Configures the two-dimensional floating-point position (X, Y) of a HUD component.
 * Converts the integer coordinates and stores them sequentially in the pointer at offset 0x34.
 * Original Ghidra address: 0x00338600 (PAL)
 *
 * @param p_widget Base address of the interface widget structure (param_1).
 * @param x_coord Horizontal coordinate in pixels (param_2).
 * @param y_coord Vertical coordinate in pixels (param_3).
 */
void hud_set_widget_position_2d(u32* p_widget, s32 x_coord, s32 y_coord) {
	if (p_widget != NULL) {
		// Offset 0x34 is index 13 in an array of 32-bit integers (13 * 4 = 52 bytes)
		f32** pp_vector_target = (f32**)((u8*)p_widget + 0x34);
		f32* p_vector = *pp_vector_target;

		if (p_vector != NULL) {
			p_vector[0] = (f32)x_coord; // Injects the X coordinate as a float
			p_vector[1] = (f32)y_coord; // Injects the Y coordinate as a contiguous float (+4 bytes)
		}
	}
}

/**
 * @brief Returns the dynamic data pointer or context linked to a HUD widget.
 * Reads the physical address stored at offset 0x0C of the structure directly.
 * Original Ghidra address: 0x00337B00 (PAL)
 *
 * @param p_widget Base address of the interface component structure (param_1).
 * @return void* Pointer to the widget's data context, or NULL if unassigned.
 */
void* hud_get_widget_data_ptr(u32* p_widget) {
	if (p_widget == NULL) {
		return NULL;
	}

	// Offset 0x0C is index 3 in an array of 32-bit integers (3 * 4 = 12 bytes)
	return (void*)((long)p_widget[3]);
}

/**
 * @brief Initializes a widget structure intended for HUD meters or bars (e.g. the experience bar).
 * Configures the base vectors with default floating-point scales (100.0f and 64.0f) and reserves their state node.
 * Original Ghidra address: 0x003383F0 (PAL)
 */
void hud_init_meter_widget(u32* p_widget, const char* text_ptr, long p_hud_pool,
	long p4, long p5, long p6, long p7, long p8) {

	// 1. Call the widget base constructor to clear and initialize the spatial matrices
	hud_clear_widget_matrices(p_widget, text_ptr, p_hud_pool, p4, p5, p6, p7, p8);

	u32* p_state_vector;

	// 2. If the pool is active, reserve and clear the meter state block at offset 0xD
	if (p_hud_pool == 0) {
		p_state_vector = (u32*)p_widget[0x0D];
	}
	else {
		int* p_node = hud_allocate_node((int*)p_widget[0x0B], 0, 0, p4, p5, p6, p7, p8);
		p_state_vector = (u32*)core_identity_stub(0x10, p_node);
		p_widget[0x0D] = (u32)p_state_vector;

		p_state_vector[0] = 0;
		p_state_vector[1] = 0;
		p_state_vector[2] = 0;
		p_state_vector[3] = 0;

		p_state_vector = (u32*)p_widget[0x0D];
	}

	p_state_vector[0] = 0;
	p_state_vector[1] = 0;

	// 3. Inject the magic floating-point initialization values (100.0f and 64.0f)
	f32* p_vector_pos = *(f32**)p_widget;       // First indexed vector
	f32* p_vector_scale = *(f32**)(&p_widget[1]); // Second indexed vector

	if (p_vector_pos != NULL) {
		p_vector_pos[0] = 100.0f; // 0x42c80000 on the PS2
		p_vector_pos[1] = 100.0f;
	}

	if (p_vector_scale != NULL) {
		p_vector_scale[0] = 64.0f; // 0x42800000 on the PS2
		p_vector_scale[1] = 64.0f;
	}

	// Zero the animation or timer flag
	p_widget[0x0E] = 0;
}


/**
 * @brief Configures the base colour or alpha transparency tint of a HUD widget.
 * Writes directly to the chromatic control property at offset 0x44.
 * Original Ghidra address: 0x00338728 (PAL)
 *
 * @param p_widget Base address of the interface component structure (param_1).
 * @param color_rgba 32-bit value encoding the colour and opacity in RGBA/colour-ID format (param_2).
 */
void hud_set_widget_color(u32* p_widget, u32 color_rgba) {
	if (p_widget != NULL) {
		// Offset 0x44 is index 0x11 in an array of 32-bit integers (17 * 4 = 68 bytes)
		p_widget[0x11] = color_rgba;
	}
}

/**
 * @brief Configures the render flags or alpha blend mode in a HUD widget structure.
 * Writes directly to the graphics control property at offset 0x40.
 * Original Ghidra address: 0x00338730 (PAL)
 *
 * @param p_widget Base address of the interface component structure (param_1).
 * @param render_flags Bit mask with the drawing and transparency options (param_2).
 */
void hud_set_widget_render_mode(u32* p_widget, u32 render_flags) {
	if (p_widget != NULL) {
		// Offset 0x40 is index 0x10 in an array of 32-bit integers (16 * 4 = 64 bytes)
		p_widget[0x10] = render_flags;
	}
}

/**
 * @brief Stores four individual components (X, Y, Z, W) directly through an indexed vector pointer.
 * Used by the engine to update the physical transformation coordinates of HUD widgets.
 * Original Ghidra address: 0x00337C68 (PAL)
 *
 * @param x Physical X component or red colour channel (param_1).
 * @param y Physical Y component or green colour channel (param_2).
 * @param z Physical Z component or blue colour channel (param_3).
 * @param w Physical W component or alpha control channel (param_4).
 * @param p_dest_struct Structure containing the real destination pointer (param_5).
 */
void math_set_vector4_ptr(u32 x, u32 y, u32 z, u32 w, void* p_dest_struct) {
	// Retrieve the real physical pointer stored at offset +4
	u32** pp_vector_target = (u32**)((u8*)p_dest_struct + 4);
	u32* p_vector = *pp_vector_target;

	if (p_vector != NULL) {
		p_vector[0] = x;   // X / R component
		p_vector[1] = y;   // Y / G component
		p_vector[2] = z;   // Z / B component
		p_vector[3] = w;   // W / A component
	}
}

/**
 * @brief Initializes the spatial transformation matrices and three-dimensional vectors of a HUD widget.
 * Reserves the required memory blocks in the pool and enables the component's visibility.
 * Original Ghidra address: 0x00337CA8 (PAL)
 */
void hud_clear_widget_matrices(u32* p_widget_transform, const char* text_ptr, long p_hud_pool,
	long p4, long p5, long p6, long p7, long p8) {

	// Store the address of the interface memory pool at offset 0xB
	p_widget_transform[0x0B] = (u32)p_hud_pool;

	if (p_hud_pool != 0) {
		u32* p_vector;

		// 1. Allocate and clear vector 0 (initial position)
		int* node0 = hud_allocate_node((int*)p_hud_pool, (long)text_ptr, p_hud_pool, p4, p5, p6, p7, p8);
		p_vector = (u32*)core_identity_stub(0x10, node0);
		p_widget_transform[0] = (u32)p_vector;
		p_vector[0] = 0; p_vector[1] = 0; p_vector[2] = 0; p_vector[3] = 0;

		// 2. Allocate and clear vector 2 (rotation)
		int* node1 = hud_allocate_node((int*)p_widget_transform[0x0B], 0, 0, 0, 0, 0, 0, 0);
		p_vector = (u32*)core_identity_stub(0x10, node1);
		p_widget_transform[2] = (u32)p_vector;
		p_vector[0] = 0; p_vector[1] = 0; p_vector[2] = 0; p_vector[3] = 0;

		// 3. Allocate and clear vector 1 (scale)
		int* node2 = hud_allocate_node((int*)p_widget_transform[0x0B], 0, 0, 0, 0, 0, 0, 0);
		p_vector = (u32*)core_identity_stub(0x10, node2);
		p_widget_transform[1] = (u32)p_vector;
		p_vector[0] = 0; p_vector[1] = 0; p_vector[2] = 0; p_vector[3] = 0;

		// 4. Allocate and clear vector 3 (velocity / interpolation)
		int* node3 = hud_allocate_node((int*)p_widget_transform[0x0B], 0, 0, 0, 0, 0, 0, 0);
		p_vector = (u32*)core_identity_stub(0x10, node3);
		p_widget_transform[3] = (u32)p_vector;
		p_vector[0] = 0; p_vector[1] = 0; p_vector[2] = 0; p_vector[3] = 0;

		// 5. Allocate and clear vector 4 (secondary offset)
		int* node4 = hud_allocate_node((int*)p_widget_transform[0x0B], 0, 0, 0, 0, 0, 0, 0);
		p_vector = (u32*)core_identity_stub(0x10, node4);
		p_widget_transform[4] = (u32)p_vector;
		p_vector[0] = 0; p_vector[1] = 0; p_vector[2] = 0; p_vector[3] = 0;
	}

	// Initialization of the graphics engine's physical flags and logical offsets
	p_widget_transform[10] = (u32)text_ptr;
	p_widget_transform[8] = 0;
	p_widget_transform[5] = 0;
	p_widget_transform[7] = 0;
	p_widget_transform[6] = 0;
	p_widget_transform[9] = 0;

	// Force visibility on so the component is rendered on screen
	hud_set_widget_visibility(p_widget_transform, 1);
}


/**
 * @brief Allocates or obtains a free memory node for a HUD visual component (pool allocator).
 * Performs strict physical bound checks and fires assertions on HUD memory overflows.
 * Original Ghidra address: 0x00338B98 (PAL)
 *
 * @param p_hud_pool Header structure of the interface memory pool (param_1).
 * @return int* Pointer to the initialized node memory block, ready for the widget.
 */
int* hud_allocate_node(int* p_hud_pool, long p2, long p3, long param_4,
	long param_5, long param_6, long param_7, long param_8) {
	int* p_allocated_node = (int*)p_hud_pool[5];

	// Case A: no reusable free nodes in the list; new memory must be carved out
	if (p_allocated_node == NULL) {
		int current_offset = p_hud_pool[3];
		u32 next_target_size = current_offset + p_hud_pool[2];

		// Overflow check of the interface memory pool
		if ((u32)p_hud_pool[1] < next_target_size) {
			// Call the kernel handler to freeze the software and report the bug's line
			sys_assert_dispatch((const char*)0x001adb18, 0x53, (const char*)0x001adb60,
				param_4, param_5, param_6, param_7, param_8);
			p_allocated_node = NULL;
		}
		else {
			p_hud_pool[3] = next_target_size;     // Advances the memory allocation pointer
			p_hud_pool[4] = p_hud_pool[4] + 1;     // Increments the active widget counter
			p_allocated_node = (int*)(*p_hud_pool + current_offset); // Computed physical address
		}
	}
	// Case B: fast path (recycles a node previously freed into the free list)
	else {
		int next_free_node = *p_allocated_node;
		p_hud_pool[4] = p_hud_pool[4] + 1;         // Increments the active widgets
		p_hud_pool[5] = next_free_node;           // Moves the head to the next free node
	}

	return p_allocated_node;
}


/**
 * @brief Links the typographic resource and configures the visual properties of HUD text.
 * Sets fonts, initial scales (1.0f), spacing (0.7f) and render flags.
 * Original Ghidra address: 0x00338688 (PAL)
 */
void hud_link_widget_text(u32* p_widget, const char* text_resource, long param_3,
	long p4, long p5, long p6, long p7, long p8) {

	// 1. Call the internal subroutine to clear the transformation matrices
	hud_clear_widget_matrices(p_widget, text_resource, param_3, p4, p5, p6, p7, p8);

	// 2. Assign the pointer of the global typographic font resource (offset 0xd)
	p_widget[0xD] = 0x002638D0;

	// 3. Set the initial X and Y render scales to 1.0f (0x3f800000)
	f32* p_scale = (f32*)p_widget[1];
	p_scale[0] = 1.0f; // X scale
	p_scale[1] = 1.0f; // Y scale

	// 4. Configure the format and spacing properties (0x3f333333 = 0.7f)
	p_widget[0xE] = 1;          // Initialization or active visibility flag
	p_widget[0x10] = 0;          // Render displacement offset
	p_widget[0x14] = 0x3f333333; // Character spacing / kerning (0.7f)
	p_widget[0x11] = 1;          // Alignment mode (e.g. centred)
	p_widget[0x13] = 0x200;      // Additional render flags (e.g. enable shadow)
	p_widget[0x12] = 0;          // Rotation or slant of the text
}


/**
 * @brief Fully initializes the central core of the HUD widgets and graphic meters (part 1).
 * Registers the master canvas and the coordinates of the Nanotech (health) bar in vector bursts.
 * Original Ghidra address: 0x0034B860 / approximate entry line (PAL)
 */
void hud_initialize_main_widgets(u32* p_hud_context, s32 state_offset, long p_hud_pool,
	long p4, long p5, long p6, long p7, long p8) {

	// Simulated flag for the animation refresh on PC
	static u32 DAT_001a7c18 = 1; // Forced to 1 (smooth refresh mode active)

	// Simulated stack variables that Ghidra extracted from the Emotion Engine
	u32 piStack_c0[16] = { 0 };
	u32 piStack_c4[16] = { 0 };
	u32 piStack_c8[16] = { 0 };
	u32 piStack_d8[16] = { 0 };
	u32 piStack_dc[16] = { 0 };
	u32 piStack_e0[16] = { 0 }; // Added pre-emptively for line 1411
	u32 piStack_e4[16] = { 0 };
	u32 piStack_ec[16] = { 0 };
	u32 piStack_f0[16] = { 0 }; // Safely initialized as a contiguous buffer
	u32 piStack_f4[16] = { 0 };
	u32 piStack_fc[16] = { 0 };
	u32 piStack_100[16] = { 0 };
	u32 piStack_104[16] = { 0 };
	u32 piStack_108[16] = { 0 };
	u32 piStack_110[16] = { 0 };
	u32 piStack_11c[16] = { 0 };
	u32 piStack_114[16] = { 0 };
	u32 piStack_118[16] = { 0 };
	u32 piStack_120[16] = { 0 };
	u32 piStack_124[16] = { 0 };
	u32 piStack_128[16] = { 0 };
	u32 piStack_130[16] = { 0 };

	// Local PS2 support variables for the extended ammo arrays
	u32 piStack_10c = { 0 };
	u32 piStack_f8 = { 0 };
	u32 piStack_cc = { 0 };

	// Temporary control variable that Ghidra extracted for the ammo array
	int iVar25 = 0;

	// 1. Clear the master canvas refresh counters at offsets +0x567 and +0x568
	p_hud_context[0x567] = 0;
	p_hud_context[0x568] = 0;

	// Computation of the dynamic offset for Insomniac's production state lookup
	s32 state_lookup_id = state_offset + 0x8710;

	// 2. Registration and coordinate injection of widget 0 (HUD master canvas)
	hud_register_widget_asset(p_hud_context, RECURSO_HUD_CANVAS, p_hud_pool, p4, p5, p6, p7, p8);
	hud_set_state_from_lookup((int)p_hud_context, state_lookup_id, 1);
	math_set_vector4(10.0f, 10.0f, 0.0f, 0.0f, p_hud_context); // 0x41200000 = 10.0f

	// 3. Registration and coordinate injection of widget 1 (health bar outline)
	u32* p_health_outline_widget = p_hud_context + 0x13; // Indexed offset param_1 + 0x13
	hud_register_widget_asset(p_health_outline_widget, RECURSO_HEALTH_OUTLINE, p_hud_pool, p4, p5, p6, p7, p8);
	hud_set_state_from_lookup((int)p_health_outline_widget, state_lookup_id, 2);
	math_set_vector4(10.0f, 10.0f, 0.0f, 0.0f, p_health_outline_widget);

	// 4. Registration and coordinate injection of widget 2 (health bar fill)
	u32* p_health_fill_widget = p_hud_context + 0x26; // Indexed offset param_1 + 0x26
	hud_register_widget_asset(p_health_fill_widget, RECURSO_HEALTH_FILL, p_hud_pool, p4, p5, p6, p7, p8);
	// (The coordinate injection and set_state continue in the next frame/code block)
	math_set_vector4(10.0f, 18.5f, 0.0f, 0.0f, p_health_fill_widget); // 0x41940000 = 18.5f

	// (This section directly continues the flow inside hud_initialize_main_widgets)
	hud_set_state_from_lookup((int)p_health_fill_widget, state_lookup_id, 5);

	// 5. Configuration and initialization of the weapon name / AmmoText widget
	u32* p_wpn_name_widget = p_hud_context + 0x40; // piVar14
	ee_memset((p_hud_context + 0x39), 0, 0x18);
	hud_link_widget_text(p_wpn_name_widget, RECURSO_WEAPON_NAME, p_hud_pool, p4, p5, p6, p7, p8);
	math_set_vector4_ptr(0x3f666666, 0, 0, 0, p_wpn_name_widget);
	hud_set_widget_render_mode_alt(p_wpn_name_widget, (u32)(p_hud_context + 0x39));
	math_set_vector4(64.0f, 150.0f, 0.0f, 0.0f, p_wpn_name_widget); // 0x42800000 = 64.0f
	hud_set_widget_color(p_wpn_name_widget, 0);

	// 6. Registration of the weapon experience bar meter (WeaXP)
	u32* p_wpn_xp_widget = p_hud_context + 0x56; // piStack_144
	hud_init_meter_widget(p_wpn_xp_widget, RECURSO_WEAPON_XP, p_hud_pool, p4, p5, p6, p7, p8);
	math_set_vector4_ptr(0x42000000, 0x42000000, 0, 0, p_wpn_xp_widget);
	math_set_vector4(20.0f, 150.0f, 0.0f, 0.0f, p_wpn_xp_widget); // 0x41a00000 = 20.0f
	u32* p_xp_data = (u32*)hud_get_widget_data_ptr(p_wpn_xp_widget);
	*p_xp_data = 0x60f0f0b0; // Colour initialization of the experience bar

	// 7. Registration of the bullet iconography widgets
	u32* p_ammo_icon = p_hud_context + 0x65; // piStack_140
	hud_register_widget_asset(p_ammo_icon, RECURSO_AMMO_ICON, p_hud_pool, p4, p5, p6, p7, p8);
	math_set_vector4(498.0f, 10.0f, 0.0f, 0.0f, p_ammo_icon); // 0x43f90000 = 498.0f
	hud_set_state_from_lookup((int)p_ammo_icon, state_lookup_id, 3);

	u32* p_ammo_icon_back = p_hud_context + 0x78; // piStack_13c
	hud_register_widget_asset(p_ammo_icon_back, RECURSO_AMMO_ICON_BACK, p_hud_pool, p4, p5, p6, p7, p8);
	math_set_vector4(498.0f, 10.0f, 0.0f, 0.0f, p_ammo_icon_back);
	hud_set_state_from_lookup((int)p_ammo_icon_back, state_lookup_id, 4);

	// 8. Registration of the bolt wallet meter (BoltText and BoltIcon)
	u32* p_bolt_text_widget = p_hud_context + 0x92; // piVar16
	ee_memset((p_hud_context + 0x8b), 0, 0x18);
	hud_link_widget_text(p_bolt_text_widget, RECURSO_BOLT_TEXT, p_hud_pool, p4, p5, p6, p7, p8);
	math_set_vector4_ptr(0x3f666666, 0, 0, 0, p_bolt_text_widget);
	hud_set_widget_render_mode_alt(p_bolt_text_widget, (u32)(p_hud_context + 0x8b));
	math_set_vector4(440.0f, 150.0f, 0.0f, 0.0f, p_bolt_text_widget); // 0x43dc0000 = 440.0f
	hud_set_widget_color(p_bolt_text_widget, 2);

	u32* p_bolt_icon_widget = p_hud_context + 0xA8; // piStack_138
	hud_init_meter_widget(p_bolt_icon_widget, RECURSO_BOLT_ICON, p_hud_pool, p4, p5, p6, p7, p8);
	math_set_vector4_ptr(0x42000000, 0x42000000, 0, 0, p_bolt_icon_widget);
	math_set_vector4(457.0f, 150.0f, 0.0f, 0.0f, p_bolt_icon_widget); // 0x43e48000 = 457.0f
	hud_set_widget_position_2d(p_bolt_icon_widget, 103, 117); // 0x7567 regional plano

	// 9. Registration of the Quick Select radial menu (base and ring)
	u32* p_qsel_back = p_hud_context + 0xb8; // piVar2
	hud_register_widget_asset(p_qsel_back, RECURSO_QSEL_BACK, p_hud_pool, p4, p5, p6, p7, p8);
	hud_set_state_from_lookup((int)p_qsel_back, state_lookup_id, 7);
	math_set_vector4_ptr(1.0f, 1.0f, 0, 0, p_qsel_back);
	math_set_vector4(500.0f, 208.0f, 0, 0, p_qsel_back); // 0x42fa0000 = 500.0f, 0x43500000 = 208.0f
	u32* p_qsel_back_data = (u32*)hud_get_widget_data_ptr(p_qsel_back);
	*p_qsel_back_data = 0x442d00;
	hud_update_widget_context(p_qsel_back, (u32*)p_hud_context[0x201]);

	u32* p_qsel_bord = p_hud_context + 0xcb; // piVar12
	hud_register_widget_asset(p_qsel_bord, RECURSO_QSEL_BORD, p_hud_pool, p4, p5, p6, p7, p8);
	hud_set_state_from_lookup((int)p_qsel_bord, state_lookup_id, 6);
	math_set_vector4_ptr(1.0f, 1.0f, 0, 0, p_qsel_bord);
	math_set_vector4(500.0f, 208.0f, 0, 0, p_qsel_bord);
	u32* p_qsel_bord_data = (u32*)hud_get_widget_data_ptr(p_qsel_bord);
	*p_qsel_bord_data = 0xf0c070;
	hud_update_widget_context(p_qsel_bord, (u32*)p_hud_context[0x201]);

	// 10. GENERATION AND BURST REGISTRATION LOOP OF THE RADIAL SLOTS (QSelBI0 - QSelBI27)
	char name_construction_buffer[16];
	u32 loop_iterator = 0;
	u32* p_dynamic_slot_widget = NULL;

	do {
		// The internal vsnprintf builds the identifiers sequentially
		// The call is simulated by passing the argument list to format "QSelBI%d"
		// txt_vsnprintf_internal(name_construction_buffer, FORMATO_SLOT_RADIAL, loop_iterator);

		p_dynamic_slot_widget = p_hud_context + (loop_iterator * 0x13) + 0xde; // dynamic piVar2
		hud_register_widget_asset(p_dynamic_slot_widget, name_construction_buffer, p_hud_pool, p4, p5, p6, p7, p8);

		loop_iterator++;

		// (This section directly continues the internal logic of hud_initialize_main_widgets)
		hud_set_state_from_lookup((int)p_dynamic_slot_widget, state_lookup_id, loop_iterator + 8);
		math_set_vector4_ptr(1.0f, 1.0f, 0, 0, p_dynamic_slot_widget);
		math_set_vector4(500.0f, 208.0f, 0, 0, p_dynamic_slot_widget);
		hud_update_widget_context(p_dynamic_slot_widget, (u32*)p_hud_context[0x201]);
		u32* p_dynamic_slot_data = (u32*)hud_get_widget_data_ptr(p_dynamic_slot_widget);
		*p_dynamic_slot_data = 0x442d00;

		// Dynamic construction of the icon string "QSelIco%d"
		// txt_vsnprintf_internal(name_construction_buffer, FORMATO_ICONO_RADIAL, loop_iterator);

		u32* p_dynamic_icon_widget = p_hud_context + (loop_iterator * 0x0F) + 0x189; // icon piVar2
		hud_init_meter_widget(p_dynamic_icon_widget, name_construction_buffer, p_hud_pool, p4, p5, p6, p7, p8);
		hud_set_widget_visibility(p_dynamic_icon_widget, 0);

		// GEOMETRIC COMPUTATION OF THE RADIAL PROJECTION ON THE VU0 HARDWARE
		f32 base_angle = ((f32)loop_iterator + (f32)loop_iterator) * 0.3926991f - 3.1415927f;
		f32 normal_angle = math_normalize_angle_rad(base_angle, 1.5707964f);

		f32 sin_val = math_vu0_sin_cos(normal_angle);
		f32 cos_val = math_vu0_cos(normal_angle);

		// Project the horizontal (X) and vertical (Y) elliptical coordinates onto the screen
		f32 projected_x = (f32)((s32)(sin_val * 82.14f) + 109); // 0x6d = 109
		f32 projected_y = (f32)((s32)(cos_val * 76.442f) + 189); // 0xbd = 189
		math_set_vector4(projected_x, projected_y, 0, 0, p_dynamic_icon_widget);

		math_set_vector4_ptr(0x42000000, 0x42000000, 0, 0, p_dynamic_icon_widget);
		u32* p_dynamic_icon_data = (u32*)hud_get_widget_data_ptr(p_dynamic_icon_widget);
		*p_dynamic_icon_data = 0xf0f0b0;

		loop_iterator++;
	} while (loop_iterator < 8);

	// 11. Burst loop synchronizing and coupling the HUD vectors (free list)
	u32* p_sync_vector_src = p_hud_context + 0xde; // piStack_12c
	u32* p_sync_target_a = p_hud_context + 0xf1;   // piVar2
	u32* p_sync_target_b = p_hud_context + 0x198;  // piVar12
	s32 sync_iterator = 6;

	do {
		sync_iterator--;
		u32* p_shared_vec_a = (u32*)hud_get_widget_vector_ptr(p_sync_vector_src);
		hud_update_widget_vector(p_sync_target_a, p_shared_vec_a);
		p_sync_target_a += 0x13;

		u32* p_shared_vec_b = (u32*)hud_get_widget_vector_ptr(p_hud_context + 0x134); // piStack_134
		hud_update_widget_vector(p_sync_target_b, p_shared_vec_b);
		p_sync_target_b += 0x0F;
	} while (sync_iterator > 0);

	// Synchronize the final vector lock at the perimeter anchor
	u32* p_final_shared_vec = (u32*)hud_get_widget_vector_ptr(p_sync_vector_src);
	hud_update_widget_vector((u32*)(p_hud_context + 0xb8), p_final_shared_vec); // piStack_d4

	// 12. Initialization of the master ammo layout and burst of secondary sub-inventories
	u32* p_ammo_layout_container = p_hud_context + 0x202; // piStack_e8
	hud_init_ammo_layout(p_ammo_layout_container, (long)state_offset, p_hud_pool, p4, p5, p6, p7, p8);
	hud_set_ammo_widget_context_2d(0x43130000, 0x41200000, p_ammo_layout_container);
	p_ammo_layout_container[3] = 0;

	// Initialization of the secondary support inventory (devices / gadgets layer)
	u32* p_sub_inv_gadgets = p_hud_context + 0x40; // piVar14
	inv_reset_weapon_inventory(p_sub_inv_gadgets);
	inv_set_weapon_inventory_mode(p_sub_inv_gadgets, 3);
	inv_set_weapon_slot_data(0, 0x442d00, 0, 0, 0, p_sub_inv_gadgets, 0);
	inv_set_weapon_slot_data(0x3E99999A, 0x60442d00, 0, 0, 0, p_sub_inv_gadgets, 1);
	inv_set_weapon_slot_data(0x3F800000, 0x60442d00, 0, 0, 0, p_sub_inv_gadgets, 2);
	inv_update_weapon_visual_pointers(0, 0x40000000, p_sub_inv_gadgets, 1);

	u32 context_data_res = (u32)hud_get_widget_data_ptr(p_sync_vector_src);
	inv_set_weapon_inventory_transition_flag(p_sub_inv_gadgets, context_data_res);
	inv_set_quick_select_open_state(p_sub_inv_gadgets, 0);
	inv_set_active_weapon_slot(p_sub_inv_gadgets, (p_hud_context + 0x11c));
	inv_set_weapon_inventory_visibility(p_sub_inv_gadgets, -1);

	// Adjust the dynamic interpolation acceleration according to the global system flags
	f32 anim_speed = (DAT_001a7c18 != 0) ? 0.035f : 0.029f; // Estimated fine interpolation values
	inv_set_animation_factor(p_sub_inv_gadgets, anim_speed);
	inv_swap_animation_lock(p_sub_inv_gadgets, 0);

	// (This section directly continues the internal logic of hud_initialize_main_widgets)
	f32 alt_anim_speed = (DAT_001a7c18 != 0) ? 0.035f : 0.029f;
	inv_set_animation_factor((u32*)piStack_f0, alt_anim_speed);
	inv_swap_animation_lock((u32*)piStack_f0, 0);

	// Initialization of the secondary support inventory (layer C)
	u32* p_sub_inv_c = (u32*)piStack_e0;
	inv_reset_weapon_inventory(p_sub_inv_c);
	inv_set_weapon_inventory_mode(p_sub_inv_c, 3);
	inv_set_weapon_slot_data(0, 0x3f99999a, 0x3f99999a, 0, 0, p_sub_inv_c, 0); // 0x3f99999a = 1.2f
	inv_set_weapon_slot_data(0x3F666666, 0x3f800000, 0x3f800000, 0, 0, p_sub_inv_c, 1);
	inv_set_weapon_slot_data(0x3F800000, 0x3f800000, 0x3f800000, 0, 0, p_sub_inv_c, 2);
	u32* p_vec_c0 = (u32*)hud_get_widget_vector_ptr((u32*)piStack_c0);
	inv_set_weapon_inventory_transition_flag(p_sub_inv_c, (u32)p_vec_c0);
	inv_set_quick_select_open_state(p_sub_inv_c, 0);
	inv_set_active_weapon_slot(p_sub_inv_c, (u32)piStack_120);
	inv_set_weapon_inventory_visibility(p_sub_inv_c, -1);
	inv_set_animation_factor(p_sub_inv_c, alt_anim_speed);
	inv_swap_animation_lock(p_sub_inv_c, 0);

	// Initialization of the secondary support inventory (layer D - special devices)
	u32* p_sub_inv_d = (u32*)piStack_128;
	inv_reset_weapon_inventory(p_sub_inv_d);
	inv_set_weapon_inventory_mode(p_sub_inv_d, 3);
	inv_set_weapon_slot_data(0, 0, 0, 0, 0, p_sub_inv_d, 0);
	inv_set_weapon_slot_data(0x3C23D70A, 0x3f800000, 0x3f800000, 0, 0, p_sub_inv_d, 1); // 0x3c23d70a = 0.01f
	inv_set_weapon_slot_data(0x3F800000, 0x3f800000, 0x3f800000, 0, 0, p_sub_inv_d, 2);
	inv_set_weapon_inventory_transition_flag(p_sub_inv_d, 0);
	inv_set_quick_select_open_state(p_sub_inv_d, 0);
	inv_set_active_weapon_slot(p_sub_inv_d, (u32)piStack_120);
	inv_set_weapon_inventory_visibility(p_sub_inv_d, -1);
	inv_set_animation_factor(p_sub_inv_d, alt_anim_speed);
	inv_swap_animation_lock(p_sub_inv_d, 0);

	// 13. CONFIGURATION AND DISPATCH OF THE EXTENDED AMMO ARRAY (control block 1)
	u32* p_ammo_matrix_1 = (u32*)piStack_10c;
	inv_reset_extended_ammo_subsystem(p_ammo_matrix_1);
	u32* p_widget_data_1 = (u32*)hud_get_widget_data_ptr((u32*)p_hud_context);
	inv_set_ammo_matrix_group_ptr(p_ammo_matrix_1, 0, (u32)p_widget_data_1);
	inv_set_ammo_matrix_group_state(p_ammo_matrix_1, 0, 0x442d00);
	inv_set_ammo_matrix_group_state(p_ammo_matrix_1, 1, 0x60442d00);
	p_ammo_matrix_1[2] = 0;
	p_ammo_matrix_1[3] = 0x40000000; // 2.0f
	u32* p_widget_vec_1 = (u32*)hud_get_widget_vector_ptr((u32*)p_hud_context);
	inv_set_ammo_matrix_group_ptr(p_ammo_matrix_1, 1, (u32)p_widget_vec_1);
	inv_set_extended_ammo_slot_data(0, 0, 0, 0, p_ammo_matrix_1, 1, 0);
	inv_set_extended_ammo_slot_data(0x3F800000, 0x3F800000, 0, 0, p_ammo_matrix_1, 1, 1);
	u32* p_widget_pos_1 = (u32*)hud_get_widget_position_vector_ptr((u32*)p_hud_context);
	inv_set_ammo_matrix_group_ptr(p_ammo_matrix_1, 2, (u32)p_widget_pos_1);
	inv_set_extended_ammo_slot_data(0x42480000, 0x41c80000, 0, 0, p_ammo_matrix_1, 2, 0); // 50.0f and 25.0f
	inv_set_extended_ammo_slot_data(0x41200000, 0x41200000, 0, 0, p_ammo_matrix_1, 2, 1); // 10.0f and 10.0f

	// 14. CONFIGURATION AND DISPATCH OF THE EXTENDED AMMO ARRAY (control block 2)
	u32* p_ammo_matrix_2 = (u32*)piStack_f8;
	inv_reset_extended_ammo_subsystem(p_ammo_matrix_2);
	u32* p_widget_data_2 = (u32*)hud_get_widget_data_ptr((u32*)piStack_cc);
	inv_set_ammo_matrix_group_ptr(p_ammo_matrix_2, 0, (u32)p_widget_data_2);
	inv_set_ammo_matrix_group_state(p_ammo_matrix_2, 0, 0xf0c070);
	inv_set_ammo_matrix_group_state(p_ammo_matrix_2, 1, 0x55f0c070);
	p_ammo_matrix_1[3] = (u32)iVar25;
	p_ammo_matrix_1[2] = 0;
	u32* p_widget_vec_2 = (u32*)hud_get_widget_vector_ptr((u32*)piStack_cc);
	inv_set_ammo_matrix_group_ptr(p_ammo_matrix_2, 1, (u32)p_widget_vec_2);
	inv_set_extended_ammo_slot_data(0x3F99999A, 0x3F99999A, 0, 0, p_ammo_matrix_2, 1, 0);
	inv_set_extended_ammo_slot_data(0x3F800000, 0x3F800000, 0, 0, p_ammo_matrix_2, 1, 1);
	u32* p_widget_pos_2 = (u32*)hud_get_widget_position_vector_ptr((u32*)piStack_cc);
	inv_set_ammo_matrix_group_ptr(p_ammo_matrix_2, 2, (u32)p_widget_pos_2);
	inv_set_extended_ammo_slot_data(0, 0, 0, 0, p_ammo_matrix_2, 2, 0);
	inv_set_extended_ammo_slot_data(0x41200000, 0x41200000, 0, 0, p_ammo_matrix_2, 2, 1);

	// 15. CONFIGURATION AND DISPATCH OF THE EXTENDED AMMO ARRAY (control block 3)
	u32* p_ammo_matrix_3 = (u32*)piStack_e4;
	inv_reset_extended_ammo_subsystem(p_ammo_matrix_3);
	u32* p_widget_data_3 = (u32*)hud_get_widget_data_ptr((u32*)piStack_114);
	inv_set_ammo_matrix_group_ptr(p_ammo_matrix_3, 0, (u32)p_widget_data_3);
	inv_set_ammo_matrix_group_state(p_ammo_matrix_3, 0, 0xf0f0f0);
	inv_set_ammo_matrix_group_state(p_ammo_matrix_3, 1, 0x80f0f0f0);
	p_ammo_matrix_3[3] = (u32)iVar25;
	p_ammo_matrix_3[2] = 0;

	// Initialization of the complementary bullet inventory (layer E)
	u32* p_sub_inv_e = (u32*)piStack_c8;
	inv_reset_weapon_inventory(p_sub_inv_e);
	inv_set_weapon_inventory_mode(p_sub_inv_e, 3);
	inv_set_weapon_slot_data(0, 0x1eff, 0, 0, 0, p_sub_inv_e, 0);
	inv_set_weapon_slot_data(0x3F000000, 0x1eff, 0, 0, 0, p_sub_inv_e, 1);
	inv_set_weapon_slot_data(0x3F800000, 0x80001eff, 0, 0, 0, p_sub_inv_e, 2);
	u32* p_vec_e0 = (u32*)hud_get_widget_data_ptr((u32*)piStack_130);
	inv_set_weapon_inventory_transition_flag(p_sub_inv_e, (u32)p_vec_e0);
	inv_set_quick_select_open_state(p_sub_inv_e, 0);
	inv_set_active_weapon_slot(p_sub_inv_e, (u32)piStack_11c);
	inv_set_weapon_inventory_visibility(p_sub_inv_e, 1);
	inv_set_animation_factor(p_sub_inv_e, 0.0666f); // 0x3d88850a \approx 0.0666f

	// 16. CONFIGURATION AND DISPATCH OF THE EXTENDED AMMO ARRAY (control blocks 4 and 5)
	u32* p_ammo_matrix_4 = (u32*)piStack_124;
	inv_reset_extended_ammo_subsystem(p_ammo_matrix_4);
	u32* p_widget_data_4 = (u32*)hud_get_widget_data_ptr((u32*)piStack_110);
	inv_set_ammo_matrix_group_ptr(p_ammo_matrix_4, 0, (u32)p_widget_data_4);
	inv_set_ammo_matrix_group_state(p_ammo_matrix_4, 0, 0xf0f0b0);
	inv_set_ammo_matrix_group_state(p_ammo_matrix_4, 1, 0x60f0f0b0);
	p_ammo_matrix_4[2] = 0;
	p_ammo_matrix_4[3] = (u32)iVar25;
	// Fixed: pass the raw numeric value directly, without impossible pointers
	inv_set_ammo_capacity_multiplier(0x0000666f, p_ammo_matrix_4); // Backpack of...

	u32* p_ammo_matrix_5 = (u32*)piStack_108;
	inv_reset_extended_ammo_subsystem(p_ammo_matrix_5);
	u32* p_widget_data_5 = (u32*)hud_get_widget_data_ptr((u32*)piStack_100);
	inv_set_ammo_matrix_group_ptr(p_ammo_matrix_5, 0, (u32)p_widget_data_5);
	inv_set_ammo_matrix_group_state(p_ammo_matrix_5, 0, 0x442d00);
	inv_set_ammo_matrix_group_state(p_ammo_matrix_5, 1, 0x60442d00);

	// (This section definitively closes the internal logic of hud_initialize_main_widgets)
	u32* p_ammo_matrix_5_base = (u32*)piStack_108;
	p_ammo_matrix_5_base[3] = 0x40000000;
	p_ammo_matrix_5_base[2] = 0; // iVar17 = 0
	u32* p_widget_vec_5 = (u32*)hud_get_widget_vector_ptr((u32*)piStack_100);
	inv_set_ammo_matrix_group_ptr(p_ammo_matrix_5_base, 1, (u32)p_widget_vec_5);
	inv_set_extended_ammo_slot_data(0, 0, 0, 0, p_ammo_matrix_5_base, 1, 0);
	inv_set_extended_ammo_slot_data(0x3F800000, 0x3F800000, 0, 0, p_ammo_matrix_5_base, 1, 1);
	u32* p_widget_pos_5 = (u32*)hud_get_widget_position_vector_ptr((u32*)piStack_100);
	inv_set_ammo_matrix_group_ptr(p_ammo_matrix_5_base, 2, (u32)p_widget_pos_2);
	inv_set_extended_ammo_slot_data(0x43E50000, 0x41C80000, 0, 0, p_ammo_matrix_5_base, 2, 0); // 458.0f and 25.0f
	inv_set_extended_ammo_slot_data(0x3F666666, 0x41200000, 0, 0, p_ammo_matrix_5_base, 2, 1);

	// 17. CONFIGURATION AND DISPATCH OF THE EXTENDED AMMO ARRAY (control blocks 6 and 7)
	u32* p_ammo_matrix_6 = (u32*)piStack_f4;
	inv_reset_extended_ammo_subsystem(p_ammo_matrix_6);
	u32* p_widget_data_6 = (u32*)hud_get_widget_data_ptr((u32*)piStack_fc);
	inv_set_ammo_matrix_group_ptr(p_ammo_matrix_6, 0, (u32)p_widget_data_6);
	inv_set_ammo_matrix_group_state(p_ammo_matrix_6, 0, 0xf0c070);
	inv_set_ammo_matrix_group_state(p_ammo_matrix_6, 1, 0x55f0c070);
	p_ammo_matrix_6[2] = 0;
	p_ammo_matrix_6[3] = (u32)iVar25;
	u32* p_widget_vec_6 = (u32*)hud_get_widget_vector_ptr((u32*)piStack_fc);
	inv_set_ammo_matrix_group_ptr(p_ammo_matrix_6, 1, (u32)p_widget_vec_6);
	inv_set_extended_ammo_slot_data(0x3F99999A, 0x3F99999A, 0, 0, p_ammo_matrix_6, 1, 0);
	inv_set_extended_ammo_slot_data(0x3F800000, 0x3F800000, 0, 0, p_ammo_matrix_6, 1, 1);
	p_hud_context[0x4AA] = 0;
	p_ammo_matrix_6[1] = (u32)iVar25;
	u32* p_widget_pos_6 = (u32*)hud_get_widget_position_vector_ptr((u32*)piStack_fc);
	inv_set_ammo_matrix_group_ptr(p_ammo_matrix_6, 2, (u32)p_widget_pos_6);
	inv_set_extended_ammo_slot_data(0x44008000, 0x40800000, 0, 0, p_ammo_matrix_6, 2, 0); // 514.0f and 4.0f
	inv_set_extended_ammo_slot_data(0x3F666666, 0x41200000, 0, 0, p_ammo_matrix_6, 2, 1);

	inv_reset_extended_ammo_subsystem((u32*)piStack_dc);
	u32* p_widget_data_7 = (u32*)hud_get_widget_data_ptr((u32*)piStack_ec);
	inv_set_ammo_matrix_group_ptr((u32*)piStack_dc, 0, (u32)p_widget_data_7);
	inv_set_ammo_matrix_group_state((u32*)piStack_dc, 0, 0xf0f0f0);
	inv_set_ammo_matrix_group_state((u32*)piStack_dc, 1, 0x80f0f0f0);
	((u32*)piStack_dc)[3] = (u32)iVar25;
	((u32*)piStack_dc)[2] = 0;

	inv_reset_extended_ammo_subsystem((u32*)piStack_c4);
	u32* p_widget_data_8 = (u32*)hud_get_widget_data_ptr((u32*)piStack_d8);
	inv_set_ammo_matrix_group_ptr((u32*)piStack_c4, 0, (u32)p_widget_data_8);
	inv_set_ammo_matrix_group_state((u32*)piStack_c4, 0, 0xf0f0b0);
	inv_set_ammo_matrix_group_state((u32*)piStack_c4, 1, 0x60f0f0b0);
	((u32*)piStack_c4)[3] = (u32)iVar25;
	((u32*)piStack_c4)[2] = 0;

	// 18. ASSIGNMENT OF THE MAIN ACTIVE INVENTORY LAYERS
	u32* p_main_inv_1 = (u32*)piStack_118;
	inv_reset_weapon_inventory(p_main_inv_1);
	inv_set_weapon_inventory_mode(p_main_inv_1, 2);
	u32* p_main_data_1 = (u32*)hud_get_widget_data_ptr((u32*)p_hud_context);
	inv_set_weapon_inventory_transition_flag(p_main_inv_1, (u32)p_main_data_1);
	inv_set_active_weapon_slot(p_main_inv_1, (u32)piStack_11c);
	inv_set_animation_factor(p_main_inv_1, 0.005f);
	inv_set_weapon_slot_data(0, 0x70202080, 0, 0, 0, p_main_inv_1, 0);
	inv_set_weapon_slot_data(0x3F800000, 0x70242335, 0, 0, 0, p_main_inv_1, 1);
	inv_set_quick_select_open_state(p_main_inv_1, 2);
	inv_set_weapon_inventory_visibility(p_main_inv_1, 1);
	inv_swap_animation_lock(p_main_inv_1, 0);

	u32* p_main_inv_2 = (u32*)piStack_104;
	inv_reset_weapon_inventory(p_main_inv_2);
	inv_set_weapon_inventory_mode(p_main_inv_2, 2);
	u32* p_main_data_2 = (u32*)hud_get_widget_data_ptr((u32*)piStack_cc);
	inv_set_weapon_inventory_transition_flag(p_main_inv_2, (u32)p_main_data_2);
	inv_set_active_weapon_slot(p_main_inv_2, (u32)piStack_11c);
	inv_set_animation_factor(p_main_inv_2, 0.005f);
	inv_set_weapon_slot_data(0, 0x606060c0, 0, 0, 0, p_main_inv_2, 0);
	inv_set_weapon_slot_data(0x3F800000, 0x60424162, 0, 0, 0, p_main_inv_2, 1);
	inv_set_quick_select_open_state(p_main_inv_2, 2);
	inv_set_weapon_inventory_visibility(p_main_inv_2, 1);
	inv_swap_animation_lock(p_main_inv_2, 0);

	// 19. FILL OF THE EMOTION ENGINE GLOBAL TRANSITION FLAGS
	p_hud_context[0xB7] = 0x001A7A80; // Strict static table address
	p_hud_context[0x561] = 0;
	p_hud_context[0x563] = 0;
	p_hud_context[0x565] = 0;
	p_hud_context[0x566] = -1; // Forces a clean load without garbage values
	p_hud_context[0x562] = -1;
	p_hud_context[0x564] = -1;

	// 20. FORCE THE REGIONAL PIXEL UPDATE OF THE CANVASES
	hud_update_layout_aspect_ratio(p_hud_context, 0);

	// 21. STORE THE DYNAMIC ROTATION PARAMETER OF THE QUICK SELECT RING
	s32 remaining_slots_count = inv_get_quick_select_remaining_space();
	p_hud_context[0x56A] = remaining_slots_count;

}

/**
 * @brief Looks up a specific identifier in an indexed table structure.
 * Arithmetic over an array with 8-byte steps (key/value pair structure).
 * Original Ghidra address: 0x00338AA8 (PAL)
 *
 * @param table_ptr Pointer to the base table structure.
 * @param target_id ID or key being searched for.
 * @return s32 The value associated with the ID found, or 0 if absent or out of bounds.
 */
s32 game_lookup_id_in_table(u8* table_ptr, s32 target_id) {
	s32 index = 0;

	// Offset +0x18 stores the maximum number of valid elements in the table
	s32 total_elements = *(s32*)(table_ptr + 0x18);

	if (0 < total_elements) {
		index = 1;

		// Engine optimization: check the first element directly (+0x1c)
		if (*(s32*)(table_ptr + 0x1c) == target_id) {
			index = *(s32*)(table_ptr + 0x20); // Returns the associated value at +0x20
		}
		else {
			// Linear search loop (do-while)
			do {
				if (*(s32*)(table_ptr + 0x18) <= index) {
					return 0; // Outside the table bounds, not found
				}

				// 8-byte key/value pair structure: 4 bytes for the ID, 4 bytes for the data
				s32* pair_ptr = (s32*)(index * 8 + (table_ptr + 0x1c));
				index++;

				if (*pair_ptr == target_id) {
					return pair_ptr[1]; // Returns the contiguous value in memory
				}
			} while (1);
		}
	}

	return index;
}

/**
 * @brief Gets a configuration value by lookup and initializes the HUD field.
 * Original Ghidra address: 0x00338070 (PAL)
 */
void hud_set_state_from_lookup(u8* p_hudState, u8* table_ptr, s32 target_id) {
	// Perform the lookup in the logical table
	s32 result_value = game_lookup_id_in_table(table_ptr, target_id);

	// Store the result at configuration offset +0x40 of the HUD component
	*(s32*)(p_hudState + 0x40) = result_value;
}

/**
 * @brief Assigns four 32-bit values consecutively in a data structure.
 * Standard behaviour for configuring spatial vectors (X, Y, Z, W) or colours (R, G, B, A).
 * Original Ghidra address: 0x00337B18 (PAL)
 *
 * @param val1 First component (e.g. X coordinate or red channel)
 * @param val2 Second component (e.g. Y coordinate or green channel)
 * @param val3 Third component (e.g. Z coordinate or blue channel)
 * @param val4 Fourth component (e.g. W coordinate or alpha/transparency channel)
 * @param p_targetDestination Pointer holding the address of the destination structure.
 */
void math_set_vector4(u32 val1, u32 val2, u32 val3, u32 val4, u32* p_targetDestination) {
	// Get the real base address of the destination object
	u32 base_address = *p_targetDestination;

	// Store the 4 components contiguously in PS2 memory (4-byte steps)
	*(u32*)(base_address + 0x0) = val1;
	*(u32*)(base_address + 0x4) = val2;
	*(u32*)(base_address + 0x8) = val3;
	*(u32*)(base_address + 0xC) = val4;
}

/**
 * @brief Fills a memory block with a specific value (memset).
 * Simplified functional version of the routine optimized for the PS2 multimedia registers.
 * Original Ghidra address: 0x00115484 (PAL)
 *
 * @param dest Pointer to the memory block to fill.
 * @param value Byte value to fill it with.
 * @param size Number of bytes to write.
 * @return void* Pointer to the destination memory.
 */
void* ee_memset(void* dest, u8 value, u32 size) {
	u8* ptr = (u8*)dest;

	// On a real PS2 an optimized 32- and 8-byte loop runs here,
	// using vector instructions if the pointer is 16-byte aligned.
	// Functionally, the exact behaviour is:
	for (u32 i = 0; i < size; i++) {
		ptr[i] = value;
	}

	return dest;
}

/**
 * @brief Controls the visibility or scale factor of a HUD component.
 * Writes 1.0f (0x3f800000) or 0.0f into the element's transformation property.
 * Original Ghidra address: 0x00337B48 (PAL)
 *
 * @param p_widget Destination visual component (param_1 / register a0)
 * @param enable Boolean state to enable or disable (param_2 / register a1)
 */
void hud_set_widget_visibility(u32* p_widget, long enable) {
	// Offset +0x10 (16 bytes) holds a pointer to the floating-point property (e.g. opacity/alpha or scale)
	f32** p_target_property = (f32**)((u8*)p_widget + 0x10);

	if (enable != 0) {
		// On the PS2 it writes 0x3f800000, which equals 1.0f (maximum visibility/scale)
		**p_target_property = 1.0f;
		return;
	}

	// If false, write 0.0f (completely hidden/disabled)
	**p_target_property = 0.0f;
	return;
}

/**
 * @brief Pointer pass-through stub function (identity function).
 * Originally used in the engine for validation macros or node abstraction.
 * Original Ghidra address: 0x00338B00 (PAL)
 *
 * @param param_1 First parameter (omitted from the return value)
 * @param p_node Secondary node pointer returned directly (param_2)
 * @return void* The same pointer received in param_2.
 */
void* core_identity_stub(long param_1, void* p_node) {
	// The Ghidra decompiler shows that the PS2 simply moves the input register
	// to the output register immediately (move $v0, $a1).
	return p_node;
}
