#include "user.h"
#include "constant.h"

/**
 * [create_new_user description]
 * @return
 * 0: create success
 * 1: connect db fail
 * 2: wrong query's params
 */
int create_new_user(char *name, char *pass) {

    MYSQL *conn = mysql_init(NULL);
    char query[1000];

    sprintf(query, "INSERT INTO users (name, password) VALUES ('%s', '%s')",
                                                    name, pass);

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

/**
 * @return
 * 0: ok to login
 * 1: connection fail
 * 2: login fail
 * 3: already login
 */

int check_user(char *name, char *pass) {

    MYSQL *conn = mysql_init(NULL);
    MYSQL_RES *result;
    MYSQL_ROW row;
    char query[1000];

    sprintf(query,
        "Select * FROM users WHERE name = '%s' AND password = '%s'", name,
                                                                        pass);

    if (conn == NULL) {
       return 1;
    }

    if (mysql_real_connect(conn, host, db_user, password, db_name,
                                        port, unix_socket, flag) == NULL) {
        mysql_close(conn);
        return 1;
    }

    if (mysql_query(conn, query)) {
        mysql_close(conn);
        return 1;
    }

    result = mysql_store_result(conn);

    if(result == NULL) {
        mysql_close(conn);
        return 2;
    }

    if(mysql_num_rows(result) == 0){
        return 2;
    }

    row = mysql_fetch_row(result);
    if(atoi(row[3]) == 1){
        return 3;
    }

    mysql_free_result(result);

    mysql_close(conn);
    return 0;
}

user_array get_users_list() {
    MYSQL *conn = mysql_init(NULL);
    MYSQL_ROW row;
    MYSQL_FIELD *field;
    MYSQL_RES *result;
    user_array arr;
    User users[1000];
    char query[1000];
    int i, count = 0;
    sprintf(query, "Select * FROM users");
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
        users[count].id = atoi(row[0]);
        strcpy(users[count].name, row[1]);
        strcpy(users[count].password, row[2]);
        users[count].state = atoi(row[3]);
        count++;
    }

    memcpy(&arr.users, &users, arr.count*sizeof(User));

    mysql_free_result(result);

    mysql_close(conn);
    return arr;
}

User *get_user_by_id( int id ){
    MYSQL *conn = mysql_init(NULL);
    MYSQL_ROW row;
    MYSQL_FIELD *field;
    MYSQL_RES *result;
    User *r;
    r = (User*)malloc(sizeof(User));
    char query[1000];
    int i;
    sprintf(query, "Select * FROM users WHERE id = %d", id);

    if (conn == NULL) {
       return NULL;
    }

    if (mysql_real_connect(conn, host, db_user, password, db_name,
                                        port, unix_socket, flag) == NULL) {
        mysql_close(conn);
        return NULL;
    }

    if (mysql_query(conn, query)) {
        mysql_close(conn);
        return NULL;
    }

    result = mysql_store_result(conn);

    if(result == NULL) {
        mysql_close(conn);
        return NULL;
    }

    while ((row = mysql_fetch_row(result))) {
        r->id = atoi(row[0]);
        strcpy(r->name, row[1]);
        strcpy(r->password, row[2]);
        r->state = atoi(row[3]);
    }

    mysql_free_result(result);

    mysql_close(conn);
    return r;
}


/**
 * @return
 * 0: success
 * 1: connection fail
 * 2: wrong agrument
 */
int change_user_state(char *name, int cur_state){
    MYSQL *conn = mysql_init(NULL);
    char query[1000];
    if(cur_state == 0){
        sprintf(query, "UPDATE users SET state = 1 WHERE name = '%s'", name);
    } else if(cur_state == 1 ){
        sprintf(query, "UPDATE users SET state = 0 WHERE name = '%s'", name);
    }


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
