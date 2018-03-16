#include <enet/unity.h>
#include <string.h>
#include <stdio.h>

ENetHost *host;
ENetPeer *peer;

enet_int32 ENetInit(enet_int32 channel)
{
  if (enet_initialize() != 0)
  {
    printf("enet_initialize failed\n");
    return 1;
  }

  host = enet_host_create(NULL, 1, channel, 0, 0);
  if (host == NULL)
  {
    printf("enet_host_create failed\n");
    return 1;
  }

  return 0;
}

enet_int32 ENetConnect(const char *ip, enet_uint16 port)
{
  ENetAddress address;
  enet_address_set_host(&address, ip);
  address.port = port;

  ENetEvent event;
  peer = enet_host_connect(host, &address, host->channelLimit, 0);
  if (enet_host_service(host, &event, 5000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT)
  {
    return 0;
  }
  else
  {
    printf("enet_host_connect failed\n");
    enet_peer_reset(peer);
    return 1;
  }
}

void ENetService(ENetUnityEvent *ue, enet_int32 ms)
{
  if (host == NULL || peer == NULL)
  {
    printf("ENetService host: %p peer: %p\n", host, peer);
    return;
  }

  ue->type = ENET_EVENT_TYPE_NONE;

  ENetEvent event;
  int ret = enet_host_service(host, &event, ms);
  if (ret > 0)
  {
    ue->type = event.type;
    switch (event.type)
    {
    case ENET_EVENT_TYPE_RECEIVE:
      ue->channelID = event.channelID;
      ue->flags = event.packet->flags;
      memcpy(ue->data, event.packet->data, event.packet->dataLength);
      enet_packet_destroy(event.packet);
      break;
    default:
      break;
    }
  }
}

void ENetSend(enet_uint8 *buf, enet_int32 len, enet_int32 channelID, enet_int32 flags)
{
  if (host == NULL || peer == NULL)
  {
    printf("ENetSend host: %p peer: %p\n", host, peer);
    return;
  }
  ENetPacket *packet = enet_packet_create(buf, len, flags);
  enet_peer_send(peer, 0, packet);
}

void ENetDestroy()
{
  enet_host_destroy(host);
  host = NULL;
  peer = NULL;
}