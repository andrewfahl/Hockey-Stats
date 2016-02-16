<?php

class SeasonTeam implements CrudInterface {

  private $seasonTeamId = 0;
  private $teamId = 0;
  private $seasonId = 0;
  private $teamName = NULL;
  private $seasonName = NULL;
  private $seasonTeamPlayers;
  private $savedDateTime = NULL;
  private $savedUserID = 0;

  function __construct($seasonTeamId = 0, $seasonId = 0, $teamId = 0, $teamName = NULL, $seasonName = NULL) {
    $this->setSeasonTeamId($seasonTeamId);
    $this->setSeasonId($seasonId);
    $this->setTeamId($teamId);
    $this->setTeamName($teamName);
    $this->setSeasonName($seasonName);
  }

  public function setSavedDateTimeAndUser($savedDateTime, $savedUserID) {
    $this->setSavedUserID($savedUserID);
    $this->setSavedDateTime($savedDateTime);
  }

  public function build() {

    $query = db_select('season_team', 'st');
    $query->innerJoin('team', 't', 't.teamid = st.teamid');
    $query->innerJoin('season', 's', 's.seasonid = st.seasonid');
    $query->fields('st');
    $query->addField('t', 'name', 'name');
    $query->addField('s', 'name', 'season_name');
    // if there is a season team id buld it from that
    if ($this->getSeasonTeamId() > 0) {
      $query->condition('season_teamid', $this->getSeasonTeamId());
    }
    // else build it using the seaonid and the teamid
    else {
      $query->condition('st.seasonid', $this->getSeasonId());
      $query->condition('st.teamid', $this->getTeamId());
    }
    $result = $query->execute();

    $record = $result->fetchAssoc();

    if (isset($record)) {
      $this->setSeasonTeamId($record['season_teamid']);
      $this->setTeamId($record['teamid']);
      $this->setSeasonId($record['seasonid']);
      $this->setTeamName($record['name']);
      $this->setSeasonName($record['season_name']);
      $this->setSavedDateTimeAndUser($record['saved_datetime'], $record['saved_userid']);
    }
    else {
      throw new Exception('Season Team Not Found');
    }
  }

  public function save() {
    if ($this->getSeasonTeamId() > 0) {
      $this->update();
    }
    else {
      $this->insert();
    }
  }

  public function update() {
    // there really isnt a need to update a season team
  }

  public function insert() {
    db_insert('season_team')
      ->fields(array(
           'seasonid' => $this->getSeasonId(),
           'teamid' => $this->getTeamId(),
           'saved_userid' => $this->getSavedUserID(),
           'saved_datetime' => $this->getSavedDateTime(),
        )
      )
      ->execute();
  }

  public function delete() {

    $this->history();

    db_delete('season_team')
      ->condition('season_teamid', $this->getSeasonTeamId())
      ->execute();
  }

  public function history() {
    $current_season_team = new SeasonTeam($this->getSeasonTeamId());
    $current_season_team->build();

    db_insert('season_team_history')
      ->fields(array(
           'season_teamid' => $current_season_team->getSeasonTeamId(),
           'seasonid' => $current_season_team->getSeasonId(),
           'teamid' => $current_season_team->getTeamId(),
           'saved_userid' => $current_season_team->getSavedUserID(),
           'saved_datetime' => $current_season_team->getSavedDateTime(),
           'history_userid' => $this->getSavedUserID(),
           'history_datetime' => $this->getSavedDateTime(),
        )
      )
      ->execute();
  }

  public function equals(CrudInterface $comparison_obj) {

    $returnValue = FALSE;

    if ($comparison_obj instanceof SeasonTeam) {
      // compare all properties for equality except the savedByUserID and the savedDateTime
      if ($this->getSeasonId() == $comparison_obj->getSeasonId() && $this->getTeamId() == $comparison_obj->getTeamId()
      ) {
        $returnValue = TRUE;
      }
    }

    return $returnValue;
  }

  private function getAllSeasonTeamPlayers() {
    $players = array();

    $query = db_select('season_team_player', 'stp');
    $query->innerJoin('player', 'p', 'p.playerid = stp.playerid');
    $query->fields('p');
    $query->addField('stp', 'number');
    $query->addField('stp', 'season_team_playerid');
    $query->addField('stp', 'season_teamid');
    $query->condition('season_teamid', $this->getSeasonTeamId());
    $query->orderBy('p.last_name');
    $query->orderBy('p.first_name');
    $result = $query->execute();

    while ($record = $result->fetchAssoc()) {
      $seasonTeamPlayer = new SeasonTeamPlayer($record['season_team_playerid'], $record['season_teamid'], $record['playerid'], $record['number'], $record['first_name'], $record['last_name']);
      $seasonTeamPlayer->setSavedDateTimeAndUser($record['saved_datetime'], $record['saved_userid']);
      array_push($players, $seasonTeamPlayer);
    }

    return $players;
  }
  
  public function getPlayerList() {
    $players = array();
    $players = $this->getSeasonTeamPlayers();
    $playerList = array();
    foreach ($players as $player) {
      $playerList[$player->getSeasonTeamPlayerId()] = $player->getFullName();
    }
    
    return $playerList;
  }

  public function getSeasonTeamPlayerIdList() {

    $playerIds = array();
    $players = $this->getSeasonTeamPlayers();

    foreach ($players as $player) {
      array_push($playerIds, $player->getSeasonTeamPlayerId());
    }

    return $playerIds;
  }

  public function getPlayerIdList() {

    $playerIds = array();
    $players = $this->getSeasonTeamPlayers();

    foreach ($players as $player) {
      array_push($playerIds, $player->getPlayerId());
    }

    return $playerIds;
  }

  public function getSeasonTeamPlayers() {

    if ($this->seasonTeamPlayers == NULL) {
      $this->setSeasonTeamPlayers($this->getAllSeasonTeamPlayers());
    }

    return $this->seasonTeamPlayers;
  }

  public function setSeasonTeamPlayers($seasonTeamPlayers) {
    $this->seasonTeamPlayers = $seasonTeamPlayers;
  }

  public function getSeasonTeamId() {
    return $this->seasonTeamId;
  }

  public function setSeasonTeamId($seasonTeamId) {
    $this->seasonTeamId = $seasonTeamId;
  }

  public function getTeamId() {
    return $this->teamId;
  }

  public function setTeamId($teamId) {
    $this->teamId = $teamId;
  }

  public function getSeasonId() {
    return $this->seasonId;
  }

  public function setSeasonId($seasonId) {
    $this->seasonId = $seasonId;
  }

  public function getTeamName() {
    return $this->teamName;
  }

  public function setTeamName($teamName) {
    $this->teamName = $teamName;
  }

  public function getSeasonName() {
    return $this->seasonName;
  }

  public function setSeasonName($seasonName) {
    $this->seasonName = $seasonName;
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

  public function getDefaultGoalie() {
    // find the goalie based on the number of games played for the team and the players position
    $goalie = NULL;
    
    // query shold be something like
    /*
    select gp.playerid, count(gp.playerid) as count
    from game g
    inner join game_player gp on gp.gameid = g.gameid
      and gp.teamid = 1
      and gp.goalie_yn = 1
    where g.seasonid = 5
    AND (g.home_teamid = 1 OR g.visiting_teamid = 1)
    GROUP BY gp.playerid
    order by count DESC
    LIMIT 1 OFFSET 0; 
    */
    
    $or = db_or()
         ->condition('g.home_teamid', $this->getTeamId())
         ->condition('g.visiting_teamid', $this->getTeamId());
    
    $query = db_select('game', 'g')
      ->condition('g.seasonid', $this->getSeasonId())
      ->condition($or);
    $query->innerJoin('game_player', 'gp', 'gp.gameid = g.gameid');
    $query->condition('gp.teamid', $this->getTeamId());
    $query->condition('gp.goalie_yn', '1');
    $query->addField('gp', 'playerid');
    $query->addExpression('COUNT(gp.playerid)', 'ncount');
    $query->groupBy('gp.playerid');
    $query->orderBy('ncount', 'DESC');
    $query->range(0,1);//LIMIT to 1 record
    $result = $query->execute();

    $record = $result->fetchAssoc();
      
    if (isset($record)) {
      $goalie = new Player($record['playerid']);
    }

    return $goalie;
  }

}
