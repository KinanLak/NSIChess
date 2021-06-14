#include <mysql.h>
#include <stdio.h>
#include <stdlib.h>

void finish_with_error(MYSQL *con)
{
    fprintf(stderr, "%s\n", mysql_error(con));
    mysql_close(con);
    exit(1);
}

int main(int argc, char **argv)
{
    MYSQL *con = mysql_init(NULL);

    if (con == NULL)
    {
        fprintf(stderr, "mysql_init() failed\n");
        exit(1);
    }

    if (mysql_real_connect(con, "35.181.56.11", "truc", "Test.123",
                           "pokedex", 3306, NULL, 0) == NULL)
    {
        printf("Non connecte\n");
        finish_with_error(con);
    }
    printf("Connecte\n");
    int caca = mysql_query(con, "SELECT COUNT(*) FROM Puzzle");
    int test = caca;
    if (test==1261833)
    {
        printf("caca -> %d",test);
    }
    
    MYSQL_RES *result = mysql_store_result(con);

    int num_fields = mysql_num_fields(result);

    MYSQL_ROW row;

    while ((row = mysql_fetch_row(result)))
    {
        for (int i = 0; i < num_fields; i++)
        {
            printf("%s ", row[i] ? row[i] : "NULL");
        }

        printf("\n");
    }

    mysql_free_result(result);
    mysql_close(con);

    exit(0);
}