<?php

function hockey_stats_game_penalties_page() {
  header('Content-type: text/plain; charset=UTF-8');
  header('Content-Disposition: inline');
  return drupal_get_form('hockey_stats_game_penalties_landing');
}

function hockey_stats_game_penalties_add_page() {
  header('Content-type: text/plain; charset=UTF-8');
  header('Content-Disposition: inline');
  return drupal_get_form('hockey_stats_game_penalties_add');
}

function hockey_stats_game_penalties_edit_page() {
  header('Content-type: text/plain; charset=UTF-8');
  header('Content-Disposition: inline');
  return drupal_get_form('hockey_stats_game_penalties_edit');
}

function hockey_stats_game_penalties_delete_page() {
  header('Content-type: text/plain; charset=UTF-8');
  header('Content-Disposition: inline');
  return drupal_get_form('hockey_stats_game_penalties_delete');
}

function hockey_stats_game_penalties_landing($form, &$form_state) {
  
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
       'penalty' => t('Penalty'),
       'player' => t('Player'),
       'edit' => t('Edit'),
       'delete' => t('Delete'),
  );

  $options = array();
  
  $penalties = $game->getPenalties();
  
  foreach ($penalties as $penalty) {
    $edit_button = array(
         '#type' => 'button',
         '#value' => t('Edit'),
         '#attributes' => array('onclick' => 'penaltyEdit(' . $penalty->getGamePenaltyId(). ');'),
    );

    $delete_button = array(
         '#type' => 'button',
         '#value' => t('Delete'),
         '#attributes' => array('onclick' => 'penaltyDelete(' . $penalty->getGamePenaltyId() . ');'),
    );
    
    $options[$penalty->getGamePenaltyId()] = array(
       'period' => $penalty->getPeriod(),
       'time' => $penalty->getTime(),
       'team' => $penalty->getTeamName(),
       'penalty' => $penalty->getPenaltyName(),
       'player' => $penalty->getPlayerLastName() . ', ' . $penalty->getPlayerFirstName(),
       'edit' => array('data' => $edit_button),
       'delete' => array('data' => $delete_button),
    );
  }
  
  $form['addLink'] = array(
       '#type' => 'link',
       '#title' => t('Add new penalty'),
       '#href' => '#',
       '#attributes' => array('onclick' => 'penaltyAdd(0)', 'class' => 'preventDefault'),
  );
  
  $form['table'] = array(
       '#theme' => 'table',
       '#header' => $header,
       '#rows' => $options,
       '#id' => 'penaltyTable',
       '#empty' => t('No Penalties Found'),
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

  $form['gamePenaltyId'] = array(
       '#type' => 'hidden',
       '#value' => '',
       '#attributes' => array('id' => 'gamePenaltyId'),
  );
  
  return $form;
}

function hockey_stats_game_penalties_add($form, &$form_state) {

  if (isset($form_state['input']['gameId'])) {
    $game = new Game($form_state['input']['gameId']);
    $game->build();
  }

  if (!isset($game)) {
    throw(new Exception("Game not found"));
  }
  
  $application = new Application();
  
  $form['penalties'] = array(
       '#type' => 'fieldset',
       '#title' => t('Penalty for ' . $game->getGameDescription()),
  );
    
  // add new penalty
  $form['penalties']['period'] = array(
       '#type' => 'select',
       '#title' => t('Period'),
       '#options' => array(0 => '') + $application->getPeriodList(), 
  );
  
  $form['penalties']['time'] = array(
       '#type' => 'textfield',
       '#title' => t('Time'),
       '#size' => 5,
       '#maxlength' => 5,
  );
  
  $form['penalties']['seasonTeamPlayerId'] = array(
       '#type' => 'select',
       '#title' => t('Player'),
       '#options' => array(0 => '') + $game->getAllPlayersList(), 
  );
  
  $form['penalties']['penaltyId'] = array(
       '#type' => 'select',
       '#title' => t('Penalty'),
       '#options' => array(0 => '') + $application->getPenalties(),  
  );  
  
  $form['penalties']['duration'] = array(
       '#type' => 'select',
       '#title' => t('Penalty Duration'),
       '#options' => array(0 => '') + $application->getPenaltyDurationList(), 
  );
  
  $form['gameId'] = array(
       '#type' => 'hidden',
       '#value' => $game->getGameId(),
  );
  
  $form['gamePenaltyId'] = array(
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

function hockey_stats_game_penalties_edit($form, &$form_state) {
  
  $form['submit'] = array(
       '#type' => 'submit',
       '#value' => 'Submit',
  );
  
  $form['cancel'] = Utility::getCancelButton();

  return $form;
}

function hockey_stats_game_penalties_delete($form, &$form_state) {
  
  $form['submit'] = array(
       '#type' => 'submit',
       '#value' => 'Submit',
  );
  
  $form['cancel'] = Utility::getCancelButton();

  return $form;
}

function hockey_stats_game_penalties_add_validate($form, &$form_state) {
  $button = $form_state['input']['op'];

  if ($button == "Submit" || $button == "Save and Add Another") {

    $penalty = hockey_stats_build_penalty_from_form($form_state);
    $application = new Application();

    // period must be 1,2,3,OT (4)
    if (!$application->isValidPeriod($penalty->getPeriod())){
       form_set_error('period', 'Period is not valid');
    }

    //  time must be valid
    if (!Utility::isValidTime($penalty->getTime())) {
      form_set_error('time', 'Time is not valid (MM:SS format)');
    }

    // player must be selected
    if ($penalty->getSeasonTeamPlayerId() == 0) {
      form_set_error('seasonTeamPlayerId', 'Player is not valid');
    }

    // penalty must be selected
    if ($penalty->getPenaltyId() == 0) {
      form_set_error('penaltyId', 'Penatly is not valid');
    }

    // penalty duratin is required
    if ($penalty->getPenaltyDurationId() == 0) {
      form_set_error('duration', 'Penalty Duration is required');
    }  
  }
}

function hockey_stats_game_penalties_add_submit($form, &$form_state) {
  $button = $form_state['input']['op'];
    
  if ($button == "Submit" || $button == "Save and Add Another") {
    $penalty = hockey_stats_build_penalty_from_form($form_state);
    $penalty->setSavedDateTimeAndUser(Utility::currentDate(), Utility::currentUserId());
    
    $penalty_original = new GamePenalty($penalty->getGamePenaltyId());
    $penalty_original->build();

    if (!isset($penalty_original)) {
      $penalty_original = new GamePenalty();
    }

    if (!$penalty->equals($penalty_original)) {
      $penalty->save();
    }

    drupal_set_message(t('The penalty has been saved'));
  }
  
  $form_state['redirect'] = array('base/game/penalties', );
}


function hockey_stats_build_penalty_from_form(&$form_state) {

  $game_penaltyid = $form_state['input']['gamePenaltyId'];
  $gameid = $form_state['input']['gameId'];
  $period = $form_state['input']['period'];
  $time = trim($form_state['input']['time']);
  $season_team_playerid = $form_state['input']['seasonTeamPlayerId'];
  $penaltyid = $form_state['input']['penaltyId'];  
  $duration = trim($form_state['input']['duration']);

  $penalty = new GamePenalty($game_penaltyid, $gameid, $period, $time, 
    $season_team_playerid, $penaltyid, $duration);

  return $penalty;
}


