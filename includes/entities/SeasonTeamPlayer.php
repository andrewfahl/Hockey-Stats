<?php
class SeasonTeamPlayer implements CrudInterface {
  
  private $seasonTeamPlayerId;
  private $seasonTeamId;
  private $number;
  private $playerId;
  private $playerFirstName;
  private $playerLastName;
  private $savedUserID;
  private $savedDateTime;

  function __construct($seasonTeamPlayerId, $seasonTeamId = NULL, $playerId = NULL, $number = NULL, $playerFirstName = NULL, $playerLastName = NULL) {
    $this->setSeasonTeamPlayerId($seasonTeamPlayerId);
    $this->setSeasonTeamId($seasonTeamId);
    $this->setNumber($number);
    $this->setPlayerId($playerId);
    $this->setPlayerFirstName($playerFirstName);
    $this->setPlayerLastName($playerLastName);
  }

  public function setSavedDateTimeAndUser($savedDateTime, $savedUserID) {
    $this->setSavedUserID($savedUserID);
    $this->setSavedDateTime($savedDateTime);
  }
  
  public function build() {

    $query = db_select('season_team_player', 'stp');
    $query->innerJoin('player', 'p', 'stp.playerid = p.playerid');
    $query->fields('stp');
    $query->addField('p', 'first_name');
    $query->addField('p', 'last_name');
    $query->condition('season_team_playerid', $this->getSeasonTeamPlayerId());
    $result = $query->execute();

    $record = $result->fetchAssoc();

    if (isset($record)) {
      $this->setSeasonTeamPlayerId($record['season_team_playerid']);
      $this->setSeasonTeamId($record['season_teamid']);
      $this->setNumber($record['number']);
      $this->setPlayerId($record['playerid']);
      $this->setPlayerFirstName($record['first_name']);
      $this->setPlayerLastName($record['last_name']);

      $this->setSavedDateTimeAndUser($record['saved_datetime'], $record['saved_userid']);
    }
    else {
      throw new Exception('Season Team Player Not Found');
    }
  }

  public function save() {
    
    if ($this->getSeasonTeamPlayerId() > 0) {
      $this->update();
    }
    else {
      $this->insert();
    } 
  }

  private function insert() {

    db_insert('season_team_player')
      ->fields(array(
         'season_teamid' => $this->getSeasonTeamId(),
         'playerid' => $this->getPlayerId(),
         'number' => $this->getNumber(),
         'saved_userid' => $this->getSavedUserID(),
         'saved_datetime' => $this->getSavedDateTime(),
        )
      )
      ->execute();
   
  }
  
  private function update() {

    $this->history();

    db_update('season_team_player')
      ->fields(array(
         'season_teamid' => $this->getSeasonTeamId(),
         'playerid' => $this->getPlayerId(),
         'number' => $this->getNumber(),
         'saved_userid' => $this->getSavedUserID(),
         'saved_datetime' => $this->getSavedDateTime(),
        )
      )
      ->condition('season_team_playerid', $this->getSeasonTeamPlayerId())
      ->execute();

  }
  
  private function history() {
    
    $current = new SeasonTeamPlayer($this->getSeasonTeamPlayerId());
    $current->build();

    db_insert('season_team_player_history')
      ->fields(array(
           'season_team_playerid' => $current->getSeasonTeamPlayerId(),
           'season_teamid' => $current->getSeasonTeamId(),
           'playerid' => $current->getPlayerId(),
           'number' => $current->getNumber(),
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

    db_delete('season_team_player')
      ->condition('season_team_playerid', $this->getSeasonTeamPlayerId())
      ->execute();
  }

  public function equals(CrudInterface $comparison_obj) {
    
    $returnValue = FALSE;

    if ($comparison_obj instanceof SeasonTeamPlayer) {
      if ($this->getSeasonTeamId() == $comparison_obj->getSeasonTeamId() 
        && $this->getPlayerId() == $comparison_obj->getPlayerId() 
        && $this->getNumber() == $comparison_obj->getNumber() 
      ) {
        $returnValue = TRUE;
      }
    }

    return $returnValue;
    
  }
  
  public function getFullName() {
    return $this->getPlayerLastName() . ', ' . $this->getPlayerFirstName();
  }
  
  public function getSeasonTeamPlayerId() {
    return $this->seasonTeamPlayerId;
  }

  public function setSeasonTeamPlayerId($seasonTeamPlayerId) {
    $this->seasonTeamPlayerId = $seasonTeamPlayerId;
  }

  public function getSeasonTeamId() {
    return $this->seasonTeamId;
  }
 
  public function setSeasonTeamId($seasonTeamId) {
    $this->seasonTeamId = $seasonTeamId;
  }

  public function getNumber() {
    return $this->number;
  }

  public function setNumber($number) {
    $this->number = $number;
  }

  public function getPlayerId() {
    return $this->playerId;
  }

  public function setPlayerId($playerId) {
    $this->playerId = $playerId;
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