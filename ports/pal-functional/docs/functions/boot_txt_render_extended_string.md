# English

# boot_txt_render_extended_string

## Purpose

Renders a text string using the
typography system used during boot/debug.

The function receives a string and seven additional parameters
that are passed on to the text rendering system.

## Observed Behavior

1. Saves the current typography callback.
2. Temporarily replaces the callback.
3. Constructs a local block with the received parameters.
4. Calls `txt_render_scientific_string()`.
5. Restores the previous callback.
6. Returns the rendering result.

## Dependencies

- `txt_render_scientific_string`
- `g_hud_typography_callback_ptr`
- `sys_debug_console_write_char`
- `custom_hud_glyph_decoder` [custom adaptation]

## Status

Implemented.

## Pending

- Confirm the exact representation of the seven parameters.
- Review the behavior of `txt_render_scientific_string`.
- Determine whether `custom_hud_glyph_decoder` is part of the
  PC adaptation or corresponds to the original behavior.

# Español

# boot_txt_render_extended_string

## Propósito

Renderiza una cadena de texto utilizando el sistema de
tipografía utilizado durante el arranque/debug.

La función recibe una cadena y siete parámetros adicionales
que son reenviados al sistema de renderizado de texto.

## Comportamiento observado

1. Guarda el callback tipográfico actual.
2. Sustituye temporalmente el callback.
3. Construye un bloque local con los parámetros recibidos.
4. Llama a `txt_render_scientific_string()`.
5. Restaura el callback anterior.
6. Devuelve el resultado del renderizado.

## Dependencias

- `txt_render_scientific_string`
- `g_hud_typography_callback_ptr`
- `sys_debug_console_write_char`
- `custom_hud_glyph_decoder` [adaptación propia]

## Estado

Implementada.

## Pendientes

- Confirmar representación exacta de los siete parámetros.
- Revisar comportamiento de `txt_render_scientific_string`.
- Determinar si `custom_hud_glyph_decoder` forma parte de la
  adaptación PC o corresponde al comportamiento original.