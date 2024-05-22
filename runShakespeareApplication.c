/**
 * runShakespeareApplication skeleton, to be modified by students
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "libpq-fe.h"

/* These constants would normally be in a header file */
/* Maximum length of string used to submit a connection */
#define MAXCONNECTIONSTRINGSIZE 501
/* Maximum length of string used to submit a SQL statement */
#define MAXSQLSTATEMENTSTRINGSIZE 2001
/* Maximum length of string version of integer; you don't have to use a value this big */
#define  MAXNUMBERSTRINGSIZE        20


/* Exit with success after closing connection to the server
 *  and freeing memory that was used by the PGconn object.
 */
static void good_exit(PGconn *conn)
{
    PQfinish(conn);
    exit(EXIT_SUCCESS);
}

/* Exit with failure after closing connection to the server
 *  and freeing memory that was used by the PGconn object.
 */
static void bad_exit(PGconn *conn)
{
    PQfinish(conn);
    exit(EXIT_FAILURE);
}

/* The three C functions that for Lab4 should appear below.
 * Write those functions, as described in Lab4 Section 4 (and Section 5,
 * which describes the Stored Function used by the third C function).
 *
 * Write the tests of those function in main, as described in Section 6
 * of Lab4.
 *
 * You may use "helper" functions to avoid having to duplicate calls and
 * printing, if you'd like, but if Lab4 says do things in a function, do them
 * in that function, and if Lab4 says do things in main, do them in main,
 * possibly using a helper function, if you'd like.
 */

/* Function: countDifferentPlayCharacters:
 * ---------------------------------------
 * Parameters:  connection, and theActorID, which should be the ID of an actor.
 * Returns the number of different play characters which that actor has played,
 * if there is an actor corresponding to theActorID.
 * Returns -1 if no such actor.
 * bad_exit if SQL statement execution fails.
 */


int countDifferentPlayCharacters(PGconn *conn, int theActorID)
{
    PGresult *res = PQexec(conn, "BEGIN TRANSACTION;"); // Begin the transaction
    if (PQresultStatus(res) != PGRES_COMMAND_OK) { // If transaction started successfully
        PQclear(res);
        bad_exit(conn);
    }
    char sql[MAXSQLSTATEMENTSTRINGSIZE]; // Create a string to store the SQL statement
    sprintf(sql, "SELECT * FROM Actors WHERE actorID = %d;", theActorID); // Find the actor with the actorID
    res = PQexec(conn, sql); // Execute the SQL statement
    if (PQresultStatus(res) != PGRES_TUPLES_OK) { // If the SQL statement execution was not successful
        printf("No data retrieved\n");
        PQclear(res);
        bad_exit(conn);
    }
    int rows = PQntuples(res); // Get the number of rows
    PQclear(res);
    if (rows == 0) { // If there are no rows, that means there is no actor with the actorID
        res = PQexec(conn, "COMMIT;"); // Commit the transaction
        if (PQresultStatus(res) != PGRES_COMMAND_OK) { // If the transaction was not committed successfully
            printf("Transaction failed. There are no actors with the actorID %d\n", theActorID);
            PQclear(res);
            bad_exit(conn);
        }
        PQclear(res);
        return -1; // Return -1
    }
    // If there is an actor with the actorID, but potentially no characters
    sprintf(sql, "SELECT COUNT(DISTINCT playTitle || characterName) FROM Roles WHERE actorID = %d;", theActorID);
    res = PQexec(conn, sql); // Execute the SQL statement
    if (PQresultStatus(res) != PGRES_TUPLES_OK) { // If the SQL statement execution was not successful
        printf("No data retrieved\n");
        PQclear(res);
        bad_exit(conn);
    }
    int count = atoi(PQgetvalue(res, 0, 0)); // Convert the value to an integer
    res = PQexec(conn, "COMMIT;"); // Commit the transaction
    if (PQresultStatus(res) != PGRES_COMMAND_OK) { // If the transaction was not committed successfully
        printf("Transaction failed\n");
        PQclear(res);
        bad_exit(conn);
    }
    PQclear(res);
    return count; // Return the count
}


/* Function: renameTheater:
 * ------------------------
 * Parameters:  connection, and character strings oldTheaterName and newTheaterName.
 * Updates the theaterName values for all theaters in Theaters which whose theaterName
 * was oldTheaterName to newTheaterName, and returns the number of addresses updates.
 *
 * If no theater names are updated (because no theaters have oldTheaterName as their
 * theaterName, return 0; that's not an error.
 *
 * If there are multiple theaters had oldTheaterName as their theaterName, then update
 * the theaterName for all of them, and return the number updated; that' also not an error.
 *
 * However, if there already was a theater whose name is newTheaterName, then return -1,
 * even if there aren't any theaters whose name is oldTheaterName.
 */

int renameTheater(PGconn *conn, char *oldTheaterName, char *newTheaterName)
{
    PGresult *res = PQexec(conn, "BEGIN TRANSACTION;"); // Begin the transaction
    if (PQresultStatus(res) != PGRES_COMMAND_OK) { // If the transaction was not started successfully
        printf("Transaction failed\n");
        PQclear(res);
        bad_exit(conn);
    }
    char sql[MAXSQLSTATEMENTSTRINGSIZE]; // Create a string to store the SQL statement
    sprintf(sql, "SELECT * FROM Theaters WHERE theaterName = '%s';", newTheaterName); // Find the theater with the newTheaterName
    res = PQexec(conn, sql); // Execute the SQL statement
    if (PQresultStatus(res) != PGRES_TUPLES_OK) { // If the SQL statement was not executed successfully
        printf("No data retrieved\n");
        PQclear(res);
        bad_exit(conn);
    }
    int rows = PQntuples(res); // Get the number of rows
    PQclear(res);
    if (rows > 0) { // If there are rows, that means there is already a theater with the newTheaterName
        res = PQexec(conn, "COMMIT;"); // Commit the transaction
        if (PQresultStatus(res) != PGRES_COMMAND_OK) { // If the transaction was not committed successfully
            PQclear(res);
            bad_exit(conn);
        }
        PQclear(res);
        return -1; // Return -1
    }
    sprintf(sql, "UPDATE Theaters SET theaterName = '%s' WHERE theaterName = '%s';", newTheaterName, oldTheaterName); // Update the theaterName
    res = PQexec(conn, sql); // Execute the SQL statement
    if (PQresultStatus(res) != PGRES_COMMAND_OK) { // If the SQL statement was not executed successfully
        PQclear(res);
        bad_exit(conn);
    }
    int updated = atoi(PQcmdTuples(res)); // Convert the value to an integer
    res = PQexec(conn, "COMMIT;"); // Commit the transaction
    if (PQresultStatus(res) != PGRES_COMMAND_OK) { // If the transaction was not committed successfully
        PQclear(res);
        bad_exit(conn);
    }
    PQclear(res);
    return updated; // Return the number of theaters renamed
}


/* Function: IncreaseSomeCastMemberSalaries:
 * -----------------------------------------
 * Parameters:  connection, a character string thePlayTitle, and integers theProdNum and
 * maxDailyCost. thePlayTitle and theProdNum should identify a production in Productions.
 *
 * IncreaseSomeCastMemberSalaries will try to increase the salaryPerDay of some cast members,
 * starting with those who have the lowest salaryPerDay, aa described in Section 5 of the Lab4 pdf.
 *
 * However, the Total Daily Cost for this production, counting the theaterFeePerDay for the
 * production and the total salaryPerDay of all cast members should not be more than maxDailyCost.
 *
 * Executes by invoking a Stored Function, IncreaseSomeCastMemberSalariesFunction, which does 
 * all of the work.
 *
 * Returns the new Total Daily Cost after any salaryPerDay increases have been applied.
 * However, if maxDailyCost is not postiive, this function returns -1.
 *
 * And it is possible that Total Daily Cost already equals (or even if greater than) maxDailyCost.
 * That's not an error.  In that case, IncreaseSomeCastMemberSalaries changes no salaries, and
 * returns (the unchanged) Total Daily Cost.
 */

float increaseSomeCastMemberSalaries(PGconn *conn, char *thePlayTitle, int theProductionNum, float maxDailyCost)
{
    PGresult *res = PQexec(conn, "BEGIN TRANSACTION;"); // Begin the transaction
    if (PQresultStatus(res) != PGRES_COMMAND_OK) { // If the transaction was not started successfully
        PQclear(res);
        bad_exit(conn);
    }
    char sql[MAXSQLSTATEMENTSTRINGSIZE]; // Create a string to store the SQL statement
    sprintf(sql, "SELECT IncreaseSomeCastMemberSalariesFunction('%s', %d, %f);", thePlayTitle, theProductionNum, maxDailyCost);
    res = PQexec(conn, sql); // Execute the SQL statement
    if (PQresultStatus(res) != PGRES_TUPLES_OK) { // If the SQL statement was not executed successfully
        printf("No data retrieved\n");
        PQclear(res);
        bad_exit(conn);
    }
    char *value = PQgetvalue(res, 0, 0); // Get the value of the first tuple
    float result = atof(value); // Convert the value to a float
    res = PQexec(conn, "COMMIT;"); // Commit the transaction
    if (PQresultStatus(res) != PGRES_COMMAND_OK) { // If the transaction was not committed successfully
        printf("No data retrieved\n");
        PQclear(res);
        bad_exit(conn);
    }
    PQclear(res);
    return result; // Return the result
}

int main(int argc, char **argv)
{
    PGconn *conn;
    int theResult;

    if (argc != 3)
    {
        fprintf(stderr, "Usage: ./runHighwayApplication <username> <password>\n");
        exit(EXIT_FAILURE);
    }

    char *userID = argv[1];
    char *pwd = argv[2];

    char conninfo[MAXCONNECTIONSTRINGSIZE] = "host=cse180-db.lt.ucsc.edu user=";
    strcat(conninfo, userID);
    strcat(conninfo, " password=");
    strcat(conninfo, pwd);

    /* Make a connection to the database */
    conn = PQconnectdb(conninfo);

    /* Check to see if the database connection was successfully made. */
    if (PQstatus(conn) != CONNECTION_OK)
    {
        fprintf(stderr, "Connection to database failed: %s\n",
                PQerrorMessage(conn));
        bad_exit(conn);
    }
    /* Perform the calls to countDifferentPlayCharacters  listed in Section 6 of Lab4,
     * and print messages as described.
     */
    // Test 1
    theResult = countDifferentPlayCharacters(conn, 1);
    if (theResult == -1) { // If there is no actor with the actorID 1
        printf("There is no actor whose actorID is 1\n");
    } else { // If there is an actor with the actorID 1
        printf("Number of different play characters for %d is %d\n", 1, theResult);
    }
    // Test 2
    theResult = countDifferentPlayCharacters(conn, 2);
    if (theResult == -1) {
        printf("There is no actor whose actorID is 2\n");
    } else {
        printf("Number of different play characters for %d is %d\n", 2, theResult);
    }
    // Test 3
    theResult = countDifferentPlayCharacters(conn, 6);
    if (theResult == -1) {
        printf("There is no actor whose actorID is 6\n");
    } else {
        printf("Number of different play characters for %d is %d\n", 6, theResult);
    }
    // Test 4
    theResult = countDifferentPlayCharacters(conn, 7);
    if (theResult == -1) {
        printf("There is no actor whose actorID is 7\n");
    } else {
        printf("Number of different play characters for %d is %d\n", 7, theResult);
    }
    /* Extra newline for readability */
    printf("\n");

    
    /* Perform the calls to renameTheater  listed in Section 6 of Lab4,
     * and print messages as described.
     */
    // Test 1
    theResult = renameTheater(conn, "West End Theater", "Olivier Theatre");
    if (theResult == -1) {
        printf("There is already a theater name whose theaterName is Olivier Theatre\n");
    } else {
        printf("%d theater names were renamed to Olivier Theatre by renameTheater\n", theResult);
    }
    // Test 2
    theResult = renameTheater(conn, "Broadway Theater", "Sondheim Theater");
    if (theResult == -1) {
        printf("There is already a theater name whose theaterName is Sondheim Theater\n");
    } else {
        printf("%d theater names were renamed to Sondheim Theater by renameTheater\n", theResult);
    }
    // Test 3
    theResult = renameTheater(conn, "Booth Theater", "New Booth Theater");
    if (theResult == -1) {
        printf("There is already a theater name whose theaterName is New Booth Theater\n");
    } else {
        printf("%d theater names were renamed to New Booth Theater by renameTheater\n", theResult);
    }
    /* Extra newline for readability */
    printf("\n");

    
    /* Perform the calls to IncreaseSomeCastMemberSalaries  listed in Section 6 of Lab4,
     * and print messages as described.
     * You may use helper functions to do this, if you want.
     */
    // Test 1
    float newTotalDailyCost = increaseSomeCastMemberSalaries(conn, "Romeo and Juliet", 1, 2000.00);
    if (newTotalDailyCost == -1) {
        printf("maxDailyCost is not positive...\n");
        bad_exit(conn);
    } else if (newTotalDailyCost == -2) {
        printf("Invalid playTitle or productionNum...\n");\
        bad_exit(conn);
    } else if (newTotalDailyCost == 20) {
        printf("So far so good...\n");
    } else {
        printf("Total Daily Cost for Romeo and Juliet, 1 is now %7.2f\n", newTotalDailyCost);
    }
    // Test 2
    newTotalDailyCost = increaseSomeCastMemberSalaries(conn, "Macbeth", 1, 990.00);
    if (newTotalDailyCost == -1) {
        printf("maxDailyCost is not positive...\n");
        bad_exit(conn);
    } else if (newTotalDailyCost == -2) {
        printf("Invalid playTitle or productionNum...\n");\
        bad_exit(conn);
    } else {
        printf("Total Daily Cost for Macbeth, 1 is now %7.2f\n", newTotalDailyCost);
    }
    // Test 3
    newTotalDailyCost = increaseSomeCastMemberSalaries(conn, "The Merry Wives of Windsor", 1, 1125.00);
    if (newTotalDailyCost == -1) {
        printf("maxDailyCost is not positive...\n");
        bad_exit(conn);
    } else if (newTotalDailyCost == -2) {
        printf("Invalid playTitle or productionNum...\n");
        bad_exit(conn);
    } else {
        printf("Total Daily Cost for The Merry Wives of Windsor, 1 is now %7.2f\n", newTotalDailyCost);
    }
    // Test 4
    newTotalDailyCost = increaseSomeCastMemberSalaries(conn, "A Midsummer Nights Dream", 1, 1100.00);
    if (newTotalDailyCost == -1) {
        printf("maxDailyCost is not positive...\n");
        bad_exit(conn);
    } else if (newTotalDailyCost == -2) {
        printf("Invalid playTitle or productionNum...\n");
        bad_exit(conn);
    } else {
        printf("Total Daily Cost for A Midsummer Nights Dream, 1 is now %7.2f\n", newTotalDailyCost);
    }
    // Test 5
    newTotalDailyCost = increaseSomeCastMemberSalaries(conn, "Henry IV, Part 1", 1, 1060.00);
    if (newTotalDailyCost == -1) {
        printf("maxDailyCost is not positive...\n");
        bad_exit(conn);
    } else if (newTotalDailyCost == -2) {
        printf("Invalid playTitle or productionNum...\n");
        bad_exit(conn);
    } else {
        printf("Total Daily Cost for Henry IV, Part 1, 1 is now %7.2f\n", newTotalDailyCost);
    }
    // Test 6
    newTotalDailyCost = increaseSomeCastMemberSalaries(conn, "Hamlet", 1, 730.00);
    if (newTotalDailyCost == -1) {
        printf("maxDailyCost is not positive...\n");
        bad_exit(conn);
    } else if (newTotalDailyCost == -2) {
        printf("Invalid playTitle or productionNum...\n");
        bad_exit(conn);
    } else {
        printf("Total Daily Cost for Hamlet, 1 is now %7.2f\n", newTotalDailyCost);
    }
    good_exit(conn);
    return 0;
}
