-- Load data for Lab 2

-- Persons(PersonID, personName, personAddress, birthDate)
COPY Persons(personID, personName, personAddress, birthDate) FROM stdin USING DELIMITERS '|';
1|John Doe|123 Main St, Anytown, USA|1990-05-15
2|Jane Smith|456 Elm St, Othertown, USA|1988-08-20
3|Alice Johnson|789 Oak St, Anycity, USA|1975-03-10
4|Michael Brown|111 Pine St, Anothercity, USA|1985-11-25
5|Emily Davis|222 Maple St, Someplace, USA|1995-07-03
6|Henry Lee|314 Sycamore Ave, Prettyhowtown, USA|1982-07-13
\.

-- Actors(actorID, isActorEquity, equityJoinDate, roleCount)
COPY Actors(actorID, isActorsEquity, equityJoinDate, roleCount) FROM stdin USING DELIMITERS '|';
1|true|\N|1
2|false|2001-01-01|1
3|false|2002-01-01|2
4|\N|\N|5
5|true|2012-09-01|0
6|true|2014-01-31|3
\.


-- ShakespearePlays(playTitle, yearWritten, category)
COPY ShakespearePlays(playTitle, yearWritten, category) FROM stdin USING DELIMITERS '|';
Hamlet|1600|T
Romeo and Juliet|1597|T
Macbeth|1606|T
Henry V|1599|H
A Midsummer Nights Dream|1595|C
Julius Caesar|1599|\N
Henry IV, Part 1|1597|H
Henry IV, Part 2|1598|H
The Merry Wives of Windsor|1600|C
\.


-- Characters(playTitle, characterName)
COPY Characters(playTitle, characterName) FROM stdin USING DELIMITERS '|';
Hamlet|Hamlet
Hamlet|Ophelia
Hamlet|Claudius
Romeo and Juliet|Romeo
Romeo and Juliet|Juliet
Macbeth|Macbeth
Henry IV, Part 1|Falstaff
Henry IV, Part 2|Falstaff
The Merry Wives of Windsor|Falstaff
Henry V|Henry V
Henry V|Catherine
A Midsummer Nights Dream|Bottom
A Midsummer Nights Dream|Titania
\.

-- Theaters(theaterID, theaterName, theaterAddress, theaterFeePerDay)
COPY Theaters(theaterID, theaterName, theaterAddress, theaterFeePerDay) FROM stdin USING DELIMITERS '|';
1|Globe Theater|London, UK|500.00
2|Broadway Theater|New York, USA|1000.00
3|West End Theater|London, UK|750.00
4|West End Theater|London, UK|800.00
5|Theater Royal|Sydney, Australia|900.00
\.

-- Productions(playTitle, productionNum, directorID, theaterID, productionStartDate, productionEndDate)
COPY Productions(playTitle, productionNum, directorID, theaterID, productionStartDate, productionEndDate) FROM stdin USING DELIMITERS '|';
Hamlet|1|1|1|2023-10-01|2023-11-01
Romeo and Juliet|1|2|2|2023-12-01|2024-01-01
Macbeth|1|3|3|2024-02-01|2024-03-01
Henry V|1|4|4|2024-04-01|2024-04-01
A Midsummer Nights Dream|1|5|5|2024-06-01|2024-07-01
Henry IV, Part 1|1|3|2|2020-02-07|2020-05-09
Henry IV, Part 2|1|3|2|2021-02-07|2021-05-09
The Merry Wives of Windsor|8|3|2|2022-02-07|2022-05-09
The Merry Wives of Windsor|9|4|2|2023-02-07|2023-05-09
\.

-- CastMembers(playTitle, productionNum, actorID, salaryPerDay, applausePosition)
COPY CastMembers(playTitle, productionNum, actorID, salaryPerDay, applausePosition) FROM stdin USING DELIMITERS '|';
Hamlet|1|1|110.00|1
Hamlet|1|3|110.00|\N
Romeo and Juliet|1|4|150.00|2
Romeo and Juliet|1|1|80.00|1
Romeo and Juliet|1|3|250.00|3
Macbeth|1|1|120.00|1
Macbeth|1|3|110.00|2
Henry V|1|4|180.00|2
Henry V|1|1|150.00|1
A Midsummer Nights Dream|1|5|130.00|3
A Midsummer Nights Dream|1|1|75.00|4
Henry IV, Part 1|1|6|48.00|3
Henry IV, Part 2|1|6|51.00|2
The Merry Wives of Windsor|8|6|120.00|1
The Merry Wives of Windsor|9|6|125.00|1
\.

-- Roles(playTitle, productionNum, actorID, characterName)
COPY Roles(playTitle, productionNum, actorID, characterName) FROM stdin USING DELIMITERS '|';
Hamlet|1|1|Hamlet
Hamlet|1|3|Ophelia
Hamlet|1|3|Claudius
Romeo and Juliet|1|4|Romeo
Romeo and Juliet|1|1|Romeo
Romeo and Juliet|1|3|Juliet
Macbeth|1|1|Macbeth
Macbeth|1|3|Macbeth
Henry V|1|4|Henry V
Henry V|1|1|Catherine
A Midsummer Nights Dream|1|5|Bottom
A Midsummer Nights Dream|1|5|Titania
A Midsummer Nights Dream|1|1|Titania
Henry IV, Part 1|1|6|Falstaff
Henry IV, Part 2|1|6|Falstaff
The Merry Wives of Windsor|8|6|Falstaff
The Merry Wives of Windsor|9|6|Falstaff
\.


