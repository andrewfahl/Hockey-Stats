<?php

/**
 * @file
 * Hockey Stats Module Team Pages and Functions
 * 
 */
function hockey_stats_team_landing_page() {
  header('Content-type: text/plain; charset=UTF-8');
  header('Content-Disposition: inline');
  return drupal_get_form('hockey_stats_team_landing');
}

/**
 * Team menu item add team page
 */
function hockey_stats_team_add_page() {
  header('Content-type: text/plain; charset=UTF-8');
  header('Content-Disposition: inline');
  return drupal_get_form('hockey_stats_team_add');
}

/**
 * Team menu item edit team page
 */
function hockey_stats_team_edit_page() {
  header('Content-type: text/plain; charset=UTF-8');
  header('Content-Disposition: inline');
  return drupal_get_form('hockey_stats_team_edit');
}

/**
 * Team menu item delete team page
 */
function hockey_stats_team_delete_page() {
  header('Content-type: text/plain; charset=UTF-8');
  header('Content-Disposition: inline');
  return drupal_get_form('hockey_stats_team_delete');
}

function hockey_stats_team_set_roster_page() {
  header('Content-type: text/plain; charset=UTF-8');
  header('Content-Disposition: inline');
  return drupal_get_form('hockey_stats_team_set_roster');
}

function hockey_stats_team_assign_numbers_page() {
  header('Content-type: text/plain; charset=UTF-8');
  header('Content-Disposition: inline');
  return drupal_get_form('hockey_stats_team_assign_numbers');
}

/**
 * Team menu item team landing page
 */
function hockey_stats_team_landing() {

  $header = array(
       'name' => t('Team Name'),
       'activeYN' => t('Active'),
       'edit' => t('Edit'),
       'set_roster' => t('Set Roster'),
       'assign_numbers' => t('Assign Player Numbers'),
       'delete' => t('Delete'),
  );

  $options = array();

  $application = new Application();
  $teams = $application->getTeams();

  foreach ($teams as $team) {

    $edit_button = array(
         '#type' => 'button',
         '#value' => t('Edit'),
         '#attributes' => array('onclick' => 'teamEdit(' . $team->getTeamId() . ');'),
    );

    $set_roster_button = array(
         '#type' => 'button',
         '#value' => t('Set Roster'),
         '#attributes' => array('onclick' => 'setRoster(' . $team->getTeamId() . ');'),
    );

    $assign_numbers_button = array(
         '#type' => 'button',
         '#value' => t('Assign Numbers'),
         '#attributes' => array('onclick' => 'teamAssignNumbers(' . $team->getTeamId() . ');'),
    );

    $delete_button = array(
         '#type' => 'button',
         '#value' => t('Delete'),
         '#attributes' => array('onclick' => 'teamDelete(' . $team->getTeamId() . ');'),
    );

    $options[$team->getTeamId()] = array(
         'name' => t($team->getName()),
         'activeYN' => $team->getActiveYN() ? 'active' : '',
         'edit' => array('data' => $edit_button),
         'set_roster' => array('data' => $set_roster_button),
         'assign_numbers' => array('data' => $assign_numbers_button),
         'delete' => array('data' => $delete_button),
    );
  }

  $form['addLink'] = array(
       '#type' => 'link',
       '#title' => t('Add new team'),
       '#href' => 'base/team/add',
  );

  $form['table'] = array(
       '#theme' => 'table',
       '#header' => $header,
       '#rows' => $options,
       '#id' => 'teamTable',
       '#empty' => t('No teams found'),
  );

  $form['formAction'] = array(
       '#type' => 'hidden',
       '#value' => '',
       '#attributes' => array('id' => 'formAction'),
  );

  $form['teamId'] = array(
       '#type' => 'hidden',
       '#value' => '',
       '#attributes' => array('id' => 'teamId'),
  );

  $form['#attached']['js'] = array(
       drupal_get_path('module', 'hockey_stats') . '/includes/main.js',
  );

  return $form;
}

function hockey_stats_team_add($form_state = NULL) {

  $form['form'] = array(
       '#type' => 'fieldset',
       '#title' => t('Add Team'),
  );

  $form['form']['name'] = array(
       '#type' => 'textfield',
       '#title' => t('Team Name'),
       '#size' => 30,
       '#maxlength' => 30,
       '#attributes' => array(
            'class' => array(
                 'setFocusOnLoad'
            ),
       ),
  );

  $form['form']['activeYN'] = array(
       '#type' => 'checkbox',
       '#title' => t('Team Is Active'),
       '#default_value' => TRUE,
  );

  $form['form']['teamId'] = array(
       '#type' => 'hidden',
       '#value' => '0',
  );

  $form['form']['submit'] = array(
       '#type' => 'submit',
       '#value' => 'Submit',
  );

  $form['form']['cancel'] = Utility::getCancelButton();

  $form['#attached']['js'] = array(
       drupal_get_path('module', 'hockey_stats') . '/includes/main.js',
  );

  return $form;
}

function hockey_stats_team_edit($form, &$form_state) {

  // if it was submitted via the form itself
  if ($form_state['input']['form_id'] == 'hockey_stats_team_edit') {
    $team = hockey_stats_build_team_from_form($form_state);
  }
  elseif (isset($form_state['input']['teamId'])) {
    $team = new Team($form_state['input']['teamId']);
    $team->build();
  }
  else {
    throw(new Exception("Team not found"));
  }

  $form['form'] = array(
       '#type' => 'fieldset',
       '#title' => t('Edit Team'),
  );

  $form['form']['name'] = array(
       '#type' => 'textfield',
       '#title' => t('Team Name'),
       '#size' => 30,
       '#maxlength' => 30,
       '#value' => $team->getName(),
       '#attributes' => array(
            'class' => array(
                 'setFocusOnLoad'
            ),
       ),
  );

  $form['form']['activeYN'] = array(
       '#type' => 'checkbox',
       '#title' => t('Active'),
       '#default_value' => $team->getActiveYN(),
  );

  $form['form']['teamId'] = array(
       '#type' => 'hidden',
       '#value' => $team->getTeamId(),
  );

  $form['form']['submit'] = array(
       '#type' => 'submit',
       '#value' => 'Submit',
  );

  $form['form']['cancel'] = Utility::getCancelButton();

  $form['#attached']['js'] = array(
       drupal_get_path('module', 'hockey_stats') . '/includes/main.js',
  );

  return $form;
}

function hockey_stats_team_add_validate($form, &$form_state) {
  hockey_stats_validate_team($form, $form_state);
}

function hockey_stats_team_edit_validate($form, &$form_state) {
  hockey_stats_validate_team($form, $form_state);
}

function hockey_stats_validate_team($form, &$form_state) {
  if ($form_state['input']['op'] == "Submit") {
    $team = hockey_stats_build_team_from_form($form_state);
    $application = new Application();

    // team name is required
    if ($team->getName() == "") {
      form_set_error('name', 'The Team Name is required');
    }

    $dup_team = $application->getTeamByName($team->getName(), $team->getTeamId());

    // team name must be unique
    if (isset($dup_team)) {
      form_set_error('name', 'The Team Name "' . $team->getName() . '" is already in use');
    }
  }
}

function hockey_stats_team_delete($form, &$form_state) {

  if (isset($form_state['input']['teamId'])) {
    $team = new Team($form_state['input']['teamId']);
    $team->build();
  }
  else {
    throw(new Exception("Team mot found"));
  }

  $form['heading'] = array(
       '#prefix' => '<p>',
       '#markup' => 'Are you sure you want to delete the "' . $team->getName() . '" Team?  All data will be lost!',
       '#suffix' => '</p>',
  );

  $form['teamId'] = array(
       '#type' => 'hidden',
       '#value' => $team->getTeamId(),
  );

  $form['submit'] = array(
       '#type' => 'submit',
       '#value' => 'Delete Team',
  );

  $form['cancel'] = Utility::getCancelButton();

  return $form;
}

function hockey_stats_team_edit_submit($form, &$form_state) {
  hockey_stats_team_add_submit($form, $form_state);
}

function hockey_stats_team_add_submit($form, &$form_state) {
  if ($form_state['input']['op'] == "Submit") {
    $team = hockey_stats_build_team_from_form($form_state);
    $team->setSavedDateTimeAndUser(Utility::currentDate(), Utility::currentUserId());

    $team_original = new Team($team->getTeamId());
    $team_original->build();

    if (!isset($team_original)) {
      $team_original = new Team();
    }

    if (!$team->equals($team_original)) {
      $team->save();
    }

    drupal_set_message(t('The team has been saved'));
  }
  $form_state['redirect'] = array('base/team', );
}

function hockey_stats_team_delete_submit($form, &$form_state) {

  if ($form_state['input']['op'] == "Delete Team") {
    $team_id = $form_state['input']['teamId'];

    if (isset($team_id)) {
      $team = new Team($team_id);
      $team->build();
    }
    else {
      throw(new Exception("Team not found"));
    }

    $team->delete();

    drupal_set_message(t('The team has been deleted'));
  }
  $form_state['redirect'] = array('base/team', );
}

function hockey_stats_build_team_from_form(&$form_state) {

  $team_id = $form_state['input']['teamId'];
  $name = trim($form_state['input']['name']);

  if (isset($form_state['input']['activeYN'])) {
    $active_yn = 1;
  }
  else {
    $active_yn = 0;
  }

  $team = new Team($team_id, $name, $active_yn);

  return $team;
}

function hockey_stats_team_set_roster($form, &$form_state) {

  if (isset($form_state['input']['teamId'])) {
    $team = new Team($form_state['input']['teamId']);
    $team->build();
  }
  else {
    throw(new Exception("Team mot found"));
  }

  $seasonid_cookie = Utility::getCookieValue(Utility::COOKIE_SEASONID);
  $seasonid = 0;

  $season_team = NULL;

  if (isset($seasonid_cookie) && $team->isPlayingInSeasonYN($seasonid_cookie)) {
    $seasonid = $seasonid_cookie;
    $season_team = $team->getSeasonTeam($seasonid);
  }
  // use the latest season
  else {
    $latest_season_team = $team->getLatestSeasonTeam();
    if (isset($latest_season_team)) {
      $season_team = $latest_season_team;
    }
  }
  
  if (!isset($season_team)) {
    $form['text'] = array('#markup' => '<p>The team must be added to a season before you can set a roster.</p>');
    $form['teamId'] = array(
         '#type' => 'hidden',
         '#value' => '',
         '#attributes' => array('id' => 'teamId'),
         '#value' => $team->getTeamId(),
    );
    $form['cancel2'] = Utility::getCancelButton();
  }
  else {
    $options = array(0 => '') + $team->getSeasonList();

    $form['seasonid'] = array(
         '#type' => 'select',
         '#title' => t('Select Season'),
         '#options' => $options,
         '#default_value' => $season_team->getSeasonId(),
         '#attributes' => array('onchange' => 'setCookieSeasonId(jQuery(this).val()); window.location.reload();',
              'id' => 'seasonid'),
    );

    $form['teamId'] = array(
         '#type' => 'hidden',
         '#value' => '',
         '#attributes' => array('id' => 'teamId'),
         '#value' => $team->getTeamId(),
    );

    $form['season_teamid'] = array(
         '#type' => 'hidden',
         '#value' => '',
         '#attributes' => array('id' => 'season_teamid'),
         '#value' => $season_team->getSeasonTeamId(),
    );

    if (isset($season_team)) {

      $current_players = $season_team->getSeasonTeamPlayers();

      $on_team_checkboxes = array(); // all of the team names
      $on_team_ischecked = array(); // if the team is in the season or not

      foreach ($current_players as $player) {
        $on_team_checkboxes[$player->getSeasonTeamPlayerId()] = $player->getFullName();
        $on_team_ischecked[$player->getSeasonTeamPlayerId()] = $player->getSeasonTeamPlayerId();
      }

      $form['form']['current_players'] = array(
           '#type' => 'fieldset',
           '#title' => t('On the Team for this Season'),
      );

      $form['form']['current_players']['current_season_team_player'] = array(
           '#type' => 'checkboxes',
           '#options' => $on_team_checkboxes,
           '#attributes' => array('checked' => 'checked'),
           '#default_value' => $on_team_ischecked,
      );

      $form['form']['submit'] = array(
           '#type' => 'submit',
           '#value' => 'Submit',
      );

      $form['form']['cancel'] = Utility::getCancelButton();


      $current_playerids = $season_team->getPlayerIdList();
      $application = new Application();
      $all_players = $application->getActivePlayers();

      $all_players_checkboxes = array();
      $former_playerids = $team->getPlayerIdList();

      foreach ($all_players as $player) {
        // if player is not a current player
        if (!in_array($player->getPlayerId(), $current_playerids)) {
          $all_players_checkboxes[$player->getPlayerId()] = $player->getFullName();
        }
      }

      $form['form']['all_players'] = array(
           '#type' => 'fieldset',
           '#title' => t('All Available Players'),
           '#collapsed' => FALSE,
           '#collapsible' => TRUE,
      );

      $form['form']['all_players']['all_player'] = array(
           '#type' => 'checkboxes',
           '#options' => $all_players_checkboxes,
      );

      $former_players = $team->getPlayers();
      $former_players_checkboxes = array();

      foreach ($former_players as $player) {

        // if player is not currently on the team add the player to the list
        if (!in_array($player->getPlayerId(), $current_playerids)) {
          $former_players_checkboxes[$player->getPlayerId()] = $player->getFullName();
        }
      }

      $form['form']['former_players'] = array(
           '#type' => 'fieldset',
           '#title' => t('Previously Played on the Team'),
           '#collapsed' => FALSE,
           '#collapsible' => TRUE,
      );

      $form['form']['former_players']['former_player'] = array(
           '#type' => 'checkboxes',
           '#options' => $former_players_checkboxes,
      );

      $form['submit2'] = array(
           '#type' => 'submit',
           '#value' => 'Submit',
      );

      $form['cancel2'] = Utility::getCancelButton();
    }
  }
  return $form;
}

function hockey_stats_team_set_roster_submit($form, &$form_state) {

  if ($form_state['input']['op'] == "Submit") {

    $remaining_players = $form_state['input']['current_season_team_player'];
    $add_players = $form_state['input']['all_player'];
    $former_players = $form_state['input']['former_player'];
    $season_teamid = $form_state['input']['season_teamid'];

    // remove the unchecked current players
    if (isset($remaining_players)) {
      foreach ($remaining_players as $remaining_player) {
        if ($remaining_player == NULL) {
          $season_team_playerid = key($remaining_players);
          $season_team_player = New SeasonTeamPlayer($season_team_playerid);
          $season_team_player->setSavedDateTimeAndUser(Utility::currentDate(), Utility::currentUserId());
          $season_team_player->delete();
        }
        // move the array pointer so we can get the key (season team player id)
        next($remaining_players);
      }
    }

    $all_adds = array();

    if (isset($former_players)) {
      $all_adds = array_merge($all_adds, $former_players);
    }

    if (isset($add_players)) {
      $all_adds = array_merge($all_adds, $add_players);
    }

    $unique_adds = array_unique($all_adds);

    foreach ($unique_adds as $playerid) {
      if ($playerid != NULL) {
        $season_team_player = New SeasonTeamPlayer(NULL, $season_teamid, $playerid);
        $season_team_player->setSavedDateTimeAndUser(Utility::currentDate(), Utility::currentUserId());
        $season_team_player->save();
      }
    }

    drupal_set_message(t('The roster has been set.  You may want to assign numbers next'));
  }

  $form_state['redirect'] = array('base/team', );
}

function hockey_stats_team_assign_numbers($form, &$form_state) {

  if (isset($form_state['input']['teamId'])) {
    $team = new Team($form_state['input']['teamId']);
    $team->build();
  }
  else {
    throw(new Exception("Team mot found"));
  }

  $seasonid_cookie = Utility::getCookieValue(Utility::COOKIE_SEASONID);
  $seasonid = 0;

  $season_team = NULL;

  if (isset($seasonid_cookie) && $team->isPlayingInSeasonYN($seasonid_cookie)) {
    $seasonid = $seasonid_cookie;
    $season_team = $team->getSeasonTeam($seasonid);
  }
  // use the latest season
  else {
    $latest_season_team = $team->getLatestSeasonTeam();
    if (isset($latest_season_team)) {
      $season_team = $latest_season_team;
    }
  }
  
  if (!isset($season_team)) {
    $form['text'] = array('#markup' => '<p>The team must be added to a season before you can assign the numbers to the players.</p>');
    $form['teamId'] = array(
         '#type' => 'hidden',
         '#value' => '',
         '#attributes' => array('id' => 'teamId'),
         '#value' => $team->getTeamId(),
    );
    $form['cancel2'] = Utility::getCancelButton();
  }
  else {
    $options = array(0 => '') + $team->getSeasonList();

    $form['seasonid'] = array(
         '#type' => 'select',
         '#title' => t('Select Season'),
         '#options' => $options,
         '#default_value' => $season_team->getSeasonId(),
         '#attributes' => array('onchange' => 'setCookieSeasonId(jQuery(this).val()); window.location.reload();',
              'id' => 'seasonid'),
    );

    $form['teamId'] = array(
         '#type' => 'hidden',
         '#value' => '',
         '#attributes' => array('id' => 'teamId'),
         '#value' => $team->getTeamId(),
    );

    $form['season_teamid'] = array(
         '#type' => 'hidden',
         '#value' => '',
         '#attributes' => array('id' => 'season_teamid'),
         '#value' => $season_team->getSeasonTeamId(),
    );

    if (isset($season_team)) {

      $players = $season_team->getSeasonTeamPlayers();

      $form['form'] = array(
           '#type' => 'fieldset',
           '#title' => t('Assign numbers to players for the ' . $season_team->getSeasonName() . ' season'),
      );

      foreach ($players as $player) {
        $form['form']['seasonTeamPlayerId' . $player->getSeasonTeamPlayerID()] = array(
             '#type' => 'textfield',
             '#title' => t($player->getFullName()),
             '#size' => 5,
             '#maxlength' => 2,
             '#value' => $player->getNumber(),
        );
      }
    }

    $form['submit'] = array(
         '#type' => 'submit',
         '#value' => 'Submit',
    );

    $form['cancel'] = Utility::getCancelButton();
  }
  return $form;
}

function hockey_stats_team_assign_numbers_submit($form, &$form_state) {
  if (isset($form_state['input']['op'])) {
    if ($form_state['input']['op'] == "Submit") {

      $season_teamid = $form_state['input']['season_teamid'];
      $season_team = new SeasonTeam($season_teamid);
      $season_team_players = $season_team->getSeasonTeamPlayers();

      foreach ($season_team_players as $season_team_player) {
        $field_name = 'seasonTeamPlayerId' . $season_team_player->getSeasonTeamPlayerId();
        $number = $form_state['input'][$field_name];

        // if the number changed
        if ($number != $season_team_player->getNumber()) {
          $season_team_player->setNumber($number);
          $season_team_player->setSavedDateTimeAndUser(Utility::currentDate(), Utility::currentUserId());
          $season_team_player->save();
        }
      }
      drupal_set_message(t('Numbers have been assigned'));
    }
  }

  $form_state['redirect'] = array('base/team', );
}

function hockey_stats_team_assign_numbers_validate($form, &$form_state) {
  if ($form_state['input']['op'] == "Submit") {

    $season_teamid = $form_state['input']['season_teamid'];
    $season_team = new SeasonTeam($season_teamid);
    $season_team_players = $season_team->getSeasonTeamPlayers();

    // all fields must contain numbers 
    foreach ($season_team_players as $season_team_player) {
      $field_name = 'seasonTeamPlayerId' . $season_team_player->getSeasonTeamPlayerId();
      $number = $form_state['input'][$field_name];

      if (!empty($number) && !is_numeric($number)) {
        form_set_error($field_name, $season_team_player->getFullName() . ' must have a valid number or be left blank');
      }
    }
  }

  $form_state['redirect'] = array('base/team', );
}
