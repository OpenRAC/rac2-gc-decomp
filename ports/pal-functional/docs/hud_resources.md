# English

# HUD Static Resource Registry - Ratchet & Clank 2 (PAL)

This document lists the RAM addresses of the text strings and mathematical constants used by the Insomniac Games engine to create and render the graphical user interface elements.

## UI Data Segment (0x001AE750 - 0x001AE767)

| Memory Address | Data Type | Value / String | Purpose / Estimated Use in the Engine |
| :--- | :--- | :--- | :--- |
| **`0x001AE750`** | `char[]` (string) | `“BoltIcon”` | Texture resource identifier for the 3D guitar icon in the HUD. |
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
| **`0x001AE740`** | `char[]` (String) | `“BoltText”` | Numeric label that displays the accumulated amount of guitones (wallet). |

## Dynamic Templates for the Quick Select Radial Menu (0x001AE075 - 0x001AE090)

| Memory Address | Data Type | Commercial Value / String | Purpose / Intended Use in the Engine |
| :--- | :--- | :--- | :--- |
| **`0x001AE078`** | `char[]` (String) | `“QSelIcon”` | Central base icon for the quick weapon selection menu. |
| **`0x001AE088`** | `char[]` (String) | `“QSelBI%d”` | Text mask formatted by vsnprintf to initialize the radial inventory slots in burst mode. |


| Memory Address | Data Type | Commercial Value / String | Purpose / Intended Use in the Engine |
| :--- | :--- | :--- | :--- |
| **`0x001AE098`** | `char[]` (String) | `“QSelIco%d”` | Dynamic template used by vsnprintf to initialize the weapon icon textures within each slot of the radial menu. |

# Español

# Registro de Recursos Estáticos del HUD - Ratchet & Clank 2 (PAL)

Este documento centraliza las direcciones de memoria RAM de cadenas de texto (strings) y constantes matemáticas utilizadas por el motor de Insomniac Games para construir y renderizar los widgets de la interfaz gráfica.

## Segmento de Datos de la Interfaz (0x001AE750 - 0x001AE767)

| Dirección de Memoria | Tipo de Dato | Valor Comercial / Cadena | Propósito / Uso Estimado en el Motor |
| :--- | :--- | :--- | :--- |
| **`0x001AE750`** | `char[]` (String) | `"BoltIcon"` | Identificador del recurso de textura para el ícono tridimensional del guitón en el HUD. |
| **`0x001AE75C`** | `float` (f32) | `4.0f` (`40800000h`) | Factor de escala flotante o desplazamiento base para el posicionamiento de íconos en pantalla. |
| **`0x001AE760`** | `char[]` (String) | `"%d/%d"` | Cadena de formato de texto pasada a `game_sprintf` para renderizar contadores de munición (Actual/Máxima). |

---

*Nota: Estas constantes quedan en reserva documental hasta que se descifren por completo las funciones de inicialización y actualización del layout visual en el bloque de la munición.*

## Segmento del Menú de Selección Rápida (0x001AE058 - 0x001AE072)

| Dirección de Memoria | Tipo de Dato | Valor Comercial / Cadena | Propósito / Uso Estimado en el Motor |
| :--- | :--- | :--- | :--- |
| **`0x001AE058`** | `char[]` (String) | `"QSelBack"` | Textura base de fondo para la interfaz del menú radial de selección rápida de armas (*Quick Select Background*). |
| **`0x001AE068`** | `char[]` (String) | `"QSelBord"` | Recurso gráfico del anillo perimetral o borde del menú radial de selección rápida (*Quick Select Border*). |

## Tablas de Control de Caracteres del Kernel (Región PAL)

| Dirección de Memoria | Tipo de Dato | Nombre Técnico Estimado | Propósito / Uso Estimado en el Motor |
| :--- | :--- | :--- | :--- |
| **`0x0013A388`** | `uint32_t[]` (Array) | `_ctype_b_ptr_array` | Arreglo de punteros de localización. El primer índice (`0x0013A388`) apunta directamente a `0x0013A3C0`, que contiene la matriz real de máscaras de bits de propiedades ASCII (dígitos, espacios, letras) utilizada por ee_strtoll. |

## Segmento del Canvas Principal del HUD (0x001AE6D8 - 0x001AE700)

| Dirección de Memoria | Tipo de Dato | Valor Comercial / Cadena | Propósito / Uso Estimado en el Motor |
| :--- | :--- | :--- | :--- |
| **`0x001AE6D8`** | `char[]` (String) | `"HudBase"` / `"HudCanvas"`| Contenedor invisible maestro o lienzo base de coordenadas del HUD. |
| **`0x001AE6E8`** | `char[]` (String) | `"HealthBarOutline"`| Recurso del contorno estético del medidor de vida (Nanotech). |
| **`0x001AE6F8`** | `char[]` (String) | `"HealthBarFill"`   | Recurso visual del relleno dinámico de la barra de Nanotech. |

## Segmento de Inicialización de Letreros y Medidores del Core (0x001AE700 - 0x001AE755)

| Dirección de Memoria | Tipo de Dato | Valor Comercial / Cadena | Propósito / Uso Estimado en el Motor |
| :--- | :--- | :--- | :--- |
| **`0x001AE700`** | `char[]` (String) | `"WeaponName"` / `"AmmoText"` | Texto dinámico que despliega el nombre del arma o la cantidad de balas en el HUD. |
| **`0x001AE710`** | `char[]` (String) | `"WeaXP"` | Componente visual de la barra de nivel o experiencia del arma en uso. |
| **`0x001AE720`** | `char[]` (String) | `"AmmoIcon"` | Textura o silueta principal de la bala/proyectil del arma seleccionada. |
| **`0x001AE730`** | `char[]` (String) | `"AmmoIconBack"` | Fondo o sombra de contraste para el ícono de la munición. |
| **`0x001AE740`** | `char[]` (String) | `"BoltText"` | Letrero numérico que renderiza la cantidad acumulada de guitones (billetera). |

## Plantillas Dinámicas del Menú Radial Quick Select (0x001AE075 - 0x001AE090)

| Dirección de Memoria | Tipo de Dato | Valor Comercial / Cadena | Propósito / Uso Estimado en el Motor |
| :--- | :--- | :--- | :--- |
| **`0x001AE078`** | `char[]` (String) | `"QSelIcon"` | Icono base central del menú rápido de selección de armas. |
| **`0x001AE088`** | `char[]` (String) | `"QSelBI%d"` | Máscara de texto formateada por vsnprintf para inicializar las ranuras del inventario radial en ráfaga. |


| Dirección de Memoria | Tipo de Dato | Valor Comercial / Cadena | Propósito / Uso Estimado en el Motor |
| :--- | :--- | :--- | :--- |
| **`0x001AE098`** | `char[]` (String) | `"QSelIco%d"` | Plantilla dinámica utilizada por vsnprintf para inicializar las texturas de los íconos de las armas dentro de cada ranura del menú radial. |

