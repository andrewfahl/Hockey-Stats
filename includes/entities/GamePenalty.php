<?php

class GamePenalty implements CrudInterface {

  private $gamePenaltyId;
  private $gameId;
  private $period;
  private $time;
  private $seasonTeamPlayerId;
  private $penaltyId;
  private $penaltyDurationId;
  private $savedDateTime = NULL;
  private $savedUserID = 0;
  
  function __construct($gamePenaltyId = 0, $gameId = 0, $period = 0, 
    $time = NULL, $seasonTeamPlayerId = 0, $penaltyId = 0, $penaltyDurationId = NULL) {

    $this->setGamePenaltyId($gamePenaltyId);
    $this->setGameId($gameId );
    $this->setPeriod($period);
    $this->setTime($time);
    $this->setSeasonTeamPlayerId($seasonTeamPlayerId);
    $this->setPenaltyId($penaltyId);
    $this->setPenaltyDurationId($penaltyDurationId);
  }
    
  public function setSavedDateTimeAndUser($savedDateTime, $savedUserID) {
    $this->setSavedUserID($savedUserID);
    $this->setSavedDateTime($savedDateTime);
  }
  
  public function build() {
    
    $result = db_select('game_penalty', 'gp')
      ->fields('gp')
      ->condition('game_penaltyid', $this->getGamePenaltyId())
      ->execute();

    $record = $result->fetchAssoc();

    if (isset($record)) {
      $this->setGamePenaltyId($record['game_penaltyid']);
      $this->setGameId($record['gameid']);
      $this->setPeriod($record['period']);
      $this->setTime($record['time']);
      $this->setSeasonTeamPlayerId($record['season_team_playerid']);
      $this->setPenaltyId($record['penaltyid']);
      $this->setPenaltyDurationId($record['penalty_durationid']);
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
           'period' => $this->getPeriod(),
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
           'period' => $this->getPeriod(),
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
           'gameid' => $this->getGameId(),
           'period' => $this->getPeriod(),
           'time' => $this->getTime(),
           'penaltyid' => $this->getPenaltyId(),
           'game_durationid' => $this->getPenaltyDurationId(),
           'season_team_playerid' => $this->getSeasonTeamPlayerId(),
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
        && $this->getPeriod() == $comparison_obj->getPeriod() 
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

  public function getPeriod() {
    return $this->period;
  }

  public function setPeriod($period) {
    $this->period = $period;
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
