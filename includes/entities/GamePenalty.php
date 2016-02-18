<?php

class GamePenalty implements CrudInterface {

  private $gamePenaltyId;
  private $gameId;
  private $periodId;
  private $periodName;
  private $time;
  private $seasonTeamPlayerId;
  private $playerFirstName;
  private $playerLastName;
  private $penaltyId;
  private $penaltyName;
  private $penaltyDurationId;
  private $penaltyDuration;
  private $teamName;
  private $savedDateTime = NULL;
  private $savedUserID = 0;
  
  function __construct($gamePenaltyId = 0, $gameId = 0, $periodId = 0, 
    $time = NULL, $seasonTeamPlayerId = 0, $penaltyId = 0, $penaltyDurationId = NULL,
    $playerFirstName = NULL, $playerLastName = NULL, $penaltyName = NULL, 
    $penaltyDuration = NULL, $teamName = NULL, $periodName = NULL) {

    $this->setGamePenaltyId($gamePenaltyId);
    $this->setGameId($gameId );
    $this->setPeriodId($periodId);
    $this->setTime($time);
    $this->setSeasonTeamPlayerId($seasonTeamPlayerId);
    $this->setPenaltyId($penaltyId);
    $this->setPenaltyDurationId($penaltyDurationId);
    $this->setPlayerFirstName($playerFirstName);
    $this->setPlayerLastName($playerLastName);
    $this->setPenaltyName($penaltyName);
    $this->setPenaltyDuration($penaltyDuration);
    $this->setTeamName($teamName);
    $this->setPeriodName($periodName);
  }
    
  public function setSavedDateTimeAndUser($savedDateTime, $savedUserID) {
    $this->setSavedUserID($savedUserID);
    $this->setSavedDateTime($savedDateTime);
  }
  
  public function build() {
    
    $query = db_select('game_penalty', 'gp');
    $query->innerJoin('season_team_player', 'stp', 'stp.season_team_playerid = gp.season_team_playerid');
    $query->innerJoin('season_team', 'st', 'st.season_teamid = stp.season_teamid');    
    $query->innerJoin('team', 't', 'st.teamid = t.teamid');
    $query->innerJoin('player', 'p', 'p.playerid = stp.playerid');
    $query->innerJoin('penalty_duration', 'gpd', 'gpd.penalty_durationid = gp.penalty_durationid');
    $query->innerJoin('penalty', 'pen', 'pen.penaltyid = gp.penaltyid');
    $query->innerJoin('period', 'per', 'per.periodid = gp.periodid');
    $query->fields('gp');
    $query->addField('p', 'first_name');
    $query->addField('p', 'last_name');
    $query->addField('t', 'name', 'teamName');
    $query->addField('gpd', 'name', 'penaltyDuration');
    $query->addField('pen', 'name', 'penaltyName');
    $query->addField('pen', 'name', 'penaltyName');
    $query->addField('per', 'name', 'periodName');
    $query->condition('game_penaltyid', $this->getGamePenaltyId());
    $result = $query->execute();
   
    $record = $result->fetchAssoc();

    if (isset($record)) {
      $this->setGamePenaltyId($record['game_penaltyid']);
      $this->setGameId($record['gameid']);
      $this->setPeriodId($record['periodid']);
      $this->setTime($record['time']);
      $this->setSeasonTeamPlayerId($record['season_team_playerid']);
      $this->setPenaltyId($record['penaltyid']);
      $this->setPenaltyDurationId($record['penalty_durationid']);
      $this->setPlayerFirstName($record['first_name']);
      $this->setPlayerLastName($record['last_name']);
      $this->setPenaltyName($record['penaltyName']);
      $this->setPenaltyDuration($record['penaltyDuration']);
      $this->setTeamName($record['teamName']);
      $this->setPeriodName($record['periodName']);
      $this->setSavedDateTimeAndUser($record['saved_datetime'], $record['saved_userid']);      
    }
    else {
      throw new Exception('Game Penalty Not Found');
    }
     
  }

  public function save() {
    if ($this->getGamePenaltyId() > 0) {
      $this->update();
    }
    else {
      $this->insert();
    }
  }

  private function insert() {

    db_insert('game_penalty')
      ->fields(array(
           'gameid' => $this->getGameId(),
           'periodid' => $this->getPeriodId(),
           'time' => $this->getTime(),
           'penaltyid' => $this->getPenaltyId(),
           'penalty_durationid' => $this->getPenaltyDurationId(),
           'season_team_playerid' => $this->getSeasonTeamPlayerId(),
           'saved_userid' => $this->getSavedUserID(),
           'saved_datetime' => $this->getSavedDateTime(),
        )
      )
      ->execute();
  }
  
  private function update() {
    
    $this->history();

    db_update('game_penalty')
      ->fields(array(
           'gameid' => $this->getGameId(),
           'periodid' => $this->getPeriodId(),
           'time' => $this->getTime(),
           'penaltyid' => $this->getPenaltyId(),
           'penalty_durationid' => $this->getPenaltyDurationId(),
           'season_team_playerid' => $this->getSeasonTeamPlayerId(),
           'saved_userid' => $this->getSavedUserID(),
           'saved_datetime' => $this->getSavedDateTime(),
        )
      )
      ->condition('game_penaltyid', $this->getGamePenaltyId())
      ->execute();
     
  }
  
  private function history() {
    
    $current = new GamePenalty($this->getGamePenaltyId());
    $current->build();

    db_insert('game_penalty_history')
      ->fields(array(
           'game_penaltyid' => $current->getGamePenaltyId(),
           'gameid' => $current->getGameId(),
           'periodid' => $current->getPeriodId(),
           'time' => $current->getTime(),
           'penaltyid' => $current->getPenaltyId(),
           'penalty_durationid' => $current->getPenaltyDurationId(),
           'season_team_playerid' => $current->getSeasonTeamPlayerId(),
           'saved_userid' => $current->getSavedUserID(),
           'saved_datetime' => $current->getSavedDateTime(),
           'history_userid' => $this->getSavedUserID(),
           'history_datetime' => $this->getSavedDateTime(),
        )
      )
      ->execute();
    
  }
  
  public function delete() {
    
    $this->history();

    db_delete('game_penalty')
      ->condition('game_penaltyid', $this->getGamePenaltyId())
      ->execute();
    
  }

  public function equals(CrudInterface $comparison_obj) {
    
    $returnValue = FALSE;

    if ($comparison_obj instanceof GamePenalty) {
      if ($this->getGameId() == $comparison_obj->getGameId() 
        && $this->getPeriodId() == $comparison_obj->getPeriodId() 
        && $this->getTime() == $comparison_obj->getTime() 
        && $this->getPenaltyId() == $comparison_obj->getPenaltyId()
        && $this->getPenaltyDurationId() == $comparison_obj->etPenaltyDurationId() 
        && $this->getSeasonTeamPlayerId() == $comparison_obj->getSeasonTeamPlayerId()
      ) {
        $returnValue = TRUE;
      }
    }

    return $returnValue;
  }

  public function getGamePenaltyId() {
    return $this->gamePenaltyId;
  }

  public function setGamePenaltyId($gamePenaltyId) {
    $this->gamePenaltyId = $gamePenaltyId;
  }

  public function getGameId() {
    return $this->gameId;
  }
 
  public function setGameId($gameId) {
    $this->gameId = $gameId;
  }

  public function getPeriodId() {
    return $this->periodId;
  }

  public function setPeriodId($periodId) {
    $this->periodId = $periodId;
  }

  public function getPeriodName() {
    return $this->periodName;
  }

  public function setPeriodName($periodName) {
    $this->periodName = $periodName;
  }
  
  public function getTime() {
    return $this->time;
  }

  public function setTime($time) {
    $this->time = $time;
  }
                 
  public function getSeasonTeamPlayerId() {
    return $this->seasonTeamPlayerId;
  }

  public function setSeasonTeamPlayerId($playerId) {
    $this->seasonTeamPlayerId = $playerId;
  }
 
  public function getPenaltyId() {
    return $this->penaltyId;
  }

  public function setPenaltyId($penaltyId) {
    $this->penaltyId = $penaltyId;
  }

  public function getPenaltyDurationId() {
    return $this->penaltyDurationId;
  }

  public function setPenaltyDurationId($penaltyDurationId) {
    $this->penaltyDurationId = $penaltyDurationId;
  }
  
  public function getPlayerFirstName() {
    return $this->playerFirstName;
  }

  public function setPlayerFirstName($playerFirstName) {
    $this->playerFirstName = $playerFirstName;
  }

  public function getPlayerLastName() {
    return $this->playerLastName;
  }

  public function setPlayerLastName($playerLastName) {
    $this->playerLastName = $playerLastName;
  }
  
  public function getPenaltyName() {
    return $this->penaltyName;
  }

  public function setPenaltyName($penaltyName) {
    $this->penaltyName = $penaltyName;
  }

  public function getPenaltyDuration() {
    return $this->penaltyDuration;
  }
  
  public function setPenaltyDuration($penaltyDuration) {
    $this->penaltyDuration = $penaltyDuration;
  }  
  
  public function getTeamName() {
    return $this->teamName;
  }
  
  public function setTeamName($teamName) {
    $this->teamName = $teamName;
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
}
