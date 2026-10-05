#include <stdint.h> // ¡ESTA LÍNEA REPARA LOS TIPOS COMO uint32_t y uint64_t!

// Declaramos la estructura para que el compilador sepa de qué tamaño es.
// Si ya la tienes definida en un archivo .h (ej. ps2_gs.h), pon el include correspondiente:
// #include "ps2_gs.h"
// De lo contrario, puedes poner este cascarón estructural arriba para cumplir con el enlace:
// Definimos la estructura real del paquete del sintetizador gráfico (GS) para PC
typedef struct {
	// Un qword de PS2 (128 bits) empaquetado se representa en PC moderna como 
	// un arreglo de enteros de 64 bits. Le asignamos un tamaño lo suficientemente
	// grande (por ejemplo, 16 o 32 elementos) para cubrir los índices [0] a [12] que usa el juego.
	uint64_t qword[32];
} Ps2GsLoadImagePacket;

uint32_t sceGsSetDefLoadImage(
	Ps2GsLoadImagePacket* packet,
	int dbp,
	int dbw,
	int dpsm,
	int dsax,
	int dsay,
	int rrw,
	int rrh
)
{
	uint64_t transfer_size = 0;

	switch (dpsm) {
	case 0:
	case 0x30:
		transfer_size = (uint64_t)(rrw * rrh >> 2);
		break;

	case 1:
	case 0x31:
		transfer_size = (uint64_t)(rrw * rrh * 3 >> 4);
		break;

	case 2:
	case 10:
	case 0x32:
	case 0x3a:
		transfer_size = (uint64_t)(rrw * rrh >> 3);
		break;

	case 0x13:
	case 0x1b:
		transfer_size = (uint64_t)(rrw * rrh >> 4);
		break;

	case 0x14:
	case 0x24:
	case 0x2c:
		transfer_size = (uint64_t)(rrw * rrh >> 5);
		break;
	}

	if (transfer_size >= 0x8000) {
		boot_txt_render_extended_string(
			(const uint8_t*)
			"sceGsSetDefLoadImage: too big size\r\n",
			(int64_t)(dbp << 16),
			transfer_size,
			dsax,
			rrh,
			(int64_t)(dsay << 16),
			rrw,
			(int64_t)(rrh << 16)
		);

		return 0;
	}

	/*
	 * Inicialización del paquete.
	 */
	for (int i = 0; i < 12; ++i) {
		packet->qword[i] = 0;
	}

	/*
	 * BITBLTBUF
	 */
	packet->qword[2] =
		((uint64_t)(dbp & 0xFFFF) << 32) |
		((uint64_t)(dbw & 0xFFFF) << 48) |
		((uint64_t)(dpsm & 0xFF) << 56);

	packet->qword[3] = 0x50;

	/*
	 * TRXPOS
	 */
	packet->qword[4] =
		((uint64_t)(dsax & 0xFFFF) << 32) |
		((uint64_t)(dsay & 0xFFFF) << 48);

	packet->qword[5] = 0x51;

	/*
	 * TRXREG
	 */
	packet->qword[6] =
		((uint64_t)(rrw & 0xFFFF)) |
		((uint64_t)(rrh & 0xFFFF) << 32);

	packet->qword[7] = 0x52;

	/*
	 * TRXDIR
	 *
	 * 0 = host -> local
	 */
	packet->qword[8] = 0;
	packet->qword[9] = 0x53;

	ps2_sync(0);

	return 6;
}