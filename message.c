#include "message.h"
#include "constant.h"

/**
 * @return
 * 0: success
 * 1: connection fail
 * 2: wrong agrument
 */
int store_message   (char *send_name, char *receive_name, char* content,
                                char* sent_time, int state) {

    MYSQL *conn = mysql_init(NULL);
    char query[1000];

    sprintf(query,
        "INSERT INTO messages (send_name, receive_name, content, time, state) VALUES ('%s', '%s', '%s', '%s', %d)",
                            send_name, receive_name, content, sent_time, state);

    if (conn == NULL) {
       return 1;
     }

    if (mysql_real_connect(conn, host, db_user, password, db_name,
                                        port, unix_socket, flag) == NULL) {
        mysql_close(conn);
        return 1;
    }

    if (mysql_query(conn,query)) {
        mysql_close(conn);
        return 2;
    }

    return 0;
}

message_array get_history(char *name1, char *name2, int page) {
    MYSQL *conn = mysql_init(NULL);
    MYSQL_ROW row;
    MYSQL_FIELD *field;
    MYSQL_RES *result;
    message_array arr;
    Message messages[1000];
    char query[1000];
    int i, count = 0;
    sprintf(query,
        "SELECT * FROM chat.messages WHERE (send_name = '%s' AND receive_name = '%s')  OR (send_name = '%s' AND receive_name = '%s') ORDER BY id DESC LIMIT 10 OFFSET %d",
        name1, name2, name2, name1, page * 10);
    arr.count = 0;
    arr.state = 0;

    if (conn == NULL) {
        arr.state = 1;
        return arr;
    }

    if (mysql_real_connect(conn, host, db_user, password, db_name,
                                        port, unix_socket, flag) == NULL) {
        mysql_close(conn);
        arr.state = 1;
        return arr;
    }

    if (mysql_query(conn, query)) {
        mysql_close(conn);
        arr.state = 1;
        return arr;
    }

    result = mysql_store_result(conn);

    if(result == NULL) {
        mysql_close(conn);
        arr.state = 2;
        return arr;
    }

    arr.count = mysql_num_rows(result);

    while ((row = mysql_fetch_row(result))) {
        messages[count].id = atoi(row[0]);
        strcpy(messages[count].send_name, row[1]);
        strcpy(messages[count].receive_name, row[2]);
        strcpy(messages[count].content, row[3]);
        strcpy(messages[count].sent_time, row[4]);
        messages[count].state = atoi(row[5]);
        count++;
    }

    memcpy(&arr.messages, &messages, arr.count*sizeof(Message));

    mysql_free_result(result);

    mysql_close(conn);
    return arr;
}

int get_offline_messages(char *sendName, char *receiveName) {
    MYSQL *conn = mysql_init(NULL);
    MYSQL_ROW row;
    MYSQL_FIELD *field;
    MYSQL_RES *result;
    char query[1000];
    int count = 0;
    sprintf(query, "Select COUNT(*) FROM chat.messages WHERE (send_name = '%s' AND receive_name = '%s') AND state = 0", sendName, receiveName);

    if (conn == NULL) {
       return -1;
    }

    if (mysql_real_connect(conn, host, db_user, password, db_name,
                                        port, unix_socket, flag) == NULL) {
        mysql_close(conn);
        return -1;
    }

    if (mysql_query(conn, query)) {
        mysql_close(conn);
        return -1;
    }

    result = mysql_store_result(conn);

    if(result == NULL) {
        mysql_close(conn);
        return -2;
    }

    row = mysql_fetch_row(result);
    count = atoi(row[0]);

    mysql_free_result(result);

    mysql_close(conn);
    return count;
}


int change_message_state(char *sendName, char *receiveName) {
    MYSQL *conn = mysql_init(NULL);
    char query[1000];

    sprintf(query,
        "UPDATE chat.messages SET state = 1 WHERE (send_name = '%s' AND receive_name = '%s') AND state = 0", sendName, receiveName);

    if (conn == NULL) {
       return 1;
    }

    if (mysql_real_connect(conn, host, db_user, password, db_name,
                                        port, unix_socket, flag) == NULL) {
        mysql_close(conn);
        return 1;
    }

    if (mysql_query(conn,query)) {
        mysql_close(conn);
        return 2;
    }

    return 0;
}

int change_message_state_on_sent(char *sendName, char *receiveName, char* sent_time) {
    MYSQL *conn = mysql_init(NULL);
    char query[1000];

    sprintf(query,
        "UPDATE chat.messages SET state = 1 WHERE send_name = '%s' AND receive_name = '%s' AND state = 0 and time = '%s'", sendName, receiveName, sent_time);

    if (conn == NULL) {
       return 1;
    }

    if (mysql_real_connect(conn, host, db_user, password, db_name,
                                        port, unix_socket, flag) == NULL) {
        mysql_close(conn);
        return 1;
    }

    if (mysql_query(conn,query)) {
        mysql_close(conn);
        return 2;
    }

    return 0;
}
