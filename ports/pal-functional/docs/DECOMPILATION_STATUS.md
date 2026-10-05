# English

# Español

# Ratchet & Clank 2 - Decompilación funcional

## Estado del proyecto

Proyecto en etapa inicial de reconstrucción funcional.

El objetivo es recuperar progresivamente el comportamiento del juego
original de Ratchet & Clank 2 (PS2 PAL), adaptando las funciones
decompiladas a C para sistemas modernos de PC.

La implementación busca mantener el comportamiento y la lógica del
juego lo más fiel posible al original, sustituyendo únicamente las
partes dependientes del hardware y APIs específicas de PlayStation 2
por equivalentes adecuados para PC.

---

## Infraestructura recuperada

### Kernel portátil

- [x] Kernel portátil
- [x] Sistema de hilos virtuales
- [x] Semáforos virtuales
- Archivo: `ps2_kernel.c`
- Referencia: `[INDEX]`

Esta capa proporciona una representación portable de algunos servicios
del kernel de PS2 necesarios para ejecutar la lógica recuperada.

---

### Bus de comunicaciones y sistema de almacenamiento

- [x] Bus de comunicaciones
- [x] Simulación del lector de DVD
- [x] Esqueleto del sistema de Memory Card
- Archivo: `ps2_sif.c`
- Referencia: `[INDEX]`

Esta capa reproduce las interfaces necesarias para que el código
recuperado pueda interactuar con los sistemas de comunicación y
almacenamiento originales.

---

### Utilerías de texto

- [x] Utilerías aceleradas de texto
- Archivo: `ps2_string.c`
- Referencia: `[INDEX]`

Adaptación de las utilerías de manipulación de texto utilizadas
por el código original.

---

## Lógica del sistema

### Secuencia de arranque / Intro

- [x] Máquina de estados raíz de la intro
- Función: `sys_boot_intro_state_machine`
- Referencia: `[INDEX]`

Esta función representa la máquina de estados principal utilizada
durante la secuencia inicial del juego.

---

## Sistemas pendientes

### Plataforma

- [ ] Graphics Canvas Init
- [ ] Game Frame / V-Sync / Engine Clock
- [ ] Pad Input Subsystem
- [ ] Punto de entrada / ejecutable

### Gráficos

- [ ] Inicialización del sistema gráfico
- [ ] Canvas / framebuffer
- [ ] Renderizado
- [ ] Presentación del frame

### Entrada

- [ ] Lectura del mando
- [ ] Estado de botones
- [ ] Sticks analógicos
- [ ] Adaptación del DualShock 2 a PC

### Tiempo

- [ ] Reloj del motor
- [ ] Sincronización de frames
- [ ] V-Sync
- [ ] Delta time / temporización

---

## Funciones decompiladas

Las funciones recuperadas se mantendrán inicialmente con su nombre
original de Ghidra cuando su propósito o estructura todavía no haya
sido confirmado.

Una función podrá recibir un nombre descriptivo posteriormente cuando
exista suficiente evidencia sobre su comportamiento.

Ejemplo:

    FUN_80012340
        ↓
    actualizar_reloj_motor

No se deben asumir nombres, estructuras o significados de offsets
sin evidencia suficiente.

---

## Documentación

La documentación de las funciones y sistemas recuperados se
almacenará en:

    docs/

La implementación en C se almacenará principalmente en:

    src/

Las declaraciones y estructuras compartidas se almacenarán en:

    include/

Las herramientas auxiliares utilizadas durante la investigación
se almacenarán en:

    tools/

---

## Criterio de adaptación

El código de PS2 se utilizará como referencia del comportamiento
original.

La adaptación para PC puede reemplazar:

- APIs específicas de PS2
- hardware de PS2
- sistema gráfico
- sistema de entrada
- temporización
- servicios del kernel

Sin embargo, la lógica propia del juego debe conservarse siempre
que sea posible.

Cuando el comportamiento de una función todavía no esté confirmado,
se debe documentar la incertidumbre en lugar de introducir una
suposición como si fuera un hecho.

---

## Progreso actual

### Infraestructura

- [x] `ps2_kernel.c`
- [x] `ps2_sif.c`
- [x] `ps2_string.c`

### Sistema de arranque

- [x] `sys_boot_intro_state_machine`

### Ejecución en PC

- [ ] Inicialización gráfica
- [ ] Reloj del motor
- [ ] Entrada
- [ ] Entry Point
- [ ] Primer arranque funcional

---

## Próximo objetivo

Implementar progresivamente las capas necesarias para conseguir
el primer arranque observable del juego en PC.

Orden inicial previsto:

1. Graphics Canvas Init
2. Game Frame / V-Sync / Engine Clock
3. Pad Input Subsystem
4. Entry Point / ejecutable

El orden puede modificarse según las dependencias descubiertas
durante la decompilación.