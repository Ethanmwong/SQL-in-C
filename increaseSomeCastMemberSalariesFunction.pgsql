CREATE OR REPLACE FUNCTION increaseSomeCastMemberSalariesFunction(thePlayTitle VARCHAR(40), theProductionNum INTEGER, maxDailyCost NUMERIC(7,2))
RETURNS NUMERIC(7,2) AS $$

DECLARE
    totalDailyCost NUMERIC(7,2);
    newSalary NUMERIC(5,2);
    rec RECORD;

DECLARE actorCursor CURSOR FOR /* Cursor to iterate through the cast members of the play */
    SELECT actorID, salaryPerDay
    FROM CastMembers
    WHERE playTitle = thePlayTitle AND productionNum = theProductionNum
    ORDER BY salaryPerDay ASC;

BEGIN
    IF maxDailyCost <= 0 THEN /* If the maximum daily cost is less than or equal to 0, return -1 */
        RETURN -1;
    END IF;
    /* Calculate the total daily cost of the cast members */
    SELECT SUM(salaryPerDay) + (SELECT theaterFeePerDay FROM Theaters WHERE theaterID = (SELECT theaterID 
                                                                                        FROM Productions 
                                                                                        WHERE playTitle = thePlayTitle 
                                                                                        AND productionNum = theProductionNum))
    INTO totalDailyCost
    FROM CastMembers
    WHERE playTitle = thePlayTitle AND productionNum = theProductionNum;
    /* If the total daily cost is NULL, that means there is no production in productions corresponding to the given play title and production number */
    IF totalDailyCost IS NULL THEN
        RETURN -2;
    END IF;

    OPEN actorCursor;

    LOOP
        FETCH NEXT FROM actorCursor INTO rec;
        EXIT WHEN NOT FOUND;

        IF rec.salaryPerDay <= 50 THEN
            newSalary := rec.salaryPerDay + 6.00;
        ELSIF rec.salaryPerDay > 50 AND rec.salaryPerDay <= 100 THEN
            newSalary := rec.salaryPerDay + 8.50;
        ELSIF rec.salaryPerDay > 100 AND rec.salaryPerDay <= 200 THEN
            newSalary := rec.salaryPerDay + 10.00;
        ELSE
            CONTINUE;
        END IF;

        IF totalDailyCost + newSalary - rec.salaryPerDay > maxDailyCost THEN
            CLOSE actorCursor;
            RETURN totalDailyCost;
        ELSE
            totalDailyCost := totalDailyCost + newSalary - rec.salaryPerDay;
            UPDATE CastMembers SET salaryPerDay = newSalary WHERE playTitle = thePlayTitle AND productionNum = theProductionNum AND actorID = rec.actorID;
        END IF;
    END LOOP;

    CLOSE actorCursor;
    RETURN totalDailyCost;
END; $$

LANGUAGE plpgsql;
