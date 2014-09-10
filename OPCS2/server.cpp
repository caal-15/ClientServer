#include <iostream>
#include <set>
#include <string>
#include <czmq.h>
#include <string.h>

using namespace std;

int main(void) {
  zctx_t* context = zctx_new();
  void* server = zsocket_new(context,ZMQ_ROUTER);
  zsocket_bind(server, "tcp://*:5555");

  int i = 0;
  while(true) {
    zmsg_t *msg = zmsg_recv(server);
    // Print the massage on the server's console
    zmsg_print(msg);
	zmsg_t *ans = zmsg_new();
    zframe_t *identity = zmsg_pop(msg);
    zframe_t *content = zmsg_pop(msg);
    assert(content);
    zmsg_destroy(&msg);

    zclock_sleep(2000);

    if (i % 5 == 0) {
      cout << "I'll answer this one!\n";
      zmsg_append(ans, &identity);
      zmsg_append(ans, &content);
      zmsg_addstr(ans, "jajajajaja");
      zmsg_send(&ans, server);
    } else {
      cout << "Skipping request\n";
    }
    i++;

    zframe_destroy(&identity);
    zframe_destroy(&content);
    zmsg_destroy(&ans);
    ans = zmsg_new();
  }
  zctx_destroy(&context);
  return 0;
}
