<?php

class Player implements CrudInterface {

  private $playerId;
  private $firstName;
  private $lastName;
  private $playerPositionId;
  private $playerPositionName;
  private $playerPositionAbbreviation;
  private $secondaryPlayerPositionId;
  private $secondaryPlayerPositionName;
  private $secondaryPlayerPositionAbbreviation;
  private $activeYN = TRUE;
  private $savedDateTime;
  private $savedUserID;

  function __construct($playerId = 0, $firstName = NULL, $lastName = NULL, 
      $playerPositionId = NULL, $secondaryPlayerPositionId = NULL, $activeYN = TRUE, 
      $playerPositionName = NULL, $playerPositionAbbreviation = NULL,
      $secondaryPlayerPositionName = NULL, $secondaryPlayerPositionAbbreviation = NULL) {
    $this->setPlayerId($playerId);
    $this->setFirstName($firstName);
    $this->setLastName($lastName);
    $this->setPlayerPositionId($playerPositionId);
    $this->setPlayerPositionName($playerPositionName);
    $this->setPlayerPositionAbbreviation($playerPositionAbbreviation);
    $this->setSecondaryPlayerPositionId($secondaryPlayerPositionId);
    $this->setSecondaryPlayerPositionName($secondaryPlayerPositionName);
    $this->setSecondaryPlayerPositionAbbreviation($secondaryPlayerPositionAbbreviation);
    $this->setActiveYN($activeYN);
  }

  public function setSavedDateTimeAndUser($savedDateTime, $savedUserID) {
    $this->setSavedUserID($savedUserID);
    $this->setSavedDateTime($savedDateTime);
  }

  public function build() {

    $query = db_select('player', 'p');
    $query->innerJoin('player_position', 'pp', 'pp.player_positionid = p.player_positionid');
    $query->innerJoin('player_position', 'sp', 'sp.player_positionid = p.secondary_player_positionid');
    $query->fields('p');
    $query->addField('pp', 'name', 'player_position_name');
    $query->addField('pp', 'abbreviation', 'player_position_abbreviation');
    $query->addField('sp', 'name', 'secondary_player_position_name');
    $query->addField('sp', 'abbreviation', 'secondary_player_position_abbreviation');
    $query->condition('playerid', $this->getPlayerId());
    $result = $query->execute();

    $record = $result->fetchAssoc();
        
    if (isset($record)) {
      $this->setPlayerId($record['playerid']);
      $this->setFirstName($record['first_name']);
      $this->setLastName($record['last_name']);
      $this->setPlayerPositionId($record['player_positionid']);
      $this->setPlayerPositionName($record['player_position_name']);
      $this->setPlayerPositionAbbreviation($record['player_position_abbreviation']);
      $this->setSecondaryPlayerPositionId($record['secondary_player_positionid']);
      $this->setSecondaryPlayerPositionName($record['secondary_player_position_name']);
      $this->setSecondaryPlayerPositionAbbreviation($record['secondary_player_position_abbreviation']);
      $this->setActiveYN($record['active_yn']);
      $this->setSavedDateTimeAndUser($record['saved_datetime'], $record['saved_userid']);
    }
    else {
      throw new Exception('Player Not Found');
    }
  }

  public function save() {
    if ($this->getPlayerId() > 0) {
      $this->update();
    }
    else {
      $this->insert();
    }
  }

  private function insert() {
    db_insert('player')
      ->fields(array(
           'first_name' => $this->getFirstName(),
           'last_name' => $this->getLastName(),
           'player_positionid' => $this->getPlayerPositionId(),
           'secondary_player_positionid' => $this->getSecondaryPlayerPositionId(),
           'active_yn' => $this->getActiveYN(),
           'saved_userid' => $this->getSavedUserID(),
           'saved_datetime' => $this->getSavedDateTime(),
        )
      )
      ->execute();
  }

  private function update() {

    $this->history();

    db_update('player')
      ->fields(array(
           'first_name' => $this->getFirstName(),
           'last_name' => $this->getLastName(),
           'player_positionid' => $this->getPlayerPositionId(),
           'secondary_player_positionid' => $this->getSecondaryPlayerPositionId(),
           'active_yn' => $this->getActiveYN(),
           'saved_userid' => $this->getSavedUserID(),
           'saved_datetime' => $this->getSavedDateTime(),
        )
      )
      ->condition('playerid', $this->getPlayerId())
      ->execute();
  }

  private function history() {

    $current_player = new Player($this->getPlayerId());
    $current_player->build();

    db_insert('player_history')
      ->fields(array(
           'playerid' => $current_player->getPlayerId(),
           'first_name' => $current_player->getFirstName(),
           'last_name' => $current_player->getLastName(),
           'player_positionid' => $current_player->getPlayerPositionId(),
           'secondary_player_positionid' => $current_player->getSecondaryPlayerPositionId(),
           'active_yn' => $current_player->getActiveYN(),
           'saved_userid' => $current_player->getSavedUserID(),
           'saved_datetime' => $current_player->getSavedDateTime(),
           'history_userid' => $this->getSavedUserID(),
           'history_datetime' => $this->getSavedDateTime(),
        )
      )
      ->execute();
  }

  public function delete() {

    $this->history();

    db_delete('player')
      ->condition('playerid', $this->getPlayerId())
      ->execute();
  }

  public function equals(CrudInterface $comparison_obj) {

    $returnValue = FALSE;

    if ($comparison_obj instanceof Player) {
      if (strtolower($this->getFirstName()) == strtolower($comparison_obj->getFirstName()) 
        && $this->getLastName() == $comparison_obj->getLastName() 
        && $this->getPlayerPositionId() == $comparison_obj->getPlayerPositionId() 
        && $this->getSecondaryPlayerPositionId() == $comparison_obj->getSecondaryPlayerPositionId() 
        && $this->getActiveYN() == $comparison_obj->getActiveYN()) {
        $returnValue = TRUE;
      }
    }

    return $returnValue;
  }
  
  public function getFullName() {
    return $this->getLastName() . ', ' . $this->getFirstName();
  }

  public function getPlayerId() {
    return $this->playerId;
  }

  public function setPlayerId($playerId) {
    $this->playerId = $playerId;
  }

  public function getFirstName() {
    return $this->firstName;
  }

  public function setFirstName($firstName) {
    $this->firstName = $firstName;
  }

  public function getLastName() {
    return $this->lastName;
  }

  public function setLastName($lastName) {
    $this->lastName = $lastName;
  }

  public function getPlayerPositionId() {
    return $this->playerPositionId;
  }

  public function setPlayerPositionId($playerPositionId) {
    $this->playerPositionId = $playerPositionId;
  }

  public function getPlayerPositionName() {
    return $this->playerPositionName;
  }
  
  public function setPlayerPositionName($playerPositionName) {
    $this->playerPositionName = $playerPositionName;
  }
  
  public function getPlayerPositionAbbreviation() {
    return $this->playerPositionAbbreviation;
  }
  
  public function setPlayerPositionAbbreviation($playerPositionAbbreviation) {
    $this->playerPositionAbbreviation = $playerPositionAbbreviation;
  }

  public function getSecondaryPlayerPositionId() {
    return $this->secondaryPlayerPositionId;
  }

  public function setSecondaryPlayerPositionId($secondaryPlayerPositionId) {
    $this->secondaryPlayerPositionId = $secondaryPlayerPositionId;
  }

  public function getSecondaryPlayerPositionName() {
    return $this->secondaryPlayerPositionName;
  }

  public function setSecondaryPlayerPositionName($secondaryPlayerPositionName) {
    $this->secondaryPlayerPositionName = $secondaryPlayerPositionName;
  }

  public function getSecondaryPlayerPositionAbbreviation() {
    return $this->secondaryPlayerPositionAbbreviation;
  }

  public function setSecondaryPlayerPositionAbbreviation($secondaryPlayerPositionAbbreviation) {
    $this->secondaryPlayerPositionAbbreviation = $secondaryPlayerPositionAbbreviation;
  }
  
  public function getActiveYN() {
    return $this->activeYN;
  }

  public function setActiveYN($activeYN) {
    $this->activeYN = $activeYN;
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
