#include "enemymanager.h"
#include "enemy.h"
#include "stdio.h"
#include "player.h"
#include "rectangle.h"
#include "debug.h"
#include "map.h"
#include "networkmanager.h"
#include "vector.h"

static Player* player;
static Enemy** enemies;
static unsigned int enemyCount;
static unsigned int time;
static List* spawnpoints;

void EM_DamagePlayerInRadius(Vector2d pos, double raio, char dmg){
	if(player == NULL) return;

	Vector2d dir = Vector2_Sub(pos, player->pos);
	double dist = Vector2_Magnitude(dir);
	if(dist <= raio){
		player->health--;
	}
}

void EM_Init(unsigned int enemyNumber){
	enemies = (Enemy**)malloc(sizeof(Enemy*)*enemyNumber);
	int i;
	for(i = 0; i < enemyNumber; i++){
		if(i == 0){
			enemies[i] = Enemy_Create(500,500,E_ROBOT);
		}else if(i%2 == 0){
			enemies[i] = Enemy_Create(100,100,E_WORM);
		}else{
			enemies[i] = Enemy_Create(500,500,E_ROBOT);
		}

		enemies[i]->health=0;
		enemies[i]->draw=0;
	}
	spawnpoints=NULL;
	player=NULL;
	enemyCount=enemyNumber;
	time=0;
}

Enemy** EM_GetEnemies(){
	return enemies;
}

unsigned int EM_GetEnemyCount(){
	return enemyCount;
}

void EM_AddPlayer(Player* p){

	player=p;

}

void EM_ReadMsg(char* msg){
	Debug_LOG("EM_ReadMsg","Inicializado");

	if(enemies == NULL) return;

	if(msg[0] != 'M'){
		Debug_LOG("EM_ReadMsg","Mensagem invalida");
		return;
	}

	int posX;
	int posY;

	int i = sizeof(char);

	char id = msg[i];
	i+= sizeof(char);

	if(enemies == NULL || enemyCount < id )
		return;


	memcpy(&posX, msg+i , sizeof(int));
	i+= sizeof(int);

	memcpy(&posY, msg+i , sizeof(int));
	Vector2* vect = (Vector2*)malloc(sizeof(Vector2));
	vect->x = posX;
	vect->y = posY;

	//enemies[id]->pos.x=posX;
	//enemies[id]->pos.y=posY;


	List* pos = List_Create();
	List_Add(pos, vect);

	Enemy_MoveTo(enemies[id], pos);

	Enemy_Update(enemies[id]);

}

void EM_Update(){
	if(enemies==NULL)return;
	time++;
	int i;

	for(i = 0; i < enemyCount; i ++){
		if(time % 20 == 0){
			if(player != NULL && enemies[i]->draw){

				if(NM_IsServer() || NM_IsSinglePlayer()){
					List* pos = (List*)Map_GetPath((int)enemies[i]->pos.x,(int)enemies[i]->pos.y, (int)player->pos.x,(int)player->pos.y);
					Enemy_MoveTo(enemies[i], pos);

					if(NM_IsServer() && !NM_IsSinglePlayer()){
						char* msg = Enemy_NetPos(enemies[i], (char)i);
						Debug_LOG("EM_UPDATE","inimigo Update");
						NM_SendMessage(msg);
						free(msg);
					}
				}
			}
		}
		if(time%20 ==0 && !enemies[i]->draw){

			if(spawnpoints==NULL)
				spawnpoints=(List*)Map_GetSpawnPoints();
			else{
				ListNode * node = spawnpoints->node->nextNode;
				if(node == NULL){
					List_Destroy(spawnpoints);
					spawnpoints=(List*)Map_GetSpawnPoints();
				}
			}

			ListNode * node = spawnpoints->node->nextNode;
			Vector2d* vector = node->conteudo;
			List_Remove(spawnpoints, vector);
			 /**/
			time=0;
			enemies[i]->health=enemies[i]->maxHealth;
			enemies[i]->pos=*vector;
			free(vector);
			Enemy_Update(enemies[i]);
		}
		Enemy_Update(enemies[i]);
	}
}

void EM_Draw(){
	int i;
	for(i = 0; i < enemyCount; i ++){
		Enemy_Draw(enemies[i]);
	}
}

void EM_Exit(){
	int i;

	if(spawnpoints!=NULL){
		ListNode* node = spawnpoints->node->nextNode;
		while(node!=NULL){
			Vector2d* vector = node->conteudo;
			node = node->nextNode;
			if(vector!=NULL){
				List_Remove(spawnpoints,vector);
				free(vector);
			}
		}
		List_Destroy(spawnpoints);
	}

	for(i = 0; i < enemyCount; i ++){
		Enemy_Delete(enemies[i]);
	}
	free(enemies);
	enemies=NULL;
	enemyCount=0;
}
