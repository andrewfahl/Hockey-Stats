<?php

class Application {

  private $leagues = NULL;
  private $teams = NULL;
  private $locations = NULL;
  private $players = NULL;
  private $playerPositions = NULL;
  
  public function getLeagues() {
    if ($this->leagues == NULL) {
      $this->leagues = $this->getAllLeagues();
    }

    return $this->leagues;
  }

  private function getAllLeagues() {

    $leagues = array();

    $result = db_select('league', 'l')
      ->fields('l')
      ->orderBy('sequence')
      ->orderBy('name')
      ->execute();

    while ($record = $result->fetchAssoc()) {
      $updateLeague = new League($record['leagueid'], $record['sequence'], $record['name']);
      $updateLeague->setSavedDateTimeAndUser($record['saved_datetime'], $record['saved_userid']);
      array_push($leagues, $updateLeague);
    }

    return $leagues;
  }

  public function getLeagueByName($name, $excludeLeagueId) {
    $returnValue = NULL;

    $leagues = $this->getLeagues();

    foreach ($leagues as $league) {
      if ($league->getName() == $name && $league->getLeagueId() != $excludeLeagueId) {
        $returnValue = $league;
        break;
      }
    }

    return $returnValue;
  }

  public function getLeagueById($id) {
    $returnValue = NULL;

    $leagues = $this->getLeagues();

    foreach ($leagues as $league) {
      if ($league->getLeagueId() == $id) {
        $returnValue = $league;
        break;
      }
    }

    return $returnValue;
  }

  /*
   * Gets the league with the lowest sequence
   */
  public function getActiveByDefaultLeague() {
    $returnValue = NULL;

    $leagues = $this->getLeagues();

    if (sizeof($leagues) > 0) {
      $returnValue = $leagues[0];
    }

    return $returnValue;
  }

  public function getLeagueList() {
    $returnValue = array();

    $leagues = $this->getLeagues();

    foreach ($leagues as $league) {
      $returnValue[$league->getLeagueId()] = $league->getName();
    }

    return $returnValue;
  }

  private function getAllTeams() {
    $teams = array();

    $result = db_select('team', 't')
      ->fields('t')
      ->orderBy('name')
      ->execute();

    while ($record = $result->fetchAssoc()) {
      $team = new Team($record['teamid'], $record['name'], $record['active_yn']);
      $team->setSavedDateTimeAndUser($record['saved_datetime'], $record['saved_userid']);
      array_push($teams, $team);
    }

    return $teams;
  }

  public function getTeams() {

    if ($this->teams == NULL) {
      $this->teams = $this->getAllTeams();
    }

    return $this->teams;
  }

  public function getTeamByName($name, $excludeTeamId) {
    $returnValue = NULL;

    $teams = $this->getTeams();

    foreach ($teams as $team) {
      if ($team->getName() == $name && $team->getTeamId() != $excludeTeamId) {
        $returnValue = $team;
        break;
      }
    }

    return $returnValue;
  }

  public function getActiveTeams() {

    $activeTeams = array();

    $teams = $this->getTeams();

    foreach ($teams as $team) {
      if ($team->getActiveYN()) {
        array_push($activeTeams, $team);
      }
    }

    return $activeTeams;
  }

  public function getLocations() {

    if ($this->locations == NULL) {
      $this->locations = $this->getAllLocations();
    }

    return $this->locations;
  }

  private function getAllLocations() {
    $locations = array();

    $result = db_select('location', 'l')
      ->fields('l')
      ->orderBy('name')
      ->execute();

    while ($record = $result->fetchAssoc()) {
      $location = new Location($record['locationid'], $record['name'], 
        $record['address'], $record['address2'], $record['city'], 
        $record['state'], $record['zip']);
      $location->setSavedDateTimeAndUser($record['saved_datetime'], $record['saved_userid']);
      array_push($locations, $location);
    }

    return $locations;
  }

  public function getLocationByName($name, $excludeLocationId) {
    $returnValue = NULL;

    $locations = $this->getLocations();

    foreach ($locations as $location) {
      if ($location->getName() == $name && $location->getLocationId() != $excludeLocationId) {
        $returnValue = $location;
        break;
      }
    }

    return $returnValue;
  }

  public function getLocationList() {
    $returnValue = array();

    $locations = $this->getLocations();

    foreach ($locations as $location) {
      $returnValue[$location->getLocationId()] = $location->getName();
    }

    return $returnValue;
  }
  
  public function getPlayers() {

    if ($this->players == NULL) {
      $this->players = $this->getAllPlayers();
    }

    return $this->players;
  }
  
  private function getAllPlayers() {
    
    $players = array();
 
    $query = db_select('player', 'p');
    $query->leftJoin('player_position', 'pp', 'pp.player_positionid = p.player_positionid');
    $query->leftJoin('player_position', 'sp', 'sp.player_positionid = p.secondary_player_positionid');
    $query->fields('p');
    $query->addField('pp', 'name', 'player_position_name');
    $query->addField('pp', 'abbreviation', 'player_position_abbreviation');
    $query->addField('sp', 'name', 'secondary_player_position_name');
    $query->addField('sp', 'abbreviation', 'secondary_player_position_abbreviation');
    $query->orderBy('p.last_name');
    $query->orderBy('p.first_name');
    $result = $query->execute();

    while ($record = $result->fetchAssoc()) {
      $player = new Player($record['playerid'], $record['first_name'], $record['last_name'], 
        $record['player_positionid'], $record['secondary_player_positionid'], $record['active_yn'], 
        $record['player_position_name'], $record['player_position_abbreviation'],
        $record['secondary_player_position_name'], $record['secondary_player_position_abbreviation']);
      
      $player->setSavedDateTimeAndUser($record['saved_datetime'], $record['saved_userid']);
      array_push($players, $player);
    }
 
    return $players;
  }
  
    public function getActivePlayers() {
  
      $activePlayers = array();
  
      $players = $this->getPlayers();
  
      foreach ($players as $player) {
        if ($player->getActiveYN()) {
          array_push($activePlayers, $player);
        }
      }
  
      return $activePlayers;
  }
 
  public function getPlayerByName($firstName, $lastName, $excludePlayerId) {
    $returnValue = NULL;

    $players = $this->getPlayers();

    foreach ($players as $player) {
      if (strtolower($player->getFirstName()) == strtolower($firstName)
        && strtolower($player->getLastName()) == strtolower($lastName)
        && $player->getPlayerId() != $excludePlayerId) {
        $returnValue = $player;
        break;
      }
    }

    return $returnValue;
  }
  
  public function getPlayerPositions() {
  
      if ($this->playerPositions == NULL) {
        $this->playerPositions = $this->getAllPlayerPositions();
      }
  
      return $this->playerPositions;
  }
  
  public function getPlayerPositionList() {
    $returnValue = array();

    $playerPositions = $this->getAllPlayerPositions();

    foreach ($playerPositions as $playerPosition) {
      $returnValue[$playerPosition->getPlayerPositionId()] = $playerPosition->getAbbreviation();
    }

    return $returnValue;
  }
  
  private function getAllPlayerPositions() {
    $playerPositions = array();

    $result = db_select('player_position', 'pp')
      ->fields('pp')
      ->execute();

    while ($record = $result->fetchAssoc()) {
      $playerPosition = new PlayerPosition($record['player_positionid'], $record['name'], $record['abbreviation']);
      array_push($playerPositions, $playerPosition);
    }

    return $playerPositions;
  }
}
