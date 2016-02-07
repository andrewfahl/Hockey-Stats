<?php

class Location implements CrudInterface {

  private $locationId = 0;
  private $name = NULL;
  private $address = NULL;
  private $address2 = NULL;
  private $city = NULL;
  private $state = NULL;
  private $zip = NULL;
  private $savedDateTime = NULL;
  private $savedUserID = 0;

  function __construct($locationId = 0, $name = NULL, $address = NULL, $address2 = NULL, $city = NULL, $state = NULL, $zip = NULL) {
    $this->setLocationId($locationId);
    $this->setName($name);
    $this->setAddress($address);
    $this->setAddress2($address2);
    $this->setCity($city);
    $this->setState($state);
    $this->setZip($zip);
  }

  public function setSavedDateTimeAndUser($savedDateTime, $savedUserID) {
    $this->setSavedUserID($savedUserID);
    $this->setSavedDateTime($savedDateTime);
  }

  public function build() {
    $result = db_select('location', 'l')
      ->fields('l')
      ->condition('locationid', $this->getLocationId())
      ->execute();

    $record = $result->fetchAssoc();

    if (isset($record)) {
      $this->setLocationId($record['locationid']);
      $this->setName($record['name']);
      $this->setAddress($record['address']);
      $this->setAddress2($record['address2']);
      $this->setCity($record['city']);
      $this->setState($record['state']);
      $this->setZip($record['zip']);
      $this->setSavedDateTimeAndUser($record['saved_datetime'], $record['saved_userid']);
    }
    else {
      throw new Exception('Location Not Found');
    }
  }

  public function save() {
    if ($this->getLocationId() > 0) {
      $this->update();
    }
    else {
      $this->insert();
    }
  }

  private function update() {

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
  }

  private function insert() {
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
  }

  public function delete() {
    $this->history();

    db_delete('location')
      ->condition('locationid', $this->getLocationId())
      ->execute();
  }

  public function equals(CrudInterface $comparison_obj) {
    $returnValue = FALSE;

    if ($comparison_obj instanceof Location) {
      if ($this->getName() == $comparison_obj->getName() && $this->getAddress() == $comparison_obj->getAddress() && $this->getAddress2() == $comparison_obj->getAddress2() && $this->getCity() == $comparison_obj->getCity() && $this->getState() == $comparison_obj->getState() && $this->getZip() == $comparison_obj->getZip()
      ) {
        $returnValue = TRUE;
      }
    }

    return $returnValue;
  }

  private function history() {

    $current_location = new Location($this->getLocationId());
    $current_location->build();

    db_insert('location_history')
      ->fields(array(
           'locationid' => $current_location->getLocationId(),
           'name' => $current_location->getName(),
           'address' => $current_location->getAddress(),
           'address2' => $current_location->getAddress2(),
           'city' => $current_location->getCity(),
           'state' => $current_location->getState(),
           'zip' => $current_location->getZip(),
           'saved_userid' => $current_location->getSavedUserID(),
           'saved_datetime' => $current_location->getSavedDateTime(),
           'history_userid' => $this->getSavedUserID(),
           'history_datetime' => $this->getSavedDateTime(),
        )
      )
      ->execute();
  }
  
  public function getLocationId() {
    return $this->locationId;
  }

  public function setLocationId($locationId) {
    $this->locationId = $locationId;
  }

  public function getName() {
    return $this->name;
  }

  public function setName($name) {
    $this->name = $name;
  }

  public function getAddress() {
    return $this->address;
  }

  public function setAddress($address) {
    $this->address = $address;
  }

  public function getAddress2() {
    return $this->address2;
  }

  public function setAddress2($address2) {
    $this->address2 = $address2;
  }

  public function getCity() {
    return $this->city;
  }

  public function setCity($city) {
    $this->city = $city;
  }

  public function getState() {
    return $this->state;
  }

  public function setState($state) {
    $this->state = $state;
  }

  public function getZip() {
    return $this->zip;
  }

  public function setZip($zip) {
    $this->zip = $zip;
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