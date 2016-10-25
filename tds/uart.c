#include "uart.h"
#include <minix/syslib.h>
#include "minix/sysutil.h"
#include <minix/drivers.h>
#include <minix/com.h>
#include "keyboard.h"
#include "debug.h"
#include "list.h"

static List* buffer;
static List* sendBuffer;
static unsigned int uartHook;
static char transEmpty =1;

char UART_TransmitterEmpty(){
	unsigned long status = 0;
	sys_inb(COM1_BASE + LSR, &status);
	return (status & BIT(5));
}

char UART_NeedToSend(){
	if(sendBuffer == NULL) return 0;
	if(UART_TransmitterEmpty())
		transEmpty=1;
	return ( ( transEmpty )&& List_Count(sendBuffer) > 0);
}

int UART_SendCharInt(unsigned int addr, char ch){

	sys_outb(addr+UART_DATA, ch);
	Debug_LOG("UART_SendCharInt","Char Enviado com sucesso");
}

void UART_ClearBuffer(char bufferID){
	List* b = NULL;
	if(bufferID)
		b = buffer;
	else
		b = sendBuffer;

	if(b == NULL)return;
	ListNode* node = b->node;
	while(node->nextNode !=NULL){
		ListNode* current = node->nextNode;
		char* conteudo = current->conteudo;

		List_Remove(b,conteudo);
		free(conteudo);
	}

}

int UART_SendNextBuffer(unsigned int addr){

	if(sendBuffer!=NULL);
	ListNode* node = sendBuffer->node->nextNode;

	if(node == NULL){

		return 0;
	}
	char *ch = (char*)node->conteudo;

	printf("A enviar caracter %c \n",*ch);
	Debug_LOGInt("UART_SendNextBuffer: List Size:", List_Count(sendBuffer));
	UART_SendChar(addr, *ch);
	List_Remove(sendBuffer,node->conteudo);
	free(ch);

	transEmpty=0;

}

int UART_SendMsg(char* msg){
	int i =0;
	printf("A Enviar Mensagem:\n");
	while(msg[i] != '!'){
		char *ch = (char*)malloc(sizeof(char));
		*ch = msg[i];
		List_Add(sendBuffer,ch);
		i++;
	}
	printf("Mensagem enviada para buffer tamanho: %d\n",i );
	Debug_LOGInt("UART_SendMsg: Chars of Msg:",i);
}


int UART_SendChar(unsigned int addr, char ch){

	unsigned long LSRReg;
	sys_inb(addr + LSR, &LSRReg);
	if(LSRReg & ( BIT(5))){
		sys_outb(addr+UART_DATA, ch);
		Debug_LOG("UART_SendChar","Char Enviado com sucesso");
		printf("Sent\n");
		return 0;
	}
	Debug_LOG("UART_SendChar","Erro a enviar Char!");
	printf("Error Sending!\n");
	transEmpty=0;
	return 1;
}

List* UART_ReturnBuffer(){
	return buffer;
}

void UART_ChangeBitrate(unsigned int addr, unsigned short bitrate){
	UART_SetDLAB(addr,1);
	unsigned long lsb = (char)(bitrate & 0xFF);
	unsigned long msb = (char)(bitrate>>8 & 0xFF);

	sys_outb(addr + DLL, lsb);
	sys_outb(addr + DLM, msb);

	UART_SetDLAB(addr,0);

}


int UART_SetConfiguration(unsigned int addr,unsigned char word_length, unsigned char stop_bits, unsigned char parity,unsigned short bitrate){
	UART_SetDLAB(addr,0);
	if(word_length > 3)
		return 1;
	if(stop_bits > 1)
		return 2;
	if(parity > 7)
		return 3;

	unsigned long reg = 0;
	sys_inb(addr + LCR, &reg);

	/*
	unsigned long debug = reg;
	int i;
	printf("Bits to write: 0x%x binary:",debug);
	for(i=0; i <= 7 ; i++){
		printf("%d" ,(debug >> (7-i))&1);
	}
	printf("\n");/**/


	//Preservar SetBreak
	reg &= BIT(6);
	printf("%d \n",parity);
	reg |= (word_length & 3);
	reg |= (stop_bits & 1) << 2;
	reg |= (parity & 7) << 3;




	sys_outb(addr + LCR, reg);

	UART_ChangeBitrate(addr,bitrate);

	return 0;
}

void UART_GetConfig(unsigned int addr){

	unsigned long res;
	sys_inb(addr + IER, &res);

	printf("Receive Data: %d \n" , ((res & BIT(0))? 1 : 0));
	printf("Transmitter empty: %d \n" , ((res & BIT(0))? 1 : 0));
	printf("Line Status Int: %d \n" , ((res & BIT(0))? 1 : 0));

}

int UART_SubscribeInt(unsigned int addr, unsigned int port){
	UART_SetDLAB(addr, 0);
	unsigned long res = BIT(0) | BIT(1) | BIT(2);
	sys_outb(addr + IER , res);

	buffer = List_Create();
	sendBuffer = List_Create();
	uartHook = port;
	return geral_subscribe_int(&uartHook, port);

}

int UART_ClearFIFO(unsigned int addr){
	Debug_LOG("UART_ClearFIFO", "Inicializado");
	unsigned long lsr;
	sys_inb(addr + LSR, &lsr);
	while(lsr & BIT(0)){
		printf("ClearFifo!\n");
		unsigned long ch;
		printf("0x%x\n",addr);
		sys_inb(addr+UART_DATA, &ch);

		printf("Lixo: %c %d \n",(char)ch, ch);
		sys_inb(addr + LSR, &lsr);
	}
}

void UART_DisableFIFO(unsigned int addr){
	Debug_LOG("UART_EnableFIFO","Inicializado");
	unsigned long command=0;
	command |= BIT(FIFO_CR_BIT);
	command |= BIT(FIFO_CT_BIT);
	command |= (FIFO_TRIGGER_8 << FIFO_INT_TRIGGER_BIT);
	command |= BIT(FIFO_64BITS);

	/*printf("\nFifo Config:");
	int i;
	for (i = 0 ; i < 8 ; i++){
		printf("%d", ((command >> (7-i)) & 1));
	}
	printf("\n");/**/

	sys_outb(addr + FCR, command);

//	UART_ClearFIFO(addr);
}

void UART_EnableFIFO(unsigned int addr){
	Debug_LOG("UART_EnableFIFO","Inicializado");
	unsigned long command=0;
	command |= BIT(FIFO_ENABLE_BIT);
	command |= BIT(FIFO_CR_BIT);
	command |= BIT(FIFO_CT_BIT);
	command |= (FIFO_TRIGGER_8 << FIFO_INT_TRIGGER_BIT);
	command |= BIT(FIFO_64BITS);

	/*printf("\nFifo Config:");
	int i;
	for (i = 0 ; i < 8 ; i++){
		printf("%d", ((command >> (7-i)) & 1));
	}
	printf("\n");/**/

	sys_outb(addr + FCR, command);

//	UART_ClearFIFO(addr);
}

int UART_UnSubscribeInt(){
	return geral_unsubscribe_int(&uartHook);
}

void UART_PrintError(unsigned int addr){
	printf("\nLSR STATUS\n");
	unsigned long status = 0;
	sys_inb(addr + LSR, &status);
	printf("Receiver: %d \n",((status & BIT(0))? 1: 0));
	printf("Overrun: %d \n",((status & BIT(1))? 1: 0));
	printf("Parity: %d \n",((status & BIT(2))? 1: 0));
	printf("Framing: %d \n",((status & BIT(3))? 1: 0));
	printf("Break: %d \n",((status & BIT(4))? 1: 0));
	printf("Transmiter Holding Empty: %d \n",((status & BIT(5))? 1: 0));


}

char UART_ReceiveChar(unsigned int addr){
	printf("UART_ReceiveChar Initialized\n");
	unsigned long LSRReg;

	int tries = 5;
	sys_inb(addr + LSR, &LSRReg);

	while(tries--){
		while(LSRReg & (BIT(0))){
			unsigned long ch;
			sys_inb(addr+UART_DATA, &ch);
			char* c = (char*)malloc(sizeof(char));
			*c= (char)ch;
			printf("Char Received: %c \n",(char)ch);
			List_Add(buffer,c);
			sys_inb(addr + LSR, &LSRReg);
		}
		tickdelay(20);
	}
}


inline char UART_IntHandler(unsigned int addr){
	unsigned long iir;
	sys_inb(addr + IIR, &iir);
	Debug_LOG("UART_INT_HANDLER","Inicializado");

	if(!(iir & BIT(0))){

		Debug_LOG("UART_INT_HANDLER","UART Interrupt");
		printf("Uart Interrupt Binary: ");
		int i; for(i=0; i <= 7 ; i++){
		printf("%d" ,(iir >> (7-i))&1);
		}/**/
		printf("\n");

		char fifoEnabled = 0;

		int teste = iir;
		teste = (iir >> 6) & 3;

		switch(teste){
		case FIFO_DISABLED:
			Debug_LOG("UART_INT_HANDLER","No FIFO");
			break;
		case FIFO_UNUSABLE:
			Debug_LOG("UART_INT_HANDLER","FIFO Unusable");
			break;
		case FIFO_ENABLED:
			Debug_LOG("UART_INT_HANDLER","FIFO Enabled");
			unsigned long lsr;
			sys_inb(addr + LSR, &lsr);
			while(lsr & BIT(0)){
				printf("FIFO MENSAGEM!\n");
				unsigned long ch;
				sys_inb(addr+UART_DATA, &ch);
				Debug_LOG("UART_INT_HANDER","Recebido");
				Debug_LOGInt("UART_INT_HANDER",ch);
				Debug_LOGInt("UART_INT_HANDER",'M');
				printf("Char Received: %c %d \n",(char)ch, ch);
				sys_inb(addr + LSR, &lsr);

				char* c = (char*)malloc(sizeof(char));
				*c= (char)ch;
				List_Add(buffer,c);
			}/**/
			fifoEnabled=1;
			printf("FIFO Enabled \n");
			break;
		default:
			break;
		}
		teste = iir >> 1;
		teste = teste & 7;
		switch( teste ){
		case INT_MODEM:
			Debug_LOG("UART_INT_HANDLER","Modem Int #####################################");
			printf("Modem Int\n");
			break;

		case INT_TRANS_EMPTY:
			Debug_LOG("UART_INT_HANDLER","Tansmiter empty ###############################");
			printf("Transmiter empty\n");
			transEmpty = 1;
			//UART_SendNextBuffer(addr);

			break;

		case INT_DATA_AVAILABLE:
			Debug_LOG("UART_INT_HANDLER","Data Available  ###############################");
			printf("Data Available\n");

			if(fifoEnabled){
				unsigned long lsr;
				sys_inb(addr + LSR, &lsr);
				while(lsr & BIT(0)){
					printf("FIFO MENSAGEM!");
					unsigned long ch;
					sys_inb(addr+UART_DATA, &ch);
					Debug_LOG("UART_INT_HANDER","Recebido");
					printf("Char Received: %c %d \n",(char)ch, ch);
					sys_inb(addr + LSR, &lsr);

					char* c = (char*)malloc(sizeof(char));
					*c= (char)ch;
					List_Add(buffer,c);
				}
			}else
				UART_ReceiveChar(addr);
			break;

		case INT_LINE_STATUS:
			Debug_LOG("UART_INT_HANDLER","Line Status  ##################################");
			printf("Line Status\n");
			UART_PrintError(addr);
			break;

		case INT_TIMEOUT:
			Debug_LOG("UART_INT_HANDLER","TimeOut  ######################################");
			printf("TimeOut\n");
			break;
		default:
			printf("Desconhecido : %d", ((iir>>1) & 7));
			return;
			break;
		}

		//sys_inb(addr + IIR, &iir);
	}

	if(transEmpty){
		UART_SendNextBuffer(addr);
	}
	return 1;
}

void UART_PrintBuffer(){
	if(buffer != NULL);
	printf("\n Inicio Buffer \n");
	ListNode* node = buffer->node->nextNode;
	while(node !=NULL){
		printf("%c", *(char*)node->conteudo);
		node = node->nextNode;
	}
	printf("\n Fim Buffer\n");

}

void UART_SetDLAB(unsigned int addr,unsigned char status){
	unsigned long reg;

	sys_inb(addr + LCR, &reg);

	if(status){
		reg |= BIT(DLA_BIT);
	}else{
		reg &= ~BIT(DLA_BIT);
	}

	sys_outb(addr + LCR , reg);

}
