<?php

class PlayerPosition {
    
  private $playerPositionId;
  private $name;
  private $abbreviation;

  function __construct($playerPositionId, $name, $abbreviation) {
    $this->setPlayerPositionId($playerPositionId);
    $this->setName($name);
    $this->setAbbreviation($abbreviation);
  }
  
  public function getPlayerPositionId() {
    return $this->playerPositionId;
  }
  
  public function setPlayerPositionId($playerPositionId) {
    $this->playerPositionId = $playerPositionId;
  }

  public function getName() {
    return $this->name;
  }

  public function setName($name) {
    $this->name = $name;
  }

  public function getAbbreviation() {
    return $this->abbreviation;
  }

  public function setAbbreviation($abbreviation) {
    $this->abbreviation = $abbreviation;
  }
}