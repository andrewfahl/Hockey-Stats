<?php

class Team implements CrudInterface {
    
    private $teamId = 0;
    private $name = '';
    private $activeYN = TRUE;
    private $savedUserID = 0;
    private $savedDateTime = NULL;
    private $players = NULL;
    private $seasons;
    
     function __construct($teamId = 0, $name = NULL, $activeYN = FALSE) {
        $this->setTeamId($teamId);
        $this->setName($name);
        $this->setActiveYN($activeYN);
    }

    public function setSavedDateTimeAndUser($savedDateTime, $savedUserID) {
        $this->setSavedUserID($savedUserID);
        $this->setSavedDateTime($savedDateTime);
    }
    
    public function build() {
        
        $result = db_select('team', 't')
            ->fields('t')
            ->condition('teamid', $this->getTeamId())  
            ->execute();
        
        $record = $result->fetchAssoc();
        
        if(isset($record)) {
            $this->setTeamId($record['teamid']);
            $this->setName($record['name']);
            $this->setActiveYN($record['active_yn']);
            $this->setSavedDateTimeAndUser($record['saved_datetime'], $record['saved_userid']);
        }
        else {
            throw new Exception('Team Not Found');
        }
    }    

     public function save() {
        if ($this->getTeamId() > 0) {
            $this->update();
        }
        else {
            $this->insert();
        }
    }

    private function insert() {
        db_insert('team')
                ->fields(array(
                    'teamid' => $this->getTeamId(),
                    'name' => $this->getName(),
                    'active_yn' => $this->getActiveYN(),
                    'saved_userid' => $this->getSavedUserID(),
                    'saved_datetime' => $this->getSavedDateTime(),
                        )
                )
                ->execute();
    }

    private function update() {
        $this->history();
        
        db_update('team')
                ->fields(array(
                    'name' => $this->getName(),
                    'active_yn' => $this->getActiveYN(),
                    'saved_userid' => $this->getSavedUserID(),
                    'saved_datetime' => $this->getSavedDateTime(),
                        )
                )
                ->condition('teamid', $this->getTeamId(), '=')
                ->execute();
    }

    public function delete() {
        $this->history();
        db_delete('team')
            ->condition('teamid', $this->getTeamId())
            ->execute();
    }

    private function history() {

        $current_team = new Team($this->getTeamId());
        $current_team->build();
        
        db_insert('team_history')
             ->fields(array(
                'teamid' => $current_team->getTeamId(),
                'name' => $current_team->getName(),
                'active_yn' => $current_team->getActiveYN(),
                'saved_userid' => $current_team->getSavedUserID(),
                'saved_datetime' => $current_team->getSavedDateTime(),
                'history_userid' => $this->getSavedUserID(),
                'history_datetime' => $this->getSavedDateTime(),
                 )
            )
            ->execute();   
    }
  
    public function equals(CrudInterface $comparison_obj) {
        $returnValue = FALSE;
        
        if($comparison_obj instanceof Team) {
            if( $this->getTeamId() == $comparison_obj->getTeamId()
                && $this->getName() == $comparison_obj->getName()
                && $this->getActiveYN() == $comparison_obj->getActiveYN()
                ) {
                $returnValue = TRUE;
            }
        }
        
        return $returnValue;
    }
   
    public function getTeamId() {
        return $this->teamId;
    }
    
    public function setTeamId($teamId) {
         $this->teamId = $teamId;
    }
    
    public function getName() {
        return $this->name;
    }
    
    public function setName($name) {
         $this->name = $name;
    }
    
    public function getActiveYN() {
        return $this->activeYN;
    }
    
    public function setSeasonTeams($seasons) {
         $this->seasons = $seasons;
    }

    public function getSeasonTeams() {
      if ($this->seasons == NULL) {
        $this->seasons = $this->getAllSeasonTeams();
      }

      return $this->seasons;  
    }
    
    private function getAllSeasonTeams() {
      
      $seasons = array();
      
      $query = db_select('season_team', 'st');
      $query->innerJoin('team', 't', 't.teamid = st.teamid');
      $query->innerJoin('season', 's', 's.seasonid = st.seasonid');
      $query->fields('st');
      $query->addField('t', 'name', 'team_name');
      $query->addField('s', 'name', 'season_name');
      $query->condition('st.teamid', $this->getTeamId());
      $query->orderBy('s.start_datetime', 'DESC');
      $result = $query->execute();

      while ($record = $result->fetchAssoc()) {
        $seasonTeam = new SeasonTeam($record['season_teamid'], $record['seasonid'], $record['teamid'], $record['team_name'], $record['season_name']);
        array_push($seasons, $seasonTeam);
      }
      
      return $seasons;
    }
    
    public function getSeasonTeamsList() {
      
      $returnValue = array();

      $seasons = $this->getSeasonTeams();
      
      foreach ($seasons as $season) {
        $returnValue[$season->getSeasonTeamId()] = $season->getSeasonName();
      }
      
      return $returnValue;
    }
    
    public function getSeasonList() {
      
      $returnValue = array();

      $seasons = $this->getSeasonTeams();
      
      foreach ($seasons as $season) {
        $returnValue[$season->getSeasonId()] = $season->getSeasonName();
      }
      
      return $returnValue;
    }
    
    public function isPlayingInSeasonYN($seasonId) {
      $returnValue = FALSE;

      $seasonTeams = $this->getSeasonTeams();
      
      foreach ($seasonTeams as $seasonTeam) {
        if($seasonTeam->getSeasonId() == $seasonId) {
          $returnValue = TRUE;
          break;
        }
      }
      
      return $returnValue;
    }
    
    public function getLatestSeasonTeam() {
      $seasonTeam = NULL;
      $seasonTeams = $this->getSeasonTeams();
      
      if(sizeof($seasonTeams) > 0) {
        $seasonTeam = $seasonTeams[0];
      }
      
      return $seasonTeam;
    }
    
    public function getSeasonTeam($seasonId) {
      $returnValue = NULL;
      $seasonTeams = $this->getSeasonTeams();
      
      foreach($seasonTeams as $seasonTeam) {
        if($seasonTeam->getSeasonId() == $seasonId) {
          $returnValue = $seasonTeam;
          break;
        }
      }
      
      return $returnValue;
    }
    
    /*
     * All of the players that have ever been on the team. Current and former
     */
    private function getAllPlayers() {
      
      $players = array();
      
      $query = db_select('season_team', 'st');
      $query->distinct();
      $query->innerJoin('season_team_player', 'stp', 'stp.season_teamid = st.season_teamid');
      $query->innerJoin('player', 'p', 'p.playerid = stp.playerid');
      $query->fields('p');
      $query->condition('st.teamid', $this->getTeamId());
      $query->orderBy('p.last_name');
      $query->orderBy('p.first_name');
      $result = $query->execute();
      while ($record = $result->fetchAssoc()) {
        $player = new Player($record['playerid'], $record['first_name'], $record['last_name']);

        array_push($players, $player);
      }
      
      return $players;
    }
    
    public function getPlayerIdList() {
      
      $playerIds = array();
      $players = $this->getPlayers();
      
      foreach($players as $player) {
        array_push($playerIds, $player->getPlayerId());
      }
      
      return $playerIds;
    }
    
    /*
     * All of the players that have ever been on the team. Current and former
     */
    public function getPlayers() {
      
      if($this->players == NULL) {
        $this->setPlayers($this->getAllPlayers());
      }
      
      return $this->players;
    }
    
    public function setPlayers($players) {
      $this->players = $players;
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