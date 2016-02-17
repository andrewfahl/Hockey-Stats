<?php

class Game implements CrudInterface {

  private $gameId = 0;
  private $homeTeamId = 0;
  private $visitingTeamId = 0;
  private $locationId = 0;
  private $seasonId = 0;
  private $startDateTime = NULL;
  private $playoffGameYN = 0;
  private $savedDateTime = NULL;
  private $savedUserID = 0;
  private $locationName = NULL;
  private $homeTeamName = NULL;
  private $visitingTeamName = NULL;
  private $homeTeamFirstPeriodGoals;
  private $visitingTeamFirstPeriodGoals;
  private $homeTeamSecondPeriodGoals;
  private $visitingTeamSecondPeriodGoals;
  private $homeTeamThirdPeriodGoals;
  private $visitingTeamThirdPeriodGoals;
  private $homeTeamOverTimeGoals;
  private $visitingTeamOverTimeGoals;
  private $homeTeamShots;
  private $visitingTeamShots;
  private $comment;
  private $homeTeamPlayers;
  private $visitingTeamPlayers;
  private $homeTeamGoalie;
  private $visitingTeamGoalie;
  private $penalties;
  
  function __construct($gameId = 0, $homeTeamId = 0, $visitingTeamId = 0, 
    $locationId = 0, $seasonId = 0, $startDateTime = NULL, $playoffGameYN = 0,
    $locationName = NULL, $homeTeamName = NULL, $visistingTeamName = NULL) {

    $this->setGameId($gameId);
    $this->setHomeTeamId($homeTeamId);
    $this->setVisitingTeamId($visitingTeamId);
    $this->setLocationId($locationId);
    $this->setSeasonId($seasonId);
    $this->setStartDateTime($startDateTime);
    $this->setPlayoffGameYN($playoffGameYN);
    $this->setHomeTeamName($homeTeamName);
    $this->setVisitingTeamName($visistingTeamName);
    $this->setLocationName($locationName);
  }
  
  public function setSavedDateTimeAndUser($savedDateTime, $savedUserID) {
    $this->setSavedUserID($savedUserID);
    $this->setSavedDateTime($savedDateTime);
  }

  public function setBoxScore($homeTeamFirstPeriodGoals, $visitingTeamFirstPeriodGoals, 
    $homeTeamSecondPeriodGoals, $visitingTeamSecondPeriodGoals, $homeTeamThirdPeriodGoals, 
    $visitingTeamThirdPeriodGoals, $homeTeamOverTimeGoals, $visitingTeamOverTimeGoals, 
    $homeTeamShots, $visitingTeamShots, $comment) {
    
    $this->setHomeTeamFirstPeriodGoals($homeTeamFirstPeriodGoals);
    $this->setVisitingTeamFirstPeriodGoals($visitingTeamFirstPeriodGoals);
    $this->setHomeTeamSecondPeriodGoals($homeTeamSecondPeriodGoals);
    $this->setVisitingTeamSecondPeriodGoals($visitingTeamSecondPeriodGoals);
    $this->setHomeTeamThirdPeriodGoals($homeTeamThirdPeriodGoals);
    $this->setVisitingTeamThirdPeriodGoals($visitingTeamThirdPeriodGoals);
    $this->setHomeTeamOverTimeGoals($homeTeamOverTimeGoals);
    $this->setVisitingTeamOverTimeGoals($visitingTeamOverTimeGoals);
    $this->setHomeTeamShots($homeTeamShots);
    $this->setVisitingTeamShots($visitingTeamShots);
    $this->setComment($comment);
  }
  
  public function build() {
    
    $query = db_select('game', 'g');
    $query->innerJoin('team', 'ht', 'ht.teamid = g.home_teamid');
    $query->innerJoin('team', 'vt', 'vt.teamid = g.visiting_teamid');
    $query->leftjoin('location', 'l', 'l.locationid = g.locationid');
    $query->fields('g');
    $query->addField('l', 'name', 'location_name');
    $query->addField('ht', 'name', 'home_team_name');
    $query->addField('vt', 'name', 'visiting_team_name');
    $query->condition('gameid', $this->getGameId());
    $result = $query->execute();
    
    $record = $result->fetchAssoc();

    if (isset($record)) {
      $this->setGameId($record['gameid']);
      $this->setHomeTeamId($record['home_teamid']);
      $this->setVisitingTeamId($record['visiting_teamid']);
      $this->setLocationId($record['locationid']);
      $this->setSeasonId($record['seasonid']);
      $this->setStartDateTime($record['start_datetime']);
      $this->setPlayoffGameYN($record['playoff_game_yn']);
      $this->setHomeTeamName($record['home_team_name']);
      $this->setVisitingTeamName($record['visiting_team_name']);
      $this->setLocationName($record['location_name']);
      $this->setSavedDateTimeAndUser($record['saved_datetime'], $record['saved_userid']);
      $this->setBoxScore($record['home_team_first_period_goals'],
                         $record['visiting_team_first_period_goals'],
                         $record['home_team_second_period_goals'],
                         $record['visiting_team_second_period_goals'],
                         $record['home_team_third_period_goals'],
                         $record['visiting_team_third_period_goals'],
                         $record['home_team_over_time_goals'],
                         $record['visiting_team_over_time_goals'],
                         $record['home_team_shots'],
                         $record['visiting_team_shots'],
                         $record['comment']);
    }
    else {
      throw new Exception('Game Not Found');
    }
  }

  public function save() {
    if ($this->getGameId() > 0) {
      $this->update();
    }
    else {
      $this->insert();
    }
  }

  private function insert() {

    db_insert('game')
      ->fields(array(
           'home_teamid' => $this->getHomeTeamId(),
           'visiting_teamid' => $this->getVisitingTeamId(),
           'locationid' => $this->getLocationId(),
           'seasonid' => $this->getSeasonId(),
           'start_datetime' => $this->getStartDateTime(),
           'playoff_game_yn' => $this->getPlayoffGameYN(),
           'saved_userid' => $this->getSavedUserID(),
           'saved_datetime' => $this->getSavedDateTime(),
           'home_team_first_period_goals' => $this->getHomeTeamFirstPeriodGoals(),
           'visiting_team_first_period_goals' => $this->getVisitingTeamFirstPeriodGoals(),
           'home_team_second_period_goals' => $this->getHomeTeamSecondPeriodGoals(),
           'visiting_team_second_period_goals' => $this->getVisitingTeamSecondPeriodGoals(),
           'home_team_third_period_goals' => $this->getHomeTeamThirdPeriodGoals(),
           'visiting_team_third_period_goals' => $this->getVisitingTeamThirdPeriodGoals(),
           'home_team_over_time_goals' => $this->getHomeTeamOverTimeGoals(),
           'visiting_team_over_time_goals' => $this->getVisitingTeamOverTimeGoals(),
           'home_team_shots' => $this->getHomeTeamShots(),
           'visiting_team_shots' => $this->getVisitingTeamShots(),
           'comment' => $this->getComment()
        )
      )
      ->execute();
  }

  private function update() {

    $this->history();

    db_update('game')
      ->fields(array(
           'home_teamid' => $this->getHomeTeamId(),
           'visiting_teamid' => $this->getVisitingTeamId(),
           'locationid' => $this->getLocationId(),
           'seasonid' => $this->getSeasonId(),
           'start_datetime' => $this->getStartDateTime(),
           'playoff_game_yn' => $this->getPlayoffGameYN(),
           'saved_userid' => $this->getSavedUserID(),
           'saved_datetime' => $this->getSavedDateTime(),
           'home_team_first_period_goals' => $this->getHomeTeamFirstPeriodGoals(),
           'visiting_team_first_period_goals' => $this->getVisitingTeamFirstPeriodGoals(),
           'home_team_second_period_goals' => $this->getHomeTeamSecondPeriodGoals(),
           'visiting_team_second_period_goals' => $this->getVisitingTeamSecondPeriodGoals(),
           'home_team_third_period_goals' => $this->getHomeTeamThirdPeriodGoals(),
           'visiting_team_third_period_goals' => $this->getVisitingTeamThirdPeriodGoals(),
           'home_team_over_time_goals' => $this->getHomeTeamOverTimeGoals(),
           'visiting_team_over_time_goals' => $this->getVisitingTeamOverTimeGoals(),
           'home_team_shots' => $this->getHomeTeamShots(),
           'visiting_team_shots' => $this->getVisitingTeamShots(),
           'comment' => $this->getComment()
        )
      )
      ->condition('gameid', $this->getGameId())
      ->execute();
  }

  private function history() {

    $current_game = new Game($this->getGameId());
    $current_game->build();

    db_insert('game_history')
      ->fields(array(
           'gameid' => $current_game->getGameId(),
           'home_teamid' => $current_game->getHomeTeamId(),
           'visiting_teamid' => $current_game->getVisitingTeamId(),
           'locationid' => $current_game->getLocationId(),
           'seasonid' => $current_game->getSeasonId(),
           'start_datetime' => $current_game->getStartDateTime(),
           'playoff_game_yn' => $current_game->getPlayoffGameYN(),
           'saved_userid' => $current_game->getSavedUserID(),
           'saved_datetime' => $current_game->getSavedDateTime(),
           'history_userid' => $this->getSavedUserID(),
           'history_datetime' => $this->getSavedDateTime(),
           'home_team_first_period_goals' => $current_game>getHomeTeamFirstPeriodGoals(),
           'visiting_team_first_period_goals' => $current_game->getVisitingTeamFirstPeriodGoals(),
           'home_team_second_period_goals' => $current_game->getHomeTeamSecondPeriodGoals(),
           'visiting_team_second_period_goals' => $current_game->getVisitingTeamSecondPeriodGoals(),
           'home_team_third_period_goals' => $current_game->getHomeTeamThirdPeriodGoals(),
           'visiting_team_third_period_goals' => $current_game->getVisitingTeamThirdPeriodGoals(),
           'home_team_over_time_goals' => $current_game->getHomeTeamOverTimeGoals(),
           'visiting_team_over_time_goals' => $current_game->getVisitingTeamOverTimeGoals(),
           'home_team_shots' => $current_game->getHomeTeamShots(),
           'visiting_team_shots' => $current_game->getVisitingTeamShots(),
           'comment' => $current_game->getComment()
        )
      )
      ->execute();
  }

  public function delete() {

    $this->history();

    db_delete('game')
      ->condition('gameid', $this->getGameId())
      ->execute();
  }

  public function equals(CrudInterface $comparison_obj) {

    $returnValue = FALSE;

    if ($comparison_obj instanceof Game) {
      if ($this->getHomeTeamId() == $comparison_obj->getHomeTeamId() 
        && $this->getVisitingTeamId() == $comparison_obj->getVisitingTeamId() 
        && $this->getLocationId() == $comparison_obj->getLocationId() 
        && $this->getSeasonId() == $comparison_obj->getSeasonId() 
        && $this->getStartDateTime() == $comparison_obj->getStartDateTime() 
        && $this->getPlayoffGameYN() == $comparison_obj->getPlayoffGameYN()
        && $this->getHomeTeamFirstPeriodGoals() == $comparison_obj->getHomeTeamFirstPeriodGoals()
        && $this->getVisitingTeamFirstPeriodGoals() == $comparison_obj->getVisitingTeamFirstPeriodGoals()
        && $this->getHomeTeamSecondPeriodGoals() == $comparison_obj->getHomeTeamSecondPeriodGoals()
        && $this->getVisitingTeamSecondPeriodGoals() == $comparison_obj->getVisitingTeamSecondPeriodGoals()
        && $this->getHomeTeamThirdPeriodGoals() == $comparison_obj->getHomeTeamThirdPeriodGoals()
        && $this->getVisitingTeamThirdPeriodGoals() == $comparison_obj->getVisitingTeamThirdPeriodGoals()
        && $this->getHomeTeamOverTimeGoals() == $comparison_obj->getHomeTeamOverTimeGoals()
        && $this->getVisitingTeamOverTimeGoals() == $comparison_obj->getVisitingTeamOverTimeGoals()
        && $this->getHomeTeamShots() == $comparison_obj->getHomeTeamShots()
        && $this->getVisitingTeamShots() == $comparison_obj->getVisitingTeamShots()
        && $this->getComment() == $comparison_obj->getComment()        
      ) {
        $returnValue = TRUE;
      }
    }

    return $returnValue;
  }

  public function getGameId() {
    return $this->gameId;
  }

  public function setGameId($gameId) {
    $this->gameId = $gameId;
  }

  public function getHomeTeamId() {
    return $this->homeTeamId;
  }

  public function setHomeTeamId($homeTeamId) {
    $this->homeTeamId = $homeTeamId;
  }

  public function getVisitingTeamId() {
    return $this->visitingTeamId;
  }

  public function setVisitingTeamId($visitingTeamId) {
    $this->visitingTeamId = $visitingTeamId;
  }

  public function getLocationId() {
    return $this->locationId;
  }

  public function setLocationId($locationId) {
    $this->locationId = $locationId;
  }

  public function getSeasonId() {
    return $this->seasonId;
  }

  public function setSeasonId($seasonId) {
    $this->seasonId = $seasonId;
  }

  public function getStartDateTime() {
    return $this->startDateTime;
  }

  public function setStartDateTime($startDateTime) {
    $this->startDateTime = $startDateTime;
  }

  public function getPlayoffGameYN() {
    return $this->playoffGameYN;
  }

  public function setPlayoffGameYN($playoffGameYN) {
    $this->playoffGameYN = $playoffGameYN;
  }

  public function getSavedUserID() {
    return $this->savedUserID;
  }

  public function setSavedUserID($savedUserID) {
    $this->savedUserID = $savedUserID;
  }

  public function getSavedDateTime() {
    return $this->savedDateTime;
  }

  public function setSavedDateTime($savedDateTime) {
    $this->savedDateTime = $savedDateTime;
  }

  public function getLocationName() {
    return $this->locationName;
  }

  public function setLocationName($locationName) {
    $this->locationName = $locationName;
  }

  public function getHomeTeamName() {
    return $this->homeTeamName;
  }
  
  public function setHomeTeamName($homeTeamName) {
    $this->homeTeamName = $homeTeamName;
  }

  public function getVisitingTeamName() {
    return $this->visitingTeamName;
  }

  public function setVisitingTeamName($visitingTeamName) {
    $this->visitingTeamName = $visitingTeamName;
  }
  
  public function getHomeTeamFirstPeriodGoals() {
    return $this->homeTeamFirstPeriodGoals;
  }

  public function setHomeTeamFirstPeriodGoals($homeTeamFirstPeriodGoals) {
    $this->homeTeamFirstPeriodGoals = $homeTeamFirstPeriodGoals == '' ? NULL : $homeTeamFirstPeriodGoals;
  }

  public function getVisitingTeamFirstPeriodGoals() {
    return $this->visitingTeamFirstPeriodGoals;
  }

  public function setVisitingTeamFirstPeriodGoals($visitingTeamFirstPeriodGoals) {
    $this->visitingTeamFirstPeriodGoals = $visitingTeamFirstPeriodGoals == '' ? NULL : $visitingTeamFirstPeriodGoals;
  }

  public function getHomeTeamSecondPeriodGoals() {
    return $this->homeTeamSecondPeriodGoals;
  }
 
  public function setHomeTeamSecondPeriodGoals($homeTeamSecondPeriodGoals) {
    $this->homeTeamSecondPeriodGoals = $homeTeamSecondPeriodGoals == '' ? NULL : $homeTeamSecondPeriodGoals;
  }

  public function getVisitingTeamSecondPeriodGoals() {
    return $this->visitingTeamSecondPeriodGoals;
  }

  public function setVisitingTeamSecondPeriodGoals($visitingTeamSecondPeriodGoals) {
    $this->visitingTeamSecondPeriodGoals = $visitingTeamSecondPeriodGoals == '' ? NULL : $visitingTeamSecondPeriodGoals;
  }

  public function getHomeTeamThirdPeriodGoals() {
    return $this->homeTeamThirdPeriodGoals;
  }

  public function setHomeTeamThirdPeriodGoals($homeTeamThirdPeriodGoals) {
    $this->homeTeamThirdPeriodGoals = $homeTeamThirdPeriodGoals == '' ? NULL : $homeTeamThirdPeriodGoals;
  }
 
  public function getVisitingTeamThirdPeriodGoals() {
    return $this->visitingTeamThirdPeriodGoals;
  }
 
  public function setVisitingTeamThirdPeriodGoals($visitingTeamThirdPeriodGoals) {
    $this->visitingTeamThirdPeriodGoals = $visitingTeamThirdPeriodGoals == '' ? NULL : $visitingTeamThirdPeriodGoals;
  }
 
  public function getHomeTeamOverTimeGoals() {
    return $this->homeTeamOverTimeGoals;
  }

  public function setHomeTeamOverTimeGoals($homeTeamOverTimeGoals) {
    $this->homeTeamOverTimeGoals = $homeTeamOverTimeGoals == '' ? NULL : $homeTeamOverTimeGoals;
  }

  public function getVisitingTeamOverTimeGoals() {
    return $this->visitingTeamOverTimeGoals;
  }
 
  public function setVisitingTeamOverTimeGoals($visitingTeamOverTimeGoals) {
    $this->visitingTeamOverTimeGoals = $visitingTeamOverTimeGoals == '' ? NULL : $visitingTeamOverTimeGoals;
  }

  public function getHomeTeamShots() {
    return $this->homeTeamShots;
  }

  public function setHomeTeamShots($homeTeamShots) {
    $this->homeTeamShots = $homeTeamShots == '' ? NULL : $homeTeamShots;
  }

  public function getVisitingTeamShots() {
    return $this->visitingTeamShots;
  }

  public function setVisitingTeamShots($visitingTeamShots) {
    $this->visitingTeamShots = $visitingTeamShots == '' ? NULL : $visitingTeamShots;
  }
 
  public function getComment() {
    return $this->comment;
  }

  public function setComment($comment) {
    $this->comment = $comment;
  }
  
  public function getPenalties() {
    if($this->penalties == NULL) {
      $this->setPenalties($this->getAllPenalties());
    }    
    return $this->penalties;
  }

  public function setPenalties($penalties) {
    $this->penalties = $penalties;
  }
  
  private function getAllPenalties() {
    $penalties = array();
    
    $query = db_select('game_penalty', 'gp');
    $query->innerJoin('season_team_player', 'stp', 'stp.season_team_playerid = gp.season_team_playerid');
    $query->innerJoin('season_team', 'st', 'st.season_teamid = stp.season_teamid');    
    $query->innerJoin('team', 't', 'st.teamid = t.teamid');
    $query->innerJoin('player', 'p', 'p.playerid = stp.playerid');
    $query->innerJoin('penalty_duration', 'gpd', 'gpd.penalty_durationid = gp.penalty_durationid');
    $query->innerJoin('penalty', 'pen', 'pen.penaltyid = gp.penaltyid');
    $query->fields('gp');
    $query->addField('p', 'first_name');
    $query->addField('p', 'last_name');
    $query->addField('t', 'name', 'teamName');
    $query->addField('gpd', 'name', 'penaltyDuration');
    $query->addField('pen', 'name', 'penaltyName');
    $query->condition('gameid', $this->getGameId());
    $query->orderBy('gp.period', 'DESC');
    $query->orderBy('gp.time', 'DESC');
    $result = $query->execute();
    
    while ($record = $result->fetchAssoc()) {
      $penalties[$record['game_penaltyid']] = new GamePenalty($record['game_penaltyid'], 
        $record['gameid'], $record['period'], $record['time'], 
        $record['season_team_playerid'], $record['penaltyid'],
        $record['penalty_durationid'], $record['first_name'], $record['last_name'], 
        $record['penaltyName'], $record['penaltyDuration'], $record['teamName']);
    }
    
    return $penalties;
  }

  public function getHomeTeamPlayerIds() {
    
    if($this->getHomeTeamPlayers() == NULL) {
      $this->loadHomePlayers();
    }
    
    $gamePlayers = $this->getHomeTeamPlayers();
    
    return $this->getPlayerIdList($gamePlayers);
  }  

  public function getVisitingTeamPlayerIds() {
    
    if($this->getVisitingTeamPlayers() == NULL) {
      $this->loadVisitingPlayers();
    }
    
    $gamePlayers = $this->getVisitingTeamPlayers();
    
    return $this->getPlayerIdList($gamePlayers);
  }
  
  private function getPlayerIdList($gamePlayers) {
    $playerIds = array();
    foreach ($gamePlayers as $gamePlayer) {
      array_push($playerIds, $gamePlayer->getPlayerID());
    }
      
    return $playerIds;
  }

  public function getHomeTeamGoalie() {
    if($this->homeTeamGoalie == NULL) {
      $gamePlayers = $this->getHomeTeamPlayers();
      $this->setHomeTeamGoalie($this->findGoalie($gamePlayers));
    }

    return $this->homeTeamGoalie;
  }
  
  public function setHomeTeamGoalie($homeTeamGoalie) {
    $this->homeTeamGoalie = $homeTeamGoalie;
  }

  public function getVisitingTeamGoalie() {
    if($this->visitingTeamGoalie == NULL) {
      $gamePlayers = $this->getVisitingTeamPlayers();
      $this->setVisitingTeamGoalie($this->findGoalie($gamePlayers));
    }
    return $this->visitingTeamGoalie;
  }

  public function setVisitingTeamGoalie($visitingTeamGoalie) {
    $this->visitingTeamGoalie = $visitingTeamGoalie;
  }

  private function findGoalie($gamePlayers) {
    $goalie = NULL;
    
    foreach($gamePlayers as $gamePlayer) {
      if($gamePlayer->getGoalieYN()) {
        $goalie = $gamePlayer;
        BREAK;
      }
    }
    
    return $goalie;
  }
  
  public function getHomeTeamPlayers() {
    if($this->homeTeamPlayers == NULL) {
      $this->loadHomePlayers();
    }    
    return $this->homeTeamPlayers;
  }

  public function setHomeTeamPlayers($homeTeamPlayers) {
    $this->homeTeamPlayers = $homeTeamPlayers;
  }

  public function getVisitingTeamPlayers() {
    if($this->visitingTeamPlayers == NULL) {
      $this->loadVisitingPlayers();
    }
    return $this->visitingTeamPlayers;
  }

  public function setVisitingPlayers($visitingTeamPlayers) {
    $this->visitingTeamPlayers = $visitingTeamPlayers;
  }
  
  private function loadVisitingPlayers() {
    $visitingPlayers = $this->loadPlayers($this->getVisitingTeamId());
    $this->setVisitingPlayers($visitingPlayers);
  }
  
  private function loadHomePlayers() {
    $homePlayers = $this->loadPlayers($this->getHomeTeamId());
    $this->setHomeTeamPlayers($homePlayers);
  }
  
  private function loadPlayers($teamid) {
    $players = array();
    
    $query = db_select('game_player', 'gp');
    $query->fields('gp');
    $query->condition('gameid', $this->getGameId());
    $query->condition('teamid', $teamid);
    $result = $query->execute();
    
    while ($record = $result->fetchAssoc()) {
      $players[$record['game_playerid']] = 
        new GamePlayer($this->getGameId(), $teamid, $record['playerid'], 
          $record['goalie_yn'], $record['game_playerid']);
    }
    
    return $players;
  }

  public function getGameDescription() {
    $value = NULL;
    
    if (isset($this->visitingTeamName) && isset($this->homeTeamName)) {
      $value = $this->visitingTeamName . ' at ' . $this->homeTeamName;
    }
    
    return $value;
  }
  
  public function getAllPlayersList() {
    $homeTeam = new SeasonTeam(NULL, $this->getSeasonId(), $this->getHomeTeamId());
    $homeTeam->build();
    $homeTeamPlayers = array($this->getHomeTeamName() => $homeTeam->getPlayerList());
    
    $visitingTeam = new SeasonTeam(NULL, $this->getSeasonId(), $this->getVisitingTeamId());
    $visitingTeam->build();
    $visitingTeamPlayers = array($this->getVisitingTeamName() => $visitingTeam->getPlayerList());
    
    return $visitingTeamPlayers + $homeTeamPlayers;
  }
  
}
