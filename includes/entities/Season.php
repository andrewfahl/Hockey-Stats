<?php

class Season implements CrudInterface {
    
    private $seasonId = 0;
    private $leagueId = 0;
    private $name = '';
    private $startDateTime = NULL;
    private $savedUserID = 0;
    private $savedDateTime = NULL;
    private $seasonTeams = NULL;
    private $games = NULL;
    
    function __construct($seasonId = 0, $leagueId = 0, $startDateTime = NULL, $name = NULL) {
        $this->setSeasonId($seasonId);
        $this->setLeagueId($leagueId);
        $this->setStartDateTime($startDateTime);
        $this->setName($name);
    }

    public function setSavedDateTimeAndUser($savedDateTime, $savedUserID) {
        $this->setSavedUserID($savedUserID);
        $this->setSavedDateTime($savedDateTime);
    }
    
    public function build() {
        
        $result = db_select('season', 's')
            ->fields('s')
            ->condition('seasonid', $this->getSeasonId())  
            ->execute();
        
        $record = $result->fetchAssoc();
        
        if(isset($record)) {
            $this->setSeasonId($record['seasonid']);
            $this->setLeagueId($record['leagueid']);
            $this->setStartDateTime($record['start_datetime']);
            $this->setName($record['name']);
            $this->setSavedDateTimeAndUser($record['saved_datetime'], $record['saved_userid']);
        }
        else {
            throw new Exception('Season Not Found');
        }
    }    
    
    public function save() {
        if ($this->getSeasonId() > 0) {
            $this->update();
        }
        else {
            $this->insert();
        }
    }
    
    private function insert() {
        db_insert('season')
                ->fields(array(
                    'leagueid' => $this->getLeagueId(),
                    'name' => $this->getName(),
                    'start_datetime' => $this->getStartDateTime(),
                    'save_userid' => $this->getSavedUserID(),
                    'saved_datetime' => $this->getSavedDateTime(),
                        )
                )
                ->execute();
    }

    private function update() {
        
        $this->history();
        
        db_update('season')
                ->fields(array(
                    'leagueid' => $this->getLeagueId(),
                    'name' => $this->getName(),
                    'start_datetime' => $this->getStartDateTime(),
                    'saved_userid' => $this->getSavedUserID(),
                    'saved_datetime' => $this->getSavedDateTime(),
                        )
                )
                ->condition('seasonid', $this->getSeasonId(), '=')
                ->execute();
    }
  
    public function delete() {
        $this->history();
        db_delete('season')
            ->condition('seasonid', $this->getSeasonId())
            ->execute();
    }

    private function history() {

        $current_season = new Season($this->getSeasonId());
        $current_season->build();
        
        db_insert('season_history')
             ->fields(array(
                'seasonid' => $current_season->getSeasonId(),
                'leagueid' => $current_season->getLeagueId(),
                'name' => $current_season->getName(),
                'start_datetime' => $current_season->getStartDateTime(),
                'saved_userid' => $current_season->getSavedUserID(),
                'saved_datetime' => $current_season->getSavedDateTime(),
                'history_userid' => $this->getSavedUserID(),
                'history_datetime' => $this->getSavedDateTime(),
                 )
            )
            ->execute();   
    }

    public function equals(CrudInterface $comparison_obj) {
        $returnValue = FALSE;
        
        if($comparison_obj instanceof Season) {
            if( $this->getLeagueId() == $comparison_obj->getLeagueId()
                && $this->getSeasonId() == $comparison_obj->getSeasonId()
                && $this->getName() == $comparison_obj->getName()
                && $this->getStartDateTime() == $comparison_obj->getStartDateTime()
                ) {
                $returnValue = TRUE;
            }
        }
        
        return $returnValue;
    }
    
    public function getGames() {
        if($this->games == null){
            $this->setGames($this->getAllGames());
        }
        
        return $this->games;
    }
    
    public function setGames($games) {
        $this->games = $games;
    }
    
    private function getAllGames(){
        
      $games = array();
        
      $query = db_select('game', 'g');
      $query->innerJoin('team', 'ht', 'ht.teamid = g.home_teamid');
      $query->innerJoin('team', 'vt', 'vt.teamid = g.visiting_teamid');
      $query->leftjoin('location', 'l', 'l.locationid = g.locationid');
      $query->fields('g');
      $query->addField('l', 'name', 'location_name');
      $query->addField('ht', 'name', 'home_team_name');
      $query->addField('vt', 'name', 'visiting_team_name');
      $query->orderBy('start_datetime', 'DESC');
      $query->condition('seasonid', $this->getSeasonId());

      $result = $query->execute();

    while($record = $result->fetchAssoc()) {
            $addGame = new Game($record['gameid'], $record['home_teamid'], 
              $record['visiting_teamid'], $record['locationid'], 
              $record['seasonid'], $record['start_datetime'], $record['playoff_game_yn'],
              $record['location_name'], $record['home_team_name'], $record['visiting_team_name']);
            
            $addGame->setSavedDateTimeAndUser($record['saved_datetime'], $record['saved_userid']);
            array_push($games, $addGame);
        }
        
        return $games;
    }
    
    public function getSeasonId() {
        return $this->seasonId;
    }
    
    public function setSeasonId($seasonId) {
        $this->seasonId = $seasonId;
    }
    
    public function getLeagueId() {
        return $this->leagueId;
    }
    
    public function setLeagueId($leagueId) {
        $this->leagueId = $leagueId;
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
    
    public function getName() {
        return $this->name;
    }

    public function setName($name) {
        $this->name = $name;
    }
    
    public function getStartDateTime() {
        return $this->startDateTime;
    }
    
    public function setStartDateTime($startDateTime) {
        $this->startDateTime = $startDateTime;
    }
    
    public function getTeamIds($seasonTeams) {
        
        $teamIds = array();
        
        foreach($seasonTeams as $seasonTeam) {
            array_push($teamIds, $seasonTeam->getTeamId());
        }
        
        return $teamIds;
    }
    
    public function setSeasonTeams($seasonTeams) {
        $this->seasonTeams = $seasonTeams;
    }
    
    public function getSeasonTeams() {
        if($this->seasonTeams == NULL) {
        
            $seasonTeams = array();
            
            $query = db_select('season_team', 'st');
            $query->innerJoin('team', 't', 't.teamid = st.teamid');
            $query->fields('st');
            $query->addField('t', 'name', 'name');
            $query->orderBy('t.name');
            $query->condition('seasonid', $this->getSeasonId());
            $result = $query->execute();
            
            while($record = $result->fetchAssoc()) {
                $seasonTeam = new SeasonTeam($record['season_teamid'], $record['seasonid'], $record['teamid'], $record['name']);
                $seasonTeam->setSavedDateTimeAndUser($record['saved_datetime'], $record['saved_userid']);
                array_push($seasonTeams, $seasonTeam);
            }

            $this->setSeasonTeams($seasonTeams);
        }
        
        return $this->seasonTeams;
    }

    public function teamIsInSeasonYN($teamId) {
        return in_array($teamId, $this->getTeamIds($this->getSeasonTeams()));
    }
    
    public function saveTeams($newSeasonTeams) {
        
        $teamIdsInSeason = $this->getTeamIds($this->getSeasonTeams());
               
        // add teams that are new
        foreach($newSeasonTeams as $seasonTeam) {
            if(!in_array($seasonTeam->getTeamId(), $teamIdsInSeason)) {
                $seasonTeam->save();
            }
        }
               
        $newTeamIds = $this->getTeamIds($newSeasonTeams);
        $seasonTeams = $this->getSeasonTeams();
        
        // remove the old ones
        foreach($seasonTeams as $seasonTeam) {
            if(!in_array($seasonTeam->getTeamId(), $newTeamIds)) {
                $seasonTeam->delete();
            }
        }
    }
    
    public function getTeamList() {
      $returnValue = array();

      $teams = $this->getSeasonTeams();

      foreach ($teams as $team) {
        $returnValue[$team->getTeamId()] = $team->getTeamName();
      }

      return $returnValue;
    }
}