<?php
class GamePlayer  {
  
  private $gameId;
  private $teamId;
  private $playerId;
  private $gamePlayerId;
  private $goalieYN;
  private $savedDateTime = NULL;
  private $savedUserID = 0;
  
  public function __construct($gameId, $teamId, $playerId, $goalieYN = 0, $gamePlayerId = NULL) {
    $this->setGameId($gameId);
    $this->setTeamId($teamId);
    $this->setPlayerId($playerId);
    $this->setGoalieYN($goalieYN);
    $this->setGamePlayerId($gamePlayerId);
  }
  
  public function setSavedDateTimeAndUser($savedDateTime, $savedUserID) {
    $this->setSavedUserID($savedUserID);
    $this->setSavedDateTime($savedDateTime);
  }
  
  // needed for the history
  public function build() {

    $result = db_select('game_player', 'gp')
      ->fields('gp')
      ->condition('gameid', $this->getGameId())
      ->condition('teamid', $this->getTeamId())
      ->condition('playerid', $this->getPlayerId())
      ->execute();

    $record = $result->fetchAssoc();

    if (isset($record)) {
      $this->setGamePlayerId($record['game_playerid']);
      $this->setGoalieYN($record['goalie_yn']);
      $this->setSavedDateTimeAndUser($record['saved_datetime'], $record['saved_userid']);
    }
    else {
      throw new Exception('Game Player Not Found');
    }
  }  
  public function save() {
    if ($this->getGamePlayerId() > 0) {
      $this->update();
    }
    else {
      $this->insert();
    }
  }
  
  private function insert() {

    db_insert('game_player')
      ->fields(array(
           'gameid' => $this->getGameId(),
           'teamid' => $this->getTeamId(),
           'playerid' => $this->getPlayerId(),
           'goalie_yn' => $this->getGoalieYN(),
           'saved_userid' => $this->getSavedUserID(),
           'saved_datetime' => $this->getSavedDateTime(),
        )
      )
      ->execute();
  }
  
  private function update() {
    $this->history();

    db_update('game_player')
      ->fields(array(
           'gameid' => $this->getGameId(),
           'teamid' => $this->getTeamId(),
           'playerid' => $this->getPlayerId(),
           'goalie_yn' => $this->getGoalieYN(),
           'saved_userid' => $this->getSavedUserID(),
           'saved_datetime' => $this->getSavedDateTime(),
        )
      )
      ->condition('game_playerid', $this->getGamePlayerId())
      ->execute();
  }
  
  private function history() {

    $current = new GamePlayer($this->getGameId(), $this->getTeamId(), $this->getPlayerId(), $this->getGoalieYN(), $this->getGamePlayerId());
    $current->build();

    db_insert('game_player_history')
      ->fields(array(
           'game_playerid' => $current->getGamePlayerId(),
           'gameid' => $current->getGameId(),
           'playerid' => $current->getPlayerId(),
           'teamid' => $current->getTeamId(),
           'goalie_yn' => $current->getGoalieYN(),
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

    db_delete('game_player')
      ->condition('gameid', $this->getGameId())
      ->condition('teamid', $this->getTeamId())
      ->condition('playerid', $this->getPlayerId())
      ->execute();
  }
  
  public function getGameId() {
    return $this->gameId;
  }

  public function setGameId($gameId) {
    $this->gameId = $gameId;
  }

  public function getTeamId() {
    return $this->teamId;
  }

  public function setTeamId($teamId) {
    $this->teamId = $teamId;
  }

  public function getPlayerId() {
    return $this->playerId;
  }

  public function setPlayerId($playerId) {
    $this->playerId = $playerId;
  }
  
  public function getGamePlayerId() {
    return $this->gamePlayerId;
  }

  public function setGamePlayerId($gamePlayerId) {
    $this->gamePlayerId = $gamePlayerId;
  }  
  
  public function getGoalieYN() {
    return $this->goalieYN;
  }

  public function setGoalieYN($goalieYN) {
    $this->goalieYN = $goalieYN;
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
