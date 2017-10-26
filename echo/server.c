#include <enet/enet.h>
#include <stdio.h>

int main(int argc, char **argv)
{
  if (enet_initialize() != 0)
  {
    fprintf(stderr, "An error occurred while initializing ENet.\n");
    return EXIT_FAILURE;
  }
  atexit(enet_deinitialize);

  ENetAddress address;
  address.host = ENET_HOST_ANY;
  address.port = 5000;

  ENetHost *server = enet_host_create(&address /* the address to bind the server host to */,
                                      0xFFF /* allow up to 32 clients and/or outgoing connections */,
                                      2 /* allow up to 2 channels to be used, 0 and 1 */,
                                      0 /* assume any amount of incoming bandwidth */,
                                      0 /* assume any amount of outgoing bandwidth */);
  if (server == NULL)
  {
    fprintf(stderr, "An error occurred while trying to create an ENet server host.\n");
    exit(EXIT_FAILURE);
  }

  ENetEvent event;
  enet_uint8 cid = 0;
  while (1)
  {
    int ret = enet_host_service(server, &event, 0);
    if (ret > 0)
    {
      switch (event.type)
      {
      case ENET_EVENT_TYPE_CONNECT:
        printf("A new client connected from %x:%u.\n", event.peer->address.host, event.peer->address.port);
        event.peer->data = "Client information";
        break;

      case ENET_EVENT_TYPE_RECEIVE:
        printf("A packet of length %u containing %s was received from %s on channel %u.\n",
               event.packet->dataLength,
               event.packet->data,
               event.peer->data,
               event.channelID);

        ENetPacket *packet = enet_packet_create(event.packet->data, event.packet->dataLength, ENET_PACKET_FLAG_RELIABLE);
        enet_host_broadcast(server, cid, packet);
        cid ^= 1;

        enet_packet_destroy(event.packet);
        break;

      case ENET_EVENT_TYPE_DISCONNECT:
        printf("%s disconnected.\n", event.peer->data);
        event.peer->data = NULL;

      case ENET_EVENT_TYPE_NONE:
        printf("enet event none\n");
      }
    }
  }

  enet_host_destroy(server);
}