#include <bluetooth/sdp.h>
#include <bluetooth/sdp_lib.h>

sdp_session_t *register_service(uint8_t rfcomm_channel);
int init_server();
int read_server(int client,char *input);
void write_server(int client, char *message);
