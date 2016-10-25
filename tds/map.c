#include "map.h"
#include "bmp.h"
#include "vector.h"
#include "stdio.h"
#include "video_gr.h"
#include "math.h"
#include "list.h"
#include "texturemanager.h"
#include "networkmanager.h"

static Vector2 dimMap;
static Vector2 dimImg;
static Vector2 dimSubImg;

static char *mapData;
static char *mapImgBuffer;
static char *mapImg;
static Node *nodes;
static char maxHP;

Vector2 Map_getDimensions(){
	Vector2 res = dimMap;
	res.x *=dimSubImg.x;
	res.y *=dimSubImg.y;
	return res;
}

int Map_Init(char * fileName, int x, int y){
	BitmapInfoHeader info;
	//mapImg = LoadBitmapFile(fileName, &info);
	info.width = TM_GetTexture(SI_MAP)->dimensions.x;
	info.height = TM_GetTexture(SI_MAP)->dimensions.y;

	mapImg = TM_GetTexture(SI_MAP)->map;

	if(mapImg == NULL) return 1; //Error
	maxHP=8;
	dimSubImg.x=50;
	dimSubImg.y=50;

	dimMap.x = x;
	dimMap.y = y;

	dimImg.x = info.width;
	dimImg.y = info.height;

	//Inicializacao Dados Mapa
	mapData=(char*)malloc(x*y);

	//Inicializacao Nodes;
	nodes = (Node*)malloc(x*y*sizeof(Node));

	int x1, y1;

	for(x1=0; x1 < dimMap.x ; x1++)

		for(y1=0; y1 < dimMap.y ; y1++){
			int pos = x1+y1*dimMap.x;
			nodes[pos].x=x1;
			nodes[pos].y=y1;
			//printf("Alterado: %d , %d\n",nodes[pos].x,nodes[pos].y);

		}

	//Inicializacao do Buffer Mapa
	mapImgBuffer=(char*)malloc(x*y*dimSubImg.x*dimSubImg.y*2);

	Map_FillData();
	return 0;
}

void Map_Brush(int x, int y, char r, char dmg){
	Debug_LOG("MAP_Brush","Inicializado");
	int raio = r;
	int x1;
	int y1;
	Vector2d p1;
	p1.x = (double)x;
	p1.y = (double)y;

	for(x1 = -r; x1< r ; x1++){
		for(y1 = -r; y1 < r ; y1++){
			Vector2d p2;
			p2.x = (double)x1*50;
			p2.y = (double)y1*50;
			double magn = Vector2_Magnitude(p2);
			if(magn<= r*50){
				int damage = (int)(double)( dmg - (magn/((double)r*50.0))*dmg);
				Debug_LOGInt("Map_Brush Dmg",(char)damage);
				Map_Damage(x1*50 + x,y1*50 + y,(char)damage);
			}
		}
	}


}

int Map_FillData(){
	int x,y;
	for(x=0; x< dimMap.x ; x++){
		for(y=0; y < dimMap.y; y++){
			int pos = x + y*dimMap.x;

			//mapData[pos] = ((int)(x*y*cos(x*1)*cos(x*1)*sin(y*1)) % 4 == 0 )? maxHP : 0;
			if( (x>dimMap.x/2-2 && x< dimMap.x/2+2) || (y < dimMap.y/2+2 && y > dimMap.y/2 -2))
				mapData[pos]=0;
			else
				mapData[pos]=maxHP;

			nodes[pos].solid=(mapData[pos])? 1 : 0;
		}
	}

}

int Map_CheckCollision(int x, int y){
	if(x<0 || y<0)
		return 1;
	int nX = x/dimSubImg.x;
	int nY = y/dimSubImg.y;

	if(nX>=dimMap.x || nY>=dimMap.y)
		return 1;

	return mapData[nX + nY * dimMap.x];
}

inline void Map_Texturize(int x1, int y1 , int imgX, int imgY){

	int c = dimSubImg.x * 2;
	imgX*= dimSubImg.x;
	imgY*= dimSubImg.y;
	y1*=dimSubImg.y;
	int y;
	for(y=0 ; y < dimSubImg.y ; y++){

		unsigned int buffPos = x1*c + (y1+y)* dimMap.x*c;
		unsigned int imgPos = imgX*2 + (imgY+y) * dimImg.x * 2;
		memcpy(mapImgBuffer + buffPos, mapImg + imgPos , dimSubImg.x*2);

	}
}

List* Map_GetSpawnPoints(){
	Debug_LOG("Map_GetSpawnPoints","Inicializado");
	int x;
	int y;
	List* listTemp = List_Create();
	for(x = 0; x < dimMap.x; x ++){
		Debug_LOGInt("Map_GetSpawnPoints: x",x);
		if(mapData[x] == 0){
			Vector2d* temp = (Vector2d*)malloc(sizeof(Vector2d));
			temp->x=x*dimSubImg.x + dimSubImg.x/2;
			temp->y=0 + dimSubImg.y/2 ;
			List_Add(listTemp,temp);
		}
		if(mapData[x + (dimMap.y-1)*dimMap.x] == 0){
			Vector2d* temp = (Vector2d*)malloc(sizeof(Vector2d));
			temp->x=x*dimSubImg.x + dimSubImg.x/2;
			temp->y=(dimMap.y-1)*dimSubImg.y + dimSubImg.y/2;
			List_Add(listTemp,temp);
		}/**/
	}
	for(y = 1; y < dimMap.y-1; y ++){
		if(mapData[y*dimMap.x] == 0){
			Vector2d* temp = (Vector2d*)malloc(sizeof(Vector2d));
			temp->x=0 + dimSubImg.x/2;
			temp->y=y*dimSubImg.y + dimSubImg.y/2;
			List_Add(listTemp,temp);
		}
		if(mapData[dimMap.x-1 + y*dimMap.x] == 0){
			Vector2d* temp = (Vector2d*)malloc(sizeof(Vector2d));
			temp->x=(dimMap.x-1)*dimSubImg.x + dimSubImg.x/2;
			temp->y=y*dimSubImg.y + dimSubImg.y/2;
			List_Add(listTemp,temp);
		}
	}
	return listTemp;
}

void Map_ApplyDecal(int x,int y, int iX, int iY, int iW, int iH,Sprite* s){

	int x1 = x;
	int y1 = y;
	char* img = s->map;
	for(x=0; x < iW ; x++ ){
		for(y=0; y < iH; y++){
			if(Map_CheckCollision(x1+x,y1+y)) continue;

			unsigned int imgPos = (iX + x)*2 + (iY + y)*s->dimensions.x *2;

			if((unsigned char)img[imgPos] == 0xFF && (unsigned char)img[imgPos+1] == 0xFF) continue;

			unsigned int buffPos = (x1 + x)*2 + (y1 + y)* dimMap.x*dimSubImg.x * 2;

			memcpy(mapImgBuffer + buffPos , img + imgPos , 2);
		}
	}

}


inline void Map_TexturizeAlpha(int x1,int y1, int imgX, int imgY){

	int x,y;
	for(x=0; x < dimSubImg.x ; x++ ){
		for(y=0; y < dimSubImg.y; y++){
			unsigned int imgPos = (imgX*dimSubImg.x + x)*2 + (imgY*dimSubImg.y + y)*dimImg.x *2;

			if((unsigned char)mapImg[imgPos] == 0xFF && (unsigned char)mapImg[imgPos+1] == 0xFF) continue;

			unsigned int buffPos = (x1*dimSubImg.x + x)*2 + (y1*dimSubImg.y + y)* dimMap.x*dimSubImg.x * 2;

			memcpy(mapImgBuffer + buffPos , mapImg + imgPos , 2);
		}
	}

}

void Map_Draw(){

	vg_drawMap(mapImgBuffer, dimMap.x * dimSubImg.x , dimMap.y * dimSubImg.y , dimSubImg.x*2);

}

inline void Map_UpdateOverlay(int x, int y){
	if(x<0 || x>= dimMap.x || y < 0 || y>=dimMap.y) return;

	int ch =(mapData[x + y * dimMap.x])/DAMAGE_STATES;
	//Debug_LOGInt("D:" , mapData[x + y * dimMap.x]);
	//Debug_LOGInt("Valor:" , ch);
	switch(ch){
	case MAP_STATE_1 :
		Map_TexturizeAlpha(x,y,6,0);
		break;
	case MAP_STATE_2 :
		Map_TexturizeAlpha(x,y,6,1);
		break;
	case MAP_STATE_3 :
		Map_TexturizeAlpha(x,y,6,2);
		break;
	}
}

inline void Map_UpdateDmg(int x, int y){
	int i;

	if(mapData[x + y * dimMap.x]>0){
		Map_UpdateOverlay(x,y);
	}else{
		nodes[x + y * dimMap.x].solid=0;
		Map_Texturize(x,y,5,2);
		for(i =-1 ; i < 2; i++){
			int nX= x+i;
			int nY= y+i;
			if(i==0) continue;

			if(Map_GetData(nX,y))
				Map_AutoTexture(nX,y);
			if(Map_GetData(x,nY))
				Map_AutoTexture(x,nY);

		}
	}
}

void Map_ReadExplosion(char * msg){
	Debug_LOG("Map_ReadExplosion", "Inicializado");

	if(msg == NULL) return ;
	int i = 0;
	if(msg[i]!= 'E'){
		Debug_LOG("Map_ReadExplosion","Tag Errada!");
		return;
	}
	i+= sizeof(char);

	int x,y;
	char dmg;

	memcpy(&x , msg + i, sizeof(int));
	i += sizeof(int);

	memcpy(&y , msg + i ,sizeof(int));
	i += sizeof(int);

	dmg = msg[i];
	i += sizeof(char);

	if(msg[i] != 'F'){
		Debug_LOG("Map_ReadExplosion","Tag final Errada!");
		return;
	}

	Map_Brush(x,y,5,dmg);

}

char* Map_SendExplosion(int x, int y, char dmg){
	char* msg = (char*)malloc(sizeof(char)*4 + sizeof(int)*2);
	int i =0;

	msg[i] = 'E';
	i += sizeof(char);

	memcpy(msg + i , &x, sizeof(int));
	i += sizeof(int);

	memcpy(msg + i , &y, sizeof(int));
	i += sizeof(int);

	msg[i] = dmg;
	i += sizeof(char);

	msg[i] = 'F';
	i += sizeof(char);

	msg[i] = '!';

	return msg;
}

char Map_SetDamage(int x, int y, char dmg){
	Debug_LOG("Map_SetDamage", "Inicializado");
	Debug_LOGInt("Map_SetDamage: posX", x);
	Debug_LOGInt("Map_SetDamage: posY", y);
	Debug_LOGInt("Map_SetDamage: dmg", dmg);

	if(x<0 || y<0 || x>=dimMap.x || y >=dimMap.y)
		return 0;

	mapData[x + y*dimMap.x] = dmg;
	Map_UpdateDmg(x,y);

	return 1;

}
void Map_NetReadDamage(char * msg){
	Debug_LOG("Map_NetReadDamage","Inicializado");
	if(msg[0] != 'T'){
		Debug_LOG("Map_NetReadDamage", "ERRO! nao tem TAG!");
		return;
	}
	int x;
	int y;
	char dmg;

	int i =0;

	i += sizeof(char);

	memcpy(&x, msg + i, sizeof(int));
	i += sizeof(int);

	memcpy(&y, msg + i, sizeof(int));
	i += sizeof(int);

	memcpy(&dmg, msg + i, sizeof(char));

	Map_SetDamage(x,y,dmg);
}

char Map_NetSendDamage(int x, int y, char dmg){
	unsigned int size = sizeof(char)*3 + sizeof(int)*2;
	Debug_LOGInt("Map_NetSendDamage",size);
	Debug_LOGInt("Map_NetSendDamage: PosX:", x);
	Debug_LOGInt("Map_NetSendDamage: PosY:", y);

	char* msg = (char*)malloc( size );

	if(msg == NULL)
		return 0;

	int i = 0;
	msg[i] = 'T';
	i += (int)sizeof(char);

	memcpy(msg + i,&x, sizeof(int));
	i += sizeof(int);

	memcpy(msg + i,&y, sizeof(int));
	i +=sizeof(int);

	memcpy(msg + i,&dmg, sizeof(char));
	i += sizeof(char);
	msg[i] = '!';

	NM_SendMessage(msg);
	free(msg);
	Debug_LOG("Map_NetSendDamage","Terminado com sucesso!");
	return 0;
}

char Map_Damage(int x, int y, char dmg){

	if(x<0 || y<0)
		return 0;
	int nX = x/dimSubImg.x;
	int nY = y/dimSubImg.y;


	if(nX>=dimMap.x || nY>=dimMap.y)
		return 0;
	if(mapData[nX+ nY *dimMap.x]==0) return 0;

	if(!NM_IsServer() && !NM_IsSinglePlayer()){
		return;
	}


	char update=0;

	char oldState = mapData[nX+ nY *dimMap.x]/DAMAGE_STATES;
	mapData[nX+ nY *dimMap.x] -= dmg;

	if(mapData[nX+ nY *dimMap.x]<=1){
		update=1;
		mapData[nX+ nY *dimMap.x]=0;
	}



	if(mapData[nX+ nY *dimMap.x]/DAMAGE_STATES != oldState)
		update=1;

	if(update){
		if(NM_IsServer() && !NM_IsSinglePlayer()){
			Map_NetSendDamage(nX,nY,mapData[nX+ nY *dimMap.x]);
		}
		Map_UpdateDmg(nX,nY);
		return 1;
	}
	return 1;
}

inline void Map_AutoTexture(int x, int y){
	if(x<0 || x>= dimMap.x || y < 0 || y>=dimMap.y) return;

	int pos = x + y*dimMap.x;
	if(mapData[pos] == 0){
		Map_Texturize(x,y,5,1);
		return;
	}

	unsigned char neigh = Map_GetNeighbours(x,y);

	switch(neigh){
	case 1: //UP
		Map_Texturize(x,y,3,2);
		break;
	case 2: //DOWN
		Map_Texturize(x,y,3,0);
		break;
	case 3:	//Connect Up n Down
		Map_Texturize(x,y,3,1);
		break;
	case 4: //LEFT
		Map_Texturize(x,y,4,0);
		break;
	case 8: //RIGHT
		Map_Texturize(x,y,4,2);
		break;
	case 12: //Connect Left n Right
		Map_Texturize(x,y,4,1);
		break;
	case 15://All Sides
		Map_Texturize(x,y,1,1);
		break;
	case 10: //Top LEft Corner
		Map_Texturize(x,y,0,0);
		break;
	case 6: //Top Right Corner
		Map_Texturize(x,y,2,0);
		break;
	case 9: // Down Left Corner
		Map_Texturize(x,y,0,2);
		break;
	case 5: // Down Right Corner
		Map_Texturize(x,y,2,2);
		break;
	case 11://Left n Sides
		Map_Texturize(x,y,0,1);
		break;
	case 14: //Top n Sides
		Map_Texturize(x,y,1,0);
		break;
	case 7: //Right n Sides
		Map_Texturize(x,y,2,1);
		break;
	case 13://DOWN n Sides
		Map_Texturize(x,y,1,2);
		break;
	case 0: //None
		Map_Texturize(x,y,5,0);
		break;
	}
	Map_UpdateOverlay(x,y);
}

void Map_FillBuffer(){

	int x;
	int y;

	for(x=0; x< dimMap.x ; x++){
		for(y=0; y < dimMap.y; y++){

			Map_AutoTexture(x,y);

		}
	}
}

inline unsigned char Map_GetNeighbours(int x, int y){
	unsigned char up = Map_GetData(x,y-1);
	unsigned char down = Map_GetData(x,y+1);
	unsigned char left = Map_GetData(x-1,y);
	unsigned char right = Map_GetData(x+1,y);

	unsigned char result;
	if(up > 0)
		result+=1;
	if(down>0)
		result+=2;
	if(left>0)
		result+=4;
	if(right>0)
		result+=8;

	return result;
}

Node* Map_GetNode(int x, int y){
	if( x < 0 || y < 0)
		return NULL;

	int nX = x/dimSubImg.x;
	int nY = y/dimSubImg.y;

	if(nX>=dimMap.x || nY>=dimMap.y)
		return NULL;


	nodes[nX + nY*dimMap.x].parent=NULL;
	nodes[nX + nY*dimMap.x].iCost=0;
	if(nodes[nX + nY*dimMap.x].solid) return NULL;
	//printf("Vou retornar: %d , %d \n",nodes[nX + nY*dimMap.x].x , nodes[nX + nY*dimMap.x].y);
	return &nodes[nX + nY*dimMap.x];
}

inline Node* GetLowCost(List* l1){
	ListNode* node = l1->node->nextNode;
	Node* curr;
	while(node !=NULL){
		Node* nodeTemp = (Node*)node->conteudo;

		if(curr ==NULL){
			curr=nodeTemp;
			continue;
		}
		if(nodeTemp->tCost < curr->tCost)
			curr = nodeTemp;

		node = node->nextNode;
	}
	return curr;
}

List* Map_NodeNeigh(int x, int y){
	List* list = List_Create();
	int i;
	for(i = -1; i < 2; i++){
		int nX = x+i;
		int nY = y+i;

		if( i==0) continue;

		if(nX>=0 && nX<dimMap.x)
			if(!nodes[nX + y*dimMap.x].solid)
				List_Add(list,&nodes[nX + y*dimMap.x]);

		if(nY>=0 && nY<dimMap.y)
			if(!nodes[x + nY*dimMap.x].solid)
				List_Add(list,&nodes[x + nY*dimMap.x]);
	}

	return list;

}

int Map_DistBetweenNodes(Node* n1, Node* n2){
	int x = abs(n2->x - n1->x);
	int y = abs(n2->y - n1->y);

	return x+y;
}

List* Map_GetPath(int xi , int yi, int xf, int yf){

	Node* nI = Map_GetNode(xi,yi);
	Node* nF = Map_GetNode(xf,yf);

	if(nI==NULL || nF ==NULL)
		return NULL;

	List* open = List_Create();
	List* closed = List_Create();

	List_Add(open, nI);

	while(List_Count(open)>0){

		Node* current = GetLowCost(open);

		List_Remove(open,current);

		List_Add(closed,current);
		if(current == nF){
			List_Destroy(open);
			List_Destroy(closed);
			List* res = List_Create();
			while(current!=NULL){
				Vector2 * nPos = (Vector2*)malloc(sizeof(Vector2));
				nPos->x = current->x;
				nPos->y = current->y;
				List_AddFront(res, nPos);
				current = current->parent;
			}

			return res;

		}

		List* neigh =Map_NodeNeigh(current->x, current->y);

		ListNode* neighNodes = neigh->node->nextNode;

		while(neighNodes != NULL){
			Node* nodeTemp = neighNodes->conteudo;
			neighNodes=neighNodes->nextNode;
			if(List_Contain(closed, nodeTemp)) continue;

			int dist = current->iCost+1;

			int openContain = List_Contain(open,nodeTemp);

			if( !openContain || dist < nodeTemp->iCost){
				int distF = Map_DistBetweenNodes(nodeTemp,nF);
				nodeTemp->iCost = dist;
				nodeTemp->fCost = distF;
				nodeTemp->tCost = dist+distF;
				nodeTemp->parent = current;
				if(!openContain){
					List_Add(open,nodeTemp);
				}

			}

		}
		List_Destroy(neigh);
	}
	List_Destroy(open);
	List_Destroy(closed);

}

inline unsigned char Map_GetData(int x, int y){
	if(x<0 || x>= dimMap.x || y<0 || y>= dimMap.y)
		return 0;

	return mapData[x + y * dimMap.x];

}

void Map_Exit(){
	if(mapData != NULL)
		free(mapData);
	if(mapImgBuffer!=NULL)
		free(mapImgBuffer);
	if(nodes != NULL)
		free(nodes);

	mapData = NULL;
	mapImgBuffer = NULL;
	mapImg = NULL;
	nodes = NULL;
}



