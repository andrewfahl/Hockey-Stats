# clean out all existing data
truncate table league;
truncate table league_history;

truncate table season;
truncate table season_history;

truncate table team;
truncate table team_history;

truncate table season_team;
truncate table season_team_history;

truncate table location;
truncate table location_history;

truncate table game;
truncate table game_history;

truncate table player;
truncate table player_history;

truncate table season_team_player;
truncate table season_team_player_history;

# add leagues
INSERT INTO league(`name`,`sequence`,`saved_userid`,`saved_datetime`)
VALUES('A League',1,1,NOW());

INSERT INTO league(`name`,`sequence`,`saved_userid`,`saved_datetime`)
VALUES('B League',2,0,NOW());

INSERT INTO league(`name`,`sequence`,`saved_userid`,`saved_datetime`)
VALUES('C League',3,0,NOW());

INSERT INTO league(`name`,`sequence`,`saved_userid`,`saved_datetime`)
VALUES('D League',4,0,NOW());

# add seasons
INSERT INTO season(`leagueid`,`name`,`start_datetime`,`saved_userid`,`saved_datetime`)
VALUES (1,'Summer 2015','2015-06-01',0,NOW());

INSERT INTO season(`leagueid`,`name`,`start_datetime`,`saved_userid`,`saved_datetime`)
VALUES (2,'Summer 2015','2015-06-01',0,NOW());

INSERT INTO season(`leagueid`,`name`,`start_datetime`,`saved_userid`,`saved_datetime`)
VALUES (3,'Summer 2015','2015-06-01',0,NOW());

INSERT INTO season(`leagueid`,`name`,`start_datetime`,`saved_userid`,`saved_datetime`)
VALUES (4,'Summer 2015','2015-06-01',0,NOW());

INSERT INTO season(`leagueid`,`name`,`start_datetime`,`saved_userid`,`saved_datetime`)
VALUES (1,'Winter 2015/2016','2015-09-01',0,NOW());

INSERT INTO season(`leagueid`,`name`,`start_datetime`,`saved_userid`,`saved_datetime`)
VALUES (2,'Winter 2015/2016','2015-09-01',0,NOW());

INSERT INTO season(`leagueid`,`name`,`start_datetime`,`saved_userid`,`saved_datetime`)
VALUES (3,'Winter 2015/2016','2015-09-01',0,NOW());

INSERT INTO season(`leagueid`,`name`,`start_datetime`,`saved_userid`,`saved_datetime`)
VALUES (4,'Winter 2015/2016','2015-09-01',0,NOW());

# add teams
INSERT INTO team(`name`,`active_yn`,`saved_userid`,`saved_datetime`)
VALUES('Team 1',1,0,NOW());

INSERT INTO team(`name`,`active_yn`,`saved_userid`,`saved_datetime`)
VALUES('Team 2',1,0,NOW());

INSERT INTO team(`name`,`active_yn`,`saved_userid`,`saved_datetime`)
VALUES('Team 3',1,0,NOW());

INSERT INTO team(`name`,`active_yn`,`saved_userid`,`saved_datetime`)
VALUES('Team 4',1,0,NOW());

INSERT INTO team(`name`,`active_yn`,`saved_userid`,`saved_datetime`)
VALUES('Team 5',1,0,NOW());

INSERT INTO team(`name`,`active_yn`,`saved_userid`,`saved_datetime`)
VALUES('Team 6',1,0,NOW());

INSERT INTO team(`name`,`active_yn`,`saved_userid`,`saved_datetime`)
VALUES('Team 7',1,0,NOW());

INSERT INTO team(`name`,`active_yn`,`saved_userid`,`saved_datetime`)
VALUES('Team 8',1,0,NOW());

INSERT INTO team(`name`,`active_yn`,`saved_userid`,`saved_datetime`)
VALUES('Team 9',1,0,NOW());

INSERT INTO team(`name`,`active_yn`,`saved_userid`,`saved_datetime`)
VALUES('Team 10',1,0,NOW());

INSERT INTO team(`name`,`active_yn`,`saved_userid`,`saved_datetime`)
VALUES('Team 11',1,0,NOW());

INSERT INTO team(`name`,`active_yn`,`saved_userid`,`saved_datetime`)
VALUES('Team 12',1,0,NOW());

INSERT INTO team(`name`,`active_yn`,`saved_userid`,`saved_datetime`)
VALUES('Team 13',1,0,NOW());

INSERT INTO team(`name`,`active_yn`,`saved_userid`,`saved_datetime`)
VALUES('Team 14',1,0,NOW());

INSERT INTO team(`name`,`active_yn`,`saved_userid`,`saved_datetime`)
VALUES('Team 15',1,0,NOW());

INSERT INTO team(`name`,`active_yn`,`saved_userid`,`saved_datetime`)
VALUES('Team 16',1,0,NOW());

INSERT INTO team(`name`,`active_yn`,`saved_userid`,`saved_datetime`)
VALUES('Team Inactive',0,0,NOW());

# add teams to seasons
INSERT INTO season_team(`seasonid`,`teamid`,`saved_userid`,`saved_datetime`)
VALUES (1,1,0,NOW());

INSERT INTO season_team(`seasonid`,`teamid`,`saved_userid`,`saved_datetime`)
VALUES (1,2,0,NOW());

INSERT INTO season_team(`seasonid`,`teamid`,`saved_userid`,`saved_datetime`)
VALUES (1,3,0,NOW());

INSERT INTO season_team(`seasonid`,`teamid`,`saved_userid`,`saved_datetime`)
VALUES (1,4,0,NOW());

INSERT INTO season_team(`seasonid`,`teamid`,`saved_userid`,`saved_datetime`)
VALUES (1,17,0,NOW());

INSERT INTO season_team(`seasonid`,`teamid`,`saved_userid`,`saved_datetime`)
VALUES (5,1,0,NOW());

INSERT INTO season_team(`seasonid`,`teamid`,`saved_userid`,`saved_datetime`)
VALUES (5,2,0,NOW());

INSERT INTO season_team(`seasonid`,`teamid`,`saved_userid`,`saved_datetime`)
VALUES (5,3,0,NOW());

INSERT INTO season_team(`seasonid`,`teamid`,`saved_userid`,`saved_datetime`)
VALUES (5,4,0,NOW());

INSERT INTO season_team(`seasonid`,`teamid`,`saved_userid`,`saved_datetime`)
VALUES (2,5,0,NOW());

INSERT INTO season_team(`seasonid`,`teamid`,`saved_userid`,`saved_datetime`)
VALUES (2,6,0,NOW());

INSERT INTO season_team(`seasonid`,`teamid`,`saved_userid`,`saved_datetime`)
VALUES (2,7,0,NOW());

INSERT INTO season_team(`seasonid`,`teamid`,`saved_userid`,`saved_datetime`)
VALUES (2,8,0,NOW());

INSERT INTO season_team(`seasonid`,`teamid`,`saved_userid`,`saved_datetime`)
VALUES (6,5,0,NOW());

INSERT INTO season_team(`seasonid`,`teamid`,`saved_userid`,`saved_datetime`)
VALUES (6,6,0,NOW());

INSERT INTO season_team(`seasonid`,`teamid`,`saved_userid`,`saved_datetime`)
VALUES (6,7,0,NOW());

INSERT INTO season_team(`seasonid`,`teamid`,`saved_userid`,`saved_datetime`)
VALUES (6,8,0,NOW());

INSERT INTO season_team(`seasonid`,`teamid`,`saved_userid`,`saved_datetime`)
VALUES (3,9,0,NOW());

INSERT INTO season_team(`seasonid`,`teamid`,`saved_userid`,`saved_datetime`)
VALUES (3,10,0,NOW());

INSERT INTO season_team(`seasonid`,`teamid`,`saved_userid`,`saved_datetime`)
VALUES (3,11,0,NOW());

INSERT INTO season_team(`seasonid`,`teamid`,`saved_userid`,`saved_datetime`)
VALUES (3,12,0,NOW());

INSERT INTO season_team(`seasonid`,`teamid`,`saved_userid`,`saved_datetime`)
VALUES (7,9,0,NOW());

INSERT INTO season_team(`seasonid`,`teamid`,`saved_userid`,`saved_datetime`)
VALUES (7,10,0,NOW());

INSERT INTO season_team(`seasonid`,`teamid`,`saved_userid`,`saved_datetime`)
VALUES (7,11,0,NOW());

INSERT INTO season_team(`seasonid`,`teamid`,`saved_userid`,`saved_datetime`)
VALUES (7,12,0,NOW());

INSERT INTO season_team(`seasonid`,`teamid`,`saved_userid`,`saved_datetime`)
VALUES (4,13,0,NOW());

INSERT INTO season_team(`seasonid`,`teamid`,`saved_userid`,`saved_datetime`)
VALUES (4,14,0,NOW());

INSERT INTO season_team(`seasonid`,`teamid`,`saved_userid`,`saved_datetime`)
VALUES (4,15,0,NOW());

INSERT INTO season_team(`seasonid`,`teamid`,`saved_userid`,`saved_datetime`)
VALUES (4,16,0,NOW());

INSERT INTO season_team(`seasonid`,`teamid`,`saved_userid`,`saved_datetime`)
VALUES (8,13,0,NOW());

INSERT INTO season_team(`seasonid`,`teamid`,`saved_userid`,`saved_datetime`)
VALUES (8,14,0,NOW());

INSERT INTO season_team(`seasonid`,`teamid`,`saved_userid`,`saved_datetime`)
VALUES (8,15,0,NOW());

INSERT INTO season_team(`seasonid`,`teamid`,`saved_userid`,`saved_datetime`)
VALUES (8,16,0,NOW());

# add locations
INSERT INTO location(`name`,`address`,`address2`,`city`,`state`,`zip`,`saved_userid`,`saved_datetime`)
VALUES('The Arena East Rink','123 Some Street',NULL,'Hockeytown','MN','11111',0,NOW());

INSERT INTO location(`name`,`address`,`address2`,`city`,`state`,`zip`,`saved_userid`,`saved_datetime`)
VALUES('The Arena West Rink','123 Some Street',NULL,'Hockeytown','MN','11111',0,NOW());

# A League games
INSERT INTO game (home_teamid, visiting_teamid, start_datetime, locationid, seasonid, playoff_game_yn, saved_userid, saved_datetime)
VALUES(1, 2, '2015-06-06 18:00:00', 1, 1, 0, 0, NOW());

INSERT INTO game (home_teamid, visiting_teamid, start_datetime, locationid, seasonid, playoff_game_yn, saved_userid, saved_datetime)
VALUES(3, 4, '2015-06-06 19:30:00', 1, 1, 0, 0, NOW());

INSERT INTO game (home_teamid, visiting_teamid, start_datetime, locationid, seasonid, playoff_game_yn, saved_userid, saved_datetime)
VALUES(2, 3, '2015-06-13 19:30:00', 2, 1, 0, 0, NOW());

INSERT INTO game (home_teamid, visiting_teamid, start_datetime, locationid, seasonid, playoff_game_yn, saved_userid, saved_datetime)
VALUES(17, 1, '2015-06-13 19:30:00', 2, 1, 0, 0, NOW());

INSERT INTO game (home_teamid, visiting_teamid, start_datetime, locationid, seasonid, playoff_game_yn, saved_userid, saved_datetime)
VALUES(2, 17, '2015-06-20 19:30:00', 1, 1, 0, 0, NOW());

INSERT INTO game (home_teamid, visiting_teamid, start_datetime, locationid, seasonid, playoff_game_yn, saved_userid, saved_datetime)
VALUES(4, 1, '2015-06-20 19:30:00', 1, 1, 0, 0, NOW());

INSERT INTO game (home_teamid, visiting_teamid, start_datetime, locationid, seasonid, playoff_game_yn, saved_userid, saved_datetime)
VALUES(17, 4, '2015-06-27 19:30:00', 2, 1, 0, 0, NOW());

INSERT INTO game (home_teamid, visiting_teamid, start_datetime, locationid, seasonid, playoff_game_yn, saved_userid, saved_datetime)
VALUES(3, 1, '2015-06-27 19:30:00', 2, 1, 0, 0, NOW());

INSERT INTO game (home_teamid, visiting_teamid, start_datetime, locationid, seasonid, playoff_game_yn, saved_userid, saved_datetime)
VALUES(3, 17, '2015-07-11 19:30:00', 1, 1, 0, 0, NOW());

INSERT INTO game (home_teamid, visiting_teamid, start_datetime, locationid, seasonid, playoff_game_yn, saved_userid, saved_datetime)
VALUES(2, 4, '2015-07-11 19:30:00', 1, 1, 0, 0, NOW());

INSERT INTO game (home_teamid, visiting_teamid, start_datetime, locationid, seasonid, playoff_game_yn, saved_userid, saved_datetime)
VALUES(1, 2, '2015-09-05 18:00:00', 2, 5, 0, 0, NOW());

INSERT INTO game (home_teamid, visiting_teamid, start_datetime, locationid, seasonid, playoff_game_yn, saved_userid, saved_datetime)
VALUES(3, 4, '2015-09-05 19:30:00', 2, 5, 0, 0, NOW());

INSERT INTO game (home_teamid, visiting_teamid, start_datetime, locationid, seasonid, playoff_game_yn, saved_userid, saved_datetime)
VALUES(2, 3, '2015-09-12 19:30:00', 1, 5, 0, 0, NOW());

INSERT INTO game (home_teamid, visiting_teamid, start_datetime, locationid, seasonid, playoff_game_yn, saved_userid, saved_datetime)
VALUES(17, 1, '2015-09-12 19:30:00', 1, 5, 0, 0, NOW());

INSERT INTO game (home_teamid, visiting_teamid, start_datetime, locationid, seasonid, playoff_game_yn, saved_userid, saved_datetime)
VALUES(2, 17, '2015-09-19 19:30:00', 2, 5, 0, 0, NOW());

INSERT INTO game (home_teamid, visiting_teamid, start_datetime, locationid, seasonid, playoff_game_yn, saved_userid, saved_datetime)
VALUES(4, 1, '2015-09-19 19:30:00', 2, 5, 0, 0, NOW());

INSERT INTO game (home_teamid, visiting_teamid, start_datetime, locationid, seasonid, playoff_game_yn, saved_userid, saved_datetime)
VALUES(17, 4, '2015-09-26 19:30:00', 1, 5, 0, 0, NOW());

INSERT INTO game (home_teamid, visiting_teamid, start_datetime, locationid, seasonid, playoff_game_yn, saved_userid, saved_datetime)
VALUES(3, 1, '2015-09-26 19:30:00', 1, 5, 0, 0, NOW());

INSERT INTO game (home_teamid, visiting_teamid, start_datetime, locationid, seasonid, playoff_game_yn, saved_userid, saved_datetime)
VALUES(3, 17, '2015-10-03 19:30:00', 2, 5, 0, 0, NOW());

INSERT INTO game (home_teamid, visiting_teamid, start_datetime, locationid, seasonid, playoff_game_yn, saved_userid, saved_datetime)
VALUES(2, 4, '2015-10-03 19:30:00', 2, 5, 0, 0, NOW());

# add players (each team gets a player for each position)
INSERT INTO player (first_name, last_name, player_positionid, secondary_player_positionid, active_yn, saved_userid, saved_datetime)
select t.name, pp.name, player_positionid, 
CASE  
	when player_positionid = 1 then 2 
	else 1 
END,
1,
0,
NOW()
from team t
cross join player_position pp;

#add players to seasons
insert into season_team_player (season_teamid, playerid, number, saved_userid, saved_datetime)
select st.season_teamid, p.playerid, NULL, 0, NOW()
from season_team st
inner join team t on st.teamid = t.teamid
inner join player p on p.first_name = t.name;


/*
# drop all tables
drop table league;
drop table league_history;
drop table season;
drop table season_history;
drop table team;
drop table teamH_istory;
drop table season_team;
drop table season_team_history;
drop table location;
drop table location_history;
drop table game;
drop table game_history;
drop table player;
drop table player_history;
drop table player_position;
drop table player_position_history;
drop table season_team_player;
drop table season_team_player_history;
drop table penalty;
drop table penalty_history;
drop table game_player;
drop table game_player_history;
drop table game_score;
drop table game_score_history;
drop table game_penalty;
drop table game_penalty_history;
*/