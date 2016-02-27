<?php

function hockey_stats_game_goals_landing_page() {
  header('Content-type: text/plain; charset=UTF-8');
  header('Content-Disposition: inline');
  return drupal_get_form('hockey_stats_game_goals_landing');
}

function hockey_stats_game_goals_add_page() {
  header('Content-type: text/plain; charset=UTF-8');
  header('Content-Disposition: inline');
  return drupal_get_form('hockey_stats_game_goals_add');
}

function hockey_stats_game_goals_edit_page() {
  header('Content-type: text/plain; charset=UTF-8');
  header('Content-Disposition: inline');
  return drupal_get_form('hockey_stats_game_goals_edit');
}

function hockey_stats_game_goals_delete_page() {
  header('Content-type: text/plain; charset=UTF-8');
  header('Content-Disposition: inline');
  return drupal_get_form('hockey_stats_game_goals_delete');
}


function hockey_stats_game_goals_landing($form, &$form_state) {

  $gameid = Utility::getCookieValue(Utility::COOKIE_GAMEID);
  
  if (isset($form_state['input']['gameId'])) {
    $game = new Game($form_state['input']['gameId']);
    $game->build();
  }
  else if(isset($gameid)) {
    $game = new Game($gameid);
    $game->build();
  }
  if (!isset($game)) {
    throw(new Exception("Game not found"));
  }
  
  Utility::setCookieValue(Utility::COOKIE_GAMEID, $game->getGameId());

  $header = array(
       'period' => t('Period'),
       'time' => t('Time'),
       'team' => t('Team'),
       'goal' => t('Goal'),
       'assist' => t('Assist'),
       '2nd_assist' => t('2nd Assist'),
       'edit' => t('Edit'),
       'delete' => t('Delete'),
  );

  $options = array();

  $goals = $game->getGoals();
  
  foreach ($goals as $goal) {
    $edit_button = array(
         '#type' => 'button',
         '#value' => t('Edit'),
         '#attributes' => array('onclick' => 'goalEdit(' . $goal->getGameGoalId(). ');'),
    );

    $delete_button = array(
         '#type' => 'button',
         '#value' => t('Delete'),
         '#attributes' => array('onclick' => 'goalDelete(' . $goal->getGameGoalId() . ');'),
    );
    
    $assistPlayerName = "";
    $secondAssistPlayerName = "";
    
    if($goal->getAssistSeasonTeamPlayerId() != NULL) {
      $assistPlayerName = $goal->getAssistPlayerLastName() . ', ' . $goal->getAssistPlayerFirstName();
    }
    
    if($goal->getSecondAssistSeasonTeamPlayerId() != NULL) {
      $secondAssistPlayerName = $goal->getSecondAssistPlayerLastName() . ', ' . $goal->getSecondAssistPlayerFirstName();
    }
    
    $options[$goal->getGameGoalId()] = array(
       'period' => $goal->getPeriodName(),
       'time' => $goal->getTime(),
       'team' => $goal->getTeamName(),
       'goal' => $goal->getPlayerLastName() . ', ' . $goal->getPlayerFirstName(), 
       'assist' => $assistPlayerName, 
       '2nd_assist' => $secondAssistPlayerName, 
       'edit' => array('data' => $edit_button),
       'delete' => array('data' => $delete_button),
    );
  }

  $form['addLink'] = array(
       '#type' => 'link',
       '#title' => t('Add new goal'),
       '#href' => '#',
       '#attributes' => array('onclick' => 'goalAdd(0)', 'class' => 'preventDefault'),
  );
  
  $form['table'] = array(
       '#theme' => 'table',
       '#header' => $header,
       '#rows' => $options,
       '#id' => 'goalTable',
       '#empty' => t('No Goals Found'),
  );  
   
  $form['gameId'] = array(
       '#type' => 'hidden',
       '#value' => $game->getGameId(),
  );

  $form['formAction'] = array(
       '#type' => 'hidden',
       '#value' => '',
       '#attributes' => array('id' => 'formAction'),
  );

  $form['gameGoalId'] = array(
       '#type' => 'hidden',
       '#value' => '',
       '#attributes' => array('id' => 'gameGoalId'),
  );
  
  return $form;
}

function hockey_stats_game_goals_add($form, &$form_state) {

  $gameid = Utility::getCookieValue(Utility::COOKIE_GAMEID);
  
  if (isset($form_state['input']['gameId'])) {
    $game = new Game($form_state['input']['gameId']);
    $game->build();
  }
  else if(isset($gameid)) {
    $game = new Game($gameid);
    $game->build();
  }
  if (!isset($game)) {
    throw(new Exception("Game not found"));
  }

  $application = new Application();
  
  $form['goal'] = array(
       '#type' => 'fieldset',
       '#title' => t('Goal for ' . $game->getGameDescription()),
  );
    
  $form['goal']['period'] = array(
       '#type' => 'select',
       '#title' => t('Period'),
       '#options' => array(0 => '') + $application->getPeriodList(), 
  );
  
  $form['goal']['time'] = array(
       '#type' => 'textfield',
       '#title' => t('Time'),
       '#size' => 5,
       '#maxlength' => 5,
  );
  
  $form['goal']['seasonTeamPlayerId'] = array(
       '#type' => 'select',
       '#title' => t('Player'),
       '#options' => array(0 => '') + $game->getAllPlayersList(), 
  );
  
  $form['goal']['assistSeasonTeamPlayerId'] = array(
       '#type' => 'select',
       '#title' => t('Assist'),
       '#options' => array(0 => '') + $game->getAllPlayersList(), 
  );  
  
  $form['goal']['secondAssistSeasonTeamPlayerId'] = array(
       '#type' => 'select',
       '#title' => t('2nd Assist'),
       '#options' => array(0 => '') + $game->getAllPlayersList(), 
  ); 

  $form['gameId'] = array(
       '#type' => 'hidden',
       '#value' => $game->getGameId(),
  );
  
  $form['gameGoalId'] = array(
       '#type' => 'hidden',
       '#value' => 0,
  );
       
  // buttons
  $form['save'] = array(
       '#type' => 'submit',
       '#value' => 'Save and Add Another',
  );
    
  $form['submit'] = array(
       '#type' => 'submit',
       '#value' => 'Submit',
  );
  
  $form['cancel'] = Utility::getCancelButton();

  return $form;
}

function hockey_stats_game_goals_edit($form, &$form_state) {

  $gameid = Utility::getCookieValue(Utility::COOKIE_GAMEID);
  
  if (isset($form_state['input']['gameId'])) {
    $game = new Game($form_state['input']['gameId']);
    $game->build();
  }
  else if(isset($gameid)) {
    $game = new Game($gameid);
    $game->build();
  }
  if (!isset($game)) {
    throw(new Exception("Game not found"));
  }

  // if it was submitted via the form itself
  if ($form_state['input']['form_id'] == 'hockey_stats_game_goal_edit') {
    $goal = hockey_stats_build_goal_from_form($form_state);
  }
  elseif (isset($form_state['input']['gameGoalId'])) {
    $goal = new GameGoal($form_state['input']['gameGoalId']);
    $goal->build();
  }
  else {
    throw(new Exception("Goal not found"));
  }
  
  $application = new Application();
  
  $form['goal'] = array(
       '#type' => 'fieldset',
       '#title' => t('Goal for ' . $game->getGameDescription()),
  );
    
  $form['goal']['period'] = array(
       '#type' => 'select',
       '#title' => t('Period'),
       '#options' => array(0 => '') + $application->getPeriodList(), 
       '#default_value' => $goal->getPeriodId(), 
  );
  
  $form['goal']['time'] = array(
       '#type' => 'textfield',
       '#title' => t('Time'),
       '#size' => 5,
       '#maxlength' => 5,
       '#default_value' => $goal->getTime(), 
  );
  
  $form['goal']['seasonTeamPlayerId'] = array(
       '#type' => 'select',
       '#title' => t('Player'),
       '#options' => array(0 => '') + $game->getAllPlayersList(),
       '#default_value' => $goal->getSeasonTeamPlayerId(), 
  );
  
  $form['goal']['assistSeasonTeamPlayerId'] = array(
       '#type' => 'select',
       '#title' => t('Assist'),
       '#options' => array(0 => '') + $game->getAllPlayersList(), 
       '#default_value' => $goal->getAssistSeasonTeamPlayerId(), 
  );  
  
  $form['goal']['secondAssistSeasonTeamPlayerId'] = array(
       '#type' => 'select',
       '#title' => t('2nd Assist'),
       '#options' => array(0 => '') + $game->getAllPlayersList(), 
       '#default_value' => $goal->getSecondAssistSeasonTeamPlayerId(), 
  ); 
  
  $form['gameId'] = array(
       '#type' => 'hidden',
       '#value' => $game->getGameId(),
  );
  
  $form['gameGoalId'] = array(
       '#type' => 'hidden',
       '#value' => $goal->getGameGoalId(),
  );
       
  $form['submit'] = array(
       '#type' => 'submit',
       '#value' => 'Submit',
  );
  
  $form['cancel'] = Utility::getCancelButton();

  return $form;
}

function hockey_stats_game_goals_delete($form, &$form_state) {
  
  /*
  if (isset($form_state['input']['gamePenaltyId'])) {
    $penalty = new GamePenalty($form_state['input']['gamePenaltyId']);
    $penalty->build();
  }
  else {
    throw(new Exception("Penalty not found"));
  }
  
  $warning_message = 'Are you sure you want to delete the ' . $penalty->getPenaltyName();
  $warning_message .= ' penalty on ' . $penalty->getPlayerLastName() . ', ' . $penalty->getPlayerFirstName();
  $warning_message .= '?';
*/
  $form['heading'] = array(
       '#prefix' => '<p>',
       '#markup' => '', //$warning_message,
       '#suffix' => '</p>',
  );
  
  $form['submit'] = array(
       '#type' => 'submit',
       '#value' => 'Delete Goal',
  );
  
  $form['cancel'] = Utility::getCancelButton();

  $form['gameGoalId'] = array(
       '#type' => 'hidden',
       '#value' => '', //$penalty->getGamePenaltyId(),
  );
    
  return $form;
}

function hockey_stats_game_goals_add_validate($form, &$form_state) {
  hockey_stats_game_goals_validate($form, $form_state);
}

function hockey_stats_game_goals_edit_validate($form, &$form_state) {
  hockey_stats_game_goals_validate($form, $form_state);
}

function hockey_stats_game_goals_validate($form, &$form_state) {
   $button = $form_state['input']['op'];

  if ($button == "Submit" || $button == "Save and Add Another") {

    $goal = hockey_stats_build_goal_from_form($form_state);
    $application = new Application();

    // period must be 1,2,3,OT (4)
    if (!$application->isValidPeriod($goal->getPeriodId())){
       form_set_error('period', 'Period is not valid');
    }

    //  time must be valid
    if (!Utility::isValidTime($goal->getTime())) {
      form_set_error('time', 'Time is not valid (MM:SS format)');
    }

    // player that scored must be selected
    if ($goal->getSeasonTeamPlayerId() == 0) {
      form_set_error('seasonTeamPlayerId', 'Player is not valid');
    }
    
    $goal_scorer_season_team_player = new SeasonTeamPlayer($goal->getSeasonTeamPlayerId());
    $goal_scorer_season_team_player->build();
    
    $season_team = new SeasonTeam($goal_scorer_season_team_player->getSeasonTeamId());
    
    if($goal->getAssistSeasonTeamPlayerId() > 0) {
      // assist cant be the same person that scored
      if($goal->getAssistSeasonTeamPlayerId() == $goal->getSeasonTeamPlayerId()) {
        form_set_error('assistSeasonTeamPlayerId', 'Assisting player cannot be the same as the goal scorer');
      }

      // player that had the assist must be on the same team as the goal scorer
      if(!$season_team->bothPlayersAreOnTeam($goal->getSeasonTeamPlayerId(), $goal->getAssistSeasonTeamPlayerId())) {
        form_set_error('assistSeasonTeamPlayerId', 'Assisting player must be on the same team as the goal scorer');
      }
    }
    
    if($goal->getSecondAssistSeasonTeamPlayerId() > 0) {
      
      // there cannot be a second assist without a first assist
      if($goal->getAssistSeasonTeamPlayerId() == NULL || $goal->getAssistSeasonTeamPlayerId() == 0) {
        form_set_error('secondAssistSeasonTeamPlayerId', 'There cannot be a second assist without a primary assist');
      }
      
      // second assist cant be the same player that scored
      if($goal->getSecondAssistSeasonTeamPlayerId() == $goal->getSeasonTeamPlayerId()) {
        form_set_error('secondAssistSeasonTeamPlayerId', 'Second assisting player cannot be the same as the goal scorer');
      }

      // assist and second assist cannot be the same player
      if($goal->getSecondAssistSeasonTeamPlayerId() == $goal->getAssistSeasonTeamPlayerId()) {
        form_set_error('secondAssistSeasonTeamPlayerId', 'Assist and second assist cannot be the same player');
      }
      
      // player that had thes second assist must be on the same team as the goal scorer
      if(!$season_team->bothPlayersAreOnTeam($goal->getSeasonTeamPlayerId(), $goal->getSecondAssistSeasonTeamPlayerId())) {
        form_set_error('secondAssistSeasonTeamPlayerId', 'Second assisting player must be on the same team as the goal scorer');
      } 
    }
  }
}

function hockey_stats_game_goals_add_submit($form, &$form_state) {
  hockey_stats_game_goals_submit($form, $form_state);
}

function hockey_stats_game_goals_edit_submit($form, &$form_state) {
  hockey_stats_game_goals_submit($form, $form_state);
}

function hockey_stats_game_goals_delete_submit($form, &$form_state) {

  if ($form_state['input']['op'] == "Delete Goal") {
   /* $game_penalty_id = $form_state['input']['gamePenaltyId'];

    if (isset($game_penalty_id)) {
      $penalty = new GamePenalty($game_penalty_id);
      $penalty->build();
    }
    else {
      throw(new Exception("Penalty not found"));
    }

    $penalty->setSavedDateTimeAndUser(Utility::currentDate(), Utility::currentUserId());
    $penalty->delete();*/

    drupal_set_message(t('The Goal has been deleted'));
  }

  $form_state['redirect'] = array('base/game/goals', );
}


function hockey_stats_game_goals_submit($form, &$form_state) {
  $button = $form_state['input']['op'];
    
  if ($button == "Submit" || $button == "Save and Add Another") {
    $goal = hockey_stats_build_goal_from_form($form_state);
    $goal->setSavedDateTimeAndUser(Utility::currentDate(), Utility::currentUserId());
    
    $goal_original = new GameGoal($goal->getGameGoalId());
    $goal_original->build();

    if (!isset($goal_original)) {
      $goal_original = new GameGoal();
    }

    if (!$goal->equals($goal_original)) {
      $goal->save();
    }

    drupal_set_message(t('The goal has been saved'));
    }
    
    if($button == "Save and Add Another") {
      $form_state['redirect'] = array('base/game/goals/add', );
    }
    else {
      $form_state['redirect'] = array('base/game/goals', );
    }
  }
  

function hockey_stats_build_goal_from_form(&$form_state) {

  $game_goalid = $form_state['input']['gameGoalId'];
  $gameid = $form_state['input']['gameId'];
  $periodid = $form_state['input']['period'];
  $time = trim($form_state['input']['time']);
  $season_team_playerid = $form_state['input']['seasonTeamPlayerId'];
  $assist_season_team_playerid = $form_state['input']['assistSeasonTeamPlayerId'];
  $second_assist_season_team_playerid = $form_state['input']['secondAssistSeasonTeamPlayerId'];
  
  $goal = new GameGoal($game_goalid, $gameid, $periodid, $time, 
    $season_team_playerid, $assist_season_team_playerid, $second_assist_season_team_playerid);

  return $goal;
}