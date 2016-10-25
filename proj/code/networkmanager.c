#include "networkmanager.h"
#include "uart.h"
#include "list.h"
#include <minix/syslib.h>
#include "map.h"
#include "player.h"
#include "ball.h"

static List* buffer;
static char isServer;
static char singlePlayer;
static Player* p1;
static Player* p2;

static int tilesLeft;
static char score2;

static char playerConnected;

void NM_ResetScore(){
	tilesLeft = 0;

}

void NM_ReadScore(char* msg){

}

void NM_SendScore(){


}

char NM_PlayerConnected(){
	return playerConnected;
}

void NM_SetPlayer(char id , Player* p){
	if(id == 1){
		p1 = p;
	}else if(id == 2){
		p2 = p;
	}else{
		Debug_LOG("NM_SetPlayer","Index Desconhecido");
	}
}

void NM_SendMessage(char* message){
	Debug_LOG("NM_SendMessage","Inicializado");
	UART_SendMsg(message);
}

char NM_PlayerDisconnected(){
	return !playerConnected;
}

void NM_SetSinglePlayer(char state){
	singlePlayer=state;
	if(!singlePlayer)
		playerConnected = 0;
}

void NM_SendPlayerInfo(){
	char* msg = Player_NetSendPos(p1);
	NM_SendMessage(msg);
	free(msg);
}

char NM_IsSinglePlayer(){
	return singlePlayer;
}

void NM_SetServer(char r){
	isServer = r;
}

char NM_IsServer(){
	return isServer;
}

char* NM_ListToArray(ListNode* begin , int size){
	Debug_LOG("NM_ListToArray","Inicializado!");
	Debug_LOGInt("NM_ListToArray: Size",size);

	if(List_Count(buffer) < size) return NULL;
	Debug_LOG("NM_ListToArray","Tamanho correto");
	char* msg = (char*)malloc(sizeof(char)*size);
	int i =0;

	while( i < size){
		char* charTemp = (char*)begin->conteudo;
		msg[i] = *charTemp;
		ListNode* tempNode = begin->nextNode;

		List_Remove(buffer,begin->conteudo);
		free(charTemp);

		begin = tempNode;
		i++;
	}
	return msg;

}

int NM_Parser(ListNode* node){
	if(node == NULL) return 1;
	char* obj = (char*)node->conteudo;
	if(obj !=NULL){
		Debug_LOGInt("NM_Parser: obj:",*obj);
	}else{
		Debug_LOG("NM_Parser","Mensagem Nula");
		List_Remove(buffer,obj);
		return;
	}
	char* msg = NULL;

	switch(*obj){

	case 'E':
		Debug_LOG("NM_Parser","Terrain Explosion!");
		msg = NM_ListToArray(node, sizeof(char)*3 + sizeof(int)*2);
		if(msg == NULL) return 1;
		Map_ReadExplosion(msg);
		free(msg);
		break;

	case 'S':
		Debug_LOG("NM_Parser","Score Sync!");
		msg = NM_ListToArray(node, sizeof(char)*3);
		if(msg == NULL) return 1;
		NM_ReadScore(msg);
		free(msg);
		break;

	case 'P':
		Debug_LOG("NM_Parser","Player Pos!");
		msg = NM_ListToArray(node, sizeof(int)*2 + sizeof(char)*1);
		if(msg == NULL) return 1;
		Player_NetReadPos(p2, msg);
		free(msg);
		break;

	case 'C'://Player connectado
		Debug_LOG("NM_Parser","Player Connectado!");
		List_Remove(buffer,obj);
		playerConnected=1;
		free(obj);
		break;

	case 'D': //Player desconectado
		Debug_LOG("NM_Parser","Player Desconectado!");
		List_Remove(buffer,obj);
		playerConnected=0;
		free(obj);
		break;

	case 'T':
		Debug_LOG("NM_Parser","Terreno!");
		msg = NM_ListToArray(node, sizeof(int)*2 + sizeof(char)*2);
		if(msg == NULL) return 1;
		Map_NetReadDamage(msg);
		free(msg);
		break;

	default:
		List_Remove(buffer,obj);
		if(obj != NULL){
			Debug_LOGInt("NM_Parser: Character unknown:",*obj);
			free(obj);
		}
		break;
	}
	return 0;

}

void NM_Update(){

	if(isServer)
		Debug_LOG("NM_Update","Servidor!");
	else
		Debug_LOG("NM_Update","Cliente!");

	if(buffer == NULL) return;

	Debug_LOG("NM_Update","Inicializado!");
	Debug_LOGInt("NM_Update : List Size",List_Count(buffer));
	Debug_LOGInt("NM_Update : List Empty",List_Empty(buffer));

	ListNode* node = buffer->node;

	while(!List_Empty(buffer)){
		if(NM_Parser(node->nextNode))
			return;
	}


}

void NM_Init(){
	buffer = UART_ReturnBuffer();
}

void NM_Exit(){
	p1 = NULL;
	p2 = NULL;
	UART_ClearBuffer(UART_BUFFER_SEND);
	UART_ClearBuffer(UART_BUFFER_RECEIVE);
}
