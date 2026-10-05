# HUD Static Resource Registry - Ratchet & Clank 2 (PAL)

This document lists the RAM addresses of the text strings and mathematical constants used by the Insomniac Games engine to create and render the graphical user interface elements.

## UI Data Segment (0x001AE750 - 0x001AE767)

| Memory Address | Data Type | Value / String | Purpose / Estimated Use in the Engine |
| :--- | :--- | :--- | :--- |
| **`0x001AE750`** | `char[]` (string) | `“BoltIcon”` | Texture resource identifier for the 3D bolt icon in the HUD. |
| **`0x001AE75C`** | `float` (f32) | `4.0f` (`40800000h`) | Floating-point scale factor or base offset for positioning icons on screen. |
| **`0x001AE760`** | `char[]` (character string) | `“%d/%d”` | Text format string passed to `game_sprintf` to render the ammunition counters (current/maximum). |

---

*Note: These constants are reserved for documentation purposes until the functions for initializing and updating the visual layout in the ammunition block are fully deciphered.*

## Quick Selection Menu Segment (0x001AE058 - 0x001AE072)

| Memory Address | Data Type | Commercial Value / String | Purpose / Intended Use in the Engine |
| :--- | :--- | :--- | :--- |
| **`0x001AE058`** | `char[]` (String) | `“QSelBack”` | Background texture for the radial Quick Select menu interface (*Quick Select Background*). |
| **`0x001AE068`** | `char[]` (String) | `“QSelBord”` | Graphic resource for the outer ring or border of the Quick Select radial menu (*Quick Select Border*). |

## Kernel Character Control Tables (PAL Region)

| Memory Address | Data Type | Estimated Technical Name | Estimated Purpose / Use in the Engine |
| :--- | :--- | :--- | :--- |
| **`0x0013A388`** | `uint32_t[]` (Array) | `_ctype_b_ptr_array` | Array of location pointers. The first index (`0x0013A388`) points directly to `0x0013A3C0`, which contains the actual array of ASCII property bitmasks (digits, spaces, letters) used by `ee_strtoll`. |

## Main HUD Canvas Segment (0x001AE6D8 - 0x001AE700)

| Memory Address | Data Type | Commercial Value / String | Purpose / Intended Use in the Engine |
| :--- | :--- | :--- | :--- |
| **`0x001AE6D8`** | `char[]` (String) | `“HudBase”` / `“HudCanvas”`| Invisible master container or base coordinate canvas for the HUD. |
| **`0x001AE6E8`** | `char[]` (String) | `“HealthBarOutline”`| Aesthetic outline resource for the health meter (Nanotech). |
| **`0x001AE6F8`** | `char[]` (String) | `“HealthBarFill”`   | Visual resource for the dynamic fill of the Nanotech bar. |

## Core Signs and Gauges Initialization Segment (0x001AE700 - 0x001AE755)

| Memory Address | Data Type | Value / String | Purpose / Estimated Use in the Engine |
| :--- | :--- | :--- | :--- |
| **`0x001AE700`** | `char[]` (String) | `“WeaponName”` / `“AmmoText”` | Dynamic text displaying the weapon name or the number of bullets on the HUD. |
| **`0x001AE710`** | `char[]` (String) | `“WeaXP”` | Visual component of the level or experience bar for the weapon in use. |
| **`0x001AE720`** | `char[]` (String) | `“AmmoIcon”` | Main texture or outline of the bullet/projectile for the selected weapon. |
| **`0x001AE730`** | `char[]` (String) | `“AmmoIconBack”` | Background or contrast shadow for the ammunition icon. |
| **`0x001AE740`** | `char[]` (String) | `“BoltText”` | Numeric label that displays the accumulated amount of bolts (wallet). |

## Dynamic Templates for the Quick Select Radial Menu (0x001AE075 - 0x001AE090)

| Memory Address | Data Type | Commercial Value / String | Purpose / Intended Use in the Engine |
| :--- | :--- | :--- | :--- |
| **`0x001AE078`** | `char[]` (String) | `“QSelIcon”` | Central base icon for the quick weapon selection menu. |
| **`0x001AE088`** | `char[]` (String) | `“QSelBI%d”` | Text mask formatted by vsnprintf to initialize the radial inventory slots in burst mode. |


| Memory Address | Data Type | Commercial Value / String | Purpose / Intended Use in the Engine |
| :--- | :--- | :--- | :--- |
| **`0x001AE098`** | `char[]` (String) | `“QSelIco%d”` | Dynamic template used by vsnprintf to initialize the weapon icon textures within each slot of the radial menu. |
