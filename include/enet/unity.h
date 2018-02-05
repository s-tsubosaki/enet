#include <enet/enet.h>

typedef struct
{
  enet_uint32 type;
  enet_uint32 channelID;
  enet_uint32 flags;
  enet_uint32 dataLength;
  enet_uint8 data[65536];
} ENetUnityEvent;

int32_t ENetInit(int32_t channel);
int32_t ENetConnect(const char *ip, enet_uint16 port);
void ENetService(ENetUnityEvent *ue, int32_t ms);
void ENetSend(enet_uint8 *data, int32_t len, int32_t channelID, int32_t flags);
void ENetDestroy();