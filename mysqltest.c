#include <mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>


int main(int argc, char **argv)
{
    MYSQL *con = mysql_init(NULL);
    if (mysql_real_connect(con, "logames.fr", "truc", "Test.123", "pokedex", 3306, NULL, 0)==NULL)
    {
        printf("Not connected");
    }
    else
    {
        printf("Connected");
    }
    int userIdConnected=1;
    int puzzle_score=1000;
    char request[]="SELECT * FROM Puzzle LEFT JOIN (SELECT * FROM Puzzle_done WHERE user_id=000) as Puzzle_done_change ON Puzzle.puzzle_id = Puzzle_done_change.puzzle_id WHERE (Puzzle_done_change.user_id is NULL) AND 0000>Puzzle.puzzle_score_min AND 0000<Puzzle.puzzle_score_max ORDER BY Puzzle.Puzzle_id LIMIT 1;";
    char* pointeurRequest=request;
    pointeurRequest[72]=48+(userIdConnected/100);
    pointeurRequest[73]=48+((userIdConnected/10)%10);
    pointeurRequest[74]=48+(userIdConnected%10);


    pointeurRequest[197]=48+(puzzle_score/1000);
    pointeurRequest[198]=48+((puzzle_score/100)%10);
    pointeurRequest[199]=48+((puzzle_score%100)/10);
    pointeurRequest[200]=48+(puzzle_score%10);
    
    pointeurRequest[230]=48+(puzzle_score/1000);
    pointeurRequest[231]=48+((puzzle_score/100)%10);
    pointeurRequest[232]=48+((puzzle_score%100)/10);
    pointeurRequest[233]=48+(puzzle_score%10);


    printf("%s\n",request);
    mysql_query(con, request);
    MYSQL_RES *result = mysql_store_result(con);
    if (result == NULL)
    {
        printf("Error result");
    }
    int num_fields = mysql_num_fields(result);
    MYSQL_ROW row;
    row = mysql_fetch_row(result);

    int actual_puzzle_id=atoi(row[0]);

    char actual_puzzle_fen[strlen(row[1])];
    char* pt_actual_puzzle_fen=actual_puzzle_fen;
    for (int i=0; i<strlen(row[1]); i++)
    {
        pt_actual_puzzle_fen[i]=row[1][i];
    }
    pt_actual_puzzle_fen[strlen(row[1])]=0;

    int average_puzzle_score= (atoi(row[2])+atoi(row[3]))/2;

    char actual_move_list[strlen(row[4])];
    char* pt_actual_move_list=actual_move_list;
    for (int i=0; i<strlen(row[4]); i++)
    {
        pt_actual_move_list[i]=row[4][i];
    }
    pt_actual_move_list[strlen(row[4])]=0;

    printf("len%d ->%s\n", strlen(row[4]), actual_move_list);

    mysql_free_result(result);
    mysql_close(con);
    exit(0);
}