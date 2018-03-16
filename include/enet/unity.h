#define ENET_BUILDING_LIB 1
#include "enet/enet.h"
#include "enet/utility.h"

typedef struct
{
  enet_uint32 type;
  enet_uint32 channelID;
  enet_uint32 flags;
  enet_uint32 dataLength;
  enet_uint8 data[65536];
} ENetUnityEvent;

enet_int32 ENetInit(enet_int32 channel);
enet_int32 ENetConnect(const char *ip, enet_uint16 port);
void ENetService(ENetUnityEvent *ue, enet_int32 ms);
void ENetSend(enet_uint8 *data, enet_int32 len, enet_int32 channelID, enet_int32 flags);
void ENetDestroy();