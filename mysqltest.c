#include <mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>


int main(int argc, char **argv)
{
    MYSQL *con = mysql_init(NULL);
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    char date[9];
    int year=tm.tm_year+1900;
    int month= tm.tm_mon + 1;
    int day = tm.tm_mday;
    date[0] = 48+(year/1000);
    date[1] = 48+(year/100)%10;
    date[2] = 48+(year/10)%10;
    date[3] = 48+(year%10);
    date[4] = 47;
    date[5] = 48 + (month/10);
    date[6] = 48 + (month%10);
    date[7] = 47;
    date[8] = 48 + (day/10);
    date[9] = 48 + (day%10);

    printf("now: %d-%d-%d\n", year, month, day);
    if (con == NULL)
    {
        fprintf(stderr, "mysql_init() failed\n");
        exit(1);
    }

    if (mysql_real_connect(con, "35.181.56.11", "truc", "Test.123",
                           "pokedex", 3306, NULL, 0) == NULL)
    {
        printf("Non connecte\n");
    }

    char prenom[]= "Julien      ";
    char* pointeurPrenom = prenom;
    int cptPrenom=6;

    char nom[]= "Chemillier      ";
    char* pointeurNom = nom;
    int cptNom=10;
    
    char password[]= "Password                              ";
    char* pointeurPassword = password;
    int cptPassword=8;
    
    char email[]= "Email                              ";
    char* pointeurEmail = email;
    int cptEmail=5;

    char request[] = "INSERT INTO User Values (";
    char endRequest[] = "', 0);";
    char midRequest[] = "', 1000, 1000, 'francais', '";
    int cptRequest=39;



    //char test[] = "SELECT Count(*) FROM User Where email='chemillier.julien@gmail.com' AND password=12345;                                   ";

    mysql_query(con, "SELECT MAX(user_id) FROM User;");
    MYSQL_RES *result = mysql_store_result(con);
    int num_fields = mysql_num_fields(result);
    MYSQL_ROW row = mysql_fetch_row(result);
    int user_id = atoi(row[0])+1;
    int len = log10(user_id)+1;
    char userIdChar[len];
    itoa(user_id, userIdChar, 10);
    printf("resultat %c", result);
    
    mysql_free_result(result);
    mysql_close(con);





    strcat(userIdChar, ",'");
    strcat(request, userIdChar);

    char newPrenom[cptPrenom];
    for (int i=0; i<cptPrenom; i++)
    {
        newPrenom[i] = pointeurPrenom[i];
    }
    newPrenom[cptPrenom]=0;

    char newNom[cptNom];
    for (int i=0; i<cptNom; i++)
    {
        newNom[i] = pointeurNom[i];
    }
    newNom[cptNom]=0;


    char newPassword[8];
    for (int i=0; i<cptPassword; i++)
    {
        newPassword[i] = pointeurPassword[i];
    }
    newPassword[cptPassword]=0;
    
    char newEmail[cptEmail];
    for (int i=0; i<cptEmail; i++)
    {
        newEmail[i] = pointeurEmail[i];
    }
    newEmail[cptEmail]=0;


    strcat(newPrenom, "','");
    strcat(newPrenom, newNom);
    strcat(newPrenom, "','");
    strcat(newPrenom, newPassword);
    strcat(newPrenom, "','");
    strcat(newPrenom, date);
    strcat(newPrenom, "', 1000, 1000, 'francais', '");
    
    strcat(newEmail, "', 0);");
    
    strcat(newPrenom, newEmail);


    strcat(request, newPrenom);


    printf("request: %s\n", request);


    MYSQL *con2 = mysql_init(NULL);

    char test[] = "INSERT INTO User Values (2,'Julien','Chemillier', 192837465,'2021/06/15', 1000, 1000, 'francais', 'Email', 0);";
    if (mysql_real_connect(con2, "35.181.56.11", "truc", "Test.123","pokedex", 3306, NULL, 0) == NULL)
    {
        printf("Non connecte\n");
    }
    if (mysql_query(con2, request))
    {
        fprintf(stderr, "%s\n", mysql_error(con2));
        mysql_close(con2);
        exit(1);
    }


    printf("finis sans erreurs");
    exit(0);
}