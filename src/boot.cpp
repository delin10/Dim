//
// Created by delinoz on 19-5-22.
//

#include "boot.h"
#include "DimClient.h"
#include "NumberUtils.h"
#include "Front.h"

#include <pthread.h>
#include <unistd.h>

/**
 * 启动receiver
 * @param client
 * @return
 */
void *startReceiver(void *client) {
    ((DimClient *) client)->start();
}

int main() {
    DimClient client;
    pthread_t th;
    int ret;
    int *thread_ret = NULL;
    //对‘pthread_create’未定义的引用
    ret = pthread_create(&th, NULL, startReceiver, &client);
    cout << "ret::" << ret << endl;
    long destHashId = NumberUtils::hexToLong("80fa5b3d6d7c", 12);
    DimTextMessage msg;
    char tt[] = "xxx";
    msg.destHashId = destHashId;
    msg.text = tt;
    msg.type = PACKET_TYPE_TEXT;
    int res = client.sendText(msg);
    cout << res << endl;
    CommandResolver resolver;
    resolver.listenCommand(&client);
}