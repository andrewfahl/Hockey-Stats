<?php

class DemoEntity implements CrudInterface {

  private $savedDateTime = NULL;
  private $savedUserID = 0;
  
  public function setSavedDateTimeAndUser($savedDateTime, $savedUserID) {
    $this->setSavedUserID($savedUserID);
    $this->setSavedDateTime($savedDateTime);
  }
  
  public function build() {
    /*
    $result = db_select('game', 'g')
      ->fields('g')
      ->condition('gameid', $this->getGameId())
      ->execute();

    $record = $result->fetchAssoc();

    if (isset($record)) {
      $this->setGameId($record['gameid']);
      $this->setHomeTeamId($record['home_teamid']);
      $this->setVisitingTeamId($record['visiting_teamid']);
      $this->setLocationId($record['locationid']);
      $this->setSeasonId($record['seasonid']);
      $this->setStartDateTime($record['start_datetime']);
      $this->setPlayoffGameYN($record['playoff_game_yn']);
      $this->setSavedDateTimeAndUser($record['saved_datetime'], $record['saved_userid']);
    }
    else {
      throw new Exception('Game Not Found');
    }
     */
  }

  public function save() {
    /*
    if ($this->getEntityId() > 0) {
      $this->update();
    }
    else {
      $this->insert();
    }
     */
  }

  private function insert() {
  /*
    db_insert('location')
      ->fields(array(
           'name' => $this->getName(),
           'address' => $this->getAddress(),
           'address2' => $this->getAddress2(),
           'city' => $this->getCity(),
           'state' => $this->getState(),
           'zip' => $this->getZip(),
           'saved_userid' => $this->getSavedUserID(),
           'saved_datetime' => $this->getSavedDateTime(),
        )
      )
      ->execute();
   */
  }
  
  private function update() {
    /*
    $this->history();

    db_update('location')
      ->fields(array(
           'name' => $this->getName(),
           'address' => $this->getAddress(),
           'address2' => $this->getAddress2(),
           'city' => $this->getCity(),
           'state' => $this->getState(),
           'zip' => $this->getZip(),
           'saved_userid' => $this->getSavedUserID(),
           'saved_datetime' => $this->getSavedDateTime(),
        )
      )
      ->condition('locationid', $this->getLocationId())
      ->execute();
     */
  }
  
  private function history() {
    /*
    $current = new Location($this->getLocationId());
    $current->build();

    db_insert('location_history')
      ->fields(array(
           'locationid' => $current_location->getLocationId(),
           'name' => $current_location->getName(),
           'address' => $current_location->getAddress(),
           'address2' => $current_location->getAddress2(),
           'city' => $current_location->getCity(),
           'state' => $current_location->getState(),
           'zip' => $current_location->getZip(),
           'saved_userid' => $current->getSavedUserID(),  #this is the current value
           'saved_datetime' => $current->getSavedDateTime(),  #this is the current value
           'history_userid' => $this->getSavedUserID(),
           'history_datetime' => $this->getSavedDateTime(),
        )
      )
      ->execute();
    */
  }
  
  public function delete() {
    /*
    $this->history();

    db_delete('location')
      ->condition('locationid', $this->getLocationId())
      ->execute();
    */
  }

  public function equals(CrudInterface $comparison_obj) {
    /*
    $returnValue = FALSE;

    if ($comparison_obj instanceof Location) {
      if ($this->getName() == $comparison_obj->getName() && $this->getAddress() == $comparison_obj->getAddress() && $this->getAddress2() == $comparison_obj->getAddress2() && $this->getCity() == $comparison_obj->getCity() && $this->getState() == $comparison_obj->getState() && $this->getZip() == $comparison_obj->getZip()
      ) {
        $returnValue = TRUE;
      }
    }

    return $returnValue;
    */
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
