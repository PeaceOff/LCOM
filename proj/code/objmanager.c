#include "objmanager.h"
#include "list.h"
#include "stdio.h"
#include "sprite.h"
#include "player.h"
#include "font.h"
#include "bullet.h"
#include "enemymanager.h"
#include "ball.h"
#include "particle.h"

static List* lista;

static ListNode* stPlane;
static ListNode* ndPlane;
static ListNode* rdPlane;


void OM_Init(){
	lista = List_Create();
	stPlane = (ListNode*)malloc(sizeof(ListNode));
	ndPlane = (ListNode*)malloc(sizeof(ListNode));
	rdPlane = (ListNode*)malloc(sizeof(ListNode));
	lista->node->conteudo=NULL;
	lista->node->nextNode = rdPlane;

	rdPlane->conteudo = NULL;
	rdPlane->nextNode = ndPlane;

	ndPlane->conteudo = NULL;
	ndPlane->nextNode = stPlane;

	stPlane->conteudo = NULL;
	stPlane->nextNode = NULL;
}
void OM_AddObject(void* obj,enum object_type tipo , enum priority p){
	//if(obj==NULL) return;
	Debug_LOG("OM_AddObject","Inicializado");
	Debug_LOGInt("OM_AddObject Tipo",tipo);
	Debug_LOGInt("OM_AddObject Prioridade",p);

	Object *obj1 = (Object*)malloc(sizeof(Object));
	obj1->type = tipo;
	obj1->object = obj;

	ListNode* listTemp =(ListNode*)malloc(sizeof(ListNode));
	listTemp->conteudo = obj1;
	ListNode* nextNode;

	switch(p){
	case FIRST_PLANE:
		Debug_LOG("OM_AddObject","Primeiro Plano");
		nextNode = stPlane->nextNode;
		stPlane->nextNode = listTemp;
		listTemp->nextNode = nextNode;

		break;
	case SECOND_PLANE:
		Debug_LOG("OM_AddObject","Segundo Plano");
		nextNode = ndPlane->nextNode;
		ndPlane->nextNode = listTemp;
		listTemp->nextNode = nextNode;

		break;
	case THIRD_PLANE:
		Debug_LOG("OM_AddObject","Terceiro Plano");
		nextNode = rdPlane->nextNode;
		rdPlane->nextNode = listTemp;
		listTemp->nextNode = nextNode;

		break;
	case USER_INTERFACE:
		Debug_LOG("OM_AddObject","UI");
		free(listTemp);
		List_Add(lista,obj1);

		break;
	}

}

inline void OM_DestroyObj(Object* obj){
	if(obj==NULL) return;

	switch(obj->type){
	case MAP:
		Debug_LOG("OM_DestroyObj","Map");
		Map_Exit();
		break;

	case OBJ_BALL:
		Debug_LOG("OM_DestroyObj","Ball");
		Ball_Delete((Ball*)obj->object);
		break;

	case PLAYER:
		Debug_LOG("OM_DestroyObj","Player");
		Player_Delete((Player*)obj->object);
		break;

	case FONT:
		Debug_LOG("OM_DestroyObj","Font");
		Font_Delete((Font*)obj->object);
		break;

	case SPRITE:
		Debug_LOG("OM_DestroyObj","Sprite");
		Sprite_Delete((Sprite*)obj->object);
		break;

	case SUB_SPRITE:
		Debug_LOG("OM_DestroyObj","Sub Sprite");
		SubSprite_Delete((SubSprite*)obj->object);
		break;

	case BULLET:
		Debug_LOG("OM_DestroyObj","Bullet");
		Bullet_Delete((Bullet*)obj->object);
		break;

	case ENEMYMANAGER:
		Debug_LOG("OM_DestroyObj","EnemyManager");
		EM_Exit();
		break;

	case OBJ_PARTICLE:
		Debug_LOG("OM_DestroyObj","Particle");
		Particle_Delete((Particle*)obj->object);
		break;
	}
	obj->object=NULL;
	free(obj);
}

inline char OM_UpdateObj(Object* obj){
	void* conteudo = obj->object;

	switch(obj->type){
	case MAP:
		Debug_LOG("OM_UpdateObj","Mapa");
		Map_Draw();
		break;

	case OBJ_BALL:
		Debug_LOG("OM_UpdateObj","Ball");
		Ball_Update( (Ball*) obj->object );
		Ball_Draw( (Ball*) obj->object );
		break;

	case PLAYER:
		Debug_LOG("OM_UpdateObj","Player");
		if(!Player_Update((Player*) obj->object )){
			OM_DestroyObj(obj);
			return 1;
		}
		Player_Draw((Player*)obj->object);

		break;

	case FONT:
		Debug_LOG("OM_UpdateObj","Fonte");
		Font_WriteFont((Font*)obj->object);
		break;

	case SPRITE:
		Debug_LOG("OM_UpdateObj","Sprite");
		if( ((Sprite*)conteudo)->destroy )
			OM_DestroyObj(obj);
		else
			Sprite_Draw((Sprite*)obj->object);

		break;
	case SUB_SPRITE:
		Debug_LOG("OM_UpdateObj","SubSprite");
		if( ((SubSprite*)conteudo)->s->destroy)
			OM_DestroyObj(obj);
		else
			SubSprite_Draw((SubSprite*)obj->object);
		break;

	case BULLET:
		Debug_LOG("OM_UpdateObj","Bullet");
		if(!Bullet_Update((Bullet*)obj->object)){
			OM_DestroyObj(obj);
			return 1;
		}
		Bullet_Draw((Bullet*)obj->object);
		break;

	case ENEMYMANAGER:
		Debug_LOG("OM_UpdateObj","Enemy Manager");
		EM_Update();
		EM_Draw();
		break;

	case OBJ_PARTICLE:
		Debug_LOG("OM_UpdateObj","Particle");
		if(!Particle_Update((Particle*)obj->object)){
			OM_DestroyObj(obj);
			return 1;
		}
		Particle_Draw((Particle*)obj->object);
		break;

	}
	return 0;
}

void OM_Update(){
	if(lista==NULL) return;

	ListNode* temp = lista->node;
	Debug_LOG("###########","##############");
	Debug_LOG("OM_Update","Inicializado");
	Debug_LOGInt("OM_Update : Items",List_Count(lista));

	while(temp ->nextNode!=NULL ){

		Object* object = (Object *)temp->nextNode->conteudo;

		if(temp->nextNode == stPlane || temp->nextNode == ndPlane || temp->nextNode == rdPlane){
			Debug_LOG("OM_Update",">>>>>Marcador<<<<<");
			temp=temp->nextNode;
			continue;
		}
		if(object==NULL)
			Debug_LOG("OM_Update","Objeto Nulo");
		else{
			Debug_LOG("OM_Update","Objeto nao Nulo");
		}

		if(OM_UpdateObj(object)){//Se For removido;
			Debug_LOG("OM_Update","Objeto removido!");
			ListNode* next = temp->nextNode->nextNode;
			free(temp->nextNode);
			temp->nextNode = next;
			continue;

		}

		temp=temp->nextNode;
	}
	Debug_LOGInt("OM_Update : Items",List_Count(lista));

}

void OM_Exit(){
	Debug_LOG("OM_Exit","Inicializado");
	if(lista==NULL) return;
	ListNode* node = lista->node->nextNode;

	while(node!=NULL){
		Debug_LOG("OM_Exit","Encontrado Obj");

		if(node == stPlane || node == ndPlane || node == rdPlane)
			Debug_LOG("OM_Update",">>>>>Marcador<<<<<");

		void* obj = node->conteudo;
		if(node->conteudo!=NULL){
			Debug_LOG("OM_Exit","ApagarObj");
			OM_DestroyObj((Object *)obj);
		}

		node=node->nextNode;
	}

	List_Destroy(lista);

	if(lista==NULL)
		Debug_LOG("OM_Update","Lista NULA!");

	lista=NULL;
	if(lista==NULL)
		Debug_LOG("OM_Update","2:Lista NULA!");

}


