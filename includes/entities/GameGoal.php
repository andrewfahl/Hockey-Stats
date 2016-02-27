<?php

class GameGoal implements CrudInterface {

  private $gameGoalId;
  private $gameId;
  private $periodId;
  private $time;
  private $seasonTeamPlayerId;
  private $assistSeasonTeamPlayerId;
  private $secondAssistSeasonTeamPlayerId; 
  private $playerFirstName;
  private $playerLastName;
  private $assistPlayerFirstName;
  private $assistPlayerLastName;  
  private $secondAssistPlayerFirstName;
  private $secondAssistPlayerLastName;  
  private $teamName;
  private $periodName;
  private $savedDateTime = NULL;
  private $savedUserID = 0;
  
  function __construct($gameGoalId = 0, $gameId = 0, $periodId = 0, 
    $time = NULL, $seasonTeamPlayerId = NULL, $assistSeasonTeamPlayerId = NULL, 
    $secondAssistSeasonTeamPlayerId = NULL, $playerFirstName = NULL, 
    $playerLastName = NULL, $teamName = NULL, $periodName = NULL,
    $assistPlayerFirstName = NULL, $assistPlayerLastName = NULL,
    $secondAssistPlayerFirstName = NULL, $secondAssistPlayerLastName = NULL) {

    $this->setGameGoalId($gameGoalId);
    $this->setGameId($gameId);
    $this->setPeriodId($periodId);
    $this->setTime($time);
    $this->setSeasonTeamPlayerId($seasonTeamPlayerId);
    $this->setAssistSeasonTeamPlayerId($assistSeasonTeamPlayerId);
    $this->setSecondAssistSeasonTeamPlayerId($secondAssistSeasonTeamPlayerId);
    $this->setPlayerFirstName($playerFirstName);
    $this->setPlayerLastName($playerLastName);
    $this->setTeamName($teamName);
    $this->setPeriodName($periodName);
    $this->setAssistPlayerFirstName($assistPlayerFirstName);
    $this->setAssistPlayerLastName($assistPlayerLastName);
    $this->setSecondAssistPlayerFirstName($secondAssistPlayerFirstName);
    $this->setSecondAssistPlayerLastName($secondAssistPlayerLastName);    
  }
  
  public function setSavedDateTimeAndUser($savedDateTime, $savedUserID) {
    $this->setSavedUserID($savedUserID);
    $this->setSavedDateTime($savedDateTime);
  }
  
  public function build() {
    
    $query = db_select('game_goal', 'gg');
    $query->innerJoin('season_team_player', 'stp', 'stp.season_team_playerid = gg.season_team_playerid');
    $query->leftJoin('season_team_player', 'ast', 'ast.season_team_playerid = gg.assist_season_team_playerid');
    $query->leftJoin('season_team_player', 'sast', 'sast.season_team_playerid = gg.second_assist_season_team_playerid');
    $query->innerJoin('season_team', 'st', 'st.season_teamid = stp.season_teamid');    
    $query->innerJoin('team', 't', 'st.teamid = t.teamid');
    $query->innerJoin('player', 'p', 'p.playerid = stp.playerid');
    $query->leftJoin('player', 'ap', 'ap.playerid = ast.playerid');
    $query->leftJoin('player', 'sap', 'sap.playerid = sast.playerid');
    $query->innerJoin('period', 'per', 'per.periodid = gg.periodid');
    $query->fields('gg');
    $query->addField('p', 'first_name');
    $query->addField('p', 'last_name');
    $query->addField('ap', 'first_name', 'assist_first_name');
    $query->addField('ap', 'last_name', 'assist_last_name');
    $query->addField('sap', 'first_name', 'second_assist_first_name');
    $query->addField('sap', 'last_name', 'second_assist_last_name');    
    $query->addField('t', 'name', 'teamName');
    $query->addField('per', 'name', 'periodName');
    $query->condition('game_goalid', $this->getGameGoalId());
    $result = $query->execute();
   
    $record = $result->fetchAssoc();

    if (isset($record)) {
      $this->setGameGoalId($record['game_goalid']);
      $this->setGameId($record['gameid']);
      $this->setPeriodId($record['periodid']);
      $this->setTime($record['time']);
      $this->setSeasonTeamPlayerId($record['season_team_playerid']);
      $this->setAssistSeasonTeamPlayerId($record['assist_season_team_playerid']);
      $this->setSecondAssistSeasonTeamPlayerId($record['second_assist_season_team_playerid']);
      $this->setPlayerFirstName($record['first_name']);
      $this->setPlayerLastName($record['last_name']);
      $this->setTeamName($record['teamName']);
      $this->setPeriodName($record['periodName']);
      $this->setAssistPlayerFirstName($record['assist_first_name']);
      $this->setAssistPlayerLastName($record['assist_last_name']);
      $this->setSecondAssistPlayerFirstName($record['second_assist_first_name']);
      $this->setSecondAssistPlayerLastName($record['second_assist_last_name']); 
      $this->setSavedDateTimeAndUser($record['saved_datetime'], $record['saved_userid']);      
    }
    else {
      throw new Exception('Game Penalty Not Found');
    }
  }

  public function save() {
    if ($this->getGameGoalId() > 0) {
      $this->update();
    }
    else {
      $this->insert();
    }
  }

  private function insert() {
    db_insert('game_goal')
      ->fields(array(
           'gameid' => $this->getGameId(),
           'periodid' => $this->getPeriodId(),
           'time' => $this->getTime(),
           'season_team_playerid' => $this->getSeasonTeamPlayerId(),
           'assist_season_team_playerid' => $this->getAssistSeasonTeamPlayerId(),
           'second_assist_season_team_playerid' => $this->getSecondAssistSeasonTeamPlayerId(),
           'saved_userid' => $this->getSavedUserID(),
           'saved_datetime' => $this->getSavedDateTime(),
        )
      )
      ->execute();
  }
  
  private function update() {

    $this->history();

    db_update('game_goal')
      ->fields(array(
           'gameid' => $this->getGameId(),
           'periodid' => $this->getPeriodId(),
           'time' => $this->getTime(),
           'season_team_playerid' => $this->getSeasonTeamPlayerId(),
           'assist_season_team_playerid' => $this->getAssistSeasonTeamPlayerId(),
           'second_assist_season_team_playerid' => $this->getSecondAssistSeasonTeamPlayerId(),
           'saved_userid' => $this->getSavedUserID(),
           'saved_datetime' => $this->getSavedDateTime(),
        )
      )
      ->condition('game_goalid', $this->getGameGoalId())
      ->execute();
  }
  
  private function history() {
    
    $current = new GameGoal($this->getGameGoalId());
    $current->build();

    db_insert('game_goal_history')
      ->fields(array(
           'game_goalid' => $current->getGameGoalId(),
           'gameid' => $current->getGameId(),
           'periodid' => $current->getPeriodId(),
           'time' => $current->getTime(),
           'season_team_playerid' => $current->getSeasonTeamPlayerId(),
           'assist_season_team_playerid' => $current->getAssistSeasonTeamPlayerId(),
           'second_assist_season_team_playerid' => $current->getSecondAssistSeasonTeamPlayerId(),
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

    db_delete('game_goal')
      ->condition('game_goalid', $this->getGameGoalId())
      ->execute();

  }

  public function equals(CrudInterface $comparison_obj) {

    $returnValue = FALSE;

    if ($comparison_obj instanceof GameGoal) {
      if ($this->getGameId() == $comparison_obj->getGameId() 
        && $this->getPeriodId() == $comparison_obj->getPeriodId() 
        && $this->getTime() == $comparison_obj->getTime() 
        && $this->getSeasonTeamPlayerId() == $comparison_obj->getSeasonTeamPlayerId()
        && $this->getAssistSeasonTeamPlayerId() == $comparison_obj->getAssistSeasonTeamPlayerId()
        && $this->getSecondAssistSeasonTeamPlayerId() == $comparison_obj->getSecondAssistSeasonTeamPlayerId()
      ) {
        $returnValue = TRUE;
      }
    }

    return $returnValue;
  }

  public function getGameGoalId() {
    return $this->gameGoalId;
  }

  public function setGameGoalId($gameGoalId) {
    $this->gameGoalId = $gameGoalId;
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
 
  public function getTime() {
    return $this->time;
  }

  public function setTime($time) {
    $this->time = $time;
  }

  public function getSeasonTeamPlayerId() {
    return $this->seasonTeamPlayerId;
  }

  public function setSeasonTeamPlayerId($seasonTeamPlayerId) {
    $this->seasonTeamPlayerId = $seasonTeamPlayerId;
  }

  public function getAssistSeasonTeamPlayerId() {
    return $this->assistSeasonTeamPlayerId;
  }

  public function setAssistSeasonTeamPlayerId($assistSeasonTeamPlayerId) {
    // optional field so it should be null if empty
    if(empty($assistSeasonTeamPlayerId) || $assistSeasonTeamPlayerId == "0") {
      $assistSeasonTeamPlayerId = NULL;
    }
    
    $this->assistSeasonTeamPlayerId = $assistSeasonTeamPlayerId;
  }
 
  public function getSecondAssistSeasonTeamPlayerId() {
    return $this->secondAssistSeasonTeamPlayerId;
  }

  public function setSecondAssistSeasonTeamPlayerId($secondAssistSeasonTeamPlayerId) {
    // optional field so it should be null if empty
    if(empty($secondAssistSeasonTeamPlayerId) || $secondAssistSeasonTeamPlayerId == "0") {
      $secondAssistSeasonTeamPlayerId = NULL;
    }
    
    $this->secondAssistSeasonTeamPlayerId = $secondAssistSeasonTeamPlayerId;
  }
 
  public function getPeriodName() {
    return $this->periodName;
  }

  public function setPeriodName($periodName) {
    $this->periodName = $periodName;
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
  
  public function getTeamName() {
    return $this->teamName;
  }
  
  public function setTeamName($teamName) {
    $this->teamName = $teamName;
  }
  
  public function getAssistPlayerFirstName() {
    return $this->assistPlayerFirstName;
  }

  public function setAssistPlayerFirstName($assistPlayerFirstName) {
    $this->assistPlayerFirstName = $assistPlayerFirstName;
  }

  public function getAssistPlayerLastName() {
    return $this->assistPlayerLastName;
  }

  public function setAssistPlayerLastName($assistPlayerLastName) {
    $this->assistPlayerLastName = $assistPlayerLastName;
  }

  public function getSecondAssistPlayerFirstName() {
    return $this->secondAssistPlayerFirstName;
  }

  public function setSecondAssistPlayerFirstName($secondAssistPlayerFirstName) {
    $this->secondAssistPlayerFirstName = $secondAssistPlayerFirstName;
  }
 
  public function getSecondAssistPlayerLastName() {
    return $this->secondAssistPlayerLastName;
  }

  public function setSecondAssistPlayerLastName($secondAssistPlayerLastName) {
    $this->secondAssistPlayerLastName = $secondAssistPlayerLastName;
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
