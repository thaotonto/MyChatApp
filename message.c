#include "message.h"
#include "constant.h"

/**
 * @return
 * 0: success
 * 1: connection fail
 * 2: wrong agrument
 */
int store_message   (int s_id, int r_id, char* content,
                                int timestamp, int state) {

    MYSQL *conn = mysql_init(NULL);
    char query[1000];

    sprintf(query,
        "INSERT INTO messages (send_id, receive_id, content, time, state) VALUES (%d, %d, '%s', %d, %d)",
                            s_id, r_id, content, timestamp, state);

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
        "SELECT * FROM chat.messages WHERE (send_name = '%s' AND receive_name = '%s')  OR (send_name = '%s' AND receive_name = '%s') ORDER BY timestamp DESC LIMIT 10 OFFSET %d",
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
        messages[count].s_id = atoi(row[0]);
        messages[count].r_id = atoi(row[1]);
        strcpy(messages[count].content, row[2]);
        messages[count].timestamp = atoi(row[3]);
        messages[count].state = atoi(row[4]);
        count++;
    }

    memcpy(&arr.messages, &messages, arr.count*sizeof(Message));

    mysql_free_result(result);

    mysql_close(conn);
    return arr;
}

message_array get_offline_messages() {
    MYSQL *conn = mysql_init(NULL);
    MYSQL_ROW row;
    MYSQL_FIELD *field;
    MYSQL_RES *result;
    message_array arr;
    Message messages[1000];
    char query[1000];
    int i, count = 0;
    sprintf(query, "Select * FROM messages WHERE state = 0");
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
        messages[count].s_id = atoi(row[0]);
        messages[count].r_id = atoi(row[1]);
        strcpy(messages[count].content, row[2]);
        messages[count].timestamp = atoi(row[3]);
        messages[count].state = atoi(row[4]);
        count++;
    }

    memcpy(&arr.messages, &messages, arr.count*sizeof(Message));

    mysql_free_result(result);

    mysql_close(conn);
    return arr;
}


int change_message_state(int s_id, int r_id, int timestamp) {
    MYSQL *conn = mysql_init(NULL);
    char query[1000];

    sprintf(query,
        "UPDATE messages SET state = 1 WHERE send_id = %d AND receive_id = %d AND timestamp = %d", s_id, r_id, timestamp);

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
