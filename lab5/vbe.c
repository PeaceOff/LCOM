#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>

#include "vbe.h"
#include "lmlib.h"
#include "graphic_info.h"
#include "i8042.h"

#define LINEAR_MODEL_BIT 14


int vbe_get_mode_info(unsigned short mode, vbe_mode_info_t *vmi_p) {

	mmap_t map;
	lm_init();
	if (lm_alloc(sizeof(vbe_mode_info_t), &map) == NULL)
		return 1;

	struct reg86u reg86;

	reg86.u.w.ax = 0x4F01; /* VBE get mode info *//* translate the buffer linear address to a far pointer */
	reg86.u.w.es = PB2BASE(map.phys); /* set a segment base */
	reg86.u.w.di = PB2OFF(map.phys); /* set the offset accordingly */
	reg86.u.w.cx = mode;
	reg86.u.b.intno = 0x10;

	if (sys_int86(&reg86) != OK) {
		lm_free(&map);
		return 1;
	}

	*vmi_p = *((vbe_mode_info_t *) map.virtual);

	return 0;
}

int vbe_return_info(vbe_info_block *vib,uint16_t **video_modes, unsigned int *n_video_modes){
	if (lm_init() == OK) return 1;
	mmap_t map;

	if(lm_alloc(sizeof(vbe_info_block),&map) == NULL) return 1;
	//Setting the signature to "VBE2"
	vib->VbeSignature[0] = 'V';
	vib->VbeSignature[1] = 'B';
	vib->VbeSignature[2] = 'E';
	vib->VbeSignature[3] = '2';

	struct reg86u r;

	r.u.b.intno = BIOS_INTERFACE; //Bios
 	r.u.b.ah = VBE_FUNCTION_CODE; //Codigo para indicar que pretendemos chamar uma funcao
 	r.u.b.al = VBE_RETURN_CONTROLLER_INFO; //Codigo da funcao VBE return mode info
 	r.u.w.es = PB2BASE(map.phys);
 	r.u.w.di = PB2OFF(map.phys);

 	if(sys_int86(&r) != OK) return 1;
 	if (r.u.w.ax != VBE_FUNCTION_SUPPORTED | VBE_FUNCTION_SUCCESSFULL) return 1;//Verifica se a funcao foi sucedida
 	*vib = *((vbe_info_block *)map.virtual); //Apontar para a o endereço virtual para utilizarmos as variaveis;
 	if (memcmp(vib->VbeSignature, VBE_SIGNATURE_VESA, sizeof(vib->VbeSignature) ) != 0) return 1; //Compara em memoria a signature

 	/**/
 	void *modesPtr = (void *)((vib->VideoModePtr & 0xffff0000) >> 12);
 	modesPtr += PB2OFF(vib->VideoModePtr);
 	modesPtr += (uint32_t)map.virtual & 0xF0000000;
 	/**/

 	//VideoModePtr end at 0FFFFh
 	lm_free(&map);//Limpar map da memoria
 	int16_t *m = modesPtr;
 	*n_video_modes = 0;
 	size_t i;
 	for (i = 0; *m != END_OF_VIDEO_MODES_PTR; m++) //Contar o numero de video modes, termina com 0FFFFh
 		*n_video_modes += 1;
 	//Alocar memoria para os modos de video (Retornar 1 no caso de nao ser possivel)
 	if((*video_modes = malloc(*n_video_modes * sizeof(uint16_t))) == NULL)
 		return 1;
 	//Copiar os modos de video para a variavel que é recebida por referencia e na qual enviamos os modos
 	for (i = 0; i < *n_video_modes; i++){
 		(*video_modes)[i] = *(int * )modesPtr;
 		modesPtr++;
 	}
 	return 0;
}

int print_vbe_info(vbe_info_block *v,uint16_t **video_modes, unsigned int *n_video_modes){
	printf("Capabilities: 0x%X\n", v->Capabilities);
	if(v->Capabilities & BIT(0)){
		printf("DAC width is switchable to 8 bits per primary color\n");
	} else {
		printf("DAC is fixed width, with 6 bits per primary color\n");
	}
	if(v->Capabilities & BIT(1)){
		printf("Controller is not VGA compatible\n");
	} else {
		printf("Controller is VGA compatible\n");
	}
	if(v->Capabilities & BIT(2)){
		printf("When programming large blocks of information to the RAMDAC, use the blank bit in Function 09h.\n");
	} else {
		printf("Normal RAMDAC operation\n");
	}
	printf("Video modes: ");
	size_t i;
	if((*n_video_modes) > 0) printf("0x%X", video_modes[0]);
	for (i = 1; i < (*n_video_modes); i++) {
		printf(", 0x%X\n", video_modes[i]);
	}
	free(*video_modes);
	printf("Size of VRAM memory: %lu KB\n", v->TotalMemory * 64);
	return 0;
}

