<?php

class League implements CrudInterface {

    private $leagueId = 0;
    private $name = '';
    private $sequence = 0;
    private $savedUserID = 0;
    private $savedDateTime = NULL;
    private $seasons = NULL;

    function __construct($leagueId = 0, $sequence = 0, $name = NULL) {
        $this->setLeagueId($leagueId);
        $this->setSequence($sequence);
        $this->setName($name);
    }

    public function setSavedDateTimeAndUser($savedDateTime, $savedUserID) {
        $this->setSavedUserID($savedUserID);
        $this->setSavedDateTime($savedDateTime);
    }
    
    public function build() {
        
        $result = db_select('league', 'l')
            ->fields('l')
            ->condition('leagueid', $this->getLeagueId())  
            ->execute();
        
        $record = $result->fetchAssoc();
        
        if(isset($record)) {
            $this->setSequence($record['sequence']);
            $this->setName($record['name']);
            $this->setSavedDateTimeAndUser($record['saved_datetime'], $record['saved_userid']);
        }
        else {
            throw new Exception('League Not Found');
        }
    }
    
    public function save() {
        if ($this->getLeagueId() > 0) {
            $this->update();
        }
        else {
            $this->insert();
        }
    }

    private function insert() {
        db_insert('league')
                ->fields(array(
                    'name' => $this->getName(),
                    'sequence' => $this->getSequence(),
                    'saved_userid' => $this->getSavedUserID(),
                    'saved_datetime' => $this->getSavedDateTime(),
                        )
                )
                ->execute();
    }

    private function update() {
        $this->history();
        db_update('league')
                ->fields(array(
                    'name' => $this->getName(),
                    'sequence' => $this->getSequence(),
                    'saved_userid' => $this->getSavedUserID(),
                    'saved_datetime' => $this->getSavedDateTime(),
                        )
                )
                ->condition('leagueid', $this->getLeagueId(), '=')
                ->execute();
    }
    
    public function delete() {
        $this->history();
        db_delete('league')
            ->condition('leagueid', $this->getLeagueId())
            ->execute();
    }

    private function history() {

        $current_league = new League($this->getLeagueId());
        $current_league->build();
        
        db_insert('league_history')
             ->fields(array(
                'leagueid' => $current_league->getLeagueId(),
                'name' => $current_league->getName(),
                'sequence' => $current_league->getSequence(),
                'saved_userid' => $current_league->getSavedUserID(),
                'saved_datetime' => $current_league->getSavedDateTime(),
                'history_userid' => $this->getSavedUserID(),
                'history_datetime' => $this->getSavedDateTime(),
                 )
            )
            ->execute();   
    }

    public function equals(CrudInterface $comparison_obj) {
        $returnValue = FALSE;
        
        if($comparison_obj instanceof League) {
            // compare all properties for equality except the savedByUserID and the savedDateTime
            if( $this->getLeagueId() == $comparison_obj->getLeagueId()
                && $this->getName() == $comparison_obj->getName()
                && $this->getSequence() == $comparison_obj->getSequence()
                ) {
                $returnValue = TRUE;
            }
        }
        
        return $returnValue;
    }
    
    public function getSeasonByName($name, $excludeSeasonId) {
        $returnValue = null;
        
        $seasons = $this->getSeasons();
        
        foreach($seasons as $season) {
            if($season->getName() == $name && $season->getSeasonId() != $excludeSeasonId) {
                $returnValue = $season;
                break;
            }
        }
        
        return $returnValue;
    }
  
    public function getSeasons() {
        if($this->seasons == null){
            $this->setSeasons($this->getAllSeasons());
        }
        
        return $this->seasons;
    }
    
    public function setSeasons($seasons) {
        $this->seasons = $seasons;
    }
    
    private function getAllSeasons(){
        
        $seasons = array();
        
        $result = db_select('season', 's')
            ->fields('s')
            ->orderBy('start_datetime', 'DESC') 
            ->condition('leagueid', $this->getLeagueId())  
            ->execute();
        
        while($record = $result->fetchAssoc()) {
            $updateSeason = new Season($record['seasonid'], $record['leagueid'], $record['start_datetime'], $record['name']);
            $updateSeason->setSavedDateTimeAndUser($record['saved_datetime'], $record['saved_userid']);
            array_push($seasons, $updateSeason);
        }
        
        return $seasons;
    }
    
    public function getSeasonList() {
      $returnValue = array();

      $seasons = $this->getSeasons();
      
      foreach ($seasons as $season) {
        $returnValue[$season->getSeasonId()] = $season->getName();
      }
      
      return $returnValue;
    }
    
    public function containsSeason($seasonId) {
        $returnValue = FALSE;
        
        $seasons = $this->getSeasons();
        
        foreach($seasons as $season) {
            if($season->getSeasonId() == $seasonId) {
                $returnValue = TRUE;
                break;
            }
        }
        
        return $returnValue;
    }
    
    /*
     * Gets the latest season 
     */
    public function getActiveByDefaultSeason() {
      
      $seasons = $this->getSeasons();
      $season = NULL;
      
      if(isset($seasons)) {
        $season = $seasons[0];
      }
      
      return $season;
    }
    
    public function getActiveByDefaultSeasonId() {
      $seasonid = 0;
      $season = $this->getActiveByDefaultSeason();
      if (isset($season)) {
        $seasonid = $season->getSeasonId();
      }
      
      return $seasonid;
    }
    
    public function getLeagueId() {
        return $this->leagueId;
    }

    public function setLeagueId($leagueId) {
        $this->leagueId = $leagueId;
    }

    public function getName() {
        return $this->name;
    }

    public function setName($name) {
        $this->name = $name;
    }

    public function getSequence() {
        return $this->sequence;
    }

    public function setSequence($sequence) {
        $this->sequence = $sequence;
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
