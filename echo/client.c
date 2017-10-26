#include <enet/enet.h>
#include <stdio.h>
#include <stdbool.h>

int canReadInput()
{
  fd_set fdset;
  struct timeval timeout;
  FD_ZERO(&fdset);
  FD_SET(0, &fdset);
  timeout.tv_sec = 0;
  timeout.tv_usec = 0;
  return select(1, &fdset, NULL, NULL, &timeout);
}

int main(int argc, char **argv)
{
  if (enet_initialize() != 0)
  {
    fprintf(stderr, "An error occurred while initializing ENet.\n");
    return EXIT_FAILURE;
  }
  atexit(enet_deinitialize);

  // client作成
  ENetHost *client;
  client = enet_host_create(NULL /* create a client host */,
                            1 /* only allow 1 outgoing connection */,
                            2 /* allow up 2 channels to be used, 0 and 1 */,
                            1024 * 1024 * 16 / 8 /* downstream bandwitch kbps */,
                            1024 * 1024 * 16 / 8 /* upstream bandwitch kbps */);
  if (client == NULL)
  {
    fprintf(stderr, "An error occurred while trying to create an ENet client host.\n");
    exit(EXIT_FAILURE);
  }

  // 接続
  const char *ip = "127.0.0.1";
  enet_uint16 port = 5000;

  ENetAddress address;
  enet_address_set_host(&address, ip);
  address.port = port;

  ENetEvent event;
  ENetPeer *peer;
  peer = enet_host_connect(client, &address, 2, 0);
  if (peer == NULL)
  {
    fprintf(stderr, "No available peers for initiating an ENet connection.\n");
    exit(EXIT_FAILURE);
  }
  if (enet_host_service(client, &event, 5000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT)
  {
    printf("Connection to %s:%d succeeded.\n", ip, port);
  }
  else
  {
    enet_peer_reset(peer);
    printf("Connection to %s:%d failed.\n", ip, port);
  }
  // 接続ここまで

  bool isconnected = true;
  while (isconnected)
  {
    int ret = enet_host_service(client, &event, 0);
    if (ret > 0)
    {
      switch (event.type)
      {
      case ENET_EVENT_TYPE_RECEIVE:
        printf("%s was received from server on channel %u.\n", event.packet->data, event.channelID);
        enet_packet_destroy(event.packet);
        break;

      case ENET_EVENT_TYPE_DISCONNECT:
        printf("Disconnect success.\n");
        isconnected = false;
        break;
      case ENET_EVENT_TYPE_NONE:
        printf("enet event none\n");
      }
    }
    else if (canReadInput())
    {
      char buf[1024] = {'\0'};
      read(0, buf, BUFSIZ);
      if (buf[strlen(buf) - 1] == '\n')
        buf[strlen(buf) - 1] = '\0';

      if (strlen(buf) < 1)
        continue;

      if (strcmp("quit", buf))
      {
        ENetPacket *packet = enet_packet_create(buf, strlen(buf) + 1, ENET_PACKET_FLAG_RELIABLE);
        enet_peer_send(peer, 0, packet);
      }
      else
      {
        enet_peer_disconnect(peer, 0);
      }
    }
  }

  enet_host_destroy(client);
}