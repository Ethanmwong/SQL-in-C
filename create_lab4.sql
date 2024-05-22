-- CSE 180 Fall 2023 Lab4 create file

-- This file is similar to our Lab1 solution (and the Lab2 create file you were asked
-- to modify).  It has the Foreign Key constraints that were in those files, but does
-- not have the UNIQUE and NOT NULL constraints which were in the Lab2 solution.
-- It also does not have the new CastMemberDeltas tables which used in Lab3.

DROP SCHEMA Lab4 CASCADE;
CREATE SCHEMA Lab4;


-- Persons(PersonID, personName, personAddress, birthDate)
CREATE TABLE Persons(
    personID INT PRIMARY KEY,
    personName VARCHAR(40),
    personAddress VARCHAR(60),
    birthDate DATE
);

-- Actors(actorID, isActorEquity, equityJoinDate, roleCount)
CREATE TABLE Actors(
    actorID INT PRIMARY KEY REFERENCES Persons(personID),
    isActorsEquity BOOL,
    equityJoinDate DATE,
    roleCount INT
);

-- ShakespearePlays(playTitle, yearWritten, category)
CREATE TABLE ShakespearePlays(
    playTitle VARCHAR(40) PRIMARY KEY,
    yearWritten INT,
    category CHAR(1)
);

-- Characters(playTitle, characterName)
CREATE TABLE Characters(
    playTitle VARCHAR(40) REFERENCES ShakespearePlays,
    characterName VARCHAR(40),
    PRIMARY KEY (playTitle, characterName)
);

-- Theaters(theaterID, theaterName, theaterAddress, theaterFeePerDay)
CREATE TABLE Theaters(
    theaterID INT PRIMARY KEY,
    theaterName VARCHAR(40),
    theaterAddress VARCHAR(60),
    theaterFeePerDay NUMERIC(6,2)
);

-- Productions(playTitle, productionNum, directorID, theaterID, productionStartDate, productionEndDate)
CREATE TABLE Productions(
    playTitle VARCHAR(40) REFERENCES ShakespearePlays,
    productionNum INT,
    directorID INT,
    theaterID INT,
    productionStartDate DATE,
    productionEndDate DATE,
    PRIMARY KEY (playTitle, productionNum)
);

-- CastMembers(playTitle, productionNum, actorID, salaryPerDay, applausePosition)
CREATE TABLE CastMembers(
    playTitle VARCHAR(40),
    productionNum INT,
    actorID INT REFERENCES Actors,
    salaryPerDay NUMERIC(5,2),
    applausePosition INT,
    FOREIGN KEY (playTitle, productionNum) REFERENCES Productions,
    PRIMARY KEY (playTitle, productionNum, actorID)
);

-- Roles(playTitle, productionNum, actorID, characterName)
CREATE TABLE Roles(
    playTitle VARCHAR(40),
    productionNum INT,
    actorID INT,
    characterName VARCHAR(40),
    FOREIGN KEY (playTitle, productionNum, actorID) REFERENCES CastMembers,
    PRIMARY KEY (playTitle, productionNum, actorID, characterName)
);
